float _Imaginary imaginary_value;
__m128 vector128_value;
__m128d vector128d_value;
__m128i vector128i_value;
__m256 vector256_value;
__m256d vector256d_value;
__m256i vector256i_value;
__m512 vector512_value;
__m512d vector512d_value;
__m512i vector512i_value;

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:2:1]
// DEFAULT: 1 │ float _Imaginary imaginary_value;
// DEFAULT: 2 │ __m128 vector128_value;
// DEFAULT: · ───────────────────────
// DEFAULT: 3 │ __m128d vector128d_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:3:1]
// DEFAULT: 2 │ __m128 vector128_value;
// DEFAULT: 3 │ __m128d vector128d_value;
// DEFAULT: · ─────────────────────────
// DEFAULT: 4 │ __m128i vector128i_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:4:1]
// DEFAULT: 3 │ __m128d vector128d_value;
// DEFAULT: 4 │ __m128i vector128i_value;
// DEFAULT: · ─────────────────────────
// DEFAULT: 5 │ __m256 vector256_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:5:1]
// DEFAULT: 4 │ __m128i vector128i_value;
// DEFAULT: 5 │ __m256 vector256_value;
// DEFAULT: · ───────────────────────
// DEFAULT: 6 │ __m256d vector256d_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:6:1]
// DEFAULT: 5 │ __m256 vector256_value;
// DEFAULT: 6 │ __m256d vector256d_value;
// DEFAULT: · ─────────────────────────
// DEFAULT: 7 │ __m256i vector256i_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:7:1]
// DEFAULT: 6 │ __m256d vector256d_value;
// DEFAULT: 7 │ __m256i vector256i_value;
// DEFAULT: · ─────────────────────────
// DEFAULT: 8 │ __m512 vector512_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:8:1]
// DEFAULT: 7 │ __m256i vector256i_value;
// DEFAULT: 8 │ __m512 vector512_value;
// DEFAULT: · ───────────────────────
// DEFAULT: 9 │ __m512d vector512d_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:9:1]
// DEFAULT: 8 │ __m512 vector512_value;
// DEFAULT: 9 │ __m512d vector512d_value;
// DEFAULT: · ─────────────────────────
// DEFAULT: 10 │ __m512i vector512i_value;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/target-builtin-types.c:10:1]
// DEFAULT: 9 │ __m512d vector512d_value;
// DEFAULT: 10 │ __m512i vector512i_value;
// DEFAULT: · ─────────────────────────
// DEFAULT: 11 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
