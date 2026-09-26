/* PR tree-optimization/88693 */

__attribute__((noipa)) void foo(char *p) {
  if (__builtin_strlen(p) != 9)
    __builtin_abort();
}

__attribute__((noipa)) void quux(char *p) {
  int i;
  for (i = 0; i < 100; i++)
    if (p[i] != 'x')
      __builtin_abort();
}

__attribute__((noipa)) void qux(void) {
  char b[100];
  __builtin_memset(b, 'x', sizeof(b));
  quux(b);
}

__attribute__((noipa)) void bar(void) {
  static unsigned char u[9] = "abcdefghi";
  char                 b[100];
  __builtin_memcpy(b, u, sizeof(u));
  b[sizeof(u)] = 0;
  foo(b);
}

__attribute__((noipa)) void baz(void) {
  static unsigned char u[] = {'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r'};
  char                 b[100];
  __builtin_memcpy(b, u, sizeof(u));
  b[sizeof(u)] = 0;
  foo(b);
}

int main() {
  qux();
  bar();
  baz();
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
// DEFAULT-NEXT:     global %8 u: array<u8, 9> [storage=static] = code_units<array<u8, 9>>([97, 98, 99, 100, 101, 102, 103, 104, 105]) [linkage=internal];
// DEFAULT-NEXT:     global %11 u: array<u8, 9> [storage=static] = aggregate<array<u8, 9>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(106))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(107))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(108))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(109))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(110))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(111))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(112))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(113))), index8 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(114)))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 p: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(__builtin_strlen, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @quux(%3 p: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), read<i32>(%4))))), const<i32>(120))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @qux() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 b: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%6)), const<i32>(120), const<u64>(100));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%2, array_decay<ptr<i8>, length=Some(100)>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 b: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%9)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(9)>(%8)), const<u64>(9));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(100)>(%9), const<u64>(9))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%0, array_decay<ptr<i8>, length=Some(100)>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 b: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%12)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(9)>(%11)), const<u64>(9));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(100)>(%12), const<u64>(9))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%0, array_decay<ptr<i8>, length=Some(100)>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
