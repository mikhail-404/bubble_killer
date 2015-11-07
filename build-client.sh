#!/bin/bash

mkdir client-bin
cd client-bin
rm -rf *
qmake ../client/client.pro
make

