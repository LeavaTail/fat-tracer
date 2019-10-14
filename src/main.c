/*
 * main.c
 *
 * FAT tracer interface
 *
 * MIT License
 *
 * Copyright (c) 2019 LeavaTail
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <config.h>
#include <getopt.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fat.h"

/**
 * Special Option(no short option)
 */
enum
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

/* option data {"long name", needs argument, flags, "short name"} */
static struct option const longopts[] =
{
  {"help",no_argument, NULL, GETOPT_HELP_CHAR},
  {"version",no_argument, NULL, GETOPT_VERSION_CHAR},
  {0,0,0,0}
};

/**
 * usage - print out usage.
 * @status: Status code
 */
void usage(int status)
{
  FILE *out;
  switch (status) {
    case CMDLINE_FAILURE:
      out = stderr;
      break;
    default:
      out = stdout;
  }
  fprintf(out, "Usage: %s [OPTION]... [FILE]\n",
      PROGRAM_NAME);

  exit(status);
}

/**
 * version - print out program version.
 * @command_name: command name
 * @version:      program version
 * @author:       program authoer
 */
void version(const char *command_name, const char *version,
    const char *author)
{
  FILE *out = stdout;

  fprintf(out, "%s %s\n", command_name, version);
  fprintf(out, "Written by %s.\n", author);
}

int fat_dump_reservedinfo(struct fat_reserved_info *info, FILE *out)
{
  fprintf(out, "%-28s: %x %x %x\n", "BootStrap instruction",info->BS_JmpBoot[0], info->BS_JmpBoot[1], info->BS_JmpBoot[2]);
  fprintf(out, "%-28s: %s\n", "OEM Name", info->BS_ORMName);
  fprintf(out, "%-28s: %u\n", "Bytes per Sector", info->BPB_BytesPerSec);
  fprintf(out, "%-28s: %u\n", "Sectors per cluster", info->BPB_SecPerClus);
  fprintf(out, "%-28s: %u\n", "Reserved Sector", info->BPB_RevdSecCnt);
  fprintf(out, "%-28s: %u\n", "FAT count", info->BPB_NumFATs);
  fprintf(out, "%-28s: %u\n", "Root Directory entry count", info->BPB_RootEntCnt);
  fprintf(out, "%-28s: %u\n", "Sector count in Volume", info->BPB_TotSec16);
  fprintf(out, "%-28s: %u\n", "Media", info->BPB_Media);
  fprintf(out, "%-28s: %u\n", "Sector count in FAT", info->BPB_FATSz16);
  fprintf(out, "%-28s: %u\n", "Sector count in Track", info->BPB_SecPerTrk);
  fprintf(out, "%-28s: %u\n", "Head count", info->BPB_NumHeads);
  fprintf(out, "%-28s: %u\n", "Hidden sector count", info->BPB_HiddSec);
  fprintf(out, "%-28s: %u\n", "Sector count in volume", info->BPB_TotSec32);
}

int fat_load_reservedinfo(struct fat_reserved_info *info, char *buf)
{
  size_t offset = 0;

  __memcpy(&(info->BS_JmpBoot), buf, &offset, JmpBootSIZE);
  __memcpy(&(info->BS_ORMName), buf, &offset, ORMNameSIZE);
  __memcpy(&(info->BPB_BytesPerSec), buf, &offset, BytesPerSecSIZE);
  __memcpy(&(info->BPB_SecPerClus), buf, &offset, SecPerClusSIZE);
  __memcpy(&(info->BPB_RevdSecCnt), buf, &offset, RevdSecCntSIZE);
  __memcpy(&(info->BPB_NumFATs), buf, &offset, NumFATsSIZE);
  __memcpy(&(info->BPB_RootEntCnt), buf, &offset, RootEntCntSIZE);
  __memcpy(&(info->BPB_TotSec16), buf, &offset, TotSec16SIZE);
  __memcpy(&(info->BPB_Media), buf, &offset, MediaSIZE);
  __memcpy(&(info->BPB_FATSz16), buf, &offset, FATSz16SIZE);
  __memcpy(&(info->BPB_SecPerTrk), buf, &offset, SecPerTrkSIZE);
  __memcpy(&(info->BPB_NumHeads), buf, &offset, NumHeadsSIZE);
  __memcpy(&(info->BPB_HiddSec), buf, &offset, HiddSecSIZE);
  __memcpy(&(info->BPB_TotSec32), buf, &offset, TotSec32SIZE);

  return offset;
}

/**
 * read_file - read file to output Hexadecimal.
 *
 * Return: 0 - success
 *         otherwise - error(show ERROR STATUS CODE)
 */
int read_file(const char *path)
{
  int err;
  unsigned char resv_area[RESVAREA_SIZE + 1];
  FILE *fin;
  FILE *fout = stdout;
  size_t count;
  size_t offset = 0;
  struct fat_reserved_info resv_info = {0};

  if ((fin = fopen(path, "rb")) == NULL) {
    perror("file open error");
    err = EXIT_FAILURE;
    goto out;
  }

  count = fread(resv_area, sizeof(resv_area[0]), sizeof(resv_area), fin );
  offset = fat_load_reservedinfo(&resv_info, resv_area);
  fat_dump_reservedinfo(&resv_info, fout);

  if (is_fat32format(&resv_info)) {
    fat32_load_reservedinfo(&resv_info, resv_area, offset);
  } else {
    fat12_load_reservedinfo(&resv_info, resv_area, offset);
    fat12_dump_reservedinfo(&resv_info, fout);
  }

fin_end:
  fclose(fin);
out:
  return err;
}

/**
 * main - make a hexdump or do the reverse
 * @argc: count of arguments
 * @argv: command line argument vector
 */
int main(int argc, char *argv[])
{
  int opt;
  int longindex;
  int n_files;
  bool infile = false;

  /**
   * Initialize Phase.
   * parse option, argument. set flags.
   */
  while ((opt = getopt_long(argc, argv,
          "o:",
          longopts, &longindex)) != -1) {
    switch (opt) {
      case 'o':
      case GETOPT_HELP_CHAR:
        usage(EXIT_SUCCESS);
        break;
      case GETOPT_VERSION_CHAR:
        version(PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_AUTHOR);
        exit(EXIT_SUCCESS);
        break;
      default:
        usage(CMDLINE_FAILURE);
    }
  }

  n_files = argc - optind;
  if (!n_files) {
    usage(CMDLINE_FAILURE);
    exit(EXIT_FAILURE);
  }

  read_file(argv[optind]);
  return 0;
}
