int fallback(void) { return 2; }
__attribute__((weak)) int both_weak(void) { return 20; }
int threshold = 9;
__attribute__((weak)) int shared_weak = 60;
