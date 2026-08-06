#ifndef _SLATE_TAR_H
#define _SLATE_TAR_H

enum {
  TOEXEC  = 00001,
  TOWRITE = 00002,
  TOREAD  = 00004,
  TGEXEC  = 00010,
  TGWRITE = 00020,
  TGREAD  = 00040,
  TUEXEC  = 00100,
  TUWRITE = 00200,
  TUREAD  = 00400,
  TSVTX   = 01000,
  TSGID   = 02000,
  TSUID   = 04000,
};

#define REGTYPE  '0'
#define AREGTYPE '\0'
#define LNKTYPE  '1'
#define SYMTYPE  '2'
#define CHRTYPE  '3'
#define BLKTYPE  '4'
#define DIRTYPE  '5'
#define FIFOTYPE '6'
#define CONTTYPE '7'

#define TMAGIC  "ustar"
#define TMAGLEN 6

#define TVERSION "00"
#define TVERSLEN 2

#endif
