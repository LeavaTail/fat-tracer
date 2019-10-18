#!/bin/bash

. tests/init.sh
initialize
mkdir sample/mnt12/DIR1
touch sample/mnt12/FILE1

mkdir sample/mnt16/DIR1
touch sample/mnt16/FILE1

mkdir sample/mnt32/DIR1
touch sample/mnt32/FILE1
cleanup

./fatracer sample/fat12.img
if [ $? -gt 0 ]; then
  exit 1;
fi

./fatracer sample/fat16.img
if [ $? -gt 0 ]; then
  exit 2;
fi

./fatracer sample/fat32.img
if [ $? -gt 0 ]; then
  exit 3;
fi
