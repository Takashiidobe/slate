struct identity {
  int gnuc;
  const char *version;
};

struct identity compiler_identity(void) {
  struct identity id = { __GNUC__ * 100 + __GNUC_MINOR__, __VERSION__ };
  return id;
}
