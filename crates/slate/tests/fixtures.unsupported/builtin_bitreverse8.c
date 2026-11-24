static unsigned char reverse_byte(unsigned char value) {
  return __builtin_bitreverse8(value);
}

int main(void) {
  return reverse_byte(0x12) != 0x48;
}
