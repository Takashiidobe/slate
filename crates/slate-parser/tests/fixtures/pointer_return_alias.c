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
// DEFAULT-NEXT:     global %18 .str18: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%17 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @identity_mut(%2 value: ptr<i32>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<i32>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @forward_mut(%4 value: ptr<i32>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%1, read<ptr<i32>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @identity_const(%6 value: ptr<const i32>) -> ptr<const i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<const i32>>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @choose_value(%8 first: ptr<i32>, %9 second: ptr<i32>, %10 choose_first: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             return read<ptr<i32>>(%8);
// DEFAULT-NEXT:         return read<ptr<i32>>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 first: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:         let %13 second: i32 [storage=automatic] = const<i32>(22);
// DEFAULT-NEXT:         let %14 alias: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%3, addr_of<ptr<i32>>(%12));
// DEFAULT-NEXT:         let %19: ptr<i32> [synthetic] = read<ptr<i32>>(%14);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%19)));
// DEFAULT-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%19)), read<i32>(%21));
// DEFAULT-NEXT:         let %15 read_alias: ptr<const i32> [storage=automatic] = call<ptr<const i32>, signature=fn(ptr<const i32>) -> ptr<const i32>>(%5, pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(%13)));
// DEFAULT-NEXT:         let %16 ambiguous: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>, i32) -> ptr<i32>>(%7, addr_of<ptr<i32>>(%12), addr_of<ptr<i32>>(%13), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%18)), read<i32>(%12), read<i32>(deref(read<ptr<const i32>>(%15))), read<i32>(deref(read<ptr<i32>>(%16))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
