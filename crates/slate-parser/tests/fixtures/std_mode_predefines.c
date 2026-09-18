#ifdef __STRICT_ANSI__
int strict_ansi;
#endif
#ifdef linux
int gnu_namespace_linux;
#endif
#ifdef unix
int gnu_namespace_unix;
#endif
#ifdef __GNUC_STDC_INLINE__
int stdc_inline_semantics;
#endif
#ifdef __GNUC_GNU_INLINE__
int gnu_inline_semantics;
#endif
#ifdef __CHAR8_TYPE__
__CHAR8_TYPE__ char8_unit;
#endif
#if defined(__GCC_ATOMIC_CHAR8_T_LOCK_FREE) && __GCC_ATOMIC_CHAR8_T_LOCK_FREE == 2
int char8_lock_free;
#endif
#ifdef __UINT64_FMTb__
const char *uint64_binary_format = __UINT64_FMTb__;
#endif

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES GNU89
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "strict_ansi",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// C89-NEXT: decl[{{[0-9]+}}]: Declaration(
// C89-NEXT:       Declaration {
// C89-NEXT:           specifiers: DeclarationSpecifiers {
// C89-NEXT:               ty: Integer(
// C89-NEXT:                   Ranked {
// C89-NEXT:                       rank: Int,
// C89-NEXT:                       signed: true,
// C89-NEXT:                   },
// C89-NEXT:               ),
// C89-NEXT:           },
// C89-NEXT:           declarators: [
// C89-NEXT:               InitDeclaratorKind {
// C89-NEXT:                   declarator: Name(
// C89-NEXT:                       "gnu_inline_semantics",
// C89-NEXT:                   ),
// C89-NEXT:               },
// C89-NEXT:           ],
// C89-NEXT:       },
// C89-NEXT:   )
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: decl[{{[0-9]+}}]: Declaration(
// GNU89-NEXT:       Declaration {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: Integer(
// GNU89-NEXT:                   Ranked {
// GNU89-NEXT:                       rank: Int,
// GNU89-NEXT:                       signed: true,
// GNU89-NEXT:                   },
// GNU89-NEXT:               ),
// GNU89-NEXT:           },
// GNU89-NEXT:           declarators: [
// GNU89-NEXT:               InitDeclaratorKind {
// GNU89-NEXT:                   declarator: Name(
// GNU89-NEXT:                       "gnu_namespace_linux",
// GNU89-NEXT:                   ),
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// GNU89-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU89-NEXT:       Declaration {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: Integer(
// GNU89-NEXT:                   Ranked {
// GNU89-NEXT:                       rank: Int,
// GNU89-NEXT:                       signed: true,
// GNU89-NEXT:                   },
// GNU89-NEXT:               ),
// GNU89-NEXT:           },
// GNU89-NEXT:           declarators: [
// GNU89-NEXT:               InitDeclaratorKind {
// GNU89-NEXT:                   declarator: Name(
// GNU89-NEXT:                       "gnu_namespace_unix",
// GNU89-NEXT:                   ),
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// GNU89-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU89-NEXT:       Declaration {
// GNU89-NEXT:           specifiers: DeclarationSpecifiers {
// GNU89-NEXT:               ty: Integer(
// GNU89-NEXT:                   Ranked {
// GNU89-NEXT:                       rank: Int,
// GNU89-NEXT:                       signed: true,
// GNU89-NEXT:                   },
// GNU89-NEXT:               ),
// GNU89-NEXT:           },
// GNU89-NEXT:           declarators: [
// GNU89-NEXT:               InitDeclaratorKind {
// GNU89-NEXT:                   declarator: Name(
// GNU89-NEXT:                       "gnu_inline_semantics",
// GNU89-NEXT:                   ),
// GNU89-NEXT:               },
// GNU89-NEXT:           ],
// GNU89-NEXT:       },
// GNU89-NEXT:   )
// SLATE-FILECHECK-END GNU89
// SLATE-FILECHECK-BEGIN C99
// C99: decl[{{[0-9]+}}]: Declaration(
// C99-NEXT:       Declaration {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarators: [
// C99-NEXT:               InitDeclaratorKind {
// C99-NEXT:                   declarator: Name(
// C99-NEXT:                       "strict_ansi",
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// C99-NEXT: decl[{{[0-9]+}}]: Declaration(
// C99-NEXT:       Declaration {
// C99-NEXT:           specifiers: DeclarationSpecifiers {
// C99-NEXT:               ty: Integer(
// C99-NEXT:                   Ranked {
// C99-NEXT:                       rank: Int,
// C99-NEXT:                       signed: true,
// C99-NEXT:                   },
// C99-NEXT:               ),
// C99-NEXT:           },
// C99-NEXT:           declarators: [
// C99-NEXT:               InitDeclaratorKind {
// C99-NEXT:                   declarator: Name(
// C99-NEXT:                       "stdc_inline_semantics",
// C99-NEXT:                   ),
// C99-NEXT:               },
// C99-NEXT:           ],
// C99-NEXT:       },
// C99-NEXT:   )
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C17
// C17: decl[{{[0-9]+}}]: Declaration(
// C17-NEXT:       Declaration {
// C17-NEXT:           specifiers: DeclarationSpecifiers {
// C17-NEXT:               ty: Integer(
// C17-NEXT:                   Ranked {
// C17-NEXT:                       rank: Int,
// C17-NEXT:                       signed: true,
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:           },
// C17-NEXT:           declarators: [
// C17-NEXT:               InitDeclaratorKind {
// C17-NEXT:                   declarator: Name(
// C17-NEXT:                       "strict_ansi",
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:   )
// C17-NEXT: decl[{{[0-9]+}}]: Declaration(
// C17-NEXT:       Declaration {
// C17-NEXT:           specifiers: DeclarationSpecifiers {
// C17-NEXT:               ty: Integer(
// C17-NEXT:                   Ranked {
// C17-NEXT:                       rank: Int,
// C17-NEXT:                       signed: true,
// C17-NEXT:                   },
// C17-NEXT:               ),
// C17-NEXT:           },
// C17-NEXT:           declarators: [
// C17-NEXT:               InitDeclaratorKind {
// C17-NEXT:                   declarator: Name(
// C17-NEXT:                       "stdc_inline_semantics",
// C17-NEXT:                   ),
// C17-NEXT:               },
// C17-NEXT:           ],
// C17-NEXT:       },
// C17-NEXT:   )
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: decl[{{[0-9]+}}]: Declaration(
// GNU17-NEXT:       Declaration {
// GNU17-NEXT:           specifiers: DeclarationSpecifiers {
// GNU17-NEXT:               ty: Integer(
// GNU17-NEXT:                   Ranked {
// GNU17-NEXT:                       rank: Int,
// GNU17-NEXT:                       signed: true,
// GNU17-NEXT:                   },
// GNU17-NEXT:               ),
// GNU17-NEXT:           },
// GNU17-NEXT:           declarators: [
// GNU17-NEXT:               InitDeclaratorKind {
// GNU17-NEXT:                   declarator: Name(
// GNU17-NEXT:                       "gnu_namespace_linux",
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// GNU17-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU17-NEXT:       Declaration {
// GNU17-NEXT:           specifiers: DeclarationSpecifiers {
// GNU17-NEXT:               ty: Integer(
// GNU17-NEXT:                   Ranked {
// GNU17-NEXT:                       rank: Int,
// GNU17-NEXT:                       signed: true,
// GNU17-NEXT:                   },
// GNU17-NEXT:               ),
// GNU17-NEXT:           },
// GNU17-NEXT:           declarators: [
// GNU17-NEXT:               InitDeclaratorKind {
// GNU17-NEXT:                   declarator: Name(
// GNU17-NEXT:                       "gnu_namespace_unix",
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// GNU17-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU17-NEXT:       Declaration {
// GNU17-NEXT:           specifiers: DeclarationSpecifiers {
// GNU17-NEXT:               ty: Integer(
// GNU17-NEXT:                   Ranked {
// GNU17-NEXT:                       rank: Int,
// GNU17-NEXT:                       signed: true,
// GNU17-NEXT:                   },
// GNU17-NEXT:               ),
// GNU17-NEXT:           },
// GNU17-NEXT:           declarators: [
// GNU17-NEXT:               InitDeclaratorKind {
// GNU17-NEXT:                   declarator: Name(
// GNU17-NEXT:                       "stdc_inline_semantics",
// GNU17-NEXT:                   ),
// GNU17-NEXT:               },
// GNU17-NEXT:           ],
// GNU17-NEXT:       },
// GNU17-NEXT:   )
// SLATE-FILECHECK-END GNU17
// SLATE-FILECHECK-BEGIN C23
// C23: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "strict_ansi",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "stdc_inline_semantics",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Char {
// C23-NEXT:                       signed: Some(
// C23-NEXT:                           false,
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "char8_unit",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "char8_lock_free",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Char {
// C23-NEXT:                       signed: None,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               qualifiers: Qualifiers {
// C23-NEXT:                   is_const: true,
// C23-NEXT:               },
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Pointer {
// C23-NEXT:                       qualifiers: Qualifiers,
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "uint64_binary_format",
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           StringLiteral(
// C23-NEXT:                               StringLiteral {
// C23-NEXT:                                   encoding: Plain,
// C23-NEXT:                                   code_units: [
// C23-NEXT:                                       108,
// C23-NEXT:                                       98,
// C23-NEXT:                                   ],
// C23-NEXT:                                   pieces: [
// C23-NEXT:                                       "lb",
// C23-NEXT:                                   ],
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
