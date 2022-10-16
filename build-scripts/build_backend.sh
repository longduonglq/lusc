#!/bin/sh

CMAKE=/opt/homebrew/Cellar/cmake/3.23.2/bin/cmake

echo "Going into directory $1"
cd $1
mkdir -p build
cd build
$CMAKE $2 ..
make

cp libluscbackend.a ../../
rm -f libluscbackend.a