static int base = 100;
static int local(int x) { return x * base; }

// @check-fn-begin
int compute(int x) { return local(x) + 1; }
// @check-fn-end
