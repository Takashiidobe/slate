/* PR target/52129 */

extern void abort(void);
struct S {
  void        *p;
  unsigned int q;
};
struct T {
  char a[64];
  char b[64];
} t;

__attribute__((noinline, noclone)) int foo(void *x, struct S s, void *y,
                                           void *z) {
  if (x != &t.a[2] || s.p != &t.b[5] || s.q != 27 || y != &t.a[17] ||
      z != &t.b[17])
    abort();
  return 29;
}

__attribute__((noinline, noclone)) int bar(void *x, void *y, void *z,
                                           struct S s, int t, struct T *u) {
  return foo(x, s, &u->a[t], &u->b[t]);
}

int main() {
  struct S s = {&t.b[5], 27};
  if (bar(&t.a[2], (void *)0, (void *)0, s, 17, &t) != 29)
    abort();
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 p: ptr<void>;
// DEFAULT-NEXT:         field1 q: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 64>;
// DEFAULT-NEXT:         field1 b: array<i8, 64>;
// DEFAULT-NEXT:     } [size=128, align=1, offsets=[0, 64]];
// DEFAULT-NEXT:     global %3 t: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 x: ptr<void>, %6 s: @type0, %7 y: ptr<void>, %8 z: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, coerce<i64, i32>, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<ptr<void>>(read<ptr<void>>(%5), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field0(%3)), const<i32>(2)))))), ne<ptr<void>>(read<ptr<void>>(field0(%6)), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field1(%3)), const<i32>(5))))))), ne<u32>(read<u32>(field1(%6)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(27)))), ne<ptr<void>>(read<ptr<void>>(%7), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field0(%3)), const<i32>(17))))))), ne<ptr<void>>(read<ptr<void>>(%8), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field1(%3)), const<i32>(17)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar(%10 x: ptr<void>, %11 y: ptr<void>, %12 z: ptr<void>, %13 s: @type0, %14 t: i32, %15 u: ptr<@type1>) -> i32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, scalar, scalar, coerce<i64, i32>, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<void>, @type0, ptr<void>, ptr<void>) -> i32, abi=sysv64(scalar, coerce<i64, i32>, scalar, scalar) -> scalar>(%4, read<ptr<void>>(%10), copy<@type0, reason=arg>(read<@type0>(%13)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field0(deref(read<ptr<@type1>>(%15)))), read<i32>(%14))))), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field1(deref(read<ptr<@type1>>(%15)))), read<i32>(%14))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field1(%3)), const<i32>(5))))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(27)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, ptr<void>, ptr<void>, @type0, i32, ptr<@type1>) -> i32, abi=sysv64(scalar, scalar, scalar, coerce<i64, i32>, scalar, scalar) -> scalar>(%9, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(field0(%3)), const<i32>(2))))), null<ptr<void>>, null<ptr<void>>, copy<@type0, reason=arg>(read<@type0>(%17)), const<i32>(17), addr_of<ptr<@type1>>(%3)), const<i32>(29))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
