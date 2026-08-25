struct operation {
  enum { MODE_NUMBER, MODE_TEXT } mode;
  union {
    int number;
    char *text;
  };
};

int op_number(struct operation *op) { return op->number; }
