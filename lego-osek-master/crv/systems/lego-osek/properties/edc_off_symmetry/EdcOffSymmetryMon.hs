module Main where

import Language.Copilot
import Copilot.Compile.C99
import Data.Int (Int8, Int32)

import Prelude hiding ((&&), (||), not, (==), (/=))

fr_edc_flag :: Stream Int8
fr_edc_flag = extern "fr_edc_flag" Nothing

fr_motor_left :: Stream Int32
fr_motor_left = extern "fr_motor_left" Nothing

fr_motor_right :: Stream Int32
fr_motor_right = extern "fr_motor_right" Nothing

spec :: Spec
spec = do
  let edcOff = fr_edc_flag == 1
  let motorsEqual = fr_motor_left == fr_motor_right
  let p = (not edcOff) || motorsEqual
  trigger "edc_off_symmetry_violation" (not p) []

main :: IO ()
main = reify spec >>= compile "edc_off_symmetry_mon"
