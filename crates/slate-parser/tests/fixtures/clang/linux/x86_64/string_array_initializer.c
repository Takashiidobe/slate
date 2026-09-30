// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

const char inferred[] = "ab";
char padded[5] = "ab";
char exact[2] = "ab";
unsigned char bytes[] = "\xff";
const unsigned short utf16[] = u"a";
static_assert(sizeof(inferred) == 3);
static_assert(sizeof(utf16) == 4);

int local(void) {
  char buf[] = "xyz";
  char wide[6] = "xy";
  return sizeof(buf) + wide[0];
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_inferred:[0-9]+]] inferred: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_padded:[0-9]+]] padded: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 98, 0, 0, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_exact:[0-9]+]] exact: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 98]) [linkage=external];
// IR-NEXT:     global %[[VALUE_bytes:[0-9]+]] bytes: array<u8, 2> [storage=static] = code_units<array<u8, 2>>([255, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_utf16:[0-9]+]] utf16: array<u16, 2> [storage=static] [const] = code_units<array<u16, 2>>([97, 0]) [linkage=external];
// IR-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([120, 121, 122, 0]);
// IR-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([120, 121, 0, 0, 0, 0]);
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_wide]]), const<i32>(0))))))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
