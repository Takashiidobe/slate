typedef int (*Handler)(int);

struct Callback {
  Handler handler;
};
