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
  , (+), (-), (*)
  , (++)
  )

import Data.Word

-- Helpers
prev :: Typed a => a -> Stream a -> Stream a
prev initVal s = [initVal] ++ s

absF :: Stream Float -> Stream Float
absF x = mux (x < 0) (0 - x) x

-- Bounded response: on rising edge of A, B must become true within k steps.
boundedResponse :: Word32 -> Stream Bool -> Stream Bool -> Stream Bool
boundedResponse k a b = expired
  where
    prevA = prev False a
    start = a && not prevA

    pending :: Stream Bool
    pending = [False] ++ nextPending
    nextPending = mux start (constant True) (mux b (constant False) pending)

    counter :: Stream Word32
    counter = [0] ++ nextCounter

    nextCounter =
      mux start
          (constant k)
          (mux b
               0
               (mux (counter > 0)
                    (counter - 1)
                    0))

    expired = (counter == 0) && pending && not b


-- External signals (must match main.c)
torque :: Stream Float
torque = extern "torque" Nothing

brake :: Stream Bool
brake = extern "brake" Nothing

acc :: Stream Bool
acc = extern "acc" Nothing

wsl :: Stream Float
wsl = extern "wsl" Nothing

wsr :: Stream Float
wsr = extern "wsr" Nothing

fault :: Stream Bool
fault = extern "fault" Nothing

safeMode :: Stream Bool
safeMode = extern "safe_mode" Nothing

stationary :: Stream Bool
stationary = extern "stationary" Nothing

rolling :: Stream Bool
rolling = extern "rolling" Nothing

collisionImminent :: Stream Bool
collisionImminent = extern "collision_imminent" Nothing

engine :: Stream Bool
engine = extern "engine" Nothing

gasPressed :: Stream Bool
gasPressed = extern "gas_pressed" Nothing

gasAngle :: Stream Float
gasAngle = extern "gas_angle" Nothing

brakeRequest :: Stream Bool
brakeRequest = extern "brake_request" Nothing

brakeTorque :: Stream Float
brakeTorque = extern "brake_torque" Nothing

abruptGas :: Stream Bool
abruptGas = extern "abrupt_gas" Nothing

-- Parameters / derived
vmax :: Stream Float
vmax = constant 120.0

-- Properties from sheet
propTorqueLimit :: Stream Bool
propTorqueLimit = torque <= 350.0

propBrakeAccConflict :: Stream Bool
propBrakeAccConflict = not (brake && acc)

-- abs(WSL - WSR) <= 0.15 * WSL
propWheelSpeed :: Stream Bool
propWheelSpeed = absF (wsl - wsr) <= (0.15 * wsl)

-- fault -> safe_mode within 3
violFaultSafeMode :: Stream Bool
violFaultSafeMode = boundedResponse 3 fault safeMode

-- Requirements / assumptions / guarantees
-- R1: (stationary & collision_imminent) -> (brake=1 & engine=0)
propR1 :: Stream Bool
propR1 = (stationary && collisionImminent) ==> (brake && not engine)

-- R2: gas_pressed -> within T (choose T=3) (brake=0 & engine=1)
violR2 :: Stream Bool
violR2 = boundedResponse 3 gasPressed (not brake && engine)

-- A.C: (stationary or rolling) and not brake_request -> engine = f(gas_angle)
-- Choose f(gas_angle) = gas_angle > 0.2
propAC :: Stream Bool
propAC = (((stationary || rolling) && not brakeRequest) ==> (engine == (gasAngle > 0.2)))

-- G.C: gas_pressed -> within 5 brake_torque = 0
violGC :: Stream Bool
violGC = boundedResponse 5 gasPressed (brakeTorque == 0)

-- G.F: abrupt_gas -> within 5 engine_torque = min (min=0)
-- Here engine_torque is the same as `torque`.
violGF :: Stream Bool
violGF = boundedResponse 5 abruptGas (torque == 0)

spec :: Spec
spec = do
  trigger "viol_torque_limit" (not propTorqueLimit) []
  trigger "viol_brake_acc_conflict" (not propBrakeAccConflict) []
  trigger "viol_wheel_speed_mismatch" (not propWheelSpeed) []
  trigger "viol_fault_safe_mode" violFaultSafeMode []

  trigger "viol_r1_collision_response" (not propR1) []
  trigger "viol_r2_gas_override" violR2 []
  trigger "viol_ac_engine_mapping" (not propAC) []
  trigger "viol_gc_brake_torque_zero" violGC []
  trigger "viol_gf_torque_min" violGF []

main :: IO ()
main = reify spec >>= compile "automotive_system"
