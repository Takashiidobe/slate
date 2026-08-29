struct Pair {
  long a;
  long b;
};

struct Version {
  int major;
  int minor;
  int micro;
};

struct Pair get_pair(void) {
  struct Pair p = {10, 20};
  return p;
}

struct Version get_version(void) {
  struct Version v = {1, 2, 3};
  return v;
}
// LOWERING-DAG: fn get_version
