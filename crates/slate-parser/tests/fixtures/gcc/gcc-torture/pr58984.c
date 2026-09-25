/* PR tree-optimization/58984 */

struct S {
  int f0 : 8;
  int    : 6;
  int f1 : 5;
};
struct T {
  char f0;
  int    : 6;
  int f1 : 5;
};

int a, *c = &a, e, n, b, m;

static int foo(struct S p) {
  const unsigned short *f[36];
  for (; e < 2; e++) {
    const unsigned short **i  = &f[0];
    *c                       ^= 1;
    if (p.f1) {
      *i = 0;
      return b;
    }
  }
  return 0;
}

static int bar(struct T p) {
  const unsigned short *f[36];
  for (; e < 2; e++) {
    const unsigned short **i  = &f[0];
    *c                       ^= 1;
    if (p.f1) {
      *i = 0;
      return b;
    }
  }
  return 0;
}

int main() {
  struct S o = {1, 1};
  foo(o);
  m = n || o.f0;
  if (a != 1)
    __builtin_abort();
  e          = 0;
  struct T p = {1, 1};
  bar(p);
  m |= n || p.f0;
  if (a != 0)
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 f0: i32 : 8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 6;
// DEFAULT-NEXT:         field2 f1: i32 : 5;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 1], bit_offsets=[Some(0), Some(8), Some(14)], bit_units=[(0, 3)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 f0: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 6;
// DEFAULT-NEXT:         field2 f1: i32 : 5;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 1], bit_offsets=[None, Some(8), Some(14)], bit_units=[(1, 2)], field_units=[None, Some(0), Some(0)]];
// DEFAULT-NEXT:     global %2 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 n: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @foo(%9 p: @type0) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 f: array<ptr<const u16>, 36> [storage=automatic];
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11 i: ptr<ptr<const u16>> [storage=automatic] = addr_of<ptr<ptr<const u16>>>(deref(ptr_offset<ptr<ptr<const u16>>, subtract=false, element=ptr<const u16>, overflow=ub>(array_decay<ptr<ptr<const u16>>, length=Some(36)>(%10), const<i32>(0))));
// DEFAULT-NEXT:                     let %23: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                     let %24: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%23)));
// DEFAULT-NEXT:                     let %25: i32 [synthetic] = xor<i32>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%23)), read<i32>(%25));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(bitfield2<unit=0, bytes=0..3, bits=14..19>(%9)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<const u16>>(deref(read<ptr<ptr<const u16>>>(%11)), null<ptr<const u16>>);
// DEFAULT-NEXT:                             return read<i32>(%6);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @bar(%13 p: @type1) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 f: array<ptr<const u16>, 36> [storage=automatic];
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %15 i: ptr<ptr<const u16>> [storage=automatic] = addr_of<ptr<ptr<const u16>>>(deref(ptr_offset<ptr<ptr<const u16>>, subtract=false, element=ptr<const u16>, overflow=ub>(array_decay<ptr<ptr<const u16>>, length=Some(36)>(%14), const<i32>(0))));
// DEFAULT-NEXT:                     let %28: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                     let %29: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%28)));
// DEFAULT-NEXT:                     let %30: i32 [synthetic] = xor<i32>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%28)), read<i32>(%30));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(bitfield2<unit=0, bytes=1..3, bits=6..11>(%13)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<const u16>>(deref(read<ptr<ptr<const u16>>>(%15)), null<ptr<const u16>>);
// DEFAULT-NEXT:                             return read<i32>(%6);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 o: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field2 = const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%8, copy<@type0, reason=arg>(read<@type0>(%17)));
// DEFAULT-NEXT:         write<i32>(%7, from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%5), const<i32>(0)), ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..8>(%17)), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:         let %18 p: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field2 = const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(@type1) -> i32, abi=sysv64(native_c) -> scalar>(%12, copy<@type1, reason=arg>(read<@type1>(%18)));
// DEFAULT-NEXT:         let %31: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = or<i32>(read<i32>(%31), from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(read<i32>(%5), const<i32>(0)), ne<i8>(read<i8>(field0(%18)), const<i8>(0)))));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%32));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
