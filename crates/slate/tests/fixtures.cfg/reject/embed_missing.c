int main(void) {
  unsigned char data[] = {
#embed "missing-embed-data.bin"
  };
  return data[0];
}
