int __attribute__((overloadable)) scale(int x) { return x * 3; }
int __attribute__((overloadable)) scale(long x) { return (int)(x * 5); }
int labelled(void) __asm__("renamed_entry");
int labelled(void) { return 11; }
