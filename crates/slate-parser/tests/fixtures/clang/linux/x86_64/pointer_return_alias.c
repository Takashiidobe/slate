#include <stdio.h>

static int *identity_mut(int *value) { return value; }

static int *forward_mut(int *value) { return identity_mut(value); }

static const int *identity_const(const int *value) { return value; }

static int *choose_value(int *first, int *second, int choose_first) {
  if (choose_first)
    return first;
  return second;
}

int main(void) {
  int  first             = 20;
  int  second            = 22;
  int *alias             = forward_mut(&first);
  *alias                += 2;
  const int *read_alias  = identity_const(&second);
  int       *ambiguous   = choose_value(&first, &second, 1);
  printf("%d %d %d\n", first, *read_alias, *ambiguous);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_identity_mut:[0-9]+]] @identity_mut(%[[VALUE_value:[0-9]+]] value: ptr<i32>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_value]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_forward_mut:[0-9]+]] @forward_mut(%[[VALUE_value_2:[0-9]+]] value: ptr<i32>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%[[VALUE_identity_mut]], read<ptr<i32>>(%[[VALUE_value_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_identity_const:[0-9]+]] @identity_const(%[[VALUE_value_3:[0-9]+]] value: ptr<const i32>) -> ptr<const i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<const i32>>(%[[VALUE_value_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_choose_value:[0-9]+]] @choose_value(%[[VALUE_first:[0-9]+]] first: ptr<i32>, %[[VALUE_second:[0-9]+]] second: ptr<i32>, %[[VALUE_choose_first:[0-9]+]] choose_first: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_choose_first]]), const<i32>(0))
// DEFAULT-NEXT:             return read<ptr<i32>>(%[[VALUE_first]]);
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_second]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_first_2:[0-9]+]] first: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:         let %[[VALUE_second_2:[0-9]+]] second: i32 [storage=automatic] = const<i32>(22);
// DEFAULT-NEXT:         let %[[VALUE_alias:[0-9]+]] alias: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%[[VALUE_forward_mut]], addr_of<ptr<i32>>(%[[VALUE_first_2]]));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_alias]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE0]])), read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE_read_alias:[0-9]+]] read_alias: ptr<const i32> [storage=automatic] = call<ptr<const i32>, signature=fn(ptr<const i32>) -> ptr<const i32>>(%[[VALUE_identity_const]], pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_second_2]])));
// DEFAULT-NEXT:         let %[[VALUE_ambiguous:[0-9]+]] ambiguous: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>, i32) -> ptr<i32>>(%[[VALUE_choose_value]], addr_of<ptr<i32>>(%[[VALUE_first_2]]), addr_of<ptr<i32>>(%[[VALUE_second_2]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(%[[VALUE_first_2]]), read<i32>(deref(read<ptr<const i32>>(%[[VALUE_read_alias]]))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_ambiguous]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
