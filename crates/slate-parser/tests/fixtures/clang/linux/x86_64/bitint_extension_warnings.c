_BitInt(8) global;
struct S {
  _BitInt(17) field;
};
_BitInt(33) function(_BitInt(9) parameter) {
  _BitInt(11) local;
  return local + parameter;
}

// SLATE-FILECHECK-ARGS -pedantic
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-WARNING C17

// SLATE-FILECHECK-BEGIN C17
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/clang/linux/x86_64/bitint_extension_warnings.c:1:1]
// C17: 1 │ _BitInt(8) global;
// C17: · ──────────────────
// C17: 2 │ struct S {
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/clang/linux/x86_64/bitint_extension_warnings.c:3:3]
// C17: 2 │ struct S {
// C17: 3 │   _BitInt(17) field;
// C17: ·   ──────────────────
// C17: 4 │ };
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/clang/linux/x86_64/bitint_extension_warnings.c:5:1]
// C17: 4 │     };
// C17: 5 │ ╭─▶ _BitInt(33) function(_BitInt(9) parameter) {
// C17: 6 │ │     _BitInt(11) local;
// C17: 7 │ │     return local + parameter;
// C17: 8 │ ╰─▶ }
// C17: 9 │
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/clang/linux/x86_64/bitint_extension_warnings.c:5:1]
// C17: 4 │     };
// C17: 5 │ ╭─▶ _BitInt(33) function(_BitInt(9) parameter) {
// C17: 6 │ │     _BitInt(11) local;
// C17: 7 │ │     return local + parameter;
// C17: 8 │ ╰─▶ }
// C17: 9 │
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/clang/linux/x86_64/bitint_extension_warnings.c:6:3]
// C17: 5 │ _BitInt(33) function(_BitInt(9) parameter) {
// C17: 6 │   _BitInt(11) local;
// C17: ·   ──────────────────
// C17: 7 │   return local + parameter;
// C17: ╰────
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN IR-C17
// IR-C17: module {
// IR-C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-C17-NEXT:         endian = little;
// IR-C17-NEXT:         pointer [size=8, align=8];
// IR-C17-NEXT:         stack_alignment = 16;
// IR-C17-NEXT:         long_double = f80;
// IR-C17-NEXT:         storage bool [size=1, align=1];
// IR-C17-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C17-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C17-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C17-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C17-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C17-NEXT:         storage bf16 [size=2, align=2];
// IR-C17-NEXT:         storage f16 [size=2, align=2];
// IR-C17-NEXT:         storage f32 [size=4, align=4];
// IR-C17-NEXT:         storage f64 [size=8, align=8];
// IR-C17-NEXT:         storage f80 [size=16, align=16];
// IR-C17-NEXT:         storage f128 [size=16, align=16];
// IR-C17-NEXT:         storage d32 [size=4, align=4];
// IR-C17-NEXT:         storage d64 [size=8, align=8];
// IR-C17-NEXT:         storage d128 [size=16, align=16];
// IR-C17-NEXT:     }
// IR-C17-NEXT:     type @type0 S = struct {
// IR-C17-NEXT:         field0 field: i17b;
// IR-C17-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-C17-NEXT:     global %0 global: i8b [storage=static] [linkage=external];
// IR-C17-NEXT:     fn %2 @function(%3 parameter: i9b) -> i33b [linkage=external] [fallthrough=ub_if_used] {
// IR-C17-NEXT:         let %4 local: i11b [storage=automatic];
// IR-C17-NEXT:         return widen<i33b, reason=return>(add<i11b, overflow=ub>(read<i11b>(%4), widen<i11b, reason=usual_arith>(read<i9b>(%3))));
// IR-C17-NEXT:     }
// IR-C17-NEXT: }
// SLATE-FILECHECK-END IR-C17
