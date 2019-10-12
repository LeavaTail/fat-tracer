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

static inline char *__memcpy(void *dist, const char *src, size_t *offset, size_t n)
{
  size_t o = *offset;
  memcpy(dist, src + o, n);
  *offset += n;
}

/**
 * FAT12 DEFINATION
 */
#define RESVAREA_SIZE 512

/**
 * FAT12 structure
 */
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
};

struct fat12_reserved_info {
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
};

#endif /*_FAT12_H */
