int weak_global __attribute__((weak)) = 13;

// @check-fn-begin
int __attribute__((weak)) fallback_value(void) { return weak_global; }
// @check-fn-end
