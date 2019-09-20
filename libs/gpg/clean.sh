#!/bin/bash
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

if [ ! -z "$1" ] && [ -d  $SCRIPTPATH/../../build/$1/gpg ]; then
    rm -rf $SCRIPTPATH/../../build/$1/gpg
fi

git submodule update --init --recursive

for LIB in $SCRIPTPATH/*/
do
    echo $LIB
    cd $LIB && git clean -xdf && git reset --hard HEAD
done
