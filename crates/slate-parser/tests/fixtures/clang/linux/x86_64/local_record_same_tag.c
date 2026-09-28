#include <stdio.h>

enum Err { ERR_NONE = 0, ERR_BAD = 5 };

static int first(void) {
  struct Rec {
    const char *text;
    enum Err    code;
  };
  struct Rec items[] = {{"a", ERR_NONE}, {"bb", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].code + (int)items[i].text[0];
  return total;
}

static int second(void) {
  struct Rec {
    unsigned long n;
    const char   *text;
    enum Err      code;
  };
  struct Rec items[] = {{5, "x", ERR_NONE}, {6, "y", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].n + (int)items[i].code + (int)items[i].text[0];
  return total;
}

int main(void) {
  printf("%d %d\n", first(), second());
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
// DEFAULT-NEXT:     type @type0 Err = enum : u32 {
// DEFAULT-NEXT:         %0 ERR_NONE = const<i32>(0);
// DEFAULT-NEXT:         %1 ERR_BAD = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 Rec = struct {
// DEFAULT-NEXT:         field0 text: ptr<const i8>;
// DEFAULT-NEXT:         field1 code: @type0;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 Rec = struct {
// DEFAULT-NEXT:         field0 n: u64;
// DEFAULT-NEXT:         field1 text: ptr<const i8>;
// DEFAULT-NEXT:         field2 code: @type0;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([98, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @first() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 items: array<@type1, 2> [storage=automatic] [align=16] = aggregate<array<@type1, 2>, zero_fill=false>(index0 = aggregate<@type1, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%17)), field1 = int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))), index1 = aggregate<@type1, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%18)), field1 = int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(5)))));
// DEFAULT-NEXT:         let %8 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%9)), div<u64, by_zero=ub>(const<u64>(32), const<u64>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: u32 [synthetic] = read<u32>(%9);
// DEFAULT-NEXT:                 let %25: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%24), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%9, read<u32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type0>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(2)>(%7), read<u32>(%9))))))), widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(2)>(%7), read<u32>(%9))))), const<i32>(0)))))));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%27));
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @second() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 items: array<@type2, 2> [storage=automatic] [align=16] = aggregate<array<@type2, 2>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%20)), field2 = int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))), index1 = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(6))), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%21)), field2 = int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(5)))));
// DEFAULT-NEXT:         let %13 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%14)), div<u64, by_zero=ub>(const<u64>(48), const<u64>(24)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: u32 [synthetic] = read<u32>(%14);
// DEFAULT-NEXT:                 let %29: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%28), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%14, read<u32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%12), read<u32>(%14))))))), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type0>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%12), read<u32>(%14)))))))), widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%12), read<u32>(%14))))), const<i32>(0)))))));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%31));
// DEFAULT-NEXT:         return read<i32>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%23)), call<i32, signature=fn() -> i32>(%5), call<i32, signature=fn() -> i32>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
