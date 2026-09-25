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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 l: i64;
// DEFAULT-NEXT:         field1 c: array<i8, 8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %47 buf: array<i8, 256> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %65 .str65: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([1, 55, 130, 167, 85, 73, 157, 191, 248, 68, 182, 85, 23, 142, 249, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([1, 55, 130, 167, 85, 73, 208, 243, 183, 42, 109, 35, 113, 73, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @mymemcmp1(%2 a: u64, %3 b: u64) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 srcp1: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(addr_of<ptr<u64>>(%2));
// DEFAULT-NEXT:         let %5 srcp2: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(addr_of<ptr<u64>>(%3));
// DEFAULT-NEXT:         let %6 a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %7 b0: u64 [storage=automatic];
// DEFAULT-NEXT:         do %54
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%6, widen<u64, reason=assign>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(int_to_ptr<ptr<u8>, reason=explicit>(read<i64>(%4)), const<i32>(0))))));
// DEFAULT-NEXT:                 write<u64>(%7, widen<u64, reason=assign>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(int_to_ptr<ptr<u8>, reason=explicit>(read<i64>(%5)), const<i32>(0))))));
// DEFAULT-NEXT:                 let %67: i64 [synthetic] = read<i64>(%4);
// DEFAULT-NEXT:                 let %68: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%67), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%4, read<i64>(%68));
// DEFAULT-NEXT:                 let %69: i64 [synthetic] = read<i64>(%5);
// DEFAULT-NEXT:                 let %70: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%69), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%5, read<i64>(%70));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while eq<u64>(read<u64>(%6), read<u64>(%7));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%6), read<u64>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @mymemcmp2(%13 srcp1: i64, %14 srcp2: i64, %15 len: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %17 a1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %18 b0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %19 b1: u64 [storage=automatic];
// DEFAULT-NEXT:         switch %58 rem<u64, by_zero=ub>(read<u64>(%15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 default %58:
// DEFAULT-NEXT:                     case %58 const<u64>(2):
// DEFAULT-NEXT:                         write<u64>(%16, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%18, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(0)))));
// DEFAULT-NEXT:                 let %71: i64 [synthetic] = read<i64>(%13);
// DEFAULT-NEXT:                 let %72: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%71)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%13, read<i64>(%72));
// DEFAULT-NEXT:                 let %73: i64 [synthetic] = read<i64>(%14);
// DEFAULT-NEXT:                 let %74: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%73)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%14, read<i64>(%74));
// DEFAULT-NEXT:                 let %75: u64 [synthetic] = read<u64>(%15);
// DEFAULT-NEXT:                 let %76: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%75), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:                 write<u64>(%15, read<u64>(%76));
// DEFAULT-NEXT:                 goto %11;
// DEFAULT-NEXT:                 case %58 const<u64>(3):
// DEFAULT-NEXT:                     write<u64>(%17, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%19, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(0)))));
// DEFAULT-NEXT:                 let %77: i64 [synthetic] = read<i64>(%13);
// DEFAULT-NEXT:                 let %78: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%77)), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%13, read<i64>(%78));
// DEFAULT-NEXT:                 let %79: i64 [synthetic] = read<i64>(%14);
// DEFAULT-NEXT:                 let %80: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%79)), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%14, read<i64>(%80));
// DEFAULT-NEXT:                 let %81: u64 [synthetic] = read<u64>(%15);
// DEFAULT-NEXT:                 let %82: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%81), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%15, read<u64>(%82));
// DEFAULT-NEXT:                 goto %10;
// DEFAULT-NEXT:                 case %58 const<u64>(0):
// DEFAULT-NEXT:                     if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 write<u64>(%16, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%18, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(0)))));
// DEFAULT-NEXT:                 goto %9;
// DEFAULT-NEXT:                 case %58 const<u64>(1):
// DEFAULT-NEXT:                     write<u64>(%17, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%19, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(0)))));
// DEFAULT-NEXT:                 let %83: i64 [synthetic] = read<i64>(%13);
// DEFAULT-NEXT:                 let %84: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%83)), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%13, read<i64>(%84));
// DEFAULT-NEXT:                 let %85: i64 [synthetic] = read<i64>(%14);
// DEFAULT-NEXT:                 let %86: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%85)), const<u64>(8)));
// DEFAULT-NEXT:                 write<i64>(%14, read<i64>(%86));
// DEFAULT-NEXT:                 let %87: u64 [synthetic] = read<u64>(%15);
// DEFAULT-NEXT:                 let %88: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%87), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%15, read<u64>(%88));
// DEFAULT-NEXT:                 if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                     goto %12;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         do %59
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%16, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%18, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(0)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%17), read<u64>(%19))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%17), read<u64>(%19));
// DEFAULT-NEXT:                 label %9 do3:
// DEFAULT-NEXT:                     write<u64>(%17, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%19, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(1)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%16), read<u64>(%18))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%16), read<u64>(%18));
// DEFAULT-NEXT:                 label %10 do2:
// DEFAULT-NEXT:                     write<u64>(%16, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(2)))));
// DEFAULT-NEXT:                 write<u64>(%18, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(2)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%17), read<u64>(%19))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%17), read<u64>(%19));
// DEFAULT-NEXT:                 label %11 do1:
// DEFAULT-NEXT:                     write<u64>(%17, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%13)), const<i32>(3)))));
// DEFAULT-NEXT:                 write<u64>(%19, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%14)), const<i32>(3)))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%16), read<u64>(%18))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%16), read<u64>(%18));
// DEFAULT-NEXT:                 let %89: i64 [synthetic] = read<i64>(%13);
// DEFAULT-NEXT:                 let %90: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%89)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%13, read<i64>(%90));
// DEFAULT-NEXT:                 let %91: i64 [synthetic] = read<i64>(%14);
// DEFAULT-NEXT:                 let %92: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%91)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%14, read<i64>(%92));
// DEFAULT-NEXT:                 let %93: u64 [synthetic] = read<u64>(%15);
// DEFAULT-NEXT:                 let %94: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%93), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))));
// DEFAULT-NEXT:                 write<u64>(%15, read<u64>(%94));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         label %12 do0:
// DEFAULT-NEXT:             if ne<u64>(read<u64>(%17), read<u64>(%19))
// DEFAULT-NEXT:                 return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%17), read<u64>(%19));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @mymemcmp3(%25 srcp1: i64, %26 srcp2: i64, %27 len: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %29 a1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %30 a2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %31 a3: u64 [storage=automatic];
// DEFAULT-NEXT:         let %32 b0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %33 b1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %34 b2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %35 b3: u64 [storage=automatic];
// DEFAULT-NEXT:         let %36 x: u64 [storage=automatic];
// DEFAULT-NEXT:         let %37 shl: i32 [storage=automatic];
// DEFAULT-NEXT:         let %38 shr: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%37, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), rem<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%25)), const<u64>(8))))));
// DEFAULT-NEXT:         write<i32>(%38, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%37)))))));
// DEFAULT-NEXT:         let %95: i64 [synthetic] = read<i64>(%25);
// DEFAULT-NEXT:         let %96: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%95)), neg<u64, overflow=wrap>(const<u64>(8))));
// DEFAULT-NEXT:         write<i64>(%25, read<i64>(%96));
// DEFAULT-NEXT:         switch %63 rem<u64, by_zero=ub>(read<u64>(%27), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 default %63:
// DEFAULT-NEXT:                     case %63 const<u64>(2):
// DEFAULT-NEXT:                         write<u64>(%29, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%30, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%34, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(0)))));
// DEFAULT-NEXT:                 let %97: i64 [synthetic] = read<i64>(%25);
// DEFAULT-NEXT:                 let %98: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%97)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%25, read<i64>(%98));
// DEFAULT-NEXT:                 let %99: i64 [synthetic] = read<i64>(%26);
// DEFAULT-NEXT:                 let %100: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%99)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%26, read<i64>(%100));
// DEFAULT-NEXT:                 let %101: u64 [synthetic] = read<u64>(%27);
// DEFAULT-NEXT:                 let %102: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%101), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:                 write<u64>(%27, read<u64>(%102));
// DEFAULT-NEXT:                 goto %23;
// DEFAULT-NEXT:                 case %63 const<u64>(3):
// DEFAULT-NEXT:                     write<u64>(%28, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%29, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%33, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(0)))));
// DEFAULT-NEXT:                 let %103: i64 [synthetic] = read<i64>(%26);
// DEFAULT-NEXT:                 let %104: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%103)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%26, read<i64>(%104));
// DEFAULT-NEXT:                 let %105: u64 [synthetic] = read<u64>(%27);
// DEFAULT-NEXT:                 let %106: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%105), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%27, read<u64>(%106));
// DEFAULT-NEXT:                 goto %22;
// DEFAULT-NEXT:                 case %63 const<u64>(0):
// DEFAULT-NEXT:                     if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%27), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 write<u64>(%31, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%28, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%32, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(0)))));
// DEFAULT-NEXT:                 let %107: i64 [synthetic] = read<i64>(%25);
// DEFAULT-NEXT:                 let %108: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%107)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%25, read<i64>(%108));
// DEFAULT-NEXT:                 goto %21;
// DEFAULT-NEXT:                 case %63 const<u64>(1):
// DEFAULT-NEXT:                     write<u64>(%30, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%31, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%35, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(0)))));
// DEFAULT-NEXT:                 let %109: i64 [synthetic] = read<i64>(%25);
// DEFAULT-NEXT:                 let %110: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%109)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%25, read<i64>(%110));
// DEFAULT-NEXT:                 let %111: i64 [synthetic] = read<i64>(%26);
// DEFAULT-NEXT:                 let %112: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%111)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%26, read<i64>(%112));
// DEFAULT-NEXT:                 let %113: u64 [synthetic] = read<u64>(%27);
// DEFAULT-NEXT:                 let %114: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%113), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%27, read<u64>(%114));
// DEFAULT-NEXT:                 if logical_and<bool>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8))), eq<u64>(read<u64>(%27), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:                     goto %24;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         do %64
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%28, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%32, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u64>(%36, or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%30), read<i32>(%37)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%31), read<i32>(%38))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%36), read<u64>(%35))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%36), read<u64>(%35));
// DEFAULT-NEXT:                 label %21 do3:
// DEFAULT-NEXT:                     write<u64>(%29, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%33, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%36, or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%31), read<i32>(%37)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%28), read<i32>(%38))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%36), read<u64>(%32))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%36), read<u64>(%32));
// DEFAULT-NEXT:                 label %22 do2:
// DEFAULT-NEXT:                     write<u64>(%30, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(2)))));
// DEFAULT-NEXT:                 write<u64>(%34, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(2)))));
// DEFAULT-NEXT:                 write<u64>(%36, or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%28), read<i32>(%37)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%29), read<i32>(%38))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%36), read<u64>(%33))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%36), read<u64>(%33));
// DEFAULT-NEXT:                 label %23 do1:
// DEFAULT-NEXT:                     write<u64>(%31, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%25)), const<i32>(3)))));
// DEFAULT-NEXT:                 write<u64>(%35, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(int_to_ptr<ptr<u64>, reason=explicit>(read<i64>(%26)), const<i32>(3)))));
// DEFAULT-NEXT:                 write<u64>(%36, or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%29), read<i32>(%37)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%30), read<i32>(%38))));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%36), read<u64>(%34))
// DEFAULT-NEXT:                     return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%36), read<u64>(%34));
// DEFAULT-NEXT:                 let %115: i64 [synthetic] = read<i64>(%25);
// DEFAULT-NEXT:                 let %116: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%115)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%25, read<i64>(%116));
// DEFAULT-NEXT:                 let %117: i64 [synthetic] = read<i64>(%26);
// DEFAULT-NEXT:                 let %118: i64 [synthetic] = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%117)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i64>(%26, read<i64>(%118));
// DEFAULT-NEXT:                 let %119: u64 [synthetic] = read<u64>(%27);
// DEFAULT-NEXT:                 let %120: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%119), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))));
// DEFAULT-NEXT:                 write<u64>(%27, read<u64>(%120));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%27), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         label %24 do0:
// DEFAULT-NEXT:             write<u64>(%36, or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%30), read<i32>(%37)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%31), read<i32>(%38))));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%36), read<u64>(%35))
// DEFAULT-NEXT:             return call<i32, signature=fn(u64, u64) -> i32>(%1, read<u64>(%36), read<u64>(%35));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @mymemcmp(%40 s1: ptr<const void>, %41 s2: ptr<const void>, %42 len: u64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 a0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %44 b0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %45 srcp1: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(read<ptr<const void>>(%40));
// DEFAULT-NEXT:         let %46 srcp2: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(read<ptr<const void>>(%41));
// DEFAULT-NEXT:         if eq<u64>(rem<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%45)), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return call<i32, signature=fn(i64, i64, u64) -> i32>(%8, read<i64>(%45), read<i64>(%46), div<u64, by_zero=ub>(read<u64>(%42), const<u64>(8)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return call<i32, signature=fn(i64, i64, u64) -> i32>(%20, read<i64>(%45), read<i64>(%46), div<u64, by_zero=ub>(read<u64>(%42), const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %49 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %51 u: @type1 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(const<u64>(8), const<u64>(8)), lt<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i64>(field0(%51), const<i64>(305419896));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%51)), const<i32>(0))))), const<i32>(120)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%51)), const<i32>(1))))), const<i32>(86))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%51)), const<i32>(2))))), const<i32>(52))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field1(%51)), const<i32>(3))))), const<i32>(18)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<ptr<i8>>(%49, ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%47), const<i32>(16)), and<i64>(ptr_to_int<i64, reason=explicit>(array_decay<ptr<i8>, length=Some(256)>(%47)), widen<i64, reason=usual_arith>(const<i32>(15)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%49), const<i32>(9))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%65)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%49), const<i32>(128)), const<i32>(24))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%66)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%39, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%49), const<i32>(9))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%49), const<i32>(128)), const<i32>(24))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(33)))), neg<i32, overflow=ub>(const<i32>(51)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
