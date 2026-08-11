union U {
  struct {
    unsigned first : 1;
    unsigned second : 1;
  } bits;
  unsigned raw;
};

int main(void) {
  union U value = {0};
  value.bits.second = 1;
  return value.bits.second != 1;
}
