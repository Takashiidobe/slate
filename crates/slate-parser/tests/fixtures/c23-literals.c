int decimal_separator __attribute__((slate_literal(1'000u)));
int binary_bitint __attribute__((slate_literal(0b1010wb)));
int hexadecimal __attribute__((slate_literal(0x2aUL)));
int decimal_float __attribute__((slate_literal(1.25e+2f)));
int hex_float __attribute__((slate_literal(0x1.fp+2)));
int character __attribute__((slate_literal('a')));
int escaped_string __attribute__((slate_literal("\N{SNOWMAN}")));
int utf8_string __attribute__((slate_literal(u8"text")));
int utf16_string __attribute__((slate_literal(u"text")));
int utf32_string __attribute__((slate_literal(U"text")));
int wide_string __attribute__((slate_literal(L"text")));
int unicode_name __attribute__((slate_literal(\u03B1name)));

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=decimal_separator [attributes=slate_literal(1000)]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=name=binary_bitint [attributes=slate_literal(10)]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=hexadecimal [attributes=slate_literal(42)]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=name=decimal_float [attributes=slate_literal(1.25e+2f)]
// DEFAULT-NEXT: decl[4]: declaration type=int declarator=name=hex_float [attributes=slate_literal(0x1.fp+2)]
// DEFAULT-NEXT: decl[5]: declaration type=int declarator=name=character [attributes=slate_literal('a')]
// DEFAULT-NEXT: decl[6]: declaration type=int declarator=name=escaped_string [attributes=slate_literal("\N{SNOWMAN}")]
// DEFAULT-NEXT: decl[7]: declaration type=int declarator=name=utf8_string [attributes=slate_literal(u8"text")]
// DEFAULT-NEXT: decl[8]: declaration type=int declarator=name=utf16_string [attributes=slate_literal(u"text")]
// DEFAULT-NEXT: decl[9]: declaration type=int declarator=name=utf32_string [attributes=slate_literal(U"text")]
// DEFAULT-NEXT: decl[10]: declaration type=int declarator=name=wide_string [attributes=slate_literal(L"text")]
// DEFAULT-NEXT: decl[11]: declaration type=int declarator=name=unicode_name [attributes=slate_literal(\u03B1name)]
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=decimal_separator [attributes=slate_literal(1000)]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=name=binary_bitint [attributes=slate_literal(10)]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=hexadecimal [attributes=slate_literal(42)]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=name=decimal_float [attributes=slate_literal(1.25e+2f)]
// DEFAULT-NEXT: decl[4]: declaration type=int declarator=name=hex_float [attributes=slate_literal(0x1.fp+2)]
// DEFAULT-NEXT: decl[5]: declaration type=int declarator=name=character [attributes=slate_literal('a')]
// DEFAULT-NEXT: decl[6]: declaration type=int declarator=name=escaped_string [attributes=slate_literal("\N{SNOWMAN}")]
// DEFAULT-NEXT: decl[7]: declaration type=int declarator=name=utf8_string [attributes=slate_literal(u8"text")]
// DEFAULT-NEXT: decl[8]: declaration type=int declarator=name=utf16_string [attributes=slate_literal(u"text")]
// DEFAULT-NEXT: decl[9]: declaration type=int declarator=name=utf32_string [attributes=slate_literal(U"text")]
// DEFAULT-NEXT: decl[10]: declaration type=int declarator=name=wide_string [attributes=slate_literal(L"text")]
// DEFAULT-NEXT: decl[11]: declaration type=int declarator=name=unicode_name [attributes=slate_literal(\u03B1name)]
// SLATE-FILECHECK-END DEFAULT
