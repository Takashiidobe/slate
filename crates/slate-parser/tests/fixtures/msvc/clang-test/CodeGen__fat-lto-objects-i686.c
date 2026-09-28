



/// Full LTO always sets EnableSplitLTOUnit when the summary is used.






/// Make sure that FatLTO generates .llvm.lto sections that are the same as the output from normal LTO compilations



/// Be sure we enable split LTO units correctly under -ffat-lto-objects.






const char* foo = "foo";

int test(void) {
  const char* bar = "bar";
  return 0xabcd;
}

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=i686-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     global %3 .str3: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %0 foo: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%3)) [linkage=external];
// DEFAULT-NEXT:     global %4 .str4: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @test() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 bar: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%4));
// DEFAULT-NEXT:         return const<i32>(43981);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
