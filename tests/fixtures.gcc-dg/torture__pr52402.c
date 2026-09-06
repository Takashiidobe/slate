/* { dg-do run } */
/* { dg-options "-w -Wno-psabi" } */
/* { dg-require-effective-target int32plus } */

typedef int v4si __attribute__((vector_size(16)));
struct T {
  v4si i[2];
  int  j;
} __attribute__((packed));

static v4si __attribute__((noinline)) foo(struct T t) { return t.i[0]; }

static struct T *__attribute__((noinline)) init() {
  char *p = __builtin_malloc(sizeof(struct T) + 1);
  p++;
  __builtin_memset(p, 1, sizeof(struct T));
  return (struct T *)p;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  struct T *p;
  p = init();
  if (foo(*p)[0] != 0x01010101)
    __builtin_abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut p: *mut T = std::ptr::null_mut();
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut T = init();
// LOWERING-DAG:     p = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut T = p;
// LOWERING-DAG:         let {{__v[0-9]+}}: T = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: [i32; 4] = foo({{__v[0-9]+}});
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 16843009;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut p: *mut T = init();
// REWRITES-DAG:     let {{__v[0-9]+}}: [i32; 4] = foo(unsafe { *p });
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}[0] != 16843009;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
