#!/bin/bash

function initialize(){
  mkdir -p sample/mnt12 sample/mnt16 sample/mnt32
  dd if=/dev/zero of=sample/fat12.img bs=1K count=1000
  dd if=/dev/zero of=sample/fat16.img bs=1K count=10000
  dd if=/dev/zero of=sample/fat32.img bs=1K count=100000
  mkfs.vfat sample/fat12.img
  mkfs.vfat sample/fat16.img
  mkfs.vfat sample/fat32.img
  sudo mount -t vfat -o loop,uid=$(id -u),gid=$(id -g) sample/fat12.img sample/mnt12
  sudo mount -t vfat -o loop,uid=$(id -u),gid=$(id -g) sample/fat16.img sample/mnt16
  sudo mount -t vfat -o loop,uid=$(id -u),gid=$(id -g) sample/fat32.img sample/mnt32
}

function cleanup(){
  sudo umount sample/mnt12
  sudo umount sample/mnt16
  sudo umount sample/mnt32
}
