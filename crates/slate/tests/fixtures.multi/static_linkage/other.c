static int base = 100;
static int local(int x) { return x * base; }

// @rewrite-fn-begin
int compute(int x) { return local(x) + 1; }
// @rewrite-fn-end
