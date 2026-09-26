/* PR tree-optimization/94734 */

__attribute__((noipa)) int foo(int n) {
  int arr[16], s = 0;
  for (int i = 0; i < n; i++) {
    if (i < 16)
      arr[i] = i;
  }
  for (int i = 0; i < 16; i++)
    s += arr[i];
  return s;
}

__attribute__((noipa)) int bar(int n, int x, unsigned long y, unsigned long z) {
  int arr[16], s = 0;
  arr[4] = 42;
  for (int i = 0; i < n; i++) {
    if (x == (i & 0x25))
      arr[y] = i;
  }
  return arr[z];
}

__attribute__((noipa)) int baz(int n, int x, unsigned long z) {
  int arr[16], s = 0;
  arr[12] = 42;
  for (int i = 0; i < n; i++) {
    if (x == (i & 0x25))
      arr[7] = i;
  }
  return arr[z];
}

int main() {
  if (foo(10374) != 15 * 16 / 2)
    __builtin_abort();
  if (bar(25, 0x25, (unsigned long)0xdeadbeefbeefdeadULL, 4) != 42)
    __builtin_abort();
  if (bar(25, 4, 15, 15) != 22)
    __builtin_abort();
  if (baz(25, 0x25, 12) != 42)
    __builtin_abort();
  if (baz(25, 4, 7) != 22)
    __builtin_abort();
  if (baz(25, 4, 12) != 42)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%1 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 arr: array<i32, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %3 s: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), read<i32>(%1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%4), const<i32>(16))
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%2), read<i32>(%4))), read<i32>(%4));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %5 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%2), read<i32>(%5)))));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%31));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 n: i32, %8 x: i32, %9 y: u64, %10 z: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 arr: array<i32, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %12 s: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%11), const<i32>(4))), const<i32>(42));
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), read<i32>(%7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%8), and<i32>(read<i32>(%13), const<i32>(37)))
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%11), read<u64>(%9))), read<i32>(%13));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%11), read<u64>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @baz(%15 n: i32, %16 x: i32, %17 z: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 arr: array<i32, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %19 s: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%18), const<i32>(12))), const<i32>(42));
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %20 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%20), read<i32>(%15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%35));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%16), and<i32>(read<i32>(%20), const<i32>(37)))
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%18), const<i32>(7))), read<i32>(%20));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%18), read<u64>(%17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(10374)), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(15), const<i32>(16)), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, u64, u64) -> i32>(%6, const<i32>(25), const<i32>(37), const<u64>(16045690984300797613), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))), const<i32>(42))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, u64, u64) -> i32>(%6, const<i32>(25), const<i32>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15)))), const<i32>(22))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, u64) -> i32>(%14, const<i32>(25), const<i32>(37), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12)))), const<i32>(42))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, u64) -> i32>(%14, const<i32>(25), const<i32>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))), const<i32>(22))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, u64) -> i32>(%14, const<i32>(25), const<i32>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12)))), const<i32>(42))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
