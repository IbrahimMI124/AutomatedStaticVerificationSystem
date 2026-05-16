module Main where

import Language.Copilot
import Copilot.Compile.C99
import Data.Int (Int32)

import Prelude hiding ((&&), (||), not, (==), (/=), (<=), (>=), (>), (<))

fr_steering_angle :: Stream Int32
fr_steering_angle = extern "fr_steering_angle" Nothing

fr_cmd_steer :: Stream Int32
fr_cmd_steer = extern "fr_cmd_steer" Nothing

fr_motor_steer :: Stream Int32
fr_motor_steer = extern "fr_motor_steer" Nothing

spec :: Spec
spec = do
  let steeringErr = ((fr_cmd_steer * 40) `Language.Copilot.div` 100) - fr_steering_angle
  let posOk = (steeringErr <= 0) || (fr_motor_steer > 0)
  let negOk = (steeringErr >= 0) || (fr_motor_steer < 0)
  let zeroOk = (steeringErr /= 0) || (fr_motor_steer == 0)
  let p = posOk && negOk && zeroOk
  trigger "steering_sign_violation" (not p) []

main :: IO ()
main = reify spec >>= compile "steering_sign_mon"
