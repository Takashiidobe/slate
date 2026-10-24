#define VC __attribute__((vectorcall))

struct pair {
  int a;
  int b;
};

typedef double(VC *vc_callback)(int, double, float, long long, double, float,
                                double, int, double);

double VC vc_mix(int a, double b, float c, long long d, double e, float f,
                 double g, int h, double i);
struct pair VC vc_swap(struct pair p, int bias);
double VC vc_invoke(vc_callback cb, int seed);
long long call_driver(int seed);

long long VC driver_vc(int a, double b, long long c, float d, int e, double f,
                       double g, double h);
