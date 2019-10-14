#!/bin/bash

. tests/init.sh
initialize
mkdir sample/mnt/dir1
touch sample/mnt/file1
cleanup
./fatracer sample/fat.img
