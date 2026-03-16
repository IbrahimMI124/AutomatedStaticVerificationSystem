module Main where

import Language.Copilot
import Prelude hiding ((>), not, (==), (-), (&&), (++))
import Copilot.Language
import Copilot.Language.Operators.Mux (mux)
import Copilot.Compile.C99
import Data.Word

-- Inputs
autopilot :: Stream Bool
autopilot = extern "autopilot" Nothing

altitude :: Stream Word64
altitude = extern "altitude" Nothing

threshold :: Stream Word64
threshold = constant 10000

-- Condition: altitude safe
safeAlt :: Stream Bool
safeAlt = altitude > threshold

-- Detect rising edge of autopilot
prevAutopilot :: Stream Bool
prevAutopilot = [False] ++ autopilot

engage :: Stream Bool
engage = autopilot && not prevAutopilot

-- Countdown state
counter :: Stream Word8
counter = [0] ++ nextCounter

nextCounter :: Stream Word8
nextCounter =
  mux engage
      3
      (mux safeAlt
           0
           (mux (counter > 0)
                (counter - 1)
                0))

-- Violation
violation :: Stream Bool
violation = (counter == 1) && not safeAlt

spec :: Spec
spec = do
  trigger "recover" violation [arg altitude, arg threshold]

main :: IO ()
main = reify spec >>= compile "plane2"

