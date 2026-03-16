module Main where

import Language.Copilot
import Copilot.Compile.C99
import Prelude hiding ((>), (<))

-- external temperature sensor value
temp :: Stream Word8
temp = extern "temperature" Nothing

-- convert byte range to Celsius
ctemp :: Stream Float
ctemp = (unsafeCast temp) * (150.0 / 255.0) - 50.0

spec :: Spec
spec = do
    trigger "heaton"  (ctemp < 18.0) [arg ctemp]
    trigger "heatoff" (ctemp > 21.0) [arg ctemp]

main = reify spec >>= compile "heater"
