#!/bin/bash

. tests/init.sh
initialize
mkdir sample/mnt12/dir1
touch sample/mnt12/file1

mkdir sample/mnt16/dir1
touch sample/mnt16/file1

mkdir sample/mnt32/dir1
touch sample/mnt32/file1
cleanup
./fatracer sample/fat12.img
./fatracer sample/fat16.img
./fatracer sample/fat32.img
