#ifndef _SYS_MTIO_H
#define _SYS_MTIO_H

#include <sys/ioctl.h>
#include <sys/types.h>

struct mtop {
  short mt_op;
  int   mt_count;
};

#define _IOT_mtop         _IOT(_IOTS(short), 1, _IOTS(int), 1, 0, 0)
#define _IOT_mtget        _IOT(_IOTS(long), 7, 0, 0, 0, 0)
#define _IOT_mtpos        _IOT_SIMPLE(long)
#define _IOT_mtconfiginfo _IOT(_IOTS(long), 2, _IOTS(short), 3, _IOTS(long), 1)

#define MTRESET        0
#define MTFSF          1
#define MTBSF          2
#define MTFSR          3
#define MTBSR          4
#define MTWEOF         5
#define MTREW          6
#define MTOFFL         7
#define MTNOP          8
#define MTRETEN        9
#define MTBSFM         10
#define MTFSFM         11
#define MTEOM          12
#define MTERASE        13
#define MTRAS1         14
#define MTRAS2         15
#define MTRAS3         16
#define MTSETBLK       20
#define MTSETDENSITY   21
#define MTSEEK         22
#define MTTELL         23
#define MTSETDRVBUFFER 24
#define MTFSS          25
#define MTBSS          26
#define MTWSM          27
#define MTLOCK         28
#define MTUNLOCK       29
#define MTLOAD         30
#define MTUNLOAD       31
#define MTCOMPRESSION  32
#define MTSETPART      33
#define MTMKPART       34

struct mtget {
  long mt_type;
  long mt_resid;
  long mt_dsreg;
  long mt_gstat;
  long mt_erreg;
  int  mt_fileno;
  int  mt_blkno;
};

#define MT_ISUNKNOWN            0x01
#define MT_ISQIC02              0x02
#define MT_ISWT5150             0x03
#define MT_ISARCHIVE_5945L2     0x04
#define MT_ISCMSJ500            0x05
#define MT_ISTDC3610            0x06
#define MT_ISARCHIVE_VP60I      0x07
#define MT_ISARCHIVE_2150L      0x08
#define MT_ISARCHIVE_2060L      0x09
#define MT_ISARCHIVESC499       0x0A
#define MT_ISQIC02_ALL_FEATURES 0x0F
#define MT_ISWT5099EEN24        0x11
#define MT_ISTEAC_MT2ST         0x12
#define MT_ISEVEREX_FT40A       0x32
#define MT_ISDDS1               0x51
#define MT_ISDDS2               0x52
#define MT_ISSCSI1              0x71
#define MT_ISSCSI2              0x72
#define MT_ISFTAPE_UNKNOWN      0x800000
#define MT_ISFTAPE_FLAG         0x800000

struct mt_tape_info {
  long  t_type;
  char *t_name;
};

#define MT_TAPE_INFO                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       \
  {\n\t\t{MT_ISUNKNOWN,\t\t "Unknown type of tape device"},\n\t\t{MT_ISQIC02,\t\t "Generic QIC-02 tape streamer"},\n\t\t{MT_ISWT5150,\t\t "Wangtek 5150, QIC-150"},\n\t\t{MT_ISARCHIVE_5945L2,\t "Archive 5945L-2"},\n\t\t{MT_ISCMSJ500,\t\t "CMS Jumbo 500"},\n\t\t{MT_ISTDC3610,\t\t "Tandberg TDC 3610, QIC-24"},\n\t\t{MT_ISARCHIVE_VP60I,\t "Archive VP60i, QIC-02"},\n\t\t{MT_ISARCHIVE_2150L,\t "Archive Viper 2150L"},\n\t\t{MT_ISARCHIVE_2060L,\t\t "Archive Viper 2060L"},\n\t\t{MT_ISARCHIVESC499,\t "Archive SC-499 QIC-36 controller"},\n\t\t{MT_ISQIC02_ALL_FEATURES, "Generic QIC-02 tape, all features"},\n\t\t{MT_ISWT5099EEN24,\t "Wangtek 5099-een24, 60MB"},\n\t\t{MT_ISTEAC_MT2ST,\t "Teac MT-2ST 155mb data cassette drive"},\n\t\t{MT_ISEVEREX_FT40A,\t "Everex FT40A, QIC-40"},\n\t\t{MT_ISSCSI1,\t\t "Generic SCSI-1 tape"},\n\t\t{MT_ISSCSI2,\t\t "Generic SCSI-2 tape"},\n\t\t{ \
       0, 0}\n}

struct mtpos {
  long mt_blkno;
};

struct mtconfiginfo {
  long           mt_type;
  long           ifc_type;
  unsigned short irqnr;
  unsigned short dmanr;
  unsigned short port;
  unsigned long  debug;
  unsigned       have_dens : 1;
  unsigned       have_bsf  : 1;
  unsigned       have_fsr  : 1;
  unsigned       have_bsr  : 1;
  unsigned       have_eod  : 1;
  unsigned       have_seek : 1;
  unsigned       have_tell : 1;
  unsigned       have_ras1 : 1;
  unsigned       have_ras2 : 1;
  unsigned       have_ras3 : 1;
  unsigned       have_qfa  : 1;
  unsigned       pad1      : 5;
  char           reserved[10];
};

#define MTIOCTOP _IOW('m', 1, struct mtop)
#define MTIOCGET _IOR('m', 2, struct mtget)
#define MTIOCPOS _IOR('m', 3, struct mtpos)

#define MTIOCGETCONFIG _IOR('m', 4, struct mtconfiginfo)
#define MTIOCSETCONFIG _IOW('m', 5, struct mtconfiginfo)

#define GMT_EOF(x)       ((x) & 0x80000000)
#define GMT_BOT(x)       ((x) & 0x40000000)
#define GMT_EOT(x)       ((x) & 0x20000000)
#define GMT_SM(x)        ((x) & 0x10000000)
#define GMT_EOD(x)       ((x) & 0x08000000)
#define GMT_WR_PROT(x)   ((x) & 0x04000000)
#define GMT_ONLINE(x)    ((x) & 0x01000000)
#define GMT_D_6250(x)    ((x) & 0x00800000)
#define GMT_D_1600(x)    ((x) & 0x00400000)
#define GMT_D_800(x)     ((x) & 0x00200000)
#define GMT_DR_OPEN(x)   ((x) & 0x00040000)
#define GMT_IM_REP_EN(x) ((x) & 0x00010000)

#define MT_ST_BLKSIZE_SHIFT 0
#define MT_ST_BLKSIZE_MASK  0xffffff
#define MT_ST_DENSITY_SHIFT 24
#define MT_ST_DENSITY_MASK  0xff000000
#define MT_ST_SOFTERR_SHIFT 0
#define MT_ST_SOFTERR_MASK  0xffff
#define MT_ST_OPTIONS       0xf0000000
#define MT_ST_BOOLEANS      0x10000000
#define MT_ST_SETBOOLEANS   0x30000000
#define MT_ST_CLEAR_DEFAULT 0xfffff

#define MT_ST_DEF_DENSITY     (MT_ST_DEF_OPTIONS | 0x100000)
#define MT_ST_DEF_COMPRESSION (MT_ST_DEF_OPTIONS | 0x200000)
#define MT_ST_DEF_DRVBUFFER   (MT_ST_DEF_OPTIONS | 0x300000)
#define MT_ST_HPLOADER_OFFSET 10000
#ifndef DEFTAPE
#define DEFTAPE "/dev/tape"
#endif

#endif
