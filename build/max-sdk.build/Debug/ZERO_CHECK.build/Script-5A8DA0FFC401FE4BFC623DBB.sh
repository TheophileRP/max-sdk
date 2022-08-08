#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build
  make -f /Users/trompette/documents/GitHub/max-sdk/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build
  make -f /Users/trompette/documents/GitHub/max-sdk/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build
  make -f /Users/trompette/documents/GitHub/max-sdk/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build
  make -f /Users/trompette/documents/GitHub/max-sdk/build/CMakeScripts/ReRunCMake.make
fi

