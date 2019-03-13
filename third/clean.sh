#!/bin/sh

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

cd $SCRIPTPATH/gpgme
git clean -xdf
git reset --hard HEAD

cd $SCRIPTPATH/libassuan
git clean -xdf
git reset --hard HEAD

cd $SCRIPTPATH/libgpg-error
git clean -xdf
git reset --hard HEAD

rm -rf $SCRIPTPATH/../local
