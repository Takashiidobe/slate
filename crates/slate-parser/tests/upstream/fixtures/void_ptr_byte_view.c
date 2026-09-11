static int all_bytes_identical(const void *src, unsigned long size) {
  unsigned char b = ((const unsigned char *)src)[0];
  for (unsigned long p = 1; p < size; p++) {
    if (((const unsigned char *)src)[p] != b) {
      return 0;
    }
  }
  return 1;
}

int main(void) {
  unsigned char buf[4] = {7, 7, 7, 7};
  return all_bytes_identical(buf, 4);
}


