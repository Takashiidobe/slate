/* { dg-do run }
   { dg-require-effective-target double64 } */
/* PR tree-optimization/108540 */

__attribute__((noipa)) void bar(const char *cp, __SIZE_TYPE__ size, char sign,
                                int dsgn) {
  if (__builtin_strcmp(cp, "ZERO") != 0 || size != 4 || sign != '-' ||
      dsgn != 1)
    __builtin_abort();
}

__attribute__((noipa)) void foo(int x, int ch, double d) {
  const char   *cp   = "";
  __SIZE_TYPE__ size = 0;
  char          sign = '\0';
  switch (x) {
  case 42:
    if (__builtin_isinf(d)) {
      if (d < 0)
        sign = '-';
      cp   = "Inf";
      size = 3;
      break;
    }
    if (__builtin_isnan(d)) {
      cp   = "NaN";
      size = 3;
      break;
    }
    if (d < 0) {
      d    = -d;
      sign = '-';
    } else if (d == 0.0 && __builtin_signbit(d))
      sign = '-';
    else
      sign = '\0';
    if (ch == 'a' || ch == 'A') {
      union U {
        __INT64_TYPE__ l;
        double         d;
      } u;
      int dsgn;
      u.d = d;
      if (u.l < 0) {
        dsgn  = 1;
        u.l  &= 0x7fffffffffffffffLL;
      } else
        dsgn = 0;
      if (__builtin_isinf(d)) {
        cp   = "INF";
        size = 3;
      } else if (__builtin_isnan(d)) {
        cp   = "NAN";
        size = 3;
      } else if (d == 0) {
        cp   = "ZERO";
        size = 4;
      } else {
        cp   = "WRONG";
        size = 5;
      }
      bar(cp, size, sign, dsgn);
    }
  }
}

int main() {
  foo(42, 'a', -0.0);
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
// DEFAULT-NEXT:     type @type0 U = union {
// DEFAULT-NEXT:         field0 l: i64;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([90, 69, 82, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([73, 110, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([78, 97, 78, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([73, 78, 70, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([78, 65, 78, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([90, 69, 82, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([87, 82, 79, 78, 71, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %18 @__builtin_strcmp(%16 <unnamed>: ptr<const i8>, %17 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @bar(%1 cp: ptr<const i8>, %2 size: u64, %3 sign: i8, %4 dsgn: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%18, read<ptr<const i8>>(%1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%19))), const<i32>(0)), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(45))), ne<i32>(read<i32>(%4), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo(%6 x: i32, %7 ch: i32, %8 d: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 cp: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(1)>(%21));
// DEFAULT-NEXT:         let %10 size: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %11 sign: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         switch %22 read<i32>(%6)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %22 const<i32>(42):
// DEFAULT-NEXT:                     if float_class<bool, test=infinite>(read<f64>(%8))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if lt<f64, exceptions=ignore>(read<f64>(%8), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                                 write<i8>(%11, truncate<i8, reason=assign, fits=always>(const<i32>(45)));
// DEFAULT-NEXT:                             write<ptr<const i8>>(%9, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%23)));
// DEFAULT-NEXT:                             write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:                             break %22;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 if float_class<bool, test=nan>(read<f64>(%8))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<const i8>>(%9, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%24)));
// DEFAULT-NEXT:                         write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:                         break %22;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if lt<f64, exceptions=ignore>(read<f64>(%8), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<f64>(%8, neg<f64>(read<f64>(%8)));
// DEFAULT-NEXT:                         write<i8>(%11, truncate<i8, reason=assign, fits=always>(const<i32>(45)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_and<bool>(eq<f64, exceptions=ignore>(read<f64>(%8), const<f64>(0.0)), float_class<bool, test=sign_bit>(read<f64>(%8)))
// DEFAULT-NEXT:                         write<i8>(%11, truncate<i8, reason=assign, fits=always>(const<i32>(45)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i8>(%11, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 if logical_or<bool>(eq<i32>(read<i32>(%7), const<i32>(97)), eq<i32>(read<i32>(%7), const<i32>(65)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %13 u: @type0 [storage=automatic];
// DEFAULT-NEXT:                         let %14 dsgn: i32 [storage=automatic];
// DEFAULT-NEXT:                         write<f64>(field1(%13), read<f64>(%8));
// DEFAULT-NEXT:                         if lt<i64>(read<i64>(field0(%13)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%14, const<i32>(1));
// DEFAULT-NEXT:                                 let %29: i64 [synthetic] = read<i64>(field0(%13));
// DEFAULT-NEXT:                                 let %30: i64 [synthetic] = and<i64>(read<i64>(%29), const<i64>(9223372036854775807));
// DEFAULT-NEXT:                                 write<i64>(field0(%13), read<i64>(%30));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                         if float_class<bool, test=infinite>(read<f64>(%8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<ptr<const i8>>(%9, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%25)));
// DEFAULT-NEXT:                                 write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if float_class<bool, test=nan>(read<f64>(%8))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<const i8>>(%9, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%26)));
// DEFAULT-NEXT:                                     write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if eq<f64, exceptions=ignore>(read<f64>(%8), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         write<ptr<const i8>>(%9, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%27)));
// DEFAULT-NEXT:                                         write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         write<ptr<const i8>>(%9, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%28)));
// DEFAULT-NEXT:                                         write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<const i8>, u64, i8, i32) -> void>(%0, read<ptr<const i8>>(%9), read<u64>(%10), read<i8>(%11), read<i32>(%14));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, f64) -> void>(%5, const<i32>(42), const<i32>(97), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
