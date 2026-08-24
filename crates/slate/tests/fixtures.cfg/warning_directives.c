#define WARNING_TOKEN expanded
#warning WARNING_TOKEN "quoted" C:\tmp

#if defined(SLATE_WARNING_FEATURE)
#warning selected warning
#endif

int main(void) { return 0; }
// DIRECTIVES-DAG: WARNING_TOKEN \"quoted\" C:\\tmp
// DIRECTIVES-DAG: #[cfg(feature = "slate_warning_feature")]
// DIRECTIVES-DAG: #[deprecated(note = "selected warning")]
