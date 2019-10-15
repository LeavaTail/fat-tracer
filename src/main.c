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
#include <time.h>
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

int fat_dateformat(struct tm *t, u_int16_t date)
{
  unsigned char y;
  unsigned char m;
  unsigned char d;

  y = (date & YEARMASK) >> YEARSHIFT;
  m = (date & MONTHMASK) >> MONTHSHIFT;
  d = (date & DAYMASK);

  t->tm_year = y;
  t->tm_mon = m;
  t->tm_mday = d;
}

int fat_timeformat(struct tm *t, u_int16_t time)
{
  unsigned char h;
  unsigned char m;
  unsigned char s;

  h = (time & HOURMASK) >> HOURSHIFT;
  m = (time & MINMASK) >> MINSHIFT;
  s = (time & SECMASK);

  t->tm_sec = s;
  t->tm_min = m;
  t->tm_hour = h;
}

int fat_attrformat(char *buf, unsigned char attr)
{
  if (attr & ATTR_LONG_FILE_NAME) {
    strcat(buf ,"LFN ");
    return 0;
  }

  if (attr & ATTR_READ_ONLY)
    strcat(buf ,"RO ");
  if (attr & ATTR_HIDDEN)
    strcat(buf ,"HIDDEN ");
  if (attr & ATTR_SYSTEM)
    strcat(buf ,"SYS ");
  if (attr & ATTR_VOLUME_ID)
    strcat(buf ,"VOLUME ");
  if (attr & ATTR_DIRECTORY)
    strcat(buf ,"DIR ");
  if (attr & ATTR_ARCHIVE)
    strcat(buf ,"ARCH ");
}

bool check_dentryfree(const char *buf)
{
  if (!buf)
    return true;
  if ((buf[0] == 0xe5) || (buf[0] == 0x00))
    return true;
  return false;
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

int fat_dump_dentry(struct fat_dentry *info, FILE *out)
{
  char attrbuf[ATTR_ONELINE] = {0};
  char ret[DENTRY_SIZE + 1] = {0};
  struct tm mtime, atime, ctime;
  u_int16_t msec = 0;

  fat_timeformat(&mtime, info->DIR_WrtTime);
  fat_dateformat(&mtime, info->DIR_WrtDate);
  fat_dateformat(&atime, info->DIR_LstAccDate);
  fat_timeformat(&ctime, info->DIR_CrtTime);
  fat_dateformat(&ctime, info->DIR_CrtDate);
  fat_attrformat(attrbuf, info->DIR_Attr);

  fprintf(out, "%-28s: %s\n", "FileName", setcharc(info->IR_Name, ret, NameSIZE));
  fprintf(out, "%-28s: %s\n", "File Attribute", attrbuf);
  fprintf(out, "%-28s: %x\n", "Smaller information", info->DIR_NTRes);
  msec = info->DIR_CrtTimeTenth;
  fprintf(out, "%-28s: %d-%02d-%02d %02d:%02d:%02d.%02d\n", "Create Time (ms)",
                1980 + ctime.tm_year, ctime.tm_mon, ctime.tm_mday,
                ctime.tm_hour, ctime.tm_min,
                (ctime.tm_sec * 2) + (msec / 100),
                msec % 100);
  fprintf(out, "%-28s: %d-%02d-%02d %02d:%02d:%02d.%02d\n", "Access Time (ms)",
                1980 + atime.tm_year, atime.tm_mon, atime.tm_mday,
                atime.tm_hour, atime.tm_min, atime.tm_sec * 2, 0);
  fprintf(out, "%-28s: %d-%02d-%02d %02d:%02d:%02d.%02d\n", "Modify Time (ms)",
                1980 + mtime.tm_year, mtime.tm_mon, mtime.tm_mday,
                mtime.tm_hour, mtime.tm_min, mtime.tm_sec * 2, 0);

  fprintf(out, "%-28s: %02x %02x\n", "First Sector", info->DIR_FstClusHI,
                                                 info->DIR_FstClusLO);
  fprintf(out, "%-28s: %x\n", "File size", info->DIR_FileSize);
}


int fat_load_dentry(struct fat_dentry *dentry, const void *buf)
{
  size_t offset = 0;

  __memcpy(&(dentry->IR_Name), buf, &offset, NameSIZE);
  __memcpy(&(dentry->DIR_Attr), buf, &offset, AttrSIZE);
  __memcpy(&(dentry->DIR_NTRes), buf, &offset, NTResSIZE);
  __memcpy(&(dentry->DIR_CrtTimeTenth), buf, &offset, CrtTimeTenthSIZE);
  __memcpy(&(dentry->DIR_CrtTime), buf, &offset, CrtTimeSIZE);
  __memcpy(&(dentry->DIR_CrtDate), buf, &offset, CrtDateSIZE);
  __memcpy(&(dentry->DIR_LstAccDate), buf, &offset, LstAccDateSIZE);
  __memcpy(&(dentry->DIR_FstClusHI), buf, &offset, FstClusHISIZE);
  __memcpy(&(dentry->DIR_WrtTime), buf, &offset, WrtTimeSIZE);
  __memcpy(&(dentry->DIR_WrtDate), buf, &offset, WrtDateSIZE);
  __memcpy(&(dentry->DIR_FstClusLO), buf, &offset, FstClusLOSIZE);
  __memcpy(&(dentry->DIR_FileSize), buf, &offset, FileSizeSIZE);

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
  int secv;
  unsigned char resv_area[RESVAREA_SIZE + 1];
  unsigned char *fat_area;
  unsigned char *root_area;
  unsigned char *data_area;
  FILE *fin;
  FILE *fout = stdout;
  size_t count;
  size_t offset = 0;
  enum FStype fstype;
  u_int16_t sector;
  u_int32_t secsPerFat;
  u_int32_t totSec;
  u_int32_t CountofClusters;
  u_int16_t FatStartSector;
  u_int32_t FatSectors;
  u_int32_t RootDirStartSector;
  u_int32_t RootDirSectors;
  u_int32_t DataStartSector;
  u_int32_t DataSectors;

  struct fat_reserved_info resv_info = {0};
  struct fat_dentry dentry = {0};

  if ((fin = fopen(path, "rb")) == NULL) {
    perror("file open error");
    err = EXIT_FAILURE;
    goto out;
  }

  count = fread(resv_area, sizeof(resv_area[0]), RESVAREA_SIZE, fin);
  offset = fat_load_reservedinfo(&resv_info, resv_area);
  fat_dump_reservedinfo(&resv_info, fout);
  sector = resv_info.BPB_BytesPerSec;

  if (is_fat32format(&resv_info)) {
    fat32_load_reservedinfo(&resv_info, resv_area, offset);
    fat32_dump_reservedinfo(&resv_info, fout);
    secsPerFat = ((struct fat32_reserved_info *)(resv_info.reserved1))->BPB_FATSz32;
    totSec = resv_info.BPB_TotSec32;
  } else {
    fat12_load_reservedinfo(&resv_info, resv_area, offset);
    fat12_dump_reservedinfo(&resv_info, fout);
    secsPerFat = resv_info.BPB_FATSz16;
    totSec = resv_info.BPB_TotSec16;
  }

  FatStartSector = resv_info.BPB_RevdSecCnt;
  FatSectors = secsPerFat * resv_info.BPB_NumFATs;
  RootDirStartSector = FatStartSector + FatSectors;
  RootDirSectors = (32 * resv_info.BPB_RootEntCnt + resv_info.BPB_BytesPerSec - 1)
                   / resv_info.BPB_BytesPerSec;
  DataStartSector = RootDirStartSector + RootDirSectors;
  DataSectors = totSec - DataStartSector;

  fprintf(fout, "%-28s: %08x - %08x\n", "Fat Table Sector", FatStartSector * sector,
          FatStartSector * sector +  FatSectors * sector - 1);
  fprintf(fout, "%-28s: %08x - %08x\n", "Root Directory Sector", RootDirStartSector * sector,
          RootDirStartSector * sector +  RootDirSectors * sector - 1);
  fprintf(fout, "%-28s: %08x - %08x\n", "Data Directory Sector", DataStartSector * sector,
          DataStartSector * sector +  DataSectors * sector - 1);

  CountofClusters = DataSectors / resv_info.BPB_SecPerClus;
  if (CountofClusters < FAT16_CLUSTERS)
    fstype = FAT12_FILESYSTEM;
  else if (CountofClusters < FAT32_CLUSTERS)
    fstype = FAT16_FILESYSTEM;
  else
    fstype = FAT32_FILESYSTEM;

  fat_area = malloc(FatSectors * sector);
  fseek(fin, FatStartSector * sector ,SEEK_SET);
  count = fread(fat_area, sizeof(fat_area[0]), FatSectors * sector, fin);
  free(fat_area);

  fprintf(fout, "\n%s:\n", "/");
  root_area = malloc(sizeof(struct fat_dentry) + 1);
  fseek(fin, RootDirStartSector * sector, SEEK_SET);
  for(secv = 0; secv < RootDirSectors * sector; secv += sizeof(struct fat_dentry)) {
    count = fread(root_area, sizeof(root_area[0]), sizeof(struct fat_dentry), fin);
    if (check_dentryfree(root_area))
      continue;
    fat_load_dentry(&dentry, root_area);
    fat_dump_dentry(&dentry, fout);
    putchar('\n');
  }
  free(root_area);


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
