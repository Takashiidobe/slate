int weak_global __attribute__((weak)) = 13;

// @rewrite-fn-begin
int __attribute__((weak)) fallback_value(void) { return weak_global; }
// @rewrite-fn-end
