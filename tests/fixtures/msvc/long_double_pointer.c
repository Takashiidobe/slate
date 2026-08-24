extern void               store_long_double(long double *);
extern const long double *load_long_double(const long double *);

long double probe(void) {
  long double value;
  store_long_double(&value);
  return *load_long_double(&value);
}

int main(void) { return 0; }
// REWRITES-MSVC-DAG: fn store_long_double(_0: *mut f64);
// REWRITES-MSVC-DAG: fn load_long_double(_0: *const f64) -> *const f64;
