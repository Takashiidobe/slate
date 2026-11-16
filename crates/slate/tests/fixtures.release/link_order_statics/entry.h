struct slate_entry {
  int id;
  const struct slate_entry *self;
  int (*value)(void);
};
