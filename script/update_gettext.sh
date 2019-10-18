#!/bin/bash

wget https://ftp.gnu.org/gnu/gettext/gettext-0.19.8.tar.gz > /dev/null 2>&1
tar xf gettext-0.19.8.tar.gz > /dev/null 2>&1
cd gettext-0.19.8
./configure > /dev/null 2>&1
make > /dev/null 2>&1
sudo make install > /dev/null 2>&1
