#ifndef SHARED_H
#define SHARED_H

typedef int (*callback_fn)(int a, int b);

struct Parser {
  callback_fn handler;
};

void set_handler(struct Parser *p, callback_fn h);
int add_ignore_b(int a, int b);

#endif
