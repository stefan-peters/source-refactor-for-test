#!/bin/bash

s=`dirname "$0"`

for p in "$s/../lib" "$s/../bindings"; do 
    echo "formating cpp in $p"
	find "$p" -name '*.cpp' | xargs clang-format -i -style llvm
done;