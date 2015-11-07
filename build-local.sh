#!/bin/bash

mkdir bin
cd bin
rm -rf *
qmake ../BubbleKiller/BubbleKiller.pro
make
cd release
./BubbleKiller
