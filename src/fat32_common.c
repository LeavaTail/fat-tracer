/*
 * fat32_common.c
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

bool is_fat32format(struct fat_reserved_info *info)
{
  return !info->BPB_FATSz16;
}

void fat32_dump_reservedinfo(struct fat_reserved_info *info, FILE *out)
{
  unsigned char ret[RESVAREA_SIZE + 1] = {0};
  struct fat32_reserved_info *fat32_info = (struct fat32_reserved_info *)(info->reserved1);

  fprintf(out, "%-28s\t: %x\n", _("Sectors Per FAT table"), fat32_info->BPB_FATSz32);
  fprintf(out, "%-28s\t: %x%x\n", _("Active flags"), fat32_info->BPB_ExtFlags[0], fat32_info->BPB_ExtFlags[1]);
  fprintf(out, "%-28s\t: %x %x\n", _("FAT32 Volume version"), fat32_info->BPB_FSVer[0], fat32_info->BPB_FSVer[1]);
  fprintf(out, "%-28s\t: %x\n", _("Root Directory First sector"), fat32_info->BPB_RootClus);
  fprintf(out, "%-28s\t: %x\n", _("FSINFO sector"), fat32_info->BPB_FSInfo);
  fprintf(out, "%-28s\t: %x\n", _("Backup Boot sector"), fat32_info->BPB_BkBootSec);
  fprintf(out, "%-28s\t: %x\n", _("BootStrap"), fat32_info->BS_DrvNum);
  fprintf(out, "%-28s\t: %s\n", _("Reserved(FAT32)"), setcharc(fat32_info->BPB_Reserved, ret, ReservedSIZE));
  fprintf(out, "%-28s\t: %x\n", _("BootStrap"), fat32_info->BS_DrvNum);
  fprintf(out, "%-28s\t: %x\n", _("Reserved"),  fat32_info->BS_Reserved1);
  fprintf(out, "%-28s\t: %x\n", _("Boot Signature"),  fat32_info->BS_BootSig);
  fprintf(out, "%-28s\t: %s\n", _("Volume ID"), setcharx(fat32_info->BS_VolID, ret, VolIDSIZE));
  fprintf(out, "%-28s\t: %s\n", _("Volume Label"), setcharc(fat32_info->BS_VolLab, ret, VolLabSIZE));
  fprintf(out, "%-28s\t: %s\n", _("FileSystem Type"), setcharc(fat32_info->BS_FilSysType, ret, FilSysTypeSIZE));
  fprintf(out, "%-28s\t: %s\n", _("BootStrap(Dep systems)"),  setcharc(fat32_info->BS_BootCode32, ret, BootCode32SIZE));
  fprintf(out, "%-28s\t: %x%x\n", _("Boot Signature"),  fat32_info->BS_BootSign[0],
      fat32_info->BS_BootSign[1]);
}

int fat32_load_reservedinfo(struct fat_reserved_info *info, unsigned char *buf, size_t offset)
{
  struct fat32_reserved_info *fat32_info = (struct fat32_reserved_info *)(info->reserved1);
  __memcpy(&(fat32_info->BPB_FATSz32), buf, &offset, FATSz32SIZE);
  __memcpy(&(fat32_info->BPB_ExtFlags), buf, &offset, ExtFlagsSIZE);
  __memcpy(&(fat32_info->BPB_FSVer), buf, &offset, FSVerSIZE);
  __memcpy(&(fat32_info->BPB_RootClus), buf, &offset, RootClusSIZE);
  __memcpy(&(fat32_info->BPB_FSInfo), buf, &offset, FSInfoSIZE);
  __memcpy(&(fat32_info->BPB_BkBootSec), buf, &offset, BkBootSecSIZE);
  __memcpy(&(fat32_info->BPB_Reserved), buf, &offset, ReservedSIZE);
  __memcpy(&(fat32_info->BS_DrvNum), buf, &offset, DrvNumSIZE);
  __memcpy(&(fat32_info->BS_Reserved1), buf, &offset, Reserved1SIZE);
  __memcpy(&(fat32_info->BS_BootSig), buf, &offset, BootSigSIZE);
  __memcpy(&(fat32_info->BS_VolID), buf, &offset, VolIDSIZE);
  __memcpy(&(fat32_info->BS_VolLab), buf, &offset, VolLabSIZE);
  __memcpy(&(fat32_info->BS_FilSysType), buf, &offset, FilSysTypeSIZE);
  __memcpy(&(fat32_info->BS_BootCode32), buf, &offset, BootCode32SIZE);
  __memcpy(&(fat32_info->BS_BootSign), buf, &offset, BootSignSIZE);

  return offset;
}

void fat32_dump_fsinfo(struct fat32_fsinfo *info, FILE *out)
{
  unsigned char ret[RESVAREA_SIZE + 1] = {0};

  fprintf(out, "\n%s\n", _("FSINFO"));

  fprintf(out, "%-28s\t: %x\n", _("signature1"), info->FSI_LeadSig);
  fprintf(out, "%-28s\t: %s\n", _("Reserved1"), setcharx(info->FSI_Reserved1, ret, FSI_Reserved1SIZE));
  fprintf(out, "%-28s\t: %x\n", _("signature2"), info->FSI_StrucSig);
  fprintf(out, "%-28s\t: %x\n", _("Free cluster count"), info->FSI_Free_Count);
  fprintf(out, "%-28s\t: %x\n", _("last allocated cluster"), info->FSI_Nxt_Free);
  fprintf(out, "%-28s\t: %s\n", _("Reserved2"), setcharx(info->FSI_Reserved2, ret, FSI_Reserved2SIZE));
  fprintf(out, "%-28s\t: %x\n\n", _("signature3"), info->FSI_TrailSig);
}

int fat32_load_fsinfo(struct fat32_fsinfo *info, unsigned char *buf)
{
  size_t offset = 0;
  __memcpy(&(info->FSI_LeadSig), buf, &offset, FSI_LeadSigSIZE);
  __memcpy(&(info->FSI_Reserved1), buf, &offset, FSI_Reserved1SIZE);
  __memcpy(&(info->FSI_StrucSig), buf, &offset, FSI_StrucSigSIZE);
  __memcpy(&(info->FSI_Free_Count), buf, &offset, FSI_Free_CountSIZE);
  __memcpy(&(info->FSI_Nxt_Free), buf, &offset, FSI_Nxt_FreeSIZE);
  __memcpy(&(info->FSI_Reserved2), buf, &offset, FSI_Reserved2SIZE);
  __memcpy(&(info->FSI_TrailSig), buf, &offset, FSI_TrailSigSIZE);

  return offset;
}
