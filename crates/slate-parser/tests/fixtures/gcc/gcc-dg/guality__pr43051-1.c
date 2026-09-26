/* PR debug/43051 */
/* { dg-do run } */
/* { dg-options "-g" } */

extern void abort(void);

static void __attribute__((noinline)) foo(const char *x, long long y, int z) {
  asm volatile("" : : "r"(x), "r"((int)y), "r"(z) : "memory");
}

struct S {
  struct S *n;
  int       v;
};

struct S a[10];

struct S *__attribute__((noinline)) bar(struct S *c, int v, struct S *e) {
#ifdef __i386__
  register int si asm("esi"), di asm("edi"),
      bx
#if !defined(__pic__) && !defined(__APPLE__)
      asm("ebx")
#endif
      ;
  asm volatile("" : "=r"(si), "=r"(di), "=r"(bx));
#endif
  while (c < e) {
    foo("c", (__UINTPTR_TYPE__)c,
        0);         /* { dg-final { gdb-test . "c" "\&a\[0\]" } } */
    foo("v", v, 1); /* { dg-final { gdb-test . "v" "1" } } */
    foo("e", (__UINTPTR_TYPE__)e,
        2); /* { dg-final { gdb-test . "e" "\&a\[1\]" } } */
    if (c->v == v)
      return c;
    foo("c", (__UINTPTR_TYPE__)c,
        3);         /* { dg-final { gdb-test . "c" "\&a\[0\]" } } */
    foo("v", v, 4); /* { dg-final { gdb-test . "v" "1" } } */
    foo("e", (__UINTPTR_TYPE__)e,
        5); /* { dg-final { gdb-test . "e" "\&a\[1\]" } } */
    c++;
  }
#ifdef __i386__
  asm volatile("" : : "r"(si), "r"(di), "r"(bx));
#endif
  return 0;
}

int
main() {
  asm volatile("" : : "r"(&a[0]) : "memory");
  if (bar(&a[a[0].v], a[0].v + 1, &a[a[0].v + 1]))
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
// DEFAULT-NEXT:         field0 n: ptr<@type0>;
// DEFAULT-NEXT:         field1 v: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %6 a: array<@type0, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([118, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([118, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([101, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: ptr<const i8>, %3 y: i64, %4 z: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" read<ptr<const i8>>(%2);
// DEFAULT-NEXT:             in 1 "r" truncate<i32, reason=explicit, fits=unknown>(read<i64>(%3));
// DEFAULT-NEXT:             in 2 "r" read<i32>(%4);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar(%8 c: ptr<@type0>, %9 v: i32, %10 e: ptr<@type0>) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %12 lt<ptr<@type0>>(read<ptr<@type0>>(%8), read<ptr<@type0>>(%10))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, i64, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%13)), reinterpret<i64, reason=arg, fits=unknown>(ptr_to_int<u64, reason=explicit>(read<ptr<@type0>>(%8))), const<i32>(0));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, i64, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%14)), widen<i64, reason=arg>(read<i32>(%9)), const<i32>(1));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, i64, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%15)), reinterpret<i64, reason=arg, fits=unknown>(ptr_to_int<u64, reason=explicit>(read<ptr<@type0>>(%10))), const<i32>(2));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%8)))), read<i32>(%9))
// DEFAULT-NEXT:                     return read<ptr<@type0>>(%8);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, i64, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%16)), reinterpret<i64, reason=arg, fits=unknown>(ptr_to_int<u64, reason=explicit>(read<ptr<@type0>>(%8))), const<i32>(3));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, i64, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%17)), widen<i64, reason=arg>(read<i32>(%9)), const<i32>(4));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, i64, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%18)), reinterpret<i64, reason=arg, fits=unknown>(ptr_to_int<u64, reason=explicit>(read<ptr<@type0>>(%10))), const<i32>(5));
// DEFAULT-NEXT:                 let %19: ptr<@type0> [synthetic] = read<ptr<@type0>>(%8);
// DEFAULT-NEXT:                 let %20: ptr<@type0> [synthetic] = ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type0>>(%8, read<ptr<@type0>>(%20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return null<ptr<@type0>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%6), const<i32>(0))));
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<ptr<@type0>>(call<ptr<@type0>, signature=fn(ptr<@type0>, i32, ptr<@type0>) -> ptr<@type0>>(%7, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%6), read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%6), const<i32>(0)))))))), add<i32, overflow=ub>(read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%6), const<i32>(0))))), const<i32>(1)), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%6), add<i32, overflow=ub>(read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%6), const<i32>(0))))), const<i32>(1)))))), null<ptr<@type0>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
