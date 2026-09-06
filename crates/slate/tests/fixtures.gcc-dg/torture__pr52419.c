/* PR middle-end/52419 */
/* { dg-do run } */

extern void abort(void);

typedef long long V
    __attribute__((vector_size(2 * sizeof(long long)), may_alias));

typedef struct S {
  V b;
} P __attribute__((aligned(1)));

struct __attribute__((packed)) T {
  char c;
  P    s;
};

__attribute__((noinline, noclone)) void foo(P *p) { p->b[1] = 5; }

int
// @lowering-fn-begin
// @rewrite-fn-begin
main() {
  V        a = {3, 4};
  struct T t;

  t.s.b = a;
  foo(&t.s);

  if (t.s.b[0] != 3 || t.s.b[1] != 5)
    abort();

  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut t: T = T {
// LOWERING-DAG:         c: 0,
// LOWERING-DAG:         s: S { b: [0; 2] },
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: [i64; 2] = [3, 4];
// LOWERING-DAG:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(t.s.b), {{__v[0-9]+}}) };
// LOWERING-DAG:     unsafe { foo(std::ptr::addr_of_mut!(t.s)) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: [i64; 2] = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(t.s.b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: [i64; 2] = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(t.s.b)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 5;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
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
// REWRITES-DAG:     let mut t: T = T {
// REWRITES-DAG:         c: 0,
// REWRITES-DAG:         s: S { b: [0; 2] },
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: [i64; 2] = [3, 4];
// REWRITES-DAG:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(t.s.b), {{__v[0-9]+}}) };
// REWRITES-DAG:     unsafe { foo(std::ptr::addr_of_mut!(t.s)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(t.s.b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}[0] != 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: [i64; 2] = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(t.s.b)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}[1] != 5;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
