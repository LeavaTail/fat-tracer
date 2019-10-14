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
#define DENTRY_SIZE 32
#define ATTR_ONELINE 36
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
  /* FAT12/16 */
  DrvNumSIZE = 1,
  Reserved1SIZE = 1,
  BootSigSIZE = 1,
  VolIDSIZE = 4,
  VolLabSIZE = 11,
  FilSysTypeSIZE = 8,
  BootCodeSIZE = 448,
  BootSignSIZE = 2,
  /* FAT32 */
  FATSz32SIZE = 4,
  ExtFlagsSIZE = 2,
  FSVerSIZE = 2,
  RootClusSIZE = 4,
  FSInfoSIZE = 2,
  BkBootSecSIZE = 2,
  ReservedSIZE = 12,
  BootCode32SIZE = 420,
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

struct fat32_reserved_info {
  u_int32_t BPB_FATSz32;
  unsigned char BPB_ExtFlags[ExtFlagsSIZE];
  unsigned char BPB_FSVer[FSVerSIZE];
  u_int32_t BPB_RootClus;
  u_int16_t BPB_FSInfo;
  u_int16_t BPB_BkBootSec;
  unsigned char BPB_Reserved[ReservedSIZE];
  unsigned char BS_DrvNum;
  u_int8_t  BS_Reserved1;
  unsigned char BS_BootSig;
  unsigned char BS_VolID[VolIDSIZE];
  unsigned char BS_VolLab[VolLabSIZE];
  unsigned char BS_FilSysType[FilSysTypeSIZE];
  unsigned char BS_BootCode32[BootCode32SIZE];
  unsigned char BS_BootSign[BootSignSIZE];
};

enum Fat12Entry
{
  FAT12_UNUSED = 0x000,
  FAT12_RESERVED = 0x001,
  FAT12_BADCLUSTER = 0xFF7,
  FAT12_DATAEND = 0xFF8,
};

enum Fat16Entry
{
  FAT16_UNUSED = 0x0000,
  FAT16_RESERVED = 0x0001,
  FAT16_BADCLUSTER = 0xFFF7,
  FAT16_DATAEND = 0xFFF8,
};

enum Fat32Entry
{
  FAT32_UNUSED = 0x00000000,
  FAT32_RESERVED = 0x00000001,
  FAT32_BADCLUSTER = 0xFFFFFFF7,
  FAT32_DATAEND = 0xFFFFFFF8,
};

enum {
  NameSIZE  = 11,
  AttrSIZE = 1,
  NTResSIZE = 1,
  CrtTimeTenthSIZE = 1,
  CrtTimeSIZE = 2,
  CrtDateSIZE = 2,
  LstAccDateSIZE = 2,
  FstClusHISIZE = 2,
  WrtTimeSIZE = 2,
  WrtDateSIZE = 2,
  FstClusLOSIZE = 2,
  FileSizeSIZE = 4,
};

struct fat_dentry {
  unsigned char IR_Name[NameSIZE];
  unsigned char DIR_Attr;
  unsigned char DIR_NTRes;
  unsigned char DIR_CrtTimeTenth;
  u_int16_t DIR_CrtTime;
  u_int16_t DIR_CrtDate;
  u_int16_t DIR_LstAccDate;
  u_int16_t DIR_FstClusHI;
  u_int16_t DIR_WrtTime;
  u_int16_t DIR_WrtDate;
  u_int16_t DIR_FstClusLO;
  u_int32_t DIR_FileSize;
};

/**
 * Time format
 * 15                    0
 * -----------------------
 * | HHHHHMMM | MMMSSSSS |
 * -----------------------
 *
 * Date format
 * 15                    0
 * -----------------------
 * | YYYYYYYM | MMMDDDDD |
 * -----------------------
 */
enum {
  HOURMASK = 0xf800,
  MINMASK = 0x07e0,
  SECMASK  = 0x001f,
  YEARMASK = 0xef00,
  MONTHMASK = 0x01e0,
  DAYMASK = 0x001f,

  YEARSHIFT = 9,
  MONTHSHIFT = 5,
  HOURSHIFT = 11,
  MINSHIFT = 5,
};

enum {
  ATTR_READ_ONLY = 0x01,
  ATTR_HIDDEN = 0x02,
  ATTR_SYSTEM = 0x04,
  ATTR_VOLUME_ID = 0x08,
  ATTR_DIRECTORY = 0x10,
  ATTR_ARCHIVE = 0x20,
  ATTR_LONG_FILE_NAME = 0x0f,
};

/**
 * FAT12 structure
 */
int fat12_dump_reservedinfo(struct fat_reserved_info *, FILE *);
int fat12_load_reservedinfo(struct fat_reserved_info *, char *, size_t);

/**
 * FAT16 structure
 */
int fat16_dump_fattable(void *);
int fat16_load_fattable(void *);

/**
 * FAT32 structure
 */
bool is_fat32format(struct fat_reserved_info *);
int fat32_dump_reservedinfo(struct fat_reserved_info *, FILE *);
int fat32_load_reservedinfo(struct fat_reserved_info *, char *, size_t);

#endif /*_FAT12_H */
