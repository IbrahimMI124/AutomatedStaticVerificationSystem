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

angle :: Stream Float
angle = extern "angle" Nothing

servo :: Stream Float
servo = extern "servo" Nothing

eStop :: Stream Bool
eStop = extern "e_stop" Nothing

motorPower :: Stream Float
motorPower = extern "motor_power" Nothing

angleMin, angleMax :: Stream Float
angleMin = constant (-1.5)
angleMax = constant (1.5)

propJointLimits :: Stream Bool
propJointLimits = (angle >= angleMin) && (angle <= angleMax)

-- Rate-of-change: abs(servo - PRE(servo)) <= 3
propServoDelta :: Stream Bool
propServoDelta = absF (servo - prev 0 servo) <= 3

-- E-stop -> motor power = 0
propEStop :: Stream Bool
propEStop = eStop ==> (motorPower == 0)

spec :: Spec
spec = do
  trigger "viol_joint_limits" (not propJointLimits) []
  trigger "viol_servo_delta" (not propServoDelta) []
  trigger "viol_estop_motor_power" (not propEStop) []

main :: IO ()
main = reify spec >>= compile "robotics_system"
