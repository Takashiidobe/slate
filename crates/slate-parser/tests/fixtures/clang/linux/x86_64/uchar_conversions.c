#include <stdio.h>
#include <uchar.h>

int main(void) {
  mbstate_t state16        = {0};
  mbstate_t state32        = {0};
  char16_t  converted16    = 0;
  char32_t  converted32    = 0;
  char      multibyte16[4] = {0};
  char      multibyte32[4] = {0};

  size_t read16  = mbrtoc16(&converted16, "A", 1, &state16);
  size_t write16 = c16rtomb(multibyte16, u'A', &state16);
  size_t read32  = mbrtoc32(&converted32, "B", 1, &state32);
  size_t write32 = c32rtomb(multibyte32, U'B', &state32);

  printf("%zu %zu %u %d %zu %zu %u %d\n", read16, write16,
         (unsigned)converted16, multibyte16[0], read32, write32,
         (unsigned)converted32, multibyte32[0]);
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
// DEFAULT-NEXT:     type @type1 __uint16_t = u16;
// DEFAULT-NEXT:     type @type2 __uint32_t = u32;
// DEFAULT-NEXT:     type @type3 __uint_least16_t = u16;
// DEFAULT-NEXT:     type @type4 __uint_least32_t = u32;
// DEFAULT-NEXT:     type @type5 = struct {
// DEFAULT-NEXT:         field0 __count: i32;
// DEFAULT-NEXT:         field1 __value: @type6;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type6 = union {
// DEFAULT-NEXT:         field0 __wch: u32;
// DEFAULT-NEXT:         field1 __wchb: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type7 __mbstate_t = @type5;
// DEFAULT-NEXT:     type @type8 mbstate_t = @type5;
// DEFAULT-NEXT:     type @type9 char16_t = u16;
// DEFAULT-NEXT:     type @type10 char32_t = u32;
// DEFAULT-NEXT:     global %42 .str42: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([37, 122, 117, 32, 37, 122, 117, 32, 37, 117, 32, 37, 100, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 117, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %8 @printf(%27 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @mbrtoc16(%28 __pc16: ptr<u16> [restrict], %29 __s: ptr<const i8> [restrict], %30 __n: u64, %31 __p: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %13 @c16rtomb(%32 __s: ptr<i8> [restrict], %33 __c16: u16, %34 __ps: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %14 @mbrtoc32(%35 __pc32: ptr<u32> [restrict], %36 __s: ptr<const i8> [restrict], %37 __n: u64, %38 __p: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %15 @c32rtomb(%39 __s: ptr<i8> [restrict], %40 __c32: u32, %41 __ps: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 state16: @type5 [storage=automatic] = aggregate<@type5, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %18 state32: @type5 [storage=automatic] = aggregate<@type5, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %19 converted16: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %20 converted32: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %21 multibyte16: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %22 multibyte32: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %23 read16: u64 [storage=automatic] = call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type5>) -> u64>(%12, addr_of<ptr<u16>>(%19), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%42)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type5>>(%17));
// DEFAULT-NEXT:         let %24 write16: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u16, ptr<@type5>) -> u64>(%13, array_decay<ptr<i8>, length=Some(4)>(%21), const<u16>(65), addr_of<ptr<@type5>>(%17));
// DEFAULT-NEXT:         let %25 read32: u64 [storage=automatic] = call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type5>) -> u64>(%14, addr_of<ptr<u32>>(%20), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%43)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type5>>(%18));
// DEFAULT-NEXT:         let %26 write32: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u32, ptr<@type5>) -> u64>(%15, array_decay<ptr<i8>, length=Some(4)>(%22), const<u32>(66), addr_of<ptr<@type5>>(%18));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%44)), read<u64>(%23), read<u64>(%24), widen<u32, reason=explicit>(read<u16>(%19)), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%21), const<i32>(0))))), read<u64>(%25), read<u64>(%26), read<u32>(%20), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%22), const<i32>(0))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
