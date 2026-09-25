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
// DEFAULT: Error:   × unsupported in numeric IR lowering: target builtin type
// SLATE-FILECHECK-END DEFAULT
