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
// DEFAULT-NEXT:     type @type[[TYPE_cpp_num_part:[0-9]+]] cpp_num_part = u32;
// DEFAULT-NEXT:     type @type[[TYPE_cpp_num:[0-9]+]] cpp_num = struct {
// DEFAULT-NEXT:         field0 high: u32;
// DEFAULT-NEXT:         field1 low: u32;
// DEFAULT-NEXT:         field2 unsignedp: i32;
// DEFAULT-NEXT:         field3 overflow: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_cpp_num_2:[0-9]+]] cpp_num = @type[[TYPE_cpp_num]];
// DEFAULT-NEXT:     global %[[VALUE_precision:[0-9]+]] precision: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(64)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(16)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_num:[0-9]+]] num: @type[[TYPE_cpp_num]] [storage=static] = aggregate<@type[[TYPE_cpp_num]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(3)), field2 = const<i32>(0), field3 = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_num_positive:[0-9]+]] @num_positive(%[[VALUE_num_2:[0-9]+]] num: @type[[TYPE_cpp_num]], %[[VALUE_precision_2:[0-9]+]] precision: u32) -> i32 [linkage=internal] [abi=sysv64(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_precision_2]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_precision_2]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE1]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_precision_2]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<u32>(and<u32>(read<u32>(field0(%[[VALUE_num_2]])), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_precision_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<u32>(and<u32>(read<u32>(field1(%[[VALUE_num_2]])), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_precision_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_num_trim:[0-9]+]] @num_trim(%[[VALUE_num_3:[0-9]+]] num: @type[[TYPE_cpp_num]], %[[VALUE_precision_3:[0-9]+]] precision: u32) -> @type[[TYPE_cpp_num]] [linkage=internal] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_precision_3]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_precision_3]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE3]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_precision_3]], read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_precision_3]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: u32 [synthetic] = read<u32>(field0(%[[VALUE_num_3]]));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: u32 [synthetic] = and<u32>(read<u32>(%[[VALUE5]]), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<u32>(%[[VALUE_precision_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                     write<u32>(field0(%[[VALUE_num_3]]), read<u32>(%[[VALUE6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_precision_3]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: u32 [synthetic] = read<u32>(field1(%[[VALUE_num_3]]));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: u32 [synthetic] = and<u32>(read<u32>(%[[VALUE7]]), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<u32>(%[[VALUE_precision_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                     write<u32>(field1(%[[VALUE_num_3]]), read<u32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 write<u32>(field0(%[[VALUE_num_3]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return copy<@type[[TYPE_cpp_num]], reason=return>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_num_rshift:[0-9]+]] @num_rshift(%[[VALUE_num_4:[0-9]+]] num: @type[[TYPE_cpp_num]], %[[VALUE_precision_4:[0-9]+]] precision: u32, %[[VALUE_n_2:[0-9]+]] n: u32) -> @type[[TYPE_cpp_num]] [linkage=internal] [abi=sysv64(native_c, scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_sign_mask:[0-9]+]] sign_mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = call<i32, signature=fn(@type[[TYPE_cpp_num]], u32) -> i32, abi=sysv64(native_c, scalar) -> scalar>(%[[VALUE_num_positive]], copy<@type[[TYPE_cpp_num]], reason=arg>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_4]])), read<u32>(%[[VALUE_precision_4]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field2(%[[VALUE_num_4]])), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_sign_mask]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE_sign_mask]], not<u32>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ge<u32>(read<u32>(%[[VALUE_n_2]]), read<u32>(%[[VALUE_precision_4]]))
// DEFAULT-NEXT:             write<u32>(field1(%[[VALUE_num_4]]), read<u32>(%[[VALUE_sign_mask]]));
// DEFAULT-NEXT:             write<u32>(field0(%[[VALUE_num_4]]), read<u32>(%[[VALUE_sign_mask]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_precision_4]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     write<u32>(field0(%[[VALUE_num_4]]), read<u32>(%[[VALUE_sign_mask]]));
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: u32 [synthetic] = read<u32>(field1(%[[VALUE_num_4]]));
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: u32 [synthetic] = or<u32>(read<u32>(%[[VALUE9]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_sign_mask]]), read<u32>(%[[VALUE_precision_4]])));
// DEFAULT-NEXT:                     write<u32>(field1(%[[VALUE_num_4]]), read<u32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_precision_4]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))))
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: u32 [synthetic] = read<u32>(field0(%[[VALUE_num_4]]));
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: u32 [synthetic] = or<u32>(read<u32>(%[[VALUE11]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_sign_mask]]), sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_precision_4]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))));
// DEFAULT-NEXT:                         write<u32>(field0(%[[VALUE_num_4]]), read<u32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 if ge<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_n_2]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_n_2]]);
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE13]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_n_2]], read<u32>(%[[VALUE14]]));
// DEFAULT-NEXT:                         write<u32>(field1(%[[VALUE_num_4]]), read<u32>(field0(%[[VALUE_num_4]])));
// DEFAULT-NEXT:                         write<u32>(field0(%[[VALUE_num_4]]), read<u32>(%[[VALUE_sign_mask]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(%[[VALUE_n_2]]), const<u32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u32>(field1(%[[VALUE_num_4]]), or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(field1(%[[VALUE_num_4]])), read<u32>(%[[VALUE_n_2]])), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(field0(%[[VALUE_num_4]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_n_2]]))))));
// DEFAULT-NEXT:                         write<u32>(field0(%[[VALUE_num_4]]), or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(field0(%[[VALUE_num_4]])), read<u32>(%[[VALUE_n_2]])), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_sign_mask]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_n_2]]))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type[[TYPE_cpp_num]]>(%[[VALUE_num_4]], copy<@type[[TYPE_cpp_num]], reason=assign>(call<@type[[TYPE_cpp_num]], signature=fn(@type[[TYPE_cpp_num]], u32) -> @type[[TYPE_cpp_num]], abi=sysv64(native_c, scalar) -> native_c>(%[[VALUE_num_trim]], copy<@type[[TYPE_cpp_num]], reason=arg>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_4]])), read<u32>(%[[VALUE_precision_4]]))));
// DEFAULT-NEXT:         copy<@type[[TYPE_cpp_num]], reason=assign>(call<@type[[TYPE_cpp_num]], signature=fn(@type[[TYPE_cpp_num]], u32) -> @type[[TYPE_cpp_num]], abi=sysv64(native_c, scalar) -> native_c>(%[[VALUE_num_trim]], copy<@type[[TYPE_cpp_num]], reason=arg>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_4]])), read<u32>(%[[VALUE_precision_4]])));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_num_4]]), const<i32>(0));
// DEFAULT-NEXT:         return copy<@type[[TYPE_cpp_num]], reason=return>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_num_lshift:[0-9]+]] @num_lshift(%[[VALUE_num_5:[0-9]+]] num: @type[[TYPE_cpp_num]], %[[VALUE_precision_5:[0-9]+]] precision: u32, %[[VALUE_n_3:[0-9]+]] n: u32) -> @type[[TYPE_cpp_num]] [linkage=external] [abi=sysv64(native_c, scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ge<u32>(read<u32>(%[[VALUE_n_3]]), read<u32>(%[[VALUE_precision_5]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(field3(%[[VALUE_num_5]]), from_bool<i32, reason=assign>(logical_and<bool>(not<bool>(ne<i32>(read<i32>(field2(%[[VALUE_num_5]])), const<i32>(0))), not<bool>(eq<u32>(or<u32>(read<u32>(field1(%[[VALUE_num_5]])), read<u32>(field0(%[[VALUE_num_5]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))))));
// DEFAULT-NEXT:                 write<u32>(field1(%[[VALUE_num_5]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u32>(field0(%[[VALUE_num_5]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_orig:[0-9]+]] orig: @type[[TYPE_cpp_num]] [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_m:[0-9]+]] m: u32 [storage=automatic] = read<u32>(%[[VALUE_n_3]]);
// DEFAULT-NEXT:                 write<@type[[TYPE_cpp_num]]>(%[[VALUE_orig]], copy<@type[[TYPE_cpp_num]], reason=assign>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_5]])));
// DEFAULT-NEXT:                 if ge<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_m]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE15:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_m]]);
// DEFAULT-NEXT:                         let %[[VALUE16:[0-9]+]]: u32 [synthetic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE15]])), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_m]], read<u32>(%[[VALUE16]]));
// DEFAULT-NEXT:                         write<u32>(field0(%[[VALUE_num_5]]), read<u32>(field1(%[[VALUE_num_5]])));
// DEFAULT-NEXT:                         write<u32>(field1(%[[VALUE_num_5]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(%[[VALUE_m]]), const<u32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u32>(field0(%[[VALUE_num_5]]), or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(field0(%[[VALUE_num_5]])), read<u32>(%[[VALUE_m]])), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(field1(%[[VALUE_num_5]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_m]]))))));
// DEFAULT-NEXT:                         let %[[VALUE17:[0-9]+]]: u32 [synthetic] = read<u32>(field1(%[[VALUE_num_5]]));
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: u32 [synthetic] = shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE17]]), read<u32>(%[[VALUE_m]]));
// DEFAULT-NEXT:                         write<u32>(field1(%[[VALUE_num_5]]), read<u32>(%[[VALUE18]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<@type[[TYPE_cpp_num]]>(%[[VALUE_num_5]], copy<@type[[TYPE_cpp_num]], reason=assign>(call<@type[[TYPE_cpp_num]], signature=fn(@type[[TYPE_cpp_num]], u32) -> @type[[TYPE_cpp_num]], abi=sysv64(native_c, scalar) -> native_c>(%[[VALUE_num_trim]], copy<@type[[TYPE_cpp_num]], reason=arg>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_5]])), read<u32>(%[[VALUE_precision_5]]))));
// DEFAULT-NEXT:                 copy<@type[[TYPE_cpp_num]], reason=assign>(call<@type[[TYPE_cpp_num]], signature=fn(@type[[TYPE_cpp_num]], u32) -> @type[[TYPE_cpp_num]], abi=sysv64(native_c, scalar) -> native_c>(%[[VALUE_num_trim]], copy<@type[[TYPE_cpp_num]], reason=arg>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_5]])), read<u32>(%[[VALUE_precision_5]])));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(field2(%[[VALUE_num_5]])), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(field3(%[[VALUE_num_5]]), const<i32>(0));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_maybe_orig:[0-9]+]] maybe_orig: @type[[TYPE_cpp_num]] [storage=automatic] = copy<@type[[TYPE_cpp_num]], reason=assign>(call<@type[[TYPE_cpp_num]], signature=fn(@type[[TYPE_cpp_num]], u32, u32) -> @type[[TYPE_cpp_num]], abi=sysv64(native_c, scalar, scalar) -> native_c>(%[[VALUE_num_rshift]], copy<@type[[TYPE_cpp_num]], reason=arg>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_5]])), read<u32>(%[[VALUE_precision_5]]), read<u32>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:                         write<i32>(field3(%[[VALUE_num_5]]), from_bool<i32, reason=assign>(not<bool>(logical_and<bool>(eq<u32>(read<u32>(field1(%[[VALUE_orig]])), read<u32>(field1(%[[VALUE_maybe_orig]]))), eq<u32>(read<u32>(field0(%[[VALUE_orig]])), read<u32>(field0(%[[VALUE_maybe_orig]])))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return copy<@type[[TYPE_cpp_num]], reason=return>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: @type[[TYPE_cpp_num]] [storage=automatic] = copy<@type[[TYPE_cpp_num]], reason=assign>(call<@type[[TYPE_cpp_num]], signature=fn(@type[[TYPE_cpp_num]], u32, u32) -> @type[[TYPE_cpp_num]], abi=sysv64(native_c, scalar, scalar) -> native_c>(%[[VALUE_num_lshift]], copy<@type[[TYPE_cpp_num]], reason=arg>(read<@type[[TYPE_cpp_num]]>(%[[VALUE_num]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(64)), read<u32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field1(%[[VALUE_res]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(196608)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field0(%[[VALUE_res]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field3(%[[VALUE_res]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
