#define HEX_PIECES "\x1" "2"
#define OCTAL_PIECES "\1" "23"
char global_hex[] = HEX_PIECES;
char global_octal[] = OCTAL_PIECES;
char global_plain[] = "a" "b";
unsigned short global_utf16[] = u"\x1" "2";
unsigned int global_utf32[] = "\1" U"23";
int global_wide[] = L"\x1" "2";
char global_utf8[] = "\x1" u8"2";

int literals(void) {
  char local_hex[] = HEX_PIECES;
  char local_octal[] = OCTAL_PIECES;
  char local_plain[] = "a" "b";
  unsigned short local_utf16[] = u"\x1" "2";
  unsigned int local_utf32[] = "\1" U"23";
  int local_wide[] = L"\x1" "2";
  char local_utf8[] = "\x1" u8"2";
  return sizeof(HEX_PIECES) + sizeof(OCTAL_PIECES) + sizeof("a" "b")
    + sizeof(u"\x1" "2") + sizeof("\1" U"23")
    + sizeof(L"\x1" "2") + sizeof("\x1" u8"2");
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
// DEFAULT-NEXT:     global %0 global_hex: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([1, 50, 0]) [linkage=external];
// DEFAULT-NEXT:     global %1 global_octal: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([1, 50, 51, 0]) [linkage=external];
// DEFAULT-NEXT:     global %2 global_plain: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=external];
// DEFAULT-NEXT:     global %3 global_utf16: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([1, 50, 0]) [linkage=external];
// DEFAULT-NEXT:     global %4 global_utf32: array<u32, 4> [storage=static] [align=16] = code_units<array<u32, 4>>([1, 50, 51, 0]) [linkage=external];
// DEFAULT-NEXT:     global %5 global_wide: array<i32, 3> [storage=static] = code_units<array<i32, 3>>([1, 50, 0]) [linkage=external];
// DEFAULT-NEXT:     global %6 global_utf8: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([1, 50, 0]) [linkage=external];
// DEFAULT-NEXT:     fn %7 @literals() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 local_hex: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([1, 50, 0]);
// DEFAULT-NEXT:         let %9 local_octal: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([1, 50, 51, 0]);
// DEFAULT-NEXT:         let %10 local_plain: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([97, 98, 0]);
// DEFAULT-NEXT:         let %11 local_utf16: array<u16, 3> [storage=automatic] = code_units<array<u16, 3>>([1, 50, 0]);
// DEFAULT-NEXT:         let %12 local_utf32: array<u32, 4> [storage=automatic] [align=16] = code_units<array<u32, 4>>([1, 50, 51, 0]);
// DEFAULT-NEXT:         let %13 local_wide: array<i32, 3> [storage=automatic] = code_units<array<i32, 3>>([1, 50, 0]);
// DEFAULT-NEXT:         let %14 local_utf8: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([1, 50, 0]);
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(3), const<u64>(4)), const<u64>(3)), const<u64>(6)), const<u64>(16)), const<u64>(12)), const<u64>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
