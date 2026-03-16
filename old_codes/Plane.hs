module Main where
import Language.Copilot
import Copilot.Language
import Copilot.Compile.C99
import Prelude hiding ((>), not)

autopilot :: Stream Bool
autopilot = extern "autopilot" Nothing

altitude :: Stream Word64
altitude = extern "altitude" Nothing

threshold :: Stream Word64
threshold = constant 10000 -- feet

property :: Stream Bool
property = autopilot ==> (altitude > threshold)

violation :: Stream Bool
violation = not property

spec = do
   trigger "recover" violation [ arg altitude, arg threshold ]

main = reify spec >>= compile "plane"
