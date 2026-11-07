long double cb_half(long double x);
long double ld__twice(long double x);

int round_trip(int n) { return (int)cb_half(ld__twice((long double)n)); }
