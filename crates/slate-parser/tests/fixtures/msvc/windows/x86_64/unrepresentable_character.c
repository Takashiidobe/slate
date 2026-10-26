// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT
// SLATE-FILECHECK-DEFINES W0
// SLATE-FILECHECK-PREFIX-ARGS W0 /W4 /W0
// SLATE-FILECHECK-DEFINES REENABLED
// SLATE-FILECHECK-WARNING REENABLED
// SLATE-FILECHECK-PREFIX-ARGS REENABLED /w /W1
// SLATE-FILECHECK-DEFINES UTF8
// SLATE-FILECHECK-PREFIX-ARGS UTF8 /execution-charset:utf-8

const char *distinct = "\u4E00\u4E01\u4E00";
const char *pieces = "\u00E9" "\u4E00"
                     "\U0001F600";
const unsigned short *wide = L"\u4E00";
const char *utf8 = u8"\u4E00";
int character = '\u4E01';
int size = sizeof("\u4E02");

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wunrepresentable-character
// DEFAULT:   ⚠ character represented by universal-character-name '\u4E00' cannot be
// DEFAULT:   │ represented in the current code page (1252)
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:2:24]
// DEFAULT:  1 │
// DEFAULT:  2 │ const char *distinct = "\u4E00\u4E01\u4E00";
// DEFAULT:    ·                        ────────────────────
// DEFAULT:  3 │ const char *pieces = "\u00E9" "\u4E00"
// DEFAULT:    ╰────
// DEFAULT: -Wunrepresentable-character
// DEFAULT:   ⚠ character represented by universal-character-name '\u4E01' cannot be
// DEFAULT:   │ represented in the current code page (1252)
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:2:24]
// DEFAULT:  1 │
// DEFAULT:  2 │ const char *distinct = "\u4E00\u4E01\u4E00";
// DEFAULT:    ·                        ────────────────────
// DEFAULT:  3 │ const char *pieces = "\u00E9" "\u4E00"
// DEFAULT:    ╰────
// DEFAULT: -Wunrepresentable-character
// DEFAULT:   ⚠ character represented by universal-character-name '\u4E00' cannot be
// DEFAULT:   │ represented in the current code page (1252)
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:3:31]
// DEFAULT:  2 │ const char *distinct = "\u4E00\u4E01\u4E00";
// DEFAULT:  3 │ const char *pieces = "\u00E9" "\u4E00"
// DEFAULT:    ·                               ────────
// DEFAULT:  4 │                      "\U0001F600";
// DEFAULT:    ╰────
// DEFAULT: -Wunrepresentable-character
// DEFAULT:   ⚠ character represented by universal-character-name '\U0001F600' cannot be
// DEFAULT:   │ represented in the current code page (1252)
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:4:22]
// DEFAULT:  3 │ const char *pieces = "\u00E9" "\u4E00"
// DEFAULT:  4 │                      "\U0001F600";
// DEFAULT:    ·                      ────────────
// DEFAULT:  5 │ const unsigned short *wide = L"\u4E00";
// DEFAULT:    ╰────
// DEFAULT: -Wunrepresentable-character
// DEFAULT:   ⚠ character represented by universal-character-name '\u4E01' cannot be
// DEFAULT:   │ represented in the current code page (1252)
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:7:17]
// DEFAULT:  6 │ const char *utf8 = u8"\u4E00";
// DEFAULT:  7 │ int character = '\u4E01';
// DEFAULT:    ·                 ────────
// DEFAULT:  8 │ int size = sizeof("\u4E02");
// DEFAULT:    ╰────
// DEFAULT: -Wunrepresentable-character
// DEFAULT:   ⚠ character represented by universal-character-name '\u4E02' cannot be
// DEFAULT:   │ represented in the current code page (1252)
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:8:19]
// DEFAULT:  7 │ int character = '\u4E01';
// DEFAULT:  8 │ int size = sizeof("\u4E02");
// DEFAULT:    ·                   ────────
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN REENABLED
// REENABLED: -Wunrepresentable-character
// REENABLED:   ⚠ character represented by universal-character-name '\u4E00' cannot be
// REENABLED:   │ represented in the current code page (1252)
// REENABLED:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:2:24]
// REENABLED:  1 │
// REENABLED:  2 │ const char *distinct = "\u4E00\u4E01\u4E00";
// REENABLED:    ·                        ────────────────────
// REENABLED:  3 │ const char *pieces = "\u00E9" "\u4E00"
// REENABLED:    ╰────
// REENABLED: -Wunrepresentable-character
// REENABLED:   ⚠ character represented by universal-character-name '\u4E01' cannot be
// REENABLED:   │ represented in the current code page (1252)
// REENABLED:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:2:24]
// REENABLED:  1 │
// REENABLED:  2 │ const char *distinct = "\u4E00\u4E01\u4E00";
// REENABLED:    ·                        ────────────────────
// REENABLED:  3 │ const char *pieces = "\u00E9" "\u4E00"
// REENABLED:    ╰────
// REENABLED: -Wunrepresentable-character
// REENABLED:   ⚠ character represented by universal-character-name '\u4E00' cannot be
// REENABLED:   │ represented in the current code page (1252)
// REENABLED:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:3:31]
// REENABLED:  2 │ const char *distinct = "\u4E00\u4E01\u4E00";
// REENABLED:  3 │ const char *pieces = "\u00E9" "\u4E00"
// REENABLED:    ·                               ────────
// REENABLED:  4 │                      "\U0001F600";
// REENABLED:    ╰────
// REENABLED: -Wunrepresentable-character
// REENABLED:   ⚠ character represented by universal-character-name '\U0001F600' cannot be
// REENABLED:   │ represented in the current code page (1252)
// REENABLED:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:4:22]
// REENABLED:  3 │ const char *pieces = "\u00E9" "\u4E00"
// REENABLED:  4 │                      "\U0001F600";
// REENABLED:    ·                      ────────────
// REENABLED:  5 │ const unsigned short *wide = L"\u4E00";
// REENABLED:    ╰────
// REENABLED: -Wunrepresentable-character
// REENABLED:   ⚠ character represented by universal-character-name '\u4E01' cannot be
// REENABLED:   │ represented in the current code page (1252)
// REENABLED:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:7:17]
// REENABLED:  6 │ const char *utf8 = u8"\u4E00";
// REENABLED:  7 │ int character = '\u4E01';
// REENABLED:    ·                 ────────
// REENABLED:  8 │ int size = sizeof("\u4E02");
// REENABLED:    ╰────
// REENABLED: -Wunrepresentable-character
// REENABLED:   ⚠ character represented by universal-character-name '\u4E02' cannot be
// REENABLED:   │ represented in the current code page (1252)
// REENABLED:    ╭─[tests/fixtures/msvc/windows/x86_64/unrepresentable_character.c:8:19]
// REENABLED:  7 │ int character = '\u4E01';
// REENABLED:  8 │ int size = sizeof("\u4E02");
// REENABLED:    ·                   ────────
// REENABLED:    ╰────
// SLATE-FILECHECK-END REENABLED
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f64;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([63, 63, 63, 0]) [linkage=internal];
// IR-DEFAULT-NEXT:     global %[[VALUE_distinct:[0-9]+]] distinct: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([233, 63, 63, 0]) [linkage=internal];
// IR-DEFAULT-NEXT:     global %[[VALUE_pieces:[0-9]+]] pieces: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([19968, 0]) [linkage=internal];
// IR-DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(2)>(%[[VALUE_str_3]])) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([228, 184, 128, 0]) [linkage=internal];
// IR-DEFAULT-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(63) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_size:[0-9]+]] size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(2))) [linkage=external];
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
// SLATE-FILECHECK-BEGIN W0
// W0: module {
// W0-NEXT:     target "x86_64-pc-windows-msvc" {
// W0-NEXT:         endian = little;
// W0-NEXT:         pointer [size=8, align=8];
// W0-NEXT:         stack_alignment = 16;
// W0-NEXT:         long_double = f64;
// W0-NEXT:         storage bool [size=1, align=1];
// W0-NEXT:         storage i8, u8 [size=1, align=1];
// W0-NEXT:         storage i16, u16 [size=2, align=2];
// W0-NEXT:         storage i32, u32 [size=4, align=4];
// W0-NEXT:         storage i64, u64 [size=8, align=8];
// W0-NEXT:         storage i128, u128 [size=16, align=16];
// W0-NEXT:         storage bf16 [size=2, align=2];
// W0-NEXT:         storage f16 [size=2, align=2];
// W0-NEXT:         storage f32 [size=4, align=4];
// W0-NEXT:         storage f64 [size=8, align=8];
// W0-NEXT:         storage f128 [size=16, align=16];
// W0-NEXT:         storage d32 [size=4, align=4];
// W0-NEXT:         storage d64 [size=8, align=8];
// W0-NEXT:         storage d128 [size=16, align=16];
// W0-NEXT:     }
// W0-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([63, 63, 63, 0]) [linkage=internal];
// W0-NEXT:     global %[[VALUE_distinct:[0-9]+]] distinct: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])) [linkage=external];
// W0-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([233, 63, 63, 0]) [linkage=internal];
// W0-NEXT:     global %[[VALUE_pieces:[0-9]+]] pieces: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])) [linkage=external];
// W0-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([19968, 0]) [linkage=internal];
// W0-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(2)>(%[[VALUE_str_3]])) [linkage=external];
// W0-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([228, 184, 128, 0]) [linkage=internal];
// W0-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])) [linkage=external];
// W0-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(63) [linkage=external];
// W0-NEXT:     global %[[VALUE_size:[0-9]+]] size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(2))) [linkage=external];
// W0-NEXT: }
// SLATE-FILECHECK-END W0
// SLATE-FILECHECK-BEGIN IR-REENABLED
// IR-REENABLED: module {
// IR-REENABLED-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-REENABLED-NEXT:         endian = little;
// IR-REENABLED-NEXT:         pointer [size=8, align=8];
// IR-REENABLED-NEXT:         stack_alignment = 16;
// IR-REENABLED-NEXT:         long_double = f64;
// IR-REENABLED-NEXT:         storage bool [size=1, align=1];
// IR-REENABLED-NEXT:         storage i8, u8 [size=1, align=1];
// IR-REENABLED-NEXT:         storage i16, u16 [size=2, align=2];
// IR-REENABLED-NEXT:         storage i32, u32 [size=4, align=4];
// IR-REENABLED-NEXT:         storage i64, u64 [size=8, align=8];
// IR-REENABLED-NEXT:         storage i128, u128 [size=16, align=16];
// IR-REENABLED-NEXT:         storage bf16 [size=2, align=2];
// IR-REENABLED-NEXT:         storage f16 [size=2, align=2];
// IR-REENABLED-NEXT:         storage f32 [size=4, align=4];
// IR-REENABLED-NEXT:         storage f64 [size=8, align=8];
// IR-REENABLED-NEXT:         storage f128 [size=16, align=16];
// IR-REENABLED-NEXT:         storage d32 [size=4, align=4];
// IR-REENABLED-NEXT:         storage d64 [size=8, align=8];
// IR-REENABLED-NEXT:         storage d128 [size=16, align=16];
// IR-REENABLED-NEXT:     }
// IR-REENABLED-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([63, 63, 63, 0]) [linkage=internal];
// IR-REENABLED-NEXT:     global %[[VALUE_distinct:[0-9]+]] distinct: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])) [linkage=external];
// IR-REENABLED-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([233, 63, 63, 0]) [linkage=internal];
// IR-REENABLED-NEXT:     global %[[VALUE_pieces:[0-9]+]] pieces: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])) [linkage=external];
// IR-REENABLED-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([19968, 0]) [linkage=internal];
// IR-REENABLED-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(2)>(%[[VALUE_str_3]])) [linkage=external];
// IR-REENABLED-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([228, 184, 128, 0]) [linkage=internal];
// IR-REENABLED-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])) [linkage=external];
// IR-REENABLED-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(63) [linkage=external];
// IR-REENABLED-NEXT:     global %[[VALUE_size:[0-9]+]] size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(2))) [linkage=external];
// IR-REENABLED-NEXT: }
// SLATE-FILECHECK-END IR-REENABLED
// SLATE-FILECHECK-BEGIN UTF8
// UTF8: module {
// UTF8-NEXT:     target "x86_64-pc-windows-msvc" {
// UTF8-NEXT:         endian = little;
// UTF8-NEXT:         pointer [size=8, align=8];
// UTF8-NEXT:         stack_alignment = 16;
// UTF8-NEXT:         long_double = f64;
// UTF8-NEXT:         storage bool [size=1, align=1];
// UTF8-NEXT:         storage i8, u8 [size=1, align=1];
// UTF8-NEXT:         storage i16, u16 [size=2, align=2];
// UTF8-NEXT:         storage i32, u32 [size=4, align=4];
// UTF8-NEXT:         storage i64, u64 [size=8, align=8];
// UTF8-NEXT:         storage i128, u128 [size=16, align=16];
// UTF8-NEXT:         storage bf16 [size=2, align=2];
// UTF8-NEXT:         storage f16 [size=2, align=2];
// UTF8-NEXT:         storage f32 [size=4, align=4];
// UTF8-NEXT:         storage f64 [size=8, align=8];
// UTF8-NEXT:         storage f128 [size=16, align=16];
// UTF8-NEXT:         storage d32 [size=4, align=4];
// UTF8-NEXT:         storage d64 [size=8, align=8];
// UTF8-NEXT:         storage d128 [size=16, align=16];
// UTF8-NEXT:     }
// UTF8-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([228, 184, 128, 228, 184, 129, 228, 184, 128, 0]) [linkage=internal];
// UTF8-NEXT:     global %[[VALUE_distinct:[0-9]+]] distinct: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])) [linkage=external];
// UTF8-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([195, 169, 228, 184, 128, 240, 159, 152, 128, 0]) [linkage=internal];
// UTF8-NEXT:     global %[[VALUE_pieces:[0-9]+]] pieces: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_2]])) [linkage=external];
// UTF8-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([19968, 0]) [linkage=internal];
// UTF8-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(2)>(%[[VALUE_str_3]])) [linkage=external];
// UTF8-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([228, 184, 128, 0]) [linkage=internal];
// UTF8-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])) [linkage=external];
// UTF8-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(14989441) [linkage=external];
// UTF8-NEXT:     global %[[VALUE_size:[0-9]+]] size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))) [linkage=external];
// UTF8-NEXT: }
// SLATE-FILECHECK-END UTF8
