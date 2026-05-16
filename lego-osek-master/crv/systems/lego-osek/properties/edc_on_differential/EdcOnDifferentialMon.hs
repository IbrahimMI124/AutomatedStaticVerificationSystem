module Main where

import Language.Copilot
import Copilot.Compile.C99
import Data.Int (Int8, Int32)

import Prelude hiding ((&&), (||), not, (==), (/=), (<=), (>=), (>), (<))

fr_edc_flag :: Stream Int8
fr_edc_flag = extern "fr_edc_flag" Nothing

fr_steering_angle :: Stream Int32
fr_steering_angle = extern "fr_steering_angle" Nothing

fr_cmd_speed :: Stream Int32
fr_cmd_speed = extern "fr_cmd_speed" Nothing

fr_motor_left :: Stream Int32
fr_motor_left = extern "fr_motor_left" Nothing

fr_motor_right :: Stream Int32
fr_motor_right = extern "fr_motor_right" Nothing

spec :: Spec
spec = do
  let edcOn = fr_edc_flag == (-1)
  let turning = (fr_steering_angle > 2) || (fr_steering_angle < (-2))
  let motorsDiffer = fr_motor_left /= fr_motor_right
  let p = (not (edcOn && turning)) || (fr_cmd_speed == 0) || motorsDiffer
  trigger "edc_on_differential_violation" (not p) []

main :: IO ()
main = reify spec >>= compile "edc_on_differential_mon"
