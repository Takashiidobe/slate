void alias_target(void);
void alias_entry(void) __attribute__((alias("alias_target")));

static inline void cleanup_fn(int *value) {
  (void)value;
}

__attribute__((constructor)) static void startup_fn(void) {}
__attribute__((destructor)) static void shutdown_fn(void) {}
__attribute__((used)) static void used_fn(void) {}
__attribute__((retain)) static void retained_fn(void) {}
