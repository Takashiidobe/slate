#ifdef _FILE_OFFSET_BITS
int offset_code(void) { return 64; }
#else
int offset_code(void) { return 0; }
#endif
