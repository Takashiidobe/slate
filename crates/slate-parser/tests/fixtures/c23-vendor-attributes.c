[[gnu::cdecl]] void caller(void);
[[gnu::stdcall]] void callee(void);
[[gnu::fastcall]] void fast(void);
[[clang::vectorcall]] void vector(void);
[[gnu::thiscall]] void method(void *);
[[gnu::ms_abi]] void ms(void);
[[gnu::sysv_abi]] void sysv(void);
[[gnu::regparm(1 + 2)]] void registers(int, int);
[[gnu::pcs("aapcs")]] void arm(void);
[[gnu::pcs("aapcs-vfp")]] void arm_vfp(void);
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C23
// C23: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       Cdecl,
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "caller",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       Stdcall,
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "callee",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       Fastcall,
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "fast",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       Vectorcall,
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "vector",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       Thiscall,
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "method",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Prototype {
// C23-NEXT:                           parameters: [
// C23-NEXT:                               ParameterDeclarationKind {
// C23-NEXT:                                   specifiers: DeclarationSpecifiers {
// C23-NEXT:                                       ty: Void,
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
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       MsAbi,
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "ms",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       SysVAbi,
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "sysv",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       RegParm(
// C23-NEXT:                           Binary {
// C23-NEXT:                               op: Add,
// C23-NEXT:                               left: IntegerLiteral(
// C23-NEXT:                                   IntegerLiteral {
// C23-NEXT:                                       value: 1,
// C23-NEXT:                                       radix: Decimal,
// C23-NEXT:                                       suffix: IntegerSuffix {
// C23-NEXT:                                           unsigned: false,
// C23-NEXT:                                           size: None,
// C23-NEXT:                                       },
// C23-NEXT:                                       spelling: "1",
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                               right: IntegerLiteral(
// C23-NEXT:                                   IntegerLiteral {
// C23-NEXT:                                       value: 2,
// C23-NEXT:                                       radix: Decimal,
// C23-NEXT:                                       suffix: IntegerSuffix {
// C23-NEXT:                                           unsigned: false,
// C23-NEXT:                                           size: None,
// C23-NEXT:                                       },
// C23-NEXT:                                       spelling: "2",
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "registers",
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
// C23-NEXT:                                   declarator: Abstract,
// C23-NEXT:                               },
// C23-NEXT:                               ParameterDeclarationKind {
// C23-NEXT:                                   specifiers: DeclarationSpecifiers {
// C23-NEXT:                                       ty: Integer(
// C23-NEXT:                                           Ranked {
// C23-NEXT:                                               rank: Int,
// C23-NEXT:                                               signed: true,
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
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       Pcs(
// C23-NEXT:                           Aapcs,
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "arm",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:               attributes: [
// C23-NEXT:                   CallingConvention(
// C23-NEXT:                       Pcs(
// C23-NEXT:                           AapcsVfp,
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ],
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Function {
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "arm_vfp",
// C23-NEXT:                       ),
// C23-NEXT:                       parameters: Void,
// C23-NEXT:                   },
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
