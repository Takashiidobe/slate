struct Flags {
  void *owner;
  int count;
  unsigned ready : 1;
  unsigned mode : 3;
};
