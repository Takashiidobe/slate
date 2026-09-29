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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least16_t:[0-9]+]] __uint_least16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least32_t:[0-9]+]] __uint_least32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __count: i32;
// DEFAULT-NEXT:         field1 __value: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = union {
// DEFAULT-NEXT:         field0 __wch: u32;
// DEFAULT-NEXT:         field1 __wchb: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE___mbstate_t:[0-9]+]] __mbstate_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_mbstate_t:[0-9]+]] mbstate_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_char16_t:[0-9]+]] char16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([37, 122, 117, 32, 37, 122, 117, 32, 37, 117, 32, 37, 100, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 117, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mbrtoc16:[0-9]+]] @mbrtoc16(%[[VALUE___pc16:[0-9]+]] __pc16: ptr<u16> [restrict], %[[VALUE___s:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___n:[0-9]+]] __n: u64, %[[VALUE___p:[0-9]+]] __p: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c16rtomb:[0-9]+]] @c16rtomb(%[[VALUE___s_2:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___c16:[0-9]+]] __c16: u16, %[[VALUE___ps:[0-9]+]] __ps: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mbrtoc32:[0-9]+]] @mbrtoc32(%[[VALUE___pc32:[0-9]+]] __pc32: ptr<u32> [restrict], %[[VALUE___s_3:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___n_2:[0-9]+]] __n: u64, %[[VALUE___p_2:[0-9]+]] __p: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c32rtomb:[0-9]+]] @c32rtomb(%[[VALUE___s_4:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___c32:[0-9]+]] __c32: u32, %[[VALUE___ps_2:[0-9]+]] __ps: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_state16:[0-9]+]] state16: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_state32:[0-9]+]] state32: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_converted16:[0-9]+]] converted16: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_converted32:[0-9]+]] converted32: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_multibyte16:[0-9]+]] multibyte16: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_multibyte32:[0-9]+]] multibyte32: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_read16:[0-9]+]] read16: u64 [storage=automatic] = call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_mbrtoc16]], addr_of<ptr<u16>>(%[[VALUE_converted16]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_state16]]));
// DEFAULT-NEXT:         let %[[VALUE_write16:[0-9]+]] write16: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u16, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_c16rtomb]], array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_multibyte16]]), const<u16>(65), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_state16]]));
// DEFAULT-NEXT:         let %[[VALUE_read32:[0-9]+]] read32: u64 [storage=automatic] = call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_mbrtoc32]], addr_of<ptr<u32>>(%[[VALUE_converted32]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_state32]]));
// DEFAULT-NEXT:         let %[[VALUE_write32:[0-9]+]] write32: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u32, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_c32rtomb]], array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_multibyte32]]), const<u32>(66), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_state32]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_3]])), read<u64>(%[[VALUE_read16]]), read<u64>(%[[VALUE_write16]]), widen<u32, reason=explicit>(read<u16>(%[[VALUE_converted16]])), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_multibyte16]]), const<i32>(0))))), read<u64>(%[[VALUE_read32]]), read<u64>(%[[VALUE_write32]]), read<u32>(%[[VALUE_converted32]]), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_multibyte32]]), const<i32>(0))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
