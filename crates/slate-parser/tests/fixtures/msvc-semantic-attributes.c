[[msvc::noinline]] void noinline_fn(void);
[[msvc::forceinline]] inline void inline_fn(void);
__declspec(thread) int thread_local_value;
__declspec(selectany) int selected = 1;
__declspec(noalias) int no_alias(int *);
__declspec(restrict) void *restricted(unsigned long);
__declspec(nothrow) void no_throw(void);
__declspec(allocate(".data.custom")) int placed;
__declspec(code_seg(".text.custom")) void code(void);
__declspec(dllimport) int imported;
__declspec(dllexport) void exported(void);
__declspec(align(32)) int aligned;
void local(void) { [[msvc::noinline]] void nested(void); }
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-FLAVOR msvc

// SLATE-FILECHECK-BEGIN C23
// C23: decl[0]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   NoInline,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "noinline_fn",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[1]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               is_inline: true,
// C23-NEXT:               attributes: [
// C23-NEXT:                   AlwaysInline,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "inline_fn",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[2]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               attributes: [
// C23-NEXT:                   ThreadLocal,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "thread_local_value",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[3]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               attributes: [
// C23-NEXT:                   SelectAny,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "selected",
// C23-NEXT:                   ),
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           IntegerLiteral(
// C23-NEXT:                               IntegerLiteral {
// C23-NEXT:                                   value: 1,
// C23-NEXT:                                   radix: Decimal,
// C23-NEXT:                                   suffix: IntegerSuffix {
// C23-NEXT:                                       unsigned: false,
// C23-NEXT:                                       size: None,
// C23-NEXT:                                   },
// C23-NEXT:                                   spelling: "1",
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[4]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               attributes: [
// C23-NEXT:                   NoAlias,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "no_alias",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Prototype {
// C23-NEXT:                           parameters: [
// C23-NEXT:                               ParameterDeclarationKind {
// C23-NEXT:                                   specifiers: DeclarationSpecifiers {
// C23-NEXT:                                       ty: Integer(
// C23-NEXT:                                           Ranked {
// C23-NEXT:                                               rank: Int,
// C23-NEXT:                                               signed: true,
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   },
// C23-NEXT:                                   declarator: Pointer {
// C23-NEXT:                                       qualifiers: Qualifiers,
// C23-NEXT:                                       inner: Abstract,
// C23-NEXT:                                   },
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[5]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   RestrictReturn,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Pointer {
// C23-NEXT:                           qualifiers: Qualifiers,
// C23-NEXT:                           inner: Name(
// C23-NEXT:                               "restricted",
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                       parameters: Prototype {
// C23-NEXT:                           parameters: [
// C23-NEXT:                               ParameterDeclarationKind {
// C23-NEXT:                                   specifiers: DeclarationSpecifiers {
// C23-NEXT:                                       ty: Integer(
// C23-NEXT:                                           Ranked {
// C23-NEXT:                                               rank: Long,
// C23-NEXT:                                               signed: false,
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   },
// C23-NEXT:                                   declarator: Abstract,
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[6]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   NoThrow,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "no_throw",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[7]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               attributes: [
// C23-NEXT:                   Section(
// C23-NEXT:                       ".data.custom",
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "placed",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[8]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CodeSeg(
// C23-NEXT:                       ".text.custom",
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "code",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[9]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               attributes: [
// C23-NEXT:                   DllImport,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "imported",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[10]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   DllExport,
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "exported",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[11]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               attributes: [
// C23-NEXT:                   Aligned(
// C23-NEXT:                       IntegerLiteral(
// C23-NEXT:                           IntegerLiteral {
// C23-NEXT:                               value: 32,
// C23-NEXT:                               radix: Decimal,
// C23-NEXT:                               suffix: IntegerSuffix {
// C23-NEXT:                                   unsigned: false,
// C23-NEXT:                                   size: None,
// C23-NEXT:                               },
// C23-NEXT:                               spelling: "32",
// C23-NEXT:                           },
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "aligned",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[12]: Function(
// C23-NEXT:       FunctionDefinition {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:           },
// C23-NEXT:           declarator: Function {
// C23-NEXT:               inner: Name(
// C23-NEXT:                   "local",
// C23-NEXT:               ),
// C23-NEXT:               parameters: Void,
// C23-NEXT:           },
// C23-NEXT:           body: [
// C23-NEXT:               Decl(
// C23-NEXT:                   Declaration {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: Void,
// C23-NEXT:                           attributes: [
// C23-NEXT:                               NoInline,
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           InitDeclaratorKind {
// C23-NEXT:                               declarator: Function {
// C23-NEXT:                                   inner: Name(
// C23-NEXT:                                       "nested",
// C23-NEXT:                                   ),
// C23-NEXT:                                   parameters: Void,
// C23-NEXT:                               },
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
