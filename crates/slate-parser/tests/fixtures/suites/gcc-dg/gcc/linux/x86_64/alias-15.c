/* { dg-do compile } */
/* { dg-additional-options  "-O2 -fcommon -fdump-ipa-cgraph" } */

/* RTL-level CSE shouldn't introduce LCO (for the string) into varpool */
char *p;

void foo ()
{
  p = "abc\n";

  while (*p != '\n')
    p++;
}

/* { dg-final { scan-ipa-dump-not "LC0" "cgraph" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fcommon
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
// DEFAULT-NEXT:     global %0 p: ptr<i8> [storage=static] [linkage=external] [common];
// DEFAULT-NEXT:     global %2 .str2: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 98, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i8>>(%0, array_decay<ptr<i8>, length=Some(5)>(%2));
// DEFAULT-NEXT:         while %3 ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%0)))), const<i32>(10))
// DEFAULT-NEXT:             let %4: ptr<i8> [synthetic] = read<ptr<i8>>(%0);
// DEFAULT-NEXT:             let %5: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%4), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%0, read<ptr<i8>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
