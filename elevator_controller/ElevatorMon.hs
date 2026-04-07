module Main where

import Language.Copilot
import Copilot.Language
import Copilot.Language.Operators.Mux (mux)
import Copilot.Compile.C99

import Prelude hiding
  ( (==), (/=)
  , (<), (<=), (>), (>=)
  , (&&), (||)
  , not
  , (+), (-)
  , (++)
  )

import Data.Word

-- Monitor constants
floors :: Word8
floors = 5

kServe :: Word8
kServe = 5

kStarve :: Word8
kStarve = 15

-- Enum encodings (match `elevator_types.h` and `elevator_monitor_io.c`)
dirIdleV, dirUpV, dirDownV :: Word8
dirIdleV = 0
dirUpV   = 1
dirDownV = 2

doorOpenV, doorClosedV :: Word8
doorOpenV   = 0
doorClosedV = 1

dirIdle, dirUp, dirDown :: Stream Word8
dirIdle = constant dirIdleV
dirUp   = constant dirUpV
dirDown = constant dirDownV

doorOpen, doorClosed :: Stream Word8
doorOpen   = constant doorOpenV
doorClosed = constant doorClosedV

-- External inputs (updated once per `system_step()`)
floorS :: Stream Word8
floorS = extern "mon_current_floor" Nothing

dirS :: Stream Word8
dirS = extern "mon_dir" Nothing

doorS :: Stream Word8
doorS = extern "mon_door" Nothing

req0, req1, req2, req3, req4 :: Stream Bool
req0 = extern "mon_req0" Nothing
req1 = extern "mon_req1" Nothing
req2 = extern "mon_req2" Nothing
req3 = extern "mon_req3" Nothing
req4 = extern "mon_req4" Nothing

servedS :: Stream Bool
servedS = extern "mon_served" Nothing

anyReq :: Stream Bool
anyReq = req0 || req1 || req2 || req3 || req4

noReq :: Stream Bool
noReq = not anyReq

prev :: Typed a => a -> Stream a -> Stream a
prev initVal s = [initVal] ++ s

-- Helper: bounded response timer for a request stream.
-- Starts a countdown on rising edge of `req` and expects `servedAt` within `k` ticks.
-- Violation: request still pending after countdown expires.
boundedReq :: Word8 -> Stream Bool -> Stream Bool -> Stream Bool
boundedReq k req servedAt = expired
  where
    prevReq = prev False req
    start   = req && not prevReq

    counter :: Stream Word8
    counter = [0] ++ nextCounter

    nextCounter :: Stream Word8
    nextCounter =
      mux start
          (constant k)
          (mux servedAt
               0
               (mux (counter > 0)
                    (counter - 1)
                    0))

    expired :: Stream Bool
    expired = (counter == 0) && req && not start && not servedAt

servedAtFloor :: Word8 -> Stream Bool
servedAtFloor i = (doorS == doorOpen) && (floorS == constant i)

-- Property 1: No movement when door is open.
-- LTL (checked via previous-state rewrite): G(door_open -> X same_floor)
-- Implemented as: (prev door_open) -> (floor == prev floor)
prop_no_move_door_open :: Stream Bool
prop_no_move_door_open = (prevDoor == doorOpen) ==> (floorS == prevFloor)
  where
    prevDoor  = prev doorClosedV doorS
    prevFloor = prev 0 floorS

-- Property 2: Floor bounds.
-- LTL: G(0 <= floor < FLOORS)
prop_floor_bounds :: Stream Bool
prop_floor_bounds = floorS < constant floors

-- Property 3: No skipping floors.
-- LTL: G(dir=UP -> X(floor=prev+1 or same)) and similarly for DOWN.
prop_no_skip_up :: Stream Bool
prop_no_skip_up = (pDir == dirUp) ==> ((floorS == pFloor) || (floorS == (pFloor + 1)))
  where
    pDir   = prev dirIdleV dirS
    pFloor = prev 0 floorS

prop_no_skip_down :: Stream Bool
prop_no_skip_down = (pDir == dirDown) ==> ((floorS == pFloor) || ((floorS + 1) == pFloor))
  where
    pDir   = prev dirIdleV dirS
    pFloor = prev 0 floorS

-- Property 4: Door opens only when serving.
-- LTL: G(door=OPEN -> request[current_floor] was true)
-- In this model, `mon_served` captures "door opened due to serving".
prop_door_open_only_when_serving :: Stream Bool
prop_door_open_only_when_serving = (doorS == doorOpen) ==> servedS

-- Property 5: Requests served within k steps (per floor).
viol_req_not_served_0 :: Stream Bool
viol_req_not_served_0 = boundedReq kServe req0 (servedAtFloor 0)

viol_req_not_served_1 :: Stream Bool
viol_req_not_served_1 = boundedReq kServe req1 (servedAtFloor 1)

viol_req_not_served_2 :: Stream Bool
viol_req_not_served_2 = boundedReq kServe req2 (servedAtFloor 2)

viol_req_not_served_3 :: Stream Bool
viol_req_not_served_3 = boundedReq kServe req3 (servedAtFloor 3)

viol_req_not_served_4 :: Stream Bool
viol_req_not_served_4 = boundedReq kServe req4 (servedAtFloor 4)

-- Property 6: Direction consistency (safety form).
-- LTL: G(dir=UP -> X(floor >= prevFloor)) and G(dir=DOWN -> X(floor <= prevFloor))
prop_dir_consistent_up :: Stream Bool
prop_dir_consistent_up = (pDir == dirUp) ==> (floorS >= pFloor)
  where
    pDir   = prev dirIdleV dirS
    pFloor = prev 0 floorS

prop_dir_consistent_down :: Stream Bool
prop_dir_consistent_down = (pDir == dirDown) ==> (floorS <= pFloor)
  where
    pDir   = prev dirIdleV dirS
    pFloor = prev 0 floorS

-- Property 7: Idle correctness.
-- In this model, requests can arrive nondeterministically each tick.
-- So the meaningful safety form is: if there are no pending requests NOW and
-- the door is closed NOW, then the controller must report IDLE NOW.
prop_idle_correctness :: Stream Bool
prop_idle_correctness = ((doorS == doorClosed) && noReq) ==> (dirS == dirIdle)

-- Property 8: No starvation (bounded for CBMC, per floor).
viol_starvation_0 :: Stream Bool
viol_starvation_0 = boundedReq kStarve req0 (not req0)

viol_starvation_1 :: Stream Bool
viol_starvation_1 = boundedReq kStarve req1 (not req1)

viol_starvation_2 :: Stream Bool
viol_starvation_2 = boundedReq kStarve req2 (not req2)

viol_starvation_3 :: Stream Bool
viol_starvation_3 = boundedReq kStarve req3 (not req3)

viol_starvation_4 :: Stream Bool
viol_starvation_4 = boundedReq kStarve req4 (not req4)

spec :: Spec
spec = do
  -- Safety triggers (one per property)
  trigger "viol_move_with_open_door" (not prop_no_move_door_open) []
  trigger "viol_floor_bounds"        (not prop_floor_bounds) []
  trigger "viol_door_open_only_when_serving" (not prop_door_open_only_when_serving) []
  trigger "viol_dir_consistent_up"   (not prop_dir_consistent_up) []
  trigger "viol_dir_consistent_down" (not prop_dir_consistent_down) []
  trigger "viol_no_skip_up"          (not prop_no_skip_up) []
  trigger "viol_no_skip_down"        (not prop_no_skip_down) []
  trigger "viol_idle_correctness"    (not prop_idle_correctness) []

  -- Temporal / bounded response triggers
  trigger "viol_req_not_served_0" viol_req_not_served_0 []
  trigger "viol_req_not_served_1" viol_req_not_served_1 []
  trigger "viol_req_not_served_2" viol_req_not_served_2 []
  trigger "viol_req_not_served_3" viol_req_not_served_3 []
  trigger "viol_req_not_served_4" viol_req_not_served_4 []

  trigger "viol_starvation_0" viol_starvation_0 []
  trigger "viol_starvation_1" viol_starvation_1 []
  trigger "viol_starvation_2" viol_starvation_2 []
  trigger "viol_starvation_3" viol_starvation_3 []
  trigger "viol_starvation_4" viol_starvation_4 []

main :: IO ()
main = reify spec >>= compile "elevator_mon"
