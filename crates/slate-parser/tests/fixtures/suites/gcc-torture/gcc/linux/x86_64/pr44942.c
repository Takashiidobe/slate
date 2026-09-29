/* PR target/44942 */

#include <stdarg.h>

void test1(int a, int b, int c, int d, int e, int f, int g, long double h,
           ...) {
  int     i;
  va_list ap;

  va_start(ap, h);
  i = va_arg(ap, int);
  if (i != 1234)
    __builtin_abort();
  va_end(ap);
}

void test2(int a, int b, int c, int d, int e, int f, int g, long double h,
           int i, long double j, int k, long double l, int m, long double n,
           ...) {
  int     o;
  va_list ap;

  va_start(ap, n);
  o = va_arg(ap, int);
  if (o != 1234)
    __builtin_abort();
  va_end(ap);
}

void test3(double a, double b, double c, double d, double e, double f, double g,
           long double h, ...) {
  double  i;
  va_list ap;

  va_start(ap, h);
  i = va_arg(ap, double);
  if (i != 1234.0)
    __builtin_abort();
  va_end(ap);
}

void test4(double a, double b, double c, double d, double e, double f, double g,
           long double h, double i, long double j, double k, long double l,
           double m, long double n, ...) {
  double  o;
  va_list ap;

  va_start(ap, n);
  o = va_arg(ap, double);
  if (o != 1234.0)
    __builtin_abort();
  va_end(ap);
}

int main() {
  test1(0, 0, 0, 0, 0, 0, 0, 0.0L, 1234);
  test2(0, 0, 0, 0, 0, 0, 0, 0.0L, 0, 0.0L, 0, 0.0L, 0, 0.0L, 1234);
  test3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0L, 1234.0);
  test4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0L, 0.0, 0.0L, 0.0, 0.0L, 0.0,
        0.0L, 1234.0);
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32, %[[VALUE_e:[0-9]+]] e: i32, %[[VALUE_f:[0-9]+]] f: i32, %[[VALUE_g:[0-9]+]] g: i32, %[[VALUE_h:[0-9]+]] h: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1234))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32, %[[VALUE_c_2:[0-9]+]] c: i32, %[[VALUE_d_2:[0-9]+]] d: i32, %[[VALUE_e_2:[0-9]+]] e: i32, %[[VALUE_f_2:[0-9]+]] f: i32, %[[VALUE_g_2:[0-9]+]] g: i32, %[[VALUE_h_2:[0-9]+]] h: f80, %[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_j:[0-9]+]] j: f80, %[[VALUE_k:[0-9]+]] k: i32, %[[VALUE_l:[0-9]+]] l: f80, %[[VALUE_m:[0-9]+]] m: i32, %[[VALUE_n:[0-9]+]] n: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_o]], va_arg<i32>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_o]]), const<i32>(1234))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c_3:[0-9]+]] c: f64, %[[VALUE_d_3:[0-9]+]] d: f64, %[[VALUE_e_3:[0-9]+]] e: f64, %[[VALUE_f_3:[0-9]+]] f: f64, %[[VALUE_g_3:[0-9]+]] g: f64, %[[VALUE_h_3:[0-9]+]] h: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         write<f64>(%[[VALUE_i_3]], va_arg<f64>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:         va_arg<f64>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_i_3]]), const<f64>(1234.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: f64, %[[VALUE_c_4:[0-9]+]] c: f64, %[[VALUE_d_4:[0-9]+]] d: f64, %[[VALUE_e_4:[0-9]+]] e: f64, %[[VALUE_f_4:[0-9]+]] f: f64, %[[VALUE_g_4:[0-9]+]] g: f64, %[[VALUE_h_4:[0-9]+]] h: f80, %[[VALUE_i_4:[0-9]+]] i: f64, %[[VALUE_j_2:[0-9]+]] j: f80, %[[VALUE_k_2:[0-9]+]] k: f64, %[[VALUE_l_2:[0-9]+]] l: f80, %[[VALUE_m_2:[0-9]+]] m: f64, %[[VALUE_n_2:[0-9]+]] n: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_o_2:[0-9]+]] o: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         write<f64>(%[[VALUE_o_2]], va_arg<f64>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:         va_arg<f64>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_o_2]]), const<f64>(1234.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, f80, ...) -> void>(%[[VALUE_test1]], const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<f80>(0), const<i32>(1234));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, f80, i32, f80, i32, f80, i32, f80, ...) -> void>(%[[VALUE_test2]], const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(1234));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, f64, f64, f64, f64, f80, ...) -> void>(%[[VALUE_test3]], const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f80>(0), const<f64>(1234.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, f64, f64, f64, f64, f80, f64, f80, f64, f80, f64, f80, ...) -> void>(%[[VALUE_test4]], const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(1234.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
