module Main where

-- import Language.Copilot
-- import Copilot.Compile.C99
-- import Prelude hiding ((>), (<))  need prelude, normal haskell import, but has things that conflict with copilot libraries, so hide them!

import Language.Copilot
import Copilot.Compile.C99

import Prelude hiding ((&&), (++), drop, (>), (<))

-- Copilot specification
propTempRiseShutOff :: Spec
propTempRiseShutOff =
  trigger "over_temp_rise"
          (overTempRise && running)
          []
  where

    maxTemp = 500.0

    -- temperature stream with initial padding
    temps :: Stream Float
    temps = [maxTemp, maxTemp, maxTemp] ++ temp

    -- external temperature input
    temp :: Stream Float
    temp = extern "temp" Nothing

    -- detect temperature rise over 2 ticks
    overTempRise :: Stream Bool
    overTempRise = drop 2 temps > (2.3 + temps)

    -- whether fuel injector is running
    running :: Stream Bool
    running = extern "running" Nothing


main :: IO ()
main = reify propTempRiseShutOff >>= compile "engine"