# FAT-tracer
[![Build Status](https://travis-ci.org/LeavaTail/fat-tracer.svg?branch=master)](https://travis-ci.org/LeavaTail/fat-tracer)
[![codecov](https://codecov.io/gh/LeavaTail/fat-tracer/branch/master/graph/badge.svg)](https://codecov.io/gh/LeavaTail/fat-tracer)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/2eb72d5997cd4a5585c4fb163859ad23)](https://www.codacy.com/manual/LeavaTail/fat-tracer?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=LeavaTail/fat-tracer&amp;utm_campaign=Badge_Grade)

`FAT tracer` prints the file system information present on device.

## Description
***DEMO:***

```
 $ ./fatracer sample/fat12.img
BootStrap instruction       : eb 3c 90
OEM Name                    : mkfs.fat
Bytes per Sector            : 512
Sectors per cluster         : 4
Reserved Sector             : 1
FAT count                   : 2
Root Directory entry count  : 512
Sector count in Volume      : 200
Media                       : 248
Sector count in FAT         : 1
Sector count in Track       : 32
Head count                  : 64
Hidden sector count         : 0
Sector count in volume      : 0
BootStrap                   : 80
Reserved                    : 0
Boot Signature              : 29
Volume ID                   : 2a1b56f9
Volume Label                : NO NAME
FileSystem Type             : FAT12
BootStrap(Dep systems)      : ...[|.".t.V.......^..2.......This is not a bootable disk.  Please insert a bootable floppy and..press any key to try again ... .................................................................................................................................................................................................................................................................................................................................
Boot Signature              : 55aa
Fat Table Sector            : 00000200 - 000005ff
Root Directory Sector       : 00000600 - 000045ff
Data Directory Sector       : 00004600 - 00018fff

/:
FileName                    : DIRECTOR
File Attribute              : 10
Smaller information         : 0
Create Time (ms)            : 2019-10-14 07:24:20.18
Access Time (ms)            : 2019-10-14 00:00:00.00
Modify Time (ms)            : 2019-10-14 07:24:20.00
First Sector                : 00 03
File size                   : 0

FileName                    : FILE1234TXT
File Attribute              : 20
Smaller information         : 0
Create Time (ms)            : 2019-10-14 07:24:29.41
Access Time (ms)            : 2019-10-14 00:00:00.00
Modify Time (ms)            : 2019-10-14 07:24:28.00
First Sector                : 00 00
File size                   : 0
```

## Requirement

- Autoconf  <http://www.gnu.org/software/autoconf/>
- Automake  <http://www.gnu.org/software/automake/>
- Git       <http://git.or.cz/>

## Installation

1. Generate configure file. `./script/bootstrap.sh`
2. Configure the package for your system. `./configure`
3. Compile the package. `make`
4. Install the program. `make install`

## Authors

[LeavaTail](https://github.com/LeavaTail)

## License

[MIT](./LICENSE)
