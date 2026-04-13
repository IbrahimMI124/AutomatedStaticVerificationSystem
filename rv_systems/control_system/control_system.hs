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

prev :: Typed a => a -> Stream a -> Stream a
prev initVal s = [initVal] ++ s

absF :: Stream Float -> Stream Float
absF x = mux (x < 0) (0 - x) x

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

-- externs
output :: Stream Float
output = extern "output" Nothing

ref :: Stream Float
ref = extern "ref" Nothing

control :: Stream Float
control = extern "control" Nothing

u :: Stream Float
u = extern "u" Nothing

velocity :: Stream Float
velocity = extern "velocity" Nothing

disturbance :: Stream Bool
disturbance = extern "disturbance" Nothing

startup :: Stream Bool
startup = extern "startup" Nothing

-- (NASA) Post condition after t>=50: abs(output-ref) <= 0.05*ref
-- Use a counter t.
ticks :: Stream Word32
ticks = [0] ++ (ticks + 1)

tolOk :: Stream Bool
tolOk = absF (output - ref) <= (0.05 * ref)

propPostCondition :: Stream Bool
propPostCondition = (ticks < 50) || tolOk

-- Startup stabilize within 50: startup -> F[0,50] tolOk
violStartupStabilize :: Stream Bool
violStartupStabilize = boundedResponse 50 startup tolOk

-- Actuator saturation: -100 <= control <= 100
propSaturation :: Stream Bool
propSaturation = (control >= (-100)) && (control <= 100)

-- Temporal consistency: |u(t)-u(t-1)| <= Δu, choose Δu=3
propTemporalConsistency :: Stream Bool
propTemporalConsistency = absF (u - prev 0 u) <= 3

-- Bounded stabilisation: disturbance -> F[0,5] (|velocity| <= eps), choose eps=1.0
propVelStable :: Stream Bool
propVelStable = absF velocity <= 1.0

violBoundedStabilisation :: Stream Bool
violBoundedStabilisation = boundedResponse 5 disturbance propVelStable

spec :: Spec
spec = do
  trigger "viol_post_condition" (not propPostCondition) []
  trigger "viol_startup_stabilize" violStartupStabilize []
  trigger "viol_actuator_saturation" (not propSaturation) []
  trigger "viol_temporal_consistency" (not propTemporalConsistency) []
  trigger "viol_bounded_stabilisation" violBoundedStabilisation []

main :: IO ()
main = reify spec >>= compile "control_system"
