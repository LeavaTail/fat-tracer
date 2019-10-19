#!/bin/bash

./fatracer --help
if [ $? -gt 0 ]; then
  exit 1;
fi

./fatracer --version
if [ $? -gt 0 ]; then
  exit 1;
fi

./fatracer --none
if [ $? -eq 0 ]; then
  exit 1;
fi

exit 0;
