#!/bin/sh
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

cd $SCRIPTPATH && rm -rf build
git submodule update --init --recursive

for LIB in "gnupg" "gpgme"  "libassuan" "libgpg-error"
do
    echo $LIB
	cd $SCRIPTPATH/$LIB && git clean -xdf && git reset --hard HEAD
done

rm -rf $SCRIPTPATH/../local
