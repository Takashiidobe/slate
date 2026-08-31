#ifndef _SLATE_BITS_FREEBSD_STDIO_H
#define _SLATE_BITS_FREEBSD_STDIO_H

struct __sbuf {
  unsigned char *_base;
  int            _size;
};

struct __sFILE {
  unsigned char *_p;
  int            _r;
  int            _w;
  short          _flags;
  short          _file;
  struct __sbuf  _bf;
  int            _lbfsize;
  void          *_cookie;
  int (*_close)(void *);
  int (*_read)(void *, char *, int);
  fpos_t (*_seek)(void *, fpos_t, int);
  int (*_write)(void *, const char *, int);
  struct __sbuf _ub;
  unsigned char *_up;
  int            _ur;
  unsigned char  _ubuf[3];
  unsigned char  _nbuf[1];
  struct __sbuf  _lb;
  int            _blksize;
  fpos_t         _offset;
  void          *_fl_mutex;
  void          *_fl_owner;
  int            _fl_count;
  int            _orientation;
  union {
    unsigned char __mbstate8[128];
    long long     __mbstateL;
  } _mbstate;
  int _flags2;
};

#endif
