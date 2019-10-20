/*
 * fat12_common.c
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

void fat12_dump_reservedinfo(struct fat_reserved_info *info, FILE *out)
{
  unsigned char ret[RESVAREA_SIZE + 1] = {0};
  struct fat12_reserved_info *fat12_info = (struct fat12_reserved_info *)(info->reserved1);

  fprintf(out, "%-28s\t: %x\n", _("BootStrap"), fat12_info->BS_DrvNum);
  fprintf(out, "%-28s\t: %x\n", _("Reserved"),  fat12_info->BS_Reserved1);
  fprintf(out, "%-28s\t: %x\n", _("Boot Signature"),  fat12_info->BS_BootSig);
  fprintf(out, "%-28s\t: %s\n", _("Volume ID"), setcharx(fat12_info->BS_VolID, ret, VolIDSIZE));
  fprintf(out, "%-28s\t: %s\n", _("Volume Label"), setcharc(fat12_info->BS_VolLab, ret, VolLabSIZE));
  fprintf(out, "%-28s\t: %s\n", _("FileSystem Type"), setcharc(fat12_info->BS_FilSysType, ret, FilSysTypeSIZE));
  fprintf(out, "%-28s\t: %s\n", _("BootStrap(Dep systems)"),  setcharc(fat12_info->BS_BootCode, ret, BootCodeSIZE));
  fprintf(out, "%-28s\t: %x%x\n", _("Boot Signature"),  fat12_info->BS_BootSign[0],
      fat12_info->BS_BootSign[1]);
}

int fat12_load_reservedinfo(struct fat_reserved_info *info, unsigned char *buf, size_t offset)
{
  struct fat12_reserved_info *fat12_info = (struct fat12_reserved_info *)(info->reserved1);
  __memcpy(&(fat12_info->BS_DrvNum), buf, &offset, DrvNumSIZE);
  __memcpy(&(fat12_info->BS_Reserved1), buf, &offset, Reserved1SIZE);
  __memcpy(&(fat12_info->BS_BootSig), buf, &offset, BootSigSIZE);
  __memcpy(&(fat12_info->BS_VolID), buf, &offset, VolIDSIZE);
  __memcpy(&(fat12_info->BS_VolLab), buf, &offset, VolLabSIZE);
  __memcpy(&(fat12_info->BS_FilSysType), buf, &offset, FilSysTypeSIZE);
  __memcpy(&(fat12_info->BS_BootCode), buf, &offset, BootCodeSIZE);
  __memcpy(&(fat12_info->BS_BootSign), buf, &offset, BootSignSIZE);

  return offset;
}
