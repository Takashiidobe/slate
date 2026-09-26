/* { dg-do run }

   # Floating-point support is incomplete.
   { dg-skip-if "AVR doubles are floats" { avr-*-* } } */
/* PR middle-end/30704 */

typedef __SIZE_TYPE__ size_t;
extern void           abort(void);
extern int            memcmp(const void *, const void *, size_t);
extern void          *memcpy(void *, const void *, size_t);

long long f1(void) {
  long long t;
  double    d = 0x0.fffffffffffff000p-1022;
  memcpy(&t, &d, sizeof(long long));
  return t;
}

double f2(void) {
  long long t = 0x000fedcba9876543LL;
  double    d;
  memcpy(&d, &t, sizeof(long long));
  return d;
}

int main() {
  union {
    long long ll;
    double    d;
  } u;

  if (sizeof(long long) != sizeof(double) || __DBL_MIN_EXP__ != -1021)
    return 0;

  u.ll = f1();
  if (u.d != 0x0.fffffffffffff000p-1022)
    abort();

  u.d = f2();
  if (u.ll != 0x000fedcba9876543LL)
    abort();

  double    b = 234.0;
  long long c;
  double    d = b;
  memcpy(&c, &b, sizeof(double));
  long long e = c;
  if (memcmp(&e, &d, sizeof(double)) != 0)
    abort();

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 ll: i64;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @memcmp(%17 <unnamed>: ptr<const void>, %18 <unnamed>: ptr<const void>, %19 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @memcpy(%20 <unnamed>: ptr<void>, %21 <unnamed>: ptr<const void>, %22 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @f1() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 t: i64 [storage=automatic];
// DEFAULT-NEXT:         let %6 d: f64 [storage=automatic] = const<f64>(2.225073858507201e-308);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(%5)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f64>>(%6)), const<u64>(8));
// DEFAULT-NEXT:         return read<i64>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f2() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 t: i64 [storage=automatic] = const<i64>(4483583629026627);
// DEFAULT-NEXT:         let %9 d: f64 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<f64>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i64>>(%8)), const<u64>(8));
// DEFAULT-NEXT:         return read<f64>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 u: @type1 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(const<u64>(8), const<u64>(8)), ne<i32>(neg<i32, overflow=ub>(const<i32>(1021)), neg<i32, overflow=ub>(const<i32>(1021))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i64>(field0(%12), call<i64, signature=fn() -> i64>(%4));
// DEFAULT-NEXT:         call<i64, signature=fn() -> i64>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%12)), const<f64>(2.225073858507201e-308))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<f64>(field1(%12), call<f64, signature=fn() -> f64>(%7));
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%7);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field0(%12)), const<i64>(4483583629026627))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %13 b: f64 [storage=automatic] = const<f64>(234.0);
// DEFAULT-NEXT:         let %14 c: i64 [storage=automatic];
// DEFAULT-NEXT:         let %15 d: f64 [storage=automatic] = read<f64>(%13);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(%14)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f64>>(%13)), const<u64>(8));
// DEFAULT-NEXT:         let %16 e: i64 [storage=automatic] = read<i64>(%14);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i64>>(%16)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f64>>(%15)), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
