// @rewrite-fn-begin
#line 700 "virtual-line.c"
#ifdef LINE_FEATURE
int line_value(void) { return __LINE__; }
int expected_line(void) { return 701; }
#else
int line_value(void) { return __LINE__; }
int expected_line(void) { return 704; }
#endif
// @rewrite-fn-end

int main(void) { return line_value() == expected_line() ? 0 : 1; }
// DIRECTIVES-DAG: #[cfg(feature = "line_feature")]
// DIRECTIVES-DAG: #[cfg(not(feature = "line_feature"))]

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn line_value() -> i32 {
// REWRITES-DAG:     704
// REWRITES-DAG: }
// REWRITES-DAG: fn expected_line() -> i32 {
// REWRITES-DAG:     704
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
