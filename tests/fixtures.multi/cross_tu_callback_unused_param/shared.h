#ifndef SHARED_H
#define SHARED_H

typedef int (*callback_fn)(int a, int b);

struct Callback {
  callback_fn handler;
};

int call_handler(struct Callback *cb, int a, int b);
int add_ignore_b(int a, int b);

#endif
