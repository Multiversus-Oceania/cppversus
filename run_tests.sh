#!/bin/bash

source setup_env.sh
echo $MULTIVERSUS_TOKEN

meson setup builddir
cd builddir

meson test
ninja doc

cd ..
