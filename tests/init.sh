#!/bin/bash

function initialize(){
  mkdir -p sample/mnt
  dd if=/dev/zero of=sample/fat.img bs=1K count=100
  mkfs.vfat sample/fat.img
  sudo mount -t vfat -o loop sample/fat.img sample/mnt
}

function cleanup(){
  sudo umount sample/mnt
}
