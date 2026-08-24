#ifndef _SLATE_BITS_DARWIN_STDIO_H
#define _SLATE_BITS_DARWIN_STDIO_H

struct __sbuf {
  unsigned char *_base;
  int            _size;
};

struct __sFILEX;

struct __sFILE {
  unsigned char  *_p;
  int             _r;
  int             _w;
  short           _flags;
  short           _file;
  struct __sbuf   _bf;
  int             _lbfsize;
  void           *_cookie;
  int (*_close)(void *);
  int (*_read)(void *, char *, int);
  fpos_t (*_seek)(void *, fpos_t, int);
  int (*_write)(void *, const char *, int);
  struct __sbuf   _ub;
  struct __sFILEX *_extra;
  int             _ur;
  unsigned char   _ubuf[3];
  unsigned char   _nbuf[1];
  struct __sbuf   _lb;
  int             _blksize;
  fpos_t          _offset;
};

#endif
