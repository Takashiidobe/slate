/* PR tree-optimization/93249 */

char a[2], b[4], c[6];

void foo(void) {
  char d[2] = {0x00, 0x11};
  __builtin_strncpy(&b[2], d, 2);
  __builtin_strncpy(&b[1], a, 2);
  if (b[0] || b[1] || b[2] || b[3])
    __builtin_abort();
}

void bar(void) {
  __builtin_strncpy(&b[2], "\0\x11", 2);
  __builtin_strncpy(&b[1], a, 2);
  if (b[0] || b[1] || b[2] || b[3])
    __builtin_abort();
}

void baz(void) {
  __builtin_strncpy(&c[2], "\x11\x11\0\x11", 4);
  __builtin_strncpy(&c[1], a, 2);
  if (c[0] || c[1] || c[2] || c[3] != 0x11 || c[4] || c[5])
    __builtin_abort();
}

int main() {
  foo();
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
// DEFAULT-NEXT:     global %0 a: array<i8, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: array<i8, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: array<i8, 6> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([0, 17, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([17, 17, 0, 17, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 d: array<i8, 2> [storage=automatic] = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(__builtin_strncpy, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(2)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(__builtin_strncpy, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(1)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(0)))), const<i8>(0)), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(1)))), const<i8>(0))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(2)))), const<i8>(0))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(3)))), const<i8>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(__builtin_strncpy, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(2)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%8)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(__builtin_strncpy, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(1)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(0)))), const<i8>(0)), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(1)))), const<i8>(0))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(2)))), const<i8>(0))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(3)))), const<i8>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(__builtin_strncpy, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(2)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(__builtin_strncpy, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(1)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(0)))), const<i8>(0)), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(1)))), const<i8>(0))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(2)))), const<i8>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(3))))), const<i32>(17))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(4)))), const<i8>(0))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), const<i32>(5)))), const<i8>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
