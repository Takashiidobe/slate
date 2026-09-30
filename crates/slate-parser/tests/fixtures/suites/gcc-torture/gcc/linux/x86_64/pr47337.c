/* PR rtl-optimization/47337 */

static unsigned int a[256], b = 0;
static char         c = 0;
static int          d = 0, *f = &d;
static long long    e = 0;

static short foo(long long x, long long y) { return x / y; }

static char bar(char x, char y) { return x - y; }

static int baz(int x, int y) {
  *f = (y != (short)(y * 3));
  for (c = 0; c < 2; c++) {
  lab:
    if (d) {
      if (e)
        e = 1;
      else
        return x;
    } else {
      d = 1;
      goto lab;
    }
    f = &d;
  }
  return x;
}

static void fnx(unsigned long long x, int y) {
  if (!y) {
    b = a[b & 1];
    b = a[b & 1];
    b = a[(b ^ (x & 1)) & 1];
    b = a[(b ^ (x & 1)) & 1];
  }
}

char *volatile w = "2";

int main() {
  int          h = 0;
  unsigned int k = 0;
  int          l[8];
  int          i, j;

  if (__builtin_strcmp(w, "1") == 0)
    h = 1;

  for (i = 0; i < 256; i++) {
    for (j = 8; j > 0; j--)
      k = 1;
    a[i] = k;
  }
  for (i = 0; i < 8; i++)
    l[i] = 0;

  d = bar(c, c);
  d = baz(c, 1 | foo(l[0], 10));
  fnx(d, h);
  fnx(e, h);

  if (d != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<u32, 256> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_d]]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: volatile ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i64, %[[VALUE_y:[0-9]+]] y: i64) -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_x]]), read<i64>(%[[VALUE_y]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i8, %[[VALUE_y_2:[0-9]+]] y: i8) -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_2]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_y_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_f]])), from_bool<i32, reason=assign>(ne<i32>(read<i32>(%[[VALUE_y_3]]), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_y_3]]), const<i32>(3)))))));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_c]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     label %[[VALUE_lab:[0-9]+]] lab:
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%[[VALUE_e]]), const<i64>(0))
// DEFAULT-NEXT:                                     write<i64>(%[[VALUE_e]], widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     return read<i32>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:                                 goto %[[VALUE_lab]];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<ptr<i32>>(%[[VALUE_f]], addr_of<ptr<i32>>(%[[VALUE_d]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnx:[0-9]+]] @fnx(%[[VALUE_x_4:[0-9]+]] x: u64, %[[VALUE_y_4:[0-9]+]] y: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_y_4]]), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_b]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_a]]), and<u32>(read<u32>(%[[VALUE_b]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_b]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_a]]), and<u32>(read<u32>(%[[VALUE_b]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_b]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_a]]), and<u64>(xor<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_b]])), and<u64>(read<u64>(%[[VALUE_x_4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_b]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_a]]), and<u64>(xor<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_b]])), and<u64>(read<u64>(%[[VALUE_x_4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcmp:[0-9]+]] @__builtin_strcmp(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: array<i32, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>, volatile>(%[[VALUE_w]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]]))), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_h]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(256))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], const<i32>(8));
// DEFAULT-NEXT:                         condition: gt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<u32>(%[[VALUE_k]], reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]]))), read<u32>(%[[VALUE_k]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%[[VALUE_l]]), read<i32>(%[[VALUE_i]]))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], widen<i32, reason=assign>(call<i8, signature=fn(i8, i8) -> i8>(%[[VALUE_bar]], read<i8>(%[[VALUE_c]]), read<i8>(%[[VALUE_c]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_baz]], widen<i32, reason=arg>(read<i8>(%[[VALUE_c]])), or<i32>(const<i32>(1), widen<i32, reason=promotion>(call<i16, signature=fn(i64, i64) -> i16>(%[[VALUE_foo]], widen<i64, reason=arg>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%[[VALUE_l]]), const<i32>(0))))), widen<i64, reason=arg>(const<i32>(10)))))));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_fnx]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_d]]))), read<i32>(%[[VALUE_h]]));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_fnx]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_e]])), read<i32>(%[[VALUE_h]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
