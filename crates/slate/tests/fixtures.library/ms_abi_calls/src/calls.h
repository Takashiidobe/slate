#define MS_ABI __attribute__((ms_abi))
#define SYSV_ABI __attribute__((sysv_abi))

struct pair {
  double a;
  double b;
};

struct triple {
  long x;
  long y;
  long z;
};

typedef long(MS_ABI *ms_callback)(long, double, long, double, long);

long MS_ABI ms_mix(int a, double b, long c, float d, long e, double f);
struct pair MS_ABI ms_swap(struct pair p, int bias);
struct triple MS_ABI ms_scale(struct triple t, long scale);
long MS_ABI ms_invoke(ms_callback cb, long seed);
long SYSV_ABI sysv_add(long a, long b);
long call_driver(long seed);

long MS_ABI driver_ms(long a, double b, long c, double d, long e);
long MS_ABI driver_vmix(int n, ...);
