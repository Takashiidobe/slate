#define WARNING_TOKEN expanded
#warning WARNING_TOKEN "quoted" C:\tmp

#if defined(SLATE_WARNING_FEATURE)
#warning selected warning
#endif

int main(void) { return 0; }
// DIRECTIVES-DAG: WARNING_TOKEN \"quoted\" C:\\tmp
// COMMON: #[deprecated(note = "WARNING_TOKEN \"quoted\" C:\\tmp")]
// COMMON-NEXT: const __SLATE_WARNING_0: () = {};
// COMMON-EMPTY:
// COMMON-NEXT: const _: () = __SLATE_WARNING_0;
// COMMON-NOT: selected warning
