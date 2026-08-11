#ifndef ANONYMOUS_HEADER_TYPES_H
#define ANONYMOUS_HEADER_TYPES_H

typedef struct {
  int count;
  union {
    int  wide;
    char bytes[4];
  } value;
} anonymous_header_state;

#endif
