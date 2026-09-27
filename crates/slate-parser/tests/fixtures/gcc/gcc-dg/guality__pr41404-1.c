/* PR debug/41404 */
/* { dg-do run } */
/* { dg-options "-g" } */

__attribute__((noinline)) int bar1(int i) {
  const char *foo = "foo";
  asm volatile("" : "+r"(i) : : "memory");
  i++; /* { dg-final { gdb-test . "*foo" "'f'" } } */
  asm volatile("" : "+r"(i) : : "memory");
  foo = "bar";
  asm volatile("" : "+r"(i) : : "memory");
  i++; /* { dg-final { gdb-test . "*foo" "'b'" } } */
  asm volatile("" : "+r"(i) : : "memory");
  return i;
}

__attribute__((noinline)) int bar2(int i) {
  const char *foo = "foo";
  asm volatile("" : "+r"(i) : : "memory");
  i++; /* { dg-final { gdb-test . "*foo" "'f'" } } */
  asm volatile("" : "+r"(i) : : "memory");
  return i;
}

__attribute__((noinline)) const char *baz(int i) { return i ? "foo" : "bar"; }

int
main(void) {
  bar1(6);
  bar2(6);
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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @bar1(%1 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 foo: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%9));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" place<i32>(%1);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %14: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%15));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" place<i32>(%1);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<const i8>>(%2, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%10)));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" place<i32>(%1);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%17));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" place<i32>(%1);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar2(%4 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 foo: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%11));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" place<i32>(%4);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %18: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%19));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" place<i32>(%4);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz(%7 i: i32) -> ptr<const i8> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(conditional<ptr<i8>>(ne<i32>(read<i32>(%7), const<i32>(0)), array_decay<ptr<i8>, length=Some(4)>(%12), array_decay<ptr<i8>, length=Some(4)>(%13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, const<i32>(6));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%3, const<i32>(6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
