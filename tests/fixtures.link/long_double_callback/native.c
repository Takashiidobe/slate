long double invoke_cb(long double (*cb)(long double, long double),
                      long double a, long double b) {
  return cb(a, b);
}
