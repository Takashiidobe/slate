_Thread_local int shared_value = 5;

// @rewrite-fn-begin
int read_shared_value(void) { return shared_value; }
// @rewrite-fn-end
