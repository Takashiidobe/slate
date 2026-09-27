/* PR rtl-optimization/94516 */
/* { dg-do run } */
/* { dg-additional-options "-fpie" { target pie } } */

struct S {
  unsigned char *a;
  unsigned int   b;
};
typedef int V __attribute__((vector_size(sizeof(int) * 4)));

__attribute__((noipa)) void foo(const char *a, const char *b, const char *c,
                                const struct S *d, int e, int f, int g, int h,
                                int i) {
  V v = {1, 2, 3, 4};
  asm volatile("" : : "g"(&v) : "memory");
  v += (V){5, 6, 7, 8};
  asm volatile("" : : "g"(&v) : "memory");
}

__attribute__((noipa)) void bar(void) {
  const struct S s = {"foobarbaz", 9};
  foo("foo", (const char *)0, "corge", &s, 0, 1, 0, -12, -31);
  foo("bar", "quux", "qux", &s, 0, 0, 9, 0, 0);
  foo("baz", (const char *)0, "qux", &s, 1, 0, 0, -12, -32);
}

int
main() {
  bar();
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
// DEFAULT-NEXT:         field0 a: ptr<u8>;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 V = vector<i32, 4>;
// DEFAULT-NEXT:     global %17 .str17: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([102, 111, 111, 98, 97, 114, 98, 97, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 111, 114, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([113, 117, 117, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([113, 117, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([98, 97, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([113, 117, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @foo(%3 a: ptr<const i8>, %4 b: ptr<const i8>, %5 c: ptr<const i8>, %6 d: ptr<const @type0>, %7 e: i32, %8 f: i32, %9 g: i32, %10 h: i32, %11 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 v: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm | sym] -> reg width 64 addr_of<ptr<vector<i32, 4>>>(%12);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %25: vector<i32, 4> [synthetic] = read<vector<i32, 4>>(%12);
// DEFAULT-NEXT:         let %26: vector<i32, 4> [synthetic] = add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%25), read<vector<i32, 4>>(compound_literal %16 [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(6), index2 = const<i32>(7), index3 = const<i32>(8))));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%12, read<vector<i32, 4>>(%26));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm | sym] -> reg width 64 addr_of<ptr<vector<i32, 4>>>(%12);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 s: @type0 [storage=automatic] [const] = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<u8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%17)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(9)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>, ptr<const i8>, ptr<const @type0>, i32, i32, i32, i32, i32) -> void>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)), null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%19)), addr_of<ptr<const @type0>>(%14), const<i32>(0), const<i32>(1), const<i32>(0), neg<i32, overflow=ub>(const<i32>(12)), neg<i32, overflow=ub>(const<i32>(31)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>, ptr<const i8>, ptr<const @type0>, i32, i32, i32, i32, i32) -> void>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%21)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), addr_of<ptr<const @type0>>(%14), const<i32>(0), const<i32>(0), const<i32>(9), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>, ptr<const i8>, ptr<const @type0>, i32, i32, i32, i32, i32) -> void>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%23)), null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%24)), addr_of<ptr<const @type0>>(%14), const<i32>(1), const<i32>(0), const<i32>(0), neg<i32, overflow=ub>(const<i32>(12)), neg<i32, overflow=ub>(const<i32>(32)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
