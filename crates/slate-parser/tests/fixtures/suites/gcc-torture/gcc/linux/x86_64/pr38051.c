typedef __SIZE_TYPE__ size_t;
static int            mymemcmp1(unsigned long int, unsigned long int)
    __attribute__((__nothrow__));

__inline static int mymemcmp1(unsigned long int a, unsigned long int b) {
  long int          srcp1 = (long int)&a;
  long int          srcp2 = (long int)&b;
  unsigned long int a0, b0;
  do {
    a0     = ((unsigned char *)srcp1)[0];
    b0     = ((unsigned char *)srcp2)[0];
    srcp1 += 1;
    srcp2 += 1;
  } while (a0 == b0);
  return a0 - b0;
}

static int mymemcmp2(long, long, size_t) __attribute__((__nothrow__));

static int mymemcmp2(long int srcp1, long int srcp2, size_t len) {
  unsigned long int a0, a1;
  unsigned long int b0, b1;
  switch (len % 4) {
  default:
  case 2:
    a0     = ((unsigned long int *)srcp1)[0];
    b0     = ((unsigned long int *)srcp2)[0];
    srcp1 -= 2 * (sizeof(unsigned long int));
    srcp2 -= 2 * (sizeof(unsigned long int));
    len   += 2;
    goto do1;
  case 3:
    a1     = ((unsigned long int *)srcp1)[0];
    b1     = ((unsigned long int *)srcp2)[0];
    srcp1 -= (sizeof(unsigned long int));
    srcp2 -= (sizeof(unsigned long int));
    len   += 1;
    goto do2;
  case 0:
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      return 0;
    a0 = ((unsigned long int *)srcp1)[0];
    b0 = ((unsigned long int *)srcp2)[0];
    goto do3;
  case 1:
    a1     = ((unsigned long int *)srcp1)[0];
    b1     = ((unsigned long int *)srcp2)[0];
    srcp1 += (sizeof(unsigned long int));
    srcp2 += (sizeof(unsigned long int));
    len   -= 1;
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      goto do0;
  }
  do {
    a0 = ((unsigned long int *)srcp1)[0];
    b0 = ((unsigned long int *)srcp2)[0];
    if (a1 != b1)
      return mymemcmp1((a1), (b1));
  do3:
    a1 = ((unsigned long int *)srcp1)[1];
    b1 = ((unsigned long int *)srcp2)[1];
    if (a0 != b0)
      return mymemcmp1((a0), (b0));
  do2:
    a0 = ((unsigned long int *)srcp1)[2];
    b0 = ((unsigned long int *)srcp2)[2];
    if (a1 != b1)
      return mymemcmp1((a1), (b1));
  do1:
    a1 = ((unsigned long int *)srcp1)[3];
    b1 = ((unsigned long int *)srcp2)[3];
    if (a0 != b0)
      return mymemcmp1((a0), (b0));
    srcp1 += 4 * (sizeof(unsigned long int));
    srcp2 += 4 * (sizeof(unsigned long int));
    len   -= 4;
  } while (len != 0);
do0:
  if (a1 != b1)
    return mymemcmp1((a1), (b1));
  return 0;
}

static int mymemcmp3(long, long, size_t) __attribute__((__nothrow__));

static int mymemcmp3(long int srcp1, long int srcp2, size_t len) {
  unsigned long int a0, a1, a2, a3;
  unsigned long int b0, b1, b2, b3;
  unsigned long int x;
  int               shl, shr;
  shl    = 8 * (srcp1 % (sizeof(unsigned long int)));
  shr    = 8 * (sizeof(unsigned long int)) - shl;
  srcp1 &= -(sizeof(unsigned long int));
  switch (len % 4) {
  default:
  case 2:
    a1     = ((unsigned long int *)srcp1)[0];
    a2     = ((unsigned long int *)srcp1)[1];
    b2     = ((unsigned long int *)srcp2)[0];
    srcp1 -= 1 * (sizeof(unsigned long int));
    srcp2 -= 2 * (sizeof(unsigned long int));
    len   += 2;
    goto do1;
  case 3:
    a0     = ((unsigned long int *)srcp1)[0];
    a1     = ((unsigned long int *)srcp1)[1];
    b1     = ((unsigned long int *)srcp2)[0];
    srcp2 -= 1 * (sizeof(unsigned long int));
    len   += 1;
    goto do2;
  case 0:
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      return 0;
    a3     = ((unsigned long int *)srcp1)[0];
    a0     = ((unsigned long int *)srcp1)[1];
    b0     = ((unsigned long int *)srcp2)[0];
    srcp1 += 1 * (sizeof(unsigned long int));
    goto do3;
  case 1:
    a2     = ((unsigned long int *)srcp1)[0];
    a3     = ((unsigned long int *)srcp1)[1];
    b3     = ((unsigned long int *)srcp2)[0];
    srcp1 += 2 * (sizeof(unsigned long int));
    srcp2 += 1 * (sizeof(unsigned long int));
    len   -= 1;
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      goto do0;
  }
  do {
    a0 = ((unsigned long int *)srcp1)[0];
    b0 = ((unsigned long int *)srcp2)[0];
    x  = (((a2) >> (shl)) | ((a3) << (shr)));
    if (x != b3)
      return mymemcmp1((x), (b3));
  do3:
    a1 = ((unsigned long int *)srcp1)[1];
    b1 = ((unsigned long int *)srcp2)[1];
    x  = (((a3) >> (shl)) | ((a0) << (shr)));
    if (x != b0)
      return mymemcmp1((x), (b0));
  do2:
    a2 = ((unsigned long int *)srcp1)[2];
    b2 = ((unsigned long int *)srcp2)[2];
    x  = (((a0) >> (shl)) | ((a1) << (shr)));
    if (x != b1)
      return mymemcmp1((x), (b1));
  do1:
    a3 = ((unsigned long int *)srcp1)[3];
    b3 = ((unsigned long int *)srcp2)[3];
    x  = (((a1) >> (shl)) | ((a2) << (shr)));
    if (x != b2)
      return mymemcmp1((x), (b2));
    srcp1 += 4 * (sizeof(unsigned long int));
    srcp2 += 4 * (sizeof(unsigned long int));
    len   -= 4;
  } while (len != 0);
do0:
  x = (((a2) >> (shl)) | ((a3) << (shr)));
  if (x != b3)
    return mymemcmp1((x), (b3));
  return 0;
}

__attribute__((noinline)) int mymemcmp(const void *s1, const void *s2,
                                       size_t len) {
  unsigned long int a0;
  unsigned long int b0;
  long int          srcp1 = (long int)s1;
  long int          srcp2 = (long int)s2;
  if (srcp1 % (sizeof(unsigned long int)) == 0)
    return mymemcmp2(srcp1, srcp2, len / (sizeof(unsigned long int)));
  else
    return mymemcmp3(srcp1, srcp2, len / (sizeof(unsigned long int)));
}

char buf[256];

int main(void) {
  char *p;
  union {
    long int l;
    char     c[sizeof(long int)];
  } u;

  /* The test above assumes little endian and long being the same size
     as pointer.  */
  if (sizeof(long int) != sizeof(void *) || sizeof(long int) < 4)
    return 0;
  u.l = 0x12345678L;
  if (u.c[0] != 0x78 || u.c[1] != 0x56 || u.c[2] != 0x34 || u.c[3] != 0x12)
    return 0;

  p = buf + 16 - (((long int)buf) & 15);
  __builtin_memcpy(
      p + 9, "\x1\x37\x82\xa7\x55\x49\x9d\xbf\xf8\x44\xb6\x55\x17\x8e\xf9", 15);
  __builtin_memcpy(
      p + 128 + 24,
      "\x1\x37\x82\xa7\x55\x49\xd0\xf3\xb7\x2a\x6d\x23\x71\x49\x6a", 15);
  if (mymemcmp(p + 9, p + 128 + 24, 33) != -51)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 l: i64;
// DEFAULT-NEXT:         field1 c: array<i8, 8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_buf:[0-9]+]] buf: array<i8, 256> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([1, 55, 130, 167, 85, 73, 157, 191, 248, 68, 182, 85, 23, 142, 249, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([1, 55, 130, 167, 85, 73, 208, 243, 183, 42, 109, 35, 113, 73, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_mymemcmp1:[0-9]+]] @mymemcmp1(%[[VALUE_a:[0-9]+]] a: u64, %[[VALUE_b:[0-9]+]] b: u64) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_srcp1:[0-9]+]] srcp1: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(addr_of<ptr<u64>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE_srcp2:[0-9]+]] srcp2: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(addr_of<ptr<u64>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_a0:[0-9]+]] a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b0:[0-9]+]] b0: u64 [storage=automatic];
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a0]], widen<u64, reason=assign>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(int_to_ptr<ptr<u8>, reason=explicit>(read<i64>(%[[VALUE_srcp1]])), const<i32>(0))))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b0]], widen<u64, reason=assign>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(int_to_ptr<ptr<u8>, reason=explicit>(read<i64>(%[[VALUE_srcp2]])), const<i32>(0))))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE1]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1]], read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE3]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2]], read<i64>(%[[VALUE4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while eq<u64>(read<u64>(%[[VALUE_a0]]), read<u64>(%[[VALUE_b0]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_a0]]), read<u64>(%[[VALUE_b0]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mymemcmp2:[0-9]+]] @mymemcmp2(%[[VALUE_srcp1_2:[0-9]+]] srcp1: i64, %[[VALUE_srcp2_2:[0-9]+]] srcp2: i64, %[[VALUE_len:[0-9]+]] len: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a0_2:[0-9]+]] a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b0_2:[0-9]+]] b0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b1:[0-9]+]] b1: u64 [storage=automatic];
// DEFAULT-NEXT:         switch %[[VALUE5:[0-9]+]] rem<u64, by_zero=ub>(read<u64>(%[[VALUE_len]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 default %[[VALUE5]]:
// DEFAULT-NEXT:                     case %[[VALUE5]] const<u64>(2):
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_a0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE6]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_2]], read<i64>(%[[VALUE7]]));
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE8]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_2]], read<i64>(%[[VALUE9]]));
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE10]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE11]]));
// DEFAULT-NEXT:                 goto %[[VALUE_do1:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE5]] const<u64>(3):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_2]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE12]])), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_2]], read<i64>(%[[VALUE13]]));
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_2]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE14]])), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_2]], read<i64>(%[[VALUE15]]));
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE16]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE17]]));
// DEFAULT-NEXT:                 goto %[[VALUE_do2:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE5]] const<u64>(0):
// DEFAULT-NEXT:                     if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%[[VALUE_len]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 goto %[[VALUE_do3:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE5]] const<u64>(1):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_2]]);
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE18]])), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_2]], read<i64>(%[[VALUE19]]));
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_2]]);
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE20]])), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_2]], read<i64>(%[[VALUE21]]));
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%[[VALUE22]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE23]]));
// DEFAULT-NEXT:                 if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%[[VALUE_len]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                     goto %[[VALUE_do0:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(0)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_a1]]), read<u64>(%[[VALUE_b1]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_a1]]), read<u64>(%[[VALUE_b1]]));
// DEFAULT-NEXT:                 label %[[VALUE_do3]] do3:
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(1)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_a0_2]]), read<u64>(%[[VALUE_b0_2]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_a0_2]]), read<u64>(%[[VALUE_b0_2]]));
// DEFAULT-NEXT:                 label %[[VALUE_do2]] do2:
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(2)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b0_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(2)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_a1]]), read<u64>(%[[VALUE_b1]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_a1]]), read<u64>(%[[VALUE_b1]]));
// DEFAULT-NEXT:                 label %[[VALUE_do1]] do1:
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_2]])), const<i32>(3)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b1]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_2]])), const<i32>(3)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_a0_2]]), read<u64>(%[[VALUE_b0_2]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_a0_2]]), read<u64>(%[[VALUE_b0_2]]));
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_2]]);
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE25]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_2]], read<i64>(%[[VALUE26]]));
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_2]]);
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE27]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_2]], read<i64>(%[[VALUE28]]));
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%[[VALUE29]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE30]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%[[VALUE_len]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         label %[[VALUE_do0]] do0:
// DEFAULT-NEXT:             if ne<u64>(read<u64>(%[[VALUE_a1]]), read<u64>(%[[VALUE_b1]]))
// DEFAULT-NEXT:                 return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_a1]]), read<u64>(%[[VALUE_b1]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mymemcmp3:[0-9]+]] @mymemcmp3(%[[VALUE_srcp1_3:[0-9]+]] srcp1: i64, %[[VALUE_srcp2_3:[0-9]+]] srcp2: i64, %[[VALUE_len_2:[0-9]+]] len: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a0_3:[0-9]+]] a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a1_2:[0-9]+]] a1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a3:[0-9]+]] a3: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b0_3:[0-9]+]] b0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b1_2:[0-9]+]] b1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b2:[0-9]+]] b2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b3:[0-9]+]] b3: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_shl:[0-9]+]] shl: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_shr:[0-9]+]] shr: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_shl]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), rem<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_srcp1_3]])), const<u64>(8))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_shr]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shl]])))))));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_3]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE31]])), neg<u64, overflow=wrap>(const<u64>(8))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_srcp1_3]], read<i64>(%[[VALUE32]]));
// DEFAULT-NEXT:         switch %[[VALUE33:[0-9]+]] rem<u64, by_zero=ub>(read<u64>(%[[VALUE_len_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 default %[[VALUE33]]:
// DEFAULT-NEXT:                     case %[[VALUE33]] const<u64>(2):
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_a1_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_3]]);
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE34]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_3]], read<i64>(%[[VALUE35]]));
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_3]]);
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE36]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_3]], read<i64>(%[[VALUE37]]));
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE38]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len_2]], read<u64>(%[[VALUE39]]));
// DEFAULT-NEXT:                 goto %[[VALUE_do1_2:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE33]] const<u64>(3):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a0_3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a1_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b1_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_3]]);
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE40]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_3]], read<i64>(%[[VALUE41]]));
// DEFAULT-NEXT:                 let %[[VALUE42:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE43:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE42]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len_2]], read<u64>(%[[VALUE43]]));
// DEFAULT-NEXT:                 goto %[[VALUE_do2_2:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE33]] const<u64>(0):
// DEFAULT-NEXT:                     if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%[[VALUE_len_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a0_3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b0_3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_3]]);
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE44]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_3]], read<i64>(%[[VALUE45]]));
// DEFAULT-NEXT:                 goto %[[VALUE_do3_2:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE33]] const<u64>(1):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE46:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_3]]);
// DEFAULT-NEXT:                 let %[[VALUE47:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE46]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_3]], read<i64>(%[[VALUE47]]));
// DEFAULT-NEXT:                 let %[[VALUE48:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_3]]);
// DEFAULT-NEXT:                 let %[[VALUE49:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE48]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_3]], read<i64>(%[[VALUE49]]));
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%[[VALUE50]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len_2]], read<u64>(%[[VALUE51]]));
// DEFAULT-NEXT:                 if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%[[VALUE_len_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                     goto %[[VALUE_do0_2:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_a0_3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b0_3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x]], or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_a2]]), read<i32>(%[[VALUE_shl]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_a3]]), read<i32>(%[[VALUE_shr]]))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b3]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b3]]));
// DEFAULT-NEXT:                 label %[[VALUE_do3_2]] do3:
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a1_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b1_2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x]], or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_a3]]), read<i32>(%[[VALUE_shl]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_a0_3]]), read<i32>(%[[VALUE_shr]]))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b0_3]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b0_3]]));
// DEFAULT-NEXT:                 label %[[VALUE_do2_2]] do2:
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(2)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b2]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(2)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x]], or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_a0_3]]), read<i32>(%[[VALUE_shl]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_a1_2]]), read<i32>(%[[VALUE_shr]]))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b1_2]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b1_2]]));
// DEFAULT-NEXT:                 label %[[VALUE_do1_2]] do1:
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_a3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp1_3]])), const<i32>(3)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_b3]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%[[VALUE_srcp2_3]])), const<i32>(3)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x]], or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_a1_2]]), read<i32>(%[[VALUE_shl]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_a2]]), read<i32>(%[[VALUE_shr]]))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b2]]))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b2]]));
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp1_3]]);
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE53]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp1_3]], read<i64>(%[[VALUE54]]));
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_srcp2_3]]);
// DEFAULT-NEXT:                 let %[[VALUE56:[0-9]+]]: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE55]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_srcp2_3]], read<i64>(%[[VALUE56]]));
// DEFAULT-NEXT:                 let %[[VALUE57:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE58:[0-9]+]]: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%[[VALUE57]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len_2]], read<u64>(%[[VALUE58]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%[[VALUE_len_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         label %[[VALUE_do0_2]] do0:
// DEFAULT-NEXT:             write<u64>(%[[VALUE_x]], or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_a2]]), read<i32>(%[[VALUE_shl]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_a3]]), read<i32>(%[[VALUE_shr]]))));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b3]]))
// DEFAULT-NEXT:             return call<i32, signature=fn(u64, u64) -> i32>(%[[VALUE_mymemcmp1]], read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_b3]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mymemcmp:[0-9]+]] @mymemcmp(%[[VALUE_s1:[0-9]+]] s1: ptr<const void>, %[[VALUE_s2:[0-9]+]] s2: ptr<const void>, %[[VALUE_len_3:[0-9]+]] len: u64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a0_4:[0-9]+]] a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b0_4:[0-9]+]] b0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_srcp1_4:[0-9]+]] srcp1: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(read<ptr<const void>>(%[[VALUE_s1]]));
// DEFAULT-NEXT:         let %[[VALUE_srcp2_4:[0-9]+]] srcp2: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(read<ptr<const void>>(%[[VALUE_s2]]));
// DEFAULT-NEXT:         if eq<u64>(rem<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_srcp1_4]])), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return call<i32, signature=fn(i64, i64, u64) -> i32>(%[[VALUE_mymemcmp2]], read<i64>(%[[VALUE_srcp1_4]]), read<i64>(%[[VALUE_srcp2_4]]), div<u64, by_zero=ub>(read<u64>(%[[VALUE_len_3]]), const<u64>(8)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return call<i32, signature=fn(i64, i64, u64) -> i32>(%[[VALUE_mymemcmp3]], read<i64>(%[[VALUE_srcp1_4]]), read<i64>(%[[VALUE_srcp2_4]]), div<u64, by_zero=ub>(read<u64>(%[[VALUE_len_3]]), const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE59:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE60:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE61:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(const<u64>(8), const<u64>(8)), lt<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_u]]), const<i64>(305419896));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%[[VALUE_u]])), const<i32>(0))))), const<i32>(120)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%[[VALUE_u]])), const<i32>(1))))), const<i32>(86))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%[[VALUE_u]])), const<i32>(2))))), const<i32>(52))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%[[VALUE_u]])), const<i32>(3))))), const<i32>(18)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_p]], ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_buf]]), const<i32>(16)), and<i64>(ptr_to_int<i64, reason=explicit>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_buf]])), widen<i64, reason=usual_arith>(const<i32>(15)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(9))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(128)), const<i32>(24))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_mymemcmp]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(9))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(128)), const<i32>(24))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(33)))), neg<i32, overflow=ub>(const<i32>(51)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
