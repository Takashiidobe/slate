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
// DEFAULT-NEXT:     global %57 .str57: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([37, 122, 117, 32, 37, 122, 117, 32, 37, 117, 32, 37, 100, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 117, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %9 @printf(%42 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @mbrtoc16(%43 __pc16: ptr<u16> [restrict], %44 __s: ptr<const i8> [restrict], %45 __n: u64, %46 __p: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %21 @c16rtomb(%47 __s: ptr<i8> [restrict], %48 __c16: u16, %49 __ps: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %26 @mbrtoc32(%50 __pc32: ptr<u32> [restrict], %51 __s: ptr<const i8> [restrict], %52 __n: u64, %53 __p: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %30 @c32rtomb(%54 __s: ptr<i8> [restrict], %55 __c32: u32, %56 __ps: ptr<@type5> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %31 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %32 state16: @type5 [storage=automatic] = aggregate<@type5, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %33 state32: @type5 [storage=automatic] = aggregate<@type5, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %34 converted16: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %35 converted32: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %36 multibyte16: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %37 multibyte32: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %38 read16: u64 [storage=automatic] = call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type5>) -> u64>(%17, addr_of<ptr<u16>>(%34), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%57)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type5>>(%32));
// DEFAULT-NEXT:         let %39 write16: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u16, ptr<@type5>) -> u64>(%21, array_decay<ptr<i8>, length=Some(4)>(%36), const<u16>(65), addr_of<ptr<@type5>>(%32));
// DEFAULT-NEXT:         let %40 read32: u64 [storage=automatic] = call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type5>) -> u64>(%26, addr_of<ptr<u32>>(%35), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%58)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type5>>(%33));
// DEFAULT-NEXT:         let %41 write32: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u32, ptr<@type5>) -> u64>(%30, array_decay<ptr<i8>, length=Some(4)>(%37), const<u32>(66), addr_of<ptr<@type5>>(%33));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%9, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%59)), read<u64>(%38), read<u64>(%39), widen<u32, reason=explicit>(read<u16>(%34)), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%36), const<i32>(0))))), read<u64>(%40), read<u64>(%41), read<u32>(%35), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%37), const<i32>(0))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
