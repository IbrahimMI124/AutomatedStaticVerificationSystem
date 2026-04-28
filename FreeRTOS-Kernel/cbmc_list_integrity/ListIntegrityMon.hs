module Main where

import Language.Copilot
import Copilot.Compile.C99

import Prelude hiding ((&&), (||), not)

-- Single safety property: after each list operation, the list must satisfy
-- basic structural invariants. C code publishes fr_list_ok.

fr_list_ok :: Stream Bool
fr_list_ok = extern "fr_list_ok" Nothing

spec :: Spec
spec = do
  -- When violated, generated C calls a user-provided function named list_integrity().
  trigger "list_integrity" (not fr_list_ok) []

main :: IO ()
main = reify spec >>= compile "list_integrity_mon"
