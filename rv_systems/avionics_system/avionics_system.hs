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

altitude :: Stream Float
altitude = extern "altitude" Nothing

airspeed :: Stream Float
airspeed = extern "airspeed" Nothing

control :: Stream Bool
control = extern "control" Nothing

altMin :: Stream Float
altMin = constant 300.0

stall :: Stream Float
stall = constant 50.0

propAltitudeMin :: Stream Bool
propAltitudeMin = altitude >= altMin

violStallControl :: Stream Bool
violStallControl = boundedResponse 2 (airspeed < stall) control

spec :: Spec
spec = do
  trigger "viol_altitude_min" (not propAltitudeMin) []
  trigger "viol_stall_control" violStallControl []

main :: IO ()
main = reify spec >>= compile "avionics_system"
