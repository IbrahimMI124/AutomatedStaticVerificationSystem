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

import Data.Int
import Data.Word

prev :: Typed a => a -> Stream a -> Stream a
prev initVal s = [initVal] ++ s

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
hr :: Stream Int32
hr = extern "hr" Nothing

pump :: Stream Bool
pump = extern "pump" Nothing

rate :: Stream Float
rate = extern "rate" Nothing

vitalsOOR :: Stream Bool
vitalsOOR = extern "vitals_out_of_range" Nothing

alarm :: Stream Bool
alarm = extern "alarm" Nothing

-- properties
-- HR < 40 -> within 10 pump == 0
violHRPumpStop :: Stream Bool
violHRPumpStop = boundedResponse 10 (hr < 40) (not pump)

-- rate <= rate_max (choose 5.0)
propRateMax :: Stream Bool
propRateMax = rate <= 5.0

-- vitals_out_of_range -> within 1 alarm
violVitalsAlarm :: Stream Bool
violVitalsAlarm = boundedResponse 1 vitalsOOR alarm

spec :: Spec
spec = do
  trigger "viol_hr_pump_stop" violHRPumpStop []
  trigger "viol_infusion_rate_max" (not propRateMax) []
  trigger "viol_vitals_alarm" violVitalsAlarm []

main :: IO ()
main = reify spec >>= compile "medical_system"
