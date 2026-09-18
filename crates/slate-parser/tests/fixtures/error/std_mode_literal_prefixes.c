int f(void) {
#if defined(U8_CHAR)
  return u8'a';
#elif defined(UTF16_STRING)
  return *u"a";
#endif
}

// SLATE-FILECHECK-DEFINES C17_U8_CHAR U8_CHAR
// SLATE-FILECHECK-STD C17_U8_CHAR c17
// SLATE-FILECHECK-ERROR C17_U8_CHAR
// SLATE-FILECHECK-DEFINES C89_UTF16_STRING UTF16_STRING
// SLATE-FILECHECK-STD C89_UTF16_STRING c89
// SLATE-FILECHECK-ERROR C89_UTF16_STRING
// SLATE-FILECHECK-ARGS --dump-ir

// SLATE-FILECHECK-BEGIN C17_U8_CHAR
// C17_U8_CHAR: Error:   × unexpected tokens after expression
// C17_U8_CHAR: ╰─▶ unexpected tokens after expression
// C17_U8_CHAR: ╭─[tests/fixtures/error/std_mode_literal_prefixes.c:3:10]
// C17_U8_CHAR: 2 │ #if defined(U8_CHAR)
// C17_U8_CHAR: 3 │   return u8'a';
// C17_U8_CHAR: ·          ──
// C17_U8_CHAR: 4 │ #elif defined(UTF16_STRING)
// C17_U8_CHAR: ╰────
// SLATE-FILECHECK-END C17_U8_CHAR
// SLATE-FILECHECK-BEGIN C89_UTF16_STRING
// C89_UTF16_STRING: Error:   × unexpected tokens after expression
// C89_UTF16_STRING: ╰─▶ unexpected tokens after expression
// C89_UTF16_STRING: ╭─[tests/fixtures/error/std_mode_literal_prefixes.c:5:10]
// C89_UTF16_STRING: 4 │ #elif defined(UTF16_STRING)
// C89_UTF16_STRING: 5 │   return *u"a";
// C89_UTF16_STRING: ·          ─
// C89_UTF16_STRING: 6 │ #endif
// C89_UTF16_STRING: ╰────
// SLATE-FILECHECK-END C89_UTF16_STRING
