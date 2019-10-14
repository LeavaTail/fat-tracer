#ifndef _FAT12_H
#define _FAT12_H

/**
 * Program Name, version, author.
 * displayed when 'usage' and 'version'
 */
#define PROGRAM_NAME	"fatracer"
#define PROGRAM_VERSION	"0.1"
#define PROGRAM_AUTHOR	"LeavaTail"
#define COPYRIGHT_YEAR	"2019"
/**
 * Debug code
 */
#ifdef FATRACER_DEBUG
#define fatracer_debug(fmt, ...)						\
	do {								\
		fprintf( stderr, "(%s: %u): %s:" fmt, \
				__FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)
#else
#define fatracer_debug(fmt, ...)	do { } while (0)
#endif

/**
 * ERROR STATUS CODE
 *  1: invalid command-line option
 */
enum
{
	CMDLINE_FAILURE = 1
};

/**
 * FAT TYPE (12/16/32)
 *  12: FAT12
 *  16: FAT16
 *  32: FAT32
 */
enum FStype
{
	FAT12_FILESYSTEM = 12,
  FAT16_FILESYSTEM = 16,
  FAT32_FILESYSTEM = 32,
};

static inline char *__memcpy(void *dist, const char *src, size_t *offset, size_t n)
{
  size_t o = *offset;
  memcpy(dist, src + o, n);
  *offset += n;
}

static inline char *setcharc(unsigned const char* buf, unsigned char* ret, size_t len)
{
  int i;
  memset(ret, '\0', len);
  for (i = 0; i < len; i++) {
    if (buf[i] >= 0x20 && buf[i] <= 0x7e)
      sprintf(ret, "%s%c", ret, buf[i]);
    else
      strcat(ret, ".");
  }
  return ret;
}

static inline char *setcharx(unsigned const char* buf, unsigned char* ret, size_t len)
{
  int i;
  for (i = 0; i < len; i++) {
    sprintf(ret, "%s%x", ret, buf[i]);
  }
  return ret;
}


/**
 * FAT DEFINATION
 */
#define RESVAREA_SIZE 512
#define FAT16_CLUSTERS 4096
#define FAT32_CLUSTERS 65526

enum {
  JmpBootSIZE  = 3,
  ORMNameSIZE  = 8,
  BytesPerSecSIZE = 2,
  SecPerClusSIZE = 1,
  RevdSecCntSIZE = 2,
  NumFATsSIZE = 1,
  RootEntCntSIZE = 2,
  TotSec16SIZE = 2,
  MediaSIZE = 1,
  FATSz16SIZE = 2,
  SecPerTrkSIZE = 2,
  NumHeadsSIZE = 2,
  HiddSecSIZE = 4,
  TotSec32SIZE = 4,
  DrvNumSIZE = 1,
  Reserved1SIZE = 1,
  BootSigSIZE = 1,
  VolIDSIZE = 4,
  VolLabSIZE = 11,
  FilSysTypeSIZE = 8,
  BootCodeSIZE = 448,
  BootSignSIZE = 2,
};

struct fat_reserved_info {
  unsigned char BS_JmpBoot[JmpBootSIZE];
  unsigned char BS_ORMName[ORMNameSIZE];
  u_int16_t BPB_BytesPerSec;
  u_int8_t  BPB_SecPerClus;
  u_int16_t BPB_RevdSecCnt;
  u_int8_t  BPB_NumFATs;
  u_int16_t BPB_RootEntCnt;
  u_int16_t BPB_TotSec16;
  u_int8_t  BPB_Media;
  u_int16_t BPB_FATSz16;
  u_int16_t BPB_SecPerTrk;
  u_int16_t BPB_NumHeads;
  u_int32_t BPB_HiddSec;
  u_int32_t BPB_TotSec32;
  unsigned char reserved1[476];
};

struct fat12_reserved_info {
  unsigned char BS_DrvNum;
  u_int8_t  BS_Reserved1;
  unsigned char BS_BootSig;
  unsigned char BS_VolID[VolIDSIZE];
  unsigned char BS_VolLab[VolLabSIZE];
  unsigned char BS_FilSysType[FilSysTypeSIZE];
  unsigned char BS_BootCode[BootCodeSIZE];
  unsigned char BS_BootSign[BootSignSIZE];
};

/**
 * FAT12 structure
 */
int fat12_dump_reservedinfo(struct fat_reserved_info *, FILE *);
int fat12_load_reservedinfo(struct fat_reserved_info *, char *, size_t);

/**
 * FAT32 structure
 */
bool is_fat32format(struct fat_reserved_info *);
int fat32_dump_reservedinfo(struct fat_reserved_info *, FILE *);
int fat32_load_reservedinfo(struct fat_reserved_info *, char *, size_t);

#endif /*_FAT12_H */
