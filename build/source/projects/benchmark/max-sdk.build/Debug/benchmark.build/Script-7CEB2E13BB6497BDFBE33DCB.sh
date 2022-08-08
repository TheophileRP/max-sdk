#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build/source/projects/benchmark
  cp /Users/trompette/documents/GitHub/max-sdk/source/max-sdk-base/script/PkgInfo /Users/trompette/documents/GitHub/max-sdk/source/projects/benchmark/../../../externals/benchmark.mxo/Contents/PkgInfo
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build/source/projects/benchmark
  cp /Users/trompette/documents/GitHub/max-sdk/source/max-sdk-base/script/PkgInfo /Users/trompette/documents/GitHub/max-sdk/source/projects/benchmark/../../../externals/benchmark.mxo/Contents/PkgInfo
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build/source/projects/benchmark
  cp /Users/trompette/documents/GitHub/max-sdk/source/max-sdk-base/script/PkgInfo /Users/trompette/documents/GitHub/max-sdk/source/projects/benchmark/../../../externals/benchmark.mxo/Contents/PkgInfo
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/trompette/documents/GitHub/max-sdk/build/source/projects/benchmark
  cp /Users/trompette/documents/GitHub/max-sdk/source/max-sdk-base/script/PkgInfo /Users/trompette/documents/GitHub/max-sdk/source/projects/benchmark/../../../externals/benchmark.mxo/Contents/PkgInfo
fi

