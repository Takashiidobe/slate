#define WARNING_TOKEN expanded
#warning WARNING_TOKEN "quoted" C:\tmp

#if defined(SLATE_WARNING_FEATURE)
#warning selected warning
#endif

int main(void) { return 0; }
