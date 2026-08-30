typedef struct {
  int windowLog;
} Params;

static void use_params(const Params *params, unsigned *out, const char *lo,
                        const char *hi) {
  long diff = hi - lo;
  *out = (unsigned)(params->windowLog + (int)diff);
}

int main(void) {
  Params p;
  p.windowLog = 5;
  char buf[8];
  unsigned out = 0;
  use_params(&p, &out, buf, buf + 4);
  return (int)out;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: fn use_params({{arg[0-9]+}}: *mut Params, {{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) {
// LOWERING-DAG: {{__slate_alloca_frame[0-9]+}}.4 = {{arg[0-9]+}};
// LOWERING-DAG: {{__slate_alloca_frame[0-9]+}}.3 = {{arg[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: fn use_params({{arg[0-9]+}}: &Params, {{arg[0-9]+}}: &mut u32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) {
// REWRITES-DAG: {{__slate_alloca_frame[0-9]+}}.4 = ({{arg[0-9]+}} as *const Params) as *mut Params;
// REWRITES-DAG: {{__slate_alloca_frame[0-9]+}}.3 = {{arg[0-9]+}} as *mut u32;
// SLATE-FILECHECK-END rewrites
