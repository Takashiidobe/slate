/* PR tree-optimization/84339 */

struct S {
  int  a;
  char b[1];
};

__attribute__((noipa)) int foo(struct S *p) {
  return __builtin_strlen(&p->b[0]);
}

__attribute__((noipa)) int bar(struct S *p) { return __builtin_strlen(p->b); }

int main() {
  struct S *p = __builtin_malloc(sizeof(struct S) + 16);
  if (p) {
    p->a = 1;
    __builtin_strcpy(p->b, "abcdefg");
    if (foo(p) != 7 || bar(p) != 7)
      __builtin_abort();
    __builtin_free(p);
  }
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
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: array<i8, 1>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([97, 98, 99, 100, 101, 102, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %8 @__builtin_strlen(%7 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%8, pointer_cast<ptr<const i8>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type0>>(%2)))), const<i32>(0))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type0>>(%4))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_malloc(%9 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @__builtin_strcpy(%11 <unnamed>: ptr<i8>, %12 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %17 @__builtin_free(%16 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 p: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%10, add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))))));
// DEFAULT-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(%6), null<ptr<@type0>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(field0(deref(read<ptr<@type0>>(%6))), const<i32>(1));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%13, array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type0>>(%6)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%14)));
// DEFAULT-NEXT:                 let %18: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%1, read<ptr<@type0>>(%6)), const<i32>(7))
// DEFAULT-NEXT:                     write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%18, ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%3, read<ptr<@type0>>(%6)), const<i32>(7)));
// DEFAULT-NEXT:                 if read<bool>(%18)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%17, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type0>>(%6)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
