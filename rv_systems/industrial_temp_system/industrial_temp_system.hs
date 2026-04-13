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

temp :: Stream Float
temp = extern "temp" Nothing

alarm :: Stream Bool
alarm = extern "alarm" Nothing

cooling :: Stream Bool
cooling = extern "cooling" Nothing

warn :: Stream Float
warn = constant 80.0

tempMax :: Stream Float
tempMax = constant 120.0

-- Properties

-- If temp > 80 then alarm within 2 steps
violAlarmWithin2 :: Stream Bool
violAlarmWithin2 = boundedResponse 2 (temp > warn) alarm

-- Reactor temp invariant
propReactorTempMax :: Stream Bool
propReactorTempMax = temp <= tempMax

-- If temp > warn then cooling within 2
violCoolingWithin2 :: Stream Bool
violCoolingWithin2 = boundedResponse 2 (temp > warn) cooling

spec :: Spec
spec = do
  trigger "viol_alarm_within_2" violAlarmWithin2 []
  trigger "viol_reactor_temp_max" (not propReactorTempMax) []
  trigger "viol_cooling_within_2" violCoolingWithin2 []

main :: IO ()
main = reify spec >>= compile "industrial_temp_system"
