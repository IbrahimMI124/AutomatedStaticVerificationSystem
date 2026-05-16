module Main where

import Language.Copilot
import Copilot.Compile.C99
import Data.Int (Int32)

import Prelude hiding ((&&), (||), not, (==), (/=), (<=), (>=))

fr_steering_angle :: Stream Int32
fr_steering_angle = extern "fr_steering_angle" Nothing

fr_motor_left :: Stream Int32
fr_motor_left = extern "fr_motor_left" Nothing

fr_motor_right :: Stream Int32
fr_motor_right = extern "fr_motor_right" Nothing

spec :: Spec
spec = do
  let inDeadzone = (fr_steering_angle <= 2) && (fr_steering_angle >= (-2))
  let motorsEqual = fr_motor_left == fr_motor_right
  let p = (not inDeadzone) || motorsEqual
  trigger "deadzone_symmetry_violation" (not p) []

main :: IO ()
main = reify spec >>= compile "deadzone_symmetry_mon"
