/* { dg-options "-Wformat" } */
extern int printf (const char *__restrict __format, ...);
void test (void)
{
  /* A very long line, so that we start a new line map.  */
  printf ("%llu01233456789012334567890123345678901233456789012334567890123345678901233456789012334567890123345678901233456789012334567890123345678901233456789"); /* { dg-warning "15: format .%llu. expects a matching" } */
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %4 .str4: array<i8, 148> [storage=static] = code_units<array<i8, 148>>([37, 108, 108, 117, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%3 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(148)>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
