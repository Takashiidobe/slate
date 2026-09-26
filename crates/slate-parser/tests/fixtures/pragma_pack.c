#include <stddef.h>
#include <stdio.h>

struct NaturalBefore {
  unsigned char tag;
  unsigned int  value;
};

#pragma pack(push, 2)
struct PackedTwo {
  unsigned char tag;
  unsigned int  value;
};

#pragma pack(push, 1)
struct PackedOne {
  unsigned char tag;
  unsigned int  value;
};
#pragma pack(pop)

struct PackedTwoAgain {
  unsigned char tag;
  unsigned int  value;
};
#pragma pack(pop)

struct NaturalAfter {
  unsigned char tag;
  unsigned int  value;
};

int main(void) {
  struct PackedOne packed = {29, 31};
  printf(
      "%d %d %d %d %d %d %d %d %d %d %d\n", (int)sizeof(struct NaturalBefore),
      (int)offsetof(struct NaturalBefore, value), (int)sizeof(struct PackedTwo),
      (int)_Alignof(struct PackedTwo), (int)offsetof(struct PackedTwo, value),
      (int)sizeof(packed), (int)_Alignof(struct PackedOne),
      (int)offsetof(struct PackedOne, value),
      (int)sizeof(struct PackedTwoAgain),
      (int)offsetof(struct PackedTwoAgain, value),
      (int)offsetof(struct NaturalAfter, value));
  return packed.tag == 29 && packed.value == 31 ? 0 : 1;
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
// DEFAULT-NEXT:     type @type0 NaturalBefore = struct {
// DEFAULT-NEXT:         field0 tag: u8;
// DEFAULT-NEXT:         field1 value: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 PackedTwo = struct {
// DEFAULT-NEXT:         field0 tag: u8;
// DEFAULT-NEXT:         field1 value: u32;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type2 PackedOne = struct {
// DEFAULT-NEXT:         field0 tag: u8;
// DEFAULT-NEXT:         field1 value: u32;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type3 PackedTwoAgain = struct {
// DEFAULT-NEXT:         field0 tag: u8;
// DEFAULT-NEXT:         field1 value: u32;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type4 NaturalAfter = struct {
// DEFAULT-NEXT:         field0 tag: u8;
// DEFAULT-NEXT:         field1 value: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 packed: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(29))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(34)>(%9)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(8))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(6))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(2))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(2))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(5))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(6))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(2))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(%7)))), const<i32>(29)), eq<u32>(read<u32>(field1(%7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(31)))), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
