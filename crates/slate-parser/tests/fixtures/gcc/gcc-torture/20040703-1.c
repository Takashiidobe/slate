/* PR 16341 */
/* { dg-require-effective-target int32plus } */

void abort(void);
void exit(int);

#define PART_PRECISION (sizeof(cpp_num_part) * 8)

typedef unsigned int   cpp_num_part;
typedef struct cpp_num cpp_num;
struct cpp_num {
  cpp_num_part high;
  cpp_num_part low;
  int          unsignedp; /* True if value should be treated as unsigned.  */
  int          overflow;  /* True if the most recent calculation overflowed.  */
};

static int num_positive(cpp_num num, unsigned int precision) {
  if (precision > PART_PRECISION) {
    precision -= PART_PRECISION;
    return (num.high & (cpp_num_part)1 << (precision - 1)) == 0;
  }

  return (num.low & (cpp_num_part)1 << (precision - 1)) == 0;
}

static cpp_num num_trim(cpp_num num, unsigned int precision) {
  if (precision > PART_PRECISION) {
    precision -= PART_PRECISION;
    if (precision < PART_PRECISION)
      num.high &= ((cpp_num_part)1 << precision) - 1;
  } else {
    if (precision < PART_PRECISION)
      num.low &= ((cpp_num_part)1 << precision) - 1;
    num.high = 0;
  }

  return num;
}

/* Shift NUM, of width PRECISION, right by N bits.  */
static cpp_num num_rshift(cpp_num num, unsigned int precision, unsigned int n) {
  cpp_num_part sign_mask;
  int          x = num_positive(num, precision);

  if (num.unsignedp || x)
    sign_mask = 0;
  else
    sign_mask = ~(cpp_num_part)0;

  if (n >= precision)
    num.high = num.low = sign_mask;
  else {
    /* Sign-extend.  */
    if (precision < PART_PRECISION)
      num.high = sign_mask, num.low |= sign_mask << precision;
    else if (precision < 2 * PART_PRECISION)
      num.high |= sign_mask << (precision - PART_PRECISION);

    if (n >= PART_PRECISION) {
      n        -= PART_PRECISION;
      num.low   = num.high;
      num.high  = sign_mask;
    }

    if (n) {
      num.low  = (num.low >> n) | (num.high << (PART_PRECISION - n));
      num.high = (num.high >> n) | (sign_mask << (PART_PRECISION - n));
    }
  }

  num          = num_trim(num, precision);
  num.overflow = 0;
  return num;
}
#define num_zerop(num)     ((num.low | num.high) == 0)
#define num_eq(num1, num2) (num1.low == num2.low && num1.high == num2.high)

cpp_num num_lshift(cpp_num num, unsigned int precision, unsigned int n) {
  if (n >= precision) {
    num.overflow = !num.unsignedp && !num_zerop(num);
    num.high = num.low = 0;
  } else {
    cpp_num      orig;
    unsigned int m = n;

    orig = num;
    if (m >= PART_PRECISION) {
      m        -= PART_PRECISION;
      num.high  = num.low;
      num.low   = 0;
    }
    if (m) {
      num.high   = (num.high << m) | (num.low >> (PART_PRECISION - m));
      num.low  <<= m;
    }
    num = num_trim(num, precision);

    if (num.unsignedp)
      num.overflow = 0;
    else {
      cpp_num maybe_orig = num_rshift(num, precision, n);
      num.overflow       = !num_eq(orig, maybe_orig);
    }
  }

  return num;
}

unsigned int precision = 64;
unsigned int n         = 16;

cpp_num num = {0, 3, 0, 0};

int main() {
  cpp_num res = num_lshift(num, 64, n);

  if (res.low != 0x30000)
    abort();

  if (res.high != 0)
    abort();

  if (res.overflow != 0)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     type @type0 cpp_num_part = u32;
// DEFAULT-NEXT:     type @type1 cpp_num = struct incomplete;
// DEFAULT-NEXT:     type @type2 cpp_num = @type1;
// DEFAULT-NEXT:     global %24 precision: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(64)) [linkage=external];
// DEFAULT-NEXT:     global %25 n: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(16)) [linkage=external];
// DEFAULT-NEXT:     global %26 num: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(3)), field2 = const<i32>(0), field3 = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%29 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @num_positive(%6 num: @type1, %7 precision: u32) -> i32 [linkage=internal] [abi=sysv64(coerce<i64, i64>, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u64>(widen<u64, reason=usual_arith>(read<u32>(%7)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %30: u32 [synthetic] = read<u32>(%7);
// DEFAULT-NEXT:                 let %31: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%30)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                 write<u32>(%7, read<u32>(%31));
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<u32>(and<u32>(read<u32>(field0(%6)), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), sub<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<u32>(and<u32>(read<u32>(field1(%6)), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), sub<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @num_trim(%9 num: @type1, %10 precision: u32) -> @type1 [linkage=internal] [abi=sysv64(coerce<i64, i64>, scalar) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u64>(widen<u64, reason=usual_arith>(read<u32>(%10)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %32: u32 [synthetic] = read<u32>(%10);
// DEFAULT-NEXT:                 let %33: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%32)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                 write<u32>(%10, read<u32>(%33));
// DEFAULT-NEXT:                 if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%10)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     let %34: u32 [synthetic] = read<u32>(field0(%9));
// DEFAULT-NEXT:                     let %35: u32 [synthetic] = and<u32>(read<u32>(%34), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<u32>(%10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                     write<u32>(field0(%9), read<u32>(%35));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%10)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     let %36: u32 [synthetic] = read<u32>(field1(%9));
// DEFAULT-NEXT:                     let %37: u32 [synthetic] = and<u32>(read<u32>(%36), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<u32>(%10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                     write<u32>(field1(%9), read<u32>(%37));
// DEFAULT-NEXT:                 write<u32>(field0(%9), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @num_rshift(%12 num: @type1, %13 precision: u32, %14 n: u32) -> @type1 [linkage=internal] [abi=sysv64(coerce<i64, i64>, scalar, scalar) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 sign_mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %16 x: i32 [storage=automatic] = call<i32, signature=fn(@type1, u32) -> i32, abi=sysv64(coerce<i64, i64>, scalar) -> scalar>(%5, copy<@type1, reason=arg>(read<@type1>(%12)), read<u32>(%13));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field2(%12)), const<i32>(0)), ne<i32>(read<i32>(%16), const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%15, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%15, not<u32>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ge<u32>(read<u32>(%14), read<u32>(%13))
// DEFAULT-NEXT:             write<u32>(field1(%12), read<u32>(%15));
// DEFAULT-NEXT:             write<u32>(field0(%12), read<u32>(%15));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%13)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     write<u32>(field0(%12), read<u32>(%15));
// DEFAULT-NEXT:                     let %38: u32 [synthetic] = read<u32>(field1(%12));
// DEFAULT-NEXT:                     let %39: u32 [synthetic] = or<u32>(read<u32>(%38), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%15), read<u32>(%13)));
// DEFAULT-NEXT:                     write<u32>(field1(%12), read<u32>(%39));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%13)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))))
// DEFAULT-NEXT:                         let %40: u32 [synthetic] = read<u32>(field0(%12));
// DEFAULT-NEXT:                         let %41: u32 [synthetic] = or<u32>(read<u32>(%40), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%15), sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%13)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))));
// DEFAULT-NEXT:                         write<u32>(field0(%12), read<u32>(%41));
// DEFAULT-NEXT:                 if ge<u64>(widen<u64, reason=usual_arith>(read<u32>(%14)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %42: u32 [synthetic] = read<u32>(%14);
// DEFAULT-NEXT:                         let %43: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%42)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                         write<u32>(%14, read<u32>(%43));
// DEFAULT-NEXT:                         write<u32>(field1(%12), read<u32>(field0(%12)));
// DEFAULT-NEXT:                         write<u32>(field0(%12), read<u32>(%15));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(%14), const<u32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u32>(field1(%12), or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(field1(%12)), read<u32>(%14)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(field0(%12)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), widen<u64, reason=usual_arith>(read<u32>(%14))))));
// DEFAULT-NEXT:                         write<u32>(field0(%12), or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(field0(%12)), read<u32>(%14)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%15), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), widen<u64, reason=usual_arith>(read<u32>(%14))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type1>(%12, copy<@type1, reason=assign>(call<@type1, signature=fn(@type1, u32) -> @type1, abi=sysv64(coerce<i64, i64>, scalar) -> coerce<i64, i64>>(%8, copy<@type1, reason=arg>(read<@type1>(%12)), read<u32>(%13))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(@type1, u32) -> @type1, abi=sysv64(coerce<i64, i64>, scalar) -> coerce<i64, i64>>(%8, copy<@type1, reason=arg>(read<@type1>(%12)), read<u32>(%13)));
// DEFAULT-NEXT:         write<i32>(field3(%12), const<i32>(0));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @num_lshift(%18 num: @type1, %19 precision: u32, %20 n: u32) -> @type1 [linkage=external] [abi=sysv64(coerce<i64, i64>, scalar, scalar) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ge<u32>(read<u32>(%20), read<u32>(%19))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(field3(%18), from_bool<i32, reason=assign>(logical_and<bool>(not<bool>(ne<i32>(read<i32>(field2(%18)), const<i32>(0))), not<bool>(eq<u32>(or<u32>(read<u32>(field1(%18)), read<u32>(field0(%18))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))))));
// DEFAULT-NEXT:                 write<u32>(field1(%18), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u32>(field0(%18), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 orig: @type1 [storage=automatic];
// DEFAULT-NEXT:                 let %22 m: u32 [storage=automatic] = read<u32>(%20);
// DEFAULT-NEXT:                 write<@type1>(%21, copy<@type1, reason=assign>(read<@type1>(%18)));
// DEFAULT-NEXT:                 if ge<u64>(widen<u64, reason=usual_arith>(read<u32>(%22)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %44: u32 [synthetic] = read<u32>(%22);
// DEFAULT-NEXT:                         let %45: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%44)), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                         write<u32>(%22, read<u32>(%45));
// DEFAULT-NEXT:                         write<u32>(field0(%18), read<u32>(field1(%18)));
// DEFAULT-NEXT:                         write<u32>(field1(%18), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(%22), const<u32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u32>(field0(%18), or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(field0(%18)), read<u32>(%22)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(field1(%18)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), widen<u64, reason=usual_arith>(read<u32>(%22))))));
// DEFAULT-NEXT:                         let %46: u32 [synthetic] = read<u32>(field1(%18));
// DEFAULT-NEXT:                         let %47: u32 [synthetic] = shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%46), read<u32>(%22));
// DEFAULT-NEXT:                         write<u32>(field1(%18), read<u32>(%47));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<@type1>(%18, copy<@type1, reason=assign>(call<@type1, signature=fn(@type1, u32) -> @type1, abi=sysv64(coerce<i64, i64>, scalar) -> coerce<i64, i64>>(%8, copy<@type1, reason=arg>(read<@type1>(%18)), read<u32>(%19))));
// DEFAULT-NEXT:                 copy<@type1, reason=assign>(call<@type1, signature=fn(@type1, u32) -> @type1, abi=sysv64(coerce<i64, i64>, scalar) -> coerce<i64, i64>>(%8, copy<@type1, reason=arg>(read<@type1>(%18)), read<u32>(%19)));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(field2(%18)), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(field3(%18), const<i32>(0));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %23 maybe_orig: @type1 [storage=automatic] = copy<@type1, reason=assign>(call<@type1, signature=fn(@type1, u32, u32) -> @type1, abi=sysv64(coerce<i64, i64>, scalar, scalar) -> coerce<i64, i64>>(%11, copy<@type1, reason=arg>(read<@type1>(%18)), read<u32>(%19), read<u32>(%20)));
// DEFAULT-NEXT:                         write<i32>(field3(%18), from_bool<i32, reason=assign>(not<bool>(logical_and<bool>(eq<u32>(read<u32>(field1(%21)), read<u32>(field1(%23))), eq<u32>(read<u32>(field0(%21)), read<u32>(field0(%23)))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %28 res: @type1 [storage=automatic] = copy<@type1, reason=assign>(call<@type1, signature=fn(@type1, u32, u32) -> @type1, abi=sysv64(coerce<i64, i64>, scalar, scalar) -> coerce<i64, i64>>(%17, copy<@type1, reason=arg>(read<@type1>(%26)), reinterpret<u32, reason=arg, fits=always>(const<i32>(64)), read<u32>(%25)));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field1(%28)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(196608)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field0(%28)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field3(%28)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
