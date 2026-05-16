module Main where

import Language.Copilot
import Copilot.Compile.C99
import Data.Int (Int8)

import Prelude hiding ((&&), (||), not, (==), (++), (/=))

-- Externs published by C glue before calling step()
fr_touch :: Stream Bool
fr_touch = extern "fr_touch" Nothing

fr_edc_flag :: Stream Int8
fr_edc_flag = extern "fr_edc_flag" Nothing

spec :: Spec
spec = do
  let prevTouch = [False] ++ fr_touch
  let prevEdc   = [0] ++ fr_edc_flag

  let first = [True] ++ false

  let touchRises  = fr_touch && not prevTouch
  let touchSteady = fr_touch == prevTouch
  let edcToggles  = fr_edc_flag /= prevEdc

  let p7_body = (not touchRises || edcToggles) && (not touchSteady || not edcToggles)
  let p7 = first || p7_body

  -- When violated, call into C to latch a violation flag.
  trigger "touch_edc_violation" (not p7) []

main :: IO ()
main = reify spec >>= compile "touch_edc_mon"
