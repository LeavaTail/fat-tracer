# FAT-tracer
[![Build Status](https://travis-ci.org/LeavaTail/fat-tracer.svg?branch=master)](https://travis-ci.org/LeavaTail/fat-tracer)
[![codecov](https://codecov.io/gh/LeavaTail/fat-tracer/branch/master/graph/badge.svg)](https://codecov.io/gh/LeavaTail/fat-tracer)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/2eb72d5997cd4a5585c4fb163859ad23)](https://www.codacy.com/manual/LeavaTail/fat-tracer?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=LeavaTail/fat-tracer&amp;utm_campaign=Badge_Grade)

`FAT tracer` print out a hexdump or do the reverse that referenced `xxd`.

## Description
`phex` creates a hex dump of a given file or standard input.
It can also convert a hex dump back to its original binary form.
Like `uuencode`(1) and `uudecode`(1) it allows the transmission of binary data in a 'mail-safe' ASCII representation, but has the advantage of decoding to standard output.

***DEMO:***
```
  $ phex AUTHORS
00000000:4c65 6176 6154 6169 6c20 3c73 7461 626f LeavaTail.<stabo
00000010:772e 6475 7374 6572 4067 6d61 696c 2e63 w.duster@gmail.c
00000020:6f6d 3e0a 0000 0000 0000 0000 0000 0000 om>.
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
