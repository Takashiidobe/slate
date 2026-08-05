#ifndef _SLATE_ARPA_FTP_H
#define _SLATE_ARPA_FTP_H

enum {
  PRELIM = 1,
  COMPLETE = 2,
  CONTINUE = 3,
  TRANSIENT = 4,
  ERROR = 5,
};

enum {
  TYPE_A = 1,
  TYPE_E = 2,
  TYPE_I = 3,
  TYPE_L = 4,
};

enum {
  FORM_N = 1,
  FORM_T = 2,
  FORM_C = 3,
};

enum {
  STRU_F = 1,
  STRU_R = 2,
  STRU_P = 3,
};

enum {
  MODE_S = 1,
  MODE_B = 2,
  MODE_C = 3,
};

enum {
  REC_ESC = '\377',
  REC_EOR = '\001',
  REC_EOF = '\002',
};

enum {
  BLK_EOR = 0x80,
  BLK_EOF = 0x40,
  BLK_ERRORS = 0x20,
  BLK_RESTART = 0x10,
  BLK_BYTECOUNT = 2,
};

#ifdef FTP_NAMES
char *modenames[] = {"0", "Stream", "Block", "Compressed"};
char *strunames[] = {"0", "File", "Record", "Page"};
char *typenames[] = {"0", "ASCII", "EBCDIC", "Image", "Local"};
char *formnames[] = {"0", "Nonprint", "Telnet", "Carriage-control"};
#endif

#endif
