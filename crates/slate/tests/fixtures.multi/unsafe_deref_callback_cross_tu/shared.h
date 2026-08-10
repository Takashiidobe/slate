struct Callback {
  int (*handler)(int *);
};

int deref_and_add(int *p);
int call_handler(struct Callback *cb, int *p);
