// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ARGS --dump-ir

#if __STDC_VERSION__ >= 202311L
const unsigned char *utf8 = u8"Ω";
#else
const char *utf8 = u8"Ω";
#endif
const unsigned short *utf16 = u"Ω";
const unsigned int *utf32 = U"Ω";

// SLATE-FILECHECK-BEGIN C11
// C11: module {
// C11-NEXT:     target "x86_64-unknown-linux-gnu" {
// C11-NEXT:         endian = little;
// C11-NEXT:         pointer [size=8, align=8];
// C11-NEXT:         stack_alignment = 16;
// C11-NEXT:         long_double = f80;
// C11-NEXT:         storage bool [size=1, align=1];
// C11-NEXT:         storage i8, u8 [size=1, align=1];
// C11-NEXT:         storage i16, u16 [size=2, align=2];
// C11-NEXT:         storage i32, u32 [size=4, align=4];
// C11-NEXT:         storage i64, u64 [size=8, align=8];
// C11-NEXT:         storage i128, u128 [size=16, align=16];
// C11-NEXT:         storage f16 [size=2, align=2];
// C11-NEXT:         storage f32 [size=4, align=4];
// C11-NEXT:         storage f64 [size=8, align=8];
// C11-NEXT:         storage f80 [size=16, align=16];
// C11-NEXT:         storage f128 [size=16, align=16];
// C11-NEXT:     }
// C11-NEXT:     global %3 .str3: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([206, 169, 0]) [linkage=internal];
// C11-NEXT:     global %0 utf8: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%3)) [linkage=external];
// C11-NEXT:     global %4 .str4: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([937, 0]) [linkage=internal];
// C11-NEXT:     global %1 utf16: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(2)>(%4)) [linkage=external];
// C11-NEXT:     global %5 .str5: array<u32, 2> [storage=static] = code_units<array<u32, 2>>([937, 0]) [linkage=internal];
// C11-NEXT:     global %2 utf32: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(2)>(%5)) [linkage=external];
// C11-NEXT: }
// SLATE-FILECHECK-END C11
// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %3 .str3: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([206, 169, 0]) [linkage=internal];
// C17-NEXT:     global %0 utf8: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%3)) [linkage=external];
// C17-NEXT:     global %4 .str4: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([937, 0]) [linkage=internal];
// C17-NEXT:     global %1 utf16: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(2)>(%4)) [linkage=external];
// C17-NEXT:     global %5 .str5: array<u32, 2> [storage=static] = code_units<array<u32, 2>>([937, 0]) [linkage=internal];
// C17-NEXT:     global %2 utf32: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(2)>(%5)) [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %3 .str3: array<u8, 3> [storage=static] = code_units<array<u8, 3>>([206, 169, 0]) [linkage=internal];
// C23-NEXT:     global %0 utf8: ptr<const u8> [storage=static] = pointer_cast<ptr<const u8>, reason=assign>(array_decay<ptr<u8>, length=Some(3)>(%3)) [linkage=external];
// C23-NEXT:     global %4 .str4: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([937, 0]) [linkage=internal];
// C23-NEXT:     global %1 utf16: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(2)>(%4)) [linkage=external];
// C23-NEXT:     global %5 .str5: array<u32, 2> [storage=static] = code_units<array<u32, 2>>([937, 0]) [linkage=internal];
// C23-NEXT:     global %2 utf32: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(2)>(%5)) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
