// { dg-options "-fblocks" }
extern void *_Block_copy(const void *);

static int (^saved)(int);

void keep(int (^callback)(int)) {
  saved = _Block_copy(^(int x) { return callback(x) + 1; });
}

int fire(int x) { return saved(x); }
