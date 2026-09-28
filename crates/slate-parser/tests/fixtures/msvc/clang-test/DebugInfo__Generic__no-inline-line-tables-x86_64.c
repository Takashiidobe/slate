
int x;
__attribute((always_inline)) void f(void) {
  x += 1;
}
int main(void) {
  f();
  x += 2;
  return x;
}

// Check that clang emits the location of the call site and not the inlined
// function in the debug info.

// Check that the no-inline-line-tables attribute is added.

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %0 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @f() -> void [linkage=external] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:         let %4: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%3), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%0, read<i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %5: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:         let %6: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%5), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%0, read<i32>(%6));
// DEFAULT-NEXT:         return read<i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
