int free_value(int x);
int ng_free_value(int x);

int combined(int x) { return free_value(x) * 100 + ng_free_value(x); }
