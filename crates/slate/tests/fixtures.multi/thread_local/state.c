_Thread_local int shared_value = 5;

// @check-fn-begin
int read_shared_value(void) { return shared_value; }
// @check-fn-end
