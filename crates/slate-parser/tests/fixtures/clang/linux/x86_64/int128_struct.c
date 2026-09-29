#include <stdio.h>

struct Wide {
  int               tag;
  __int128          value;
  unsigned __int128 uvalue;
};

int main(void) {
  struct Wide w;
  w.tag     = 7;
  w.value   = -1234567890123456789;
  w.uvalue  = 12345678901234567890ULL;
  w.value  += 1;
  w.uvalue *= 2;

  printf("%d\n", w.tag);
  printf("%llu\n", (unsigned long long)(w.value >> 64));
  printf("%llu\n", (unsigned long long)w.value);
  printf("%llu\n", (unsigned long long)(w.uvalue >> 64));
  printf("%llu\n", (unsigned long long)w.uvalue);
  printf("%zu\n", sizeof(struct Wide));
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
// DEFAULT-NEXT:     type @type[[TYPE_Wide:[0-9]+]] Wide = struct {
// DEFAULT-NEXT:         field0 tag: i32;
// DEFAULT-NEXT:         field1 value: i128;
// DEFAULT-NEXT:         field2 uvalue: u128;
// DEFAULT-NEXT:     } [size=48, align=16, offsets=[0, 16, 32]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: @type[[TYPE_Wide]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_w]]), const<i32>(7));
// DEFAULT-NEXT:         write<i128>(field1(%[[VALUE_w]]), widen<i128, reason=assign>(neg<i64, overflow=ub>(const<i64>(1234567890123456789))));
// DEFAULT-NEXT:         write<u128>(field2(%[[VALUE_w]]), widen<u128, reason=assign>(const<u64>(12345678901234567890)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i128 [synthetic] = read<i128>(field1(%[[VALUE_w]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE0]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(field1(%[[VALUE_w]]), read<i128>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u128 [synthetic] = read<u128>(field2(%[[VALUE_w]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u128 [synthetic] = mul<u128, overflow=wrap>(read<u128>(%[[VALUE2]]), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         write<u128>(field2(%[[VALUE_w]]), read<u128>(%[[VALUE3]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), read<i32>(field0(%[[VALUE_w]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]])), reinterpret<u64, reason=explicit, fits=unknown>(truncate<i64, reason=explicit, fits=unknown>(shr<i128, amount_out_of_range=ub, fill=sign_extend>(read<i128>(field1(%[[VALUE_w]])), const<i32>(64)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_3]])), reinterpret<u64, reason=explicit, fits=unknown>(truncate<i64, reason=explicit, fits=unknown>(read<i128>(field1(%[[VALUE_w]])))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_4]])), truncate<u64, reason=explicit, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(field2(%[[VALUE_w]])), const<i32>(64))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_5]])), truncate<u64, reason=explicit, fits=unknown>(read<u128>(field2(%[[VALUE_w]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_6]])), const<u64>(48));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
