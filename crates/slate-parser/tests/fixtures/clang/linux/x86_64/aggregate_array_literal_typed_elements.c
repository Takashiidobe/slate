#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct pair {
  const char *name;
  const char *value;
};

static int count_null_pairs(void) {
  struct pair pairs[] = {{NULL, NULL}};
  int         total   = 0;
  for (int i = 0; i < 1; i++) {
    if (pairs[i].name == NULL && pairs[i].value == NULL) {
      total++;
    }
  }
  return total;
}

static int count_true_flags(void) {
  bool values[] = {true, false};
  int  total    = 0;
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    if (values[i]) {
      total++;
    }
  }
  return total;
}

int main(void) {
  printf("%d %d\n", count_null_pairs(), count_true_flags());
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
// DEFAULT-NEXT:     type @type1 pair = struct {
// DEFAULT-NEXT:         field0 name: ptr<const i8>;
// DEFAULT-NEXT:         field1 value: ptr<const i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @count_null_pairs() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 pairs: array<@type1, 1> [storage=automatic] [align=16] = aggregate<array<@type1, 1>, zero_fill=false>(index0 = aggregate<@type1, zero_fill=false>(field0 = null<ptr<const i8>>, field1 = null<ptr<const i8>>));
// DEFAULT-NEXT:         let %6 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if logical_and<bool>(eq<ptr<const i8>>(read<ptr<const i8>>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(%5), read<i32>(%7))))), null<ptr<const i8>>), eq<ptr<const i8>>(read<ptr<const i8>>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(%5), read<i32>(%7))))), null<ptr<const i8>>))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%6, read<i32>(%20));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @count_true_flags() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 values: array<bool, 2> [storage=automatic] = aggregate<array<bool, 2>, zero_fill=false>(index0 = const<bool>(true), index1 = const<bool>(false));
// DEFAULT-NEXT:         let %10 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %11 i: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%11), div<u64, by_zero=ub>(const<u64>(2), const<u64>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: u64 [synthetic] = read<u64>(%11);
// DEFAULT-NEXT:                 let %22: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%21), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%11, read<u64>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if read<bool>(deref(ptr_offset<ptr<bool>, subtract=false, element=bool, overflow=ub>(array_decay<ptr<bool>, length=Some(2)>(%9), read<u64>(%11))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %23: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%24));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%16)), call<i32, signature=fn() -> i32>(%4), call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
