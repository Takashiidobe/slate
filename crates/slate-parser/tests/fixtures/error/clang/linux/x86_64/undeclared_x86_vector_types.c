// SLATE-FILECHECK-ERROR RESOLVE
// SLATE-FILECHECK-ARGS --dump-ir-names

__m128 vector128_value;
__m128d vector128d_value;
__m128i vector128i_value;
__m256 vector256_value;
__m256d vector256d_value;
__m256i vector256i_value;
__m512 vector512_value;
__m512d vector512d_value;
__m512i vector512i_value;

// SLATE-FILECHECK-BEGIN RESOLVE
// RESOLVE: Error:   × semantic analysis failed
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m128`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:2:1]
// RESOLVE: 1 │
// RESOLVE: 2 │ __m128 vector128_value;
// RESOLVE: · ──────
// RESOLVE: 3 │ __m128d vector128d_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m128d`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:3:1]
// RESOLVE: 2 │ __m128 vector128_value;
// RESOLVE: 3 │ __m128d vector128d_value;
// RESOLVE: · ───────
// RESOLVE: 4 │ __m128i vector128i_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m128i`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:4:1]
// RESOLVE: 3 │ __m128d vector128d_value;
// RESOLVE: 4 │ __m128i vector128i_value;
// RESOLVE: · ───────
// RESOLVE: 5 │ __m256 vector256_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m256`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:5:1]
// RESOLVE: 4 │ __m128i vector128i_value;
// RESOLVE: 5 │ __m256 vector256_value;
// RESOLVE: · ──────
// RESOLVE: 6 │ __m256d vector256d_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m256d`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:6:1]
// RESOLVE: 5 │ __m256 vector256_value;
// RESOLVE: 6 │ __m256d vector256d_value;
// RESOLVE: · ───────
// RESOLVE: 7 │ __m256i vector256i_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m256i`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:7:1]
// RESOLVE: 6 │ __m256d vector256d_value;
// RESOLVE: 7 │ __m256i vector256i_value;
// RESOLVE: · ───────
// RESOLVE: 8 │ __m512 vector512_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m512`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:8:1]
// RESOLVE: 7 │ __m256i vector256i_value;
// RESOLVE: 8 │ __m512 vector512_value;
// RESOLVE: · ──────
// RESOLVE: 9 │ __m512d vector512d_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m512d`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:9:1]
// RESOLVE: 8 │ __m512 vector512_value;
// RESOLVE: 9 │ __m512d vector512d_value;
// RESOLVE: · ───────
// RESOLVE: 10 │ __m512i vector512i_value;
// RESOLVE: ╰────
// RESOLVE: Error:
// RESOLVE: × unknown type name `__m512i`
// RESOLVE: ╭─[tests/fixtures/error/clang/linux/x86_64/undeclared_x86_vector_types.c:10:1]
// RESOLVE: 9 │ __m512d vector512d_value;
// RESOLVE: 10 │ __m512i vector512i_value;
// RESOLVE: · ───────
// RESOLVE: ╰────
// SLATE-FILECHECK-END RESOLVE
