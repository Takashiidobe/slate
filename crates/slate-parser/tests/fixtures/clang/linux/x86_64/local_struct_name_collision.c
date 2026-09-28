#include <stdio.h>

static int sum_docs(void) {
  struct TestCase {
    const char *doc;
    int         expectedStatus;
  };

  const struct TestCase cases[] = {
      {"a", 1},
      {"bb", 2},
      {"ccc", 3},
  };

  int total = 0;
  for (int i = 0; i < 3; i++) {
    total += (int)cases[i].doc[0] * cases[i].expectedStatus;
  }
  return total;
}

static int sum_flags(void) {
  struct TestCase {
    int usesParameterEntities;
    int weight;
  };

  const struct TestCase cases[] = {
      {1, 10},
      {0, 20},
  };

  int total = 0;
  for (int i = 0; i < 2; i++) {
    if (cases[i].usesParameterEntities) {
      total += cases[i].weight;
    } else {
      total -= cases[i].weight;
    }
  }
  return total;
}

static int sum_movements(void) {
  struct TestCase {
    int         expectedMovementInChars;
    const char *input;
  };

  struct TestCase cases[] = {
      {1, "x"},
      {2, "yy"},
      {3, "zzz"},
  };

  int total = 0;
  for (int i = 0; i < 3; i++) {
    total += cases[i].expectedMovementInChars + (int)cases[i].input[0];
  }
  return total;
}

int main(void) {
  printf("%d %d %d\n", sum_docs(), sum_flags(), sum_movements());
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
// DEFAULT-NEXT:     type @type0 TestCase = struct {
// DEFAULT-NEXT:         field0 doc: ptr<const i8>;
// DEFAULT-NEXT:         field1 expectedStatus: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 TestCase = struct {
// DEFAULT-NEXT:         field0 usesParameterEntities: i32;
// DEFAULT-NEXT:         field1 weight: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 TestCase = struct {
// DEFAULT-NEXT:         field0 expectedMovementInChars: i32;
// DEFAULT-NEXT:         field1 input: ptr<const i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([98, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 99, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([121, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([122, 122, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @sum_docs() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 cases: array<@type0, 3> [storage=automatic] [const] [align=16] = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%19)), field1 = const<i32>(1)), index1 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%20)), field1 = const<i32>(2)), index2 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%21)), field1 = const<i32>(3)));
// DEFAULT-NEXT:         let %5 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %31: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                     let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), mul<i32, overflow=ub>(widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<const @type0>, length=Some(3)>(%4), read<i32>(%6))))), const<i32>(0))))), read<i32>(field1(deref(ptr_offset<ptr<const @type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<const @type0>, length=Some(3)>(%4), read<i32>(%6)))))));
// DEFAULT-NEXT:                     write<i32>(%5, read<i32>(%32));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @sum_flags() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 cases: array<@type1, 2> [storage=automatic] [const] [align=16] = aggregate<array<@type1, 2>, zero_fill=false>(index0 = aggregate<@type1, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(10)), index1 = aggregate<@type1, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(20)));
// DEFAULT-NEXT:         let %10 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %11 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(2)>(%9), read<i32>(%11))))), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %35: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), read<i32>(field1(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(2)>(%9), read<i32>(%11))))));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%36));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %37: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %38: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%37), read<i32>(field1(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(2)>(%9), read<i32>(%11))))));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%38));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @sum_movements() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 cases: array<@type2, 3> [storage=automatic] [align=16] = aggregate<array<@type2, 3>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = const<i32>(1), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%24))), index1 = aggregate<@type2, zero_fill=false>(field0 = const<i32>(2), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%25))), index2 = aggregate<@type2, zero_fill=false>(field0 = const<i32>(3), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%26))));
// DEFAULT-NEXT:         let %15 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %16 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%40));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %41: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                     let %42: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%41), add<i32, overflow=ub>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%14), read<i32>(%16))))), widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%14), read<i32>(%16))))), const<i32>(0)))))));
// DEFAULT-NEXT:                     write<i32>(%15, read<i32>(%42));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%28)), call<i32, signature=fn() -> i32>(%2), call<i32, signature=fn() -> i32>(%7), call<i32, signature=fn() -> i32>(%12));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
