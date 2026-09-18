#define ATTR(name) __attribute__((name))
ATTR(preserve_most) void most(void);
ATTR(preserve_all) void all(void);
ATTR(preserve_none) void none(void);
ATTR(address_space(1 + 2)) int *device_memory;
ATTR(overloadable) int overloaded(int);
ATTR(overloadable) int overloaded(double);
ATTR(optnone) int unoptimized(int);
ATTR(annotate("marker")) int annotated;
ATTR(availability(macos, introduced=12.0)) int platform_api(void);
ATTR(cpu_dispatch(generic, haswell)) int dispatch(void);
ATTR(cpu_specific(haswell)) int specialized(void);
typedef int int2 ATTR(ext_vector_type(2));
ATTR(weak_import) int weak_platform;
ATTR(noinline) void noinline_fn(void);
ATTR(always_inline) inline void inline_fn(void);
// SLATE-FILECHECK-DEFINES GNU
// SLATE-FILECHECK-STD GNU c23
// SLATE-FILECHECK-FLAVOR clang

// SLATE-FILECHECK-BEGIN GNU
// GNU: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Void,
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   CallingConvention(
// GNU-NEXT:                       PreserveMost,
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "most",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Void,
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   CallingConvention(
// GNU-NEXT:                       PreserveAll,
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "all",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Void,
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   CallingConvention(
// GNU-NEXT:                       PreserveNone,
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "none",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   AddressSpace(
// GNU-NEXT:                       Binary {
// GNU-NEXT:                           op: Add,
// GNU-NEXT:                           left: IntegerLiteral(
// GNU-NEXT:                               IntegerLiteral {
// GNU-NEXT:                                   value: 1,
// GNU-NEXT:                                   radix: Decimal,
// GNU-NEXT:                                   suffix: IntegerSuffix {
// GNU-NEXT:                                       unsigned: false,
// GNU-NEXT:                                       size: None,
// GNU-NEXT:                                   },
// GNU-NEXT:                                   spelling: "1",
// GNU-NEXT:                               },
// GNU-NEXT:                           ),
// GNU-NEXT:                           right: IntegerLiteral(
// GNU-NEXT:                               IntegerLiteral {
// GNU-NEXT:                                   value: 2,
// GNU-NEXT:                                   radix: Decimal,
// GNU-NEXT:                                   suffix: IntegerSuffix {
// GNU-NEXT:                                       unsigned: false,
// GNU-NEXT:                                       size: None,
// GNU-NEXT:                                   },
// GNU-NEXT:                                   spelling: "2",
// GNU-NEXT:                               },
// GNU-NEXT:                           ),
// GNU-NEXT:                       },
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Pointer {
// GNU-NEXT:                       qualifiers: Qualifiers,
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "device_memory",
// GNU-NEXT:                       ),
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   Overloadable,
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "overloaded",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Prototype {
// GNU-NEXT:                           parameters: [
// GNU-NEXT:                               ParameterDeclarationKind {
// GNU-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU-NEXT:                                       ty: Integer(
// GNU-NEXT:                                           Ranked {
// GNU-NEXT:                                               rank: Int,
// GNU-NEXT:                                               signed: true,
// GNU-NEXT:                                           },
// GNU-NEXT:                                       ),
// GNU-NEXT:                                   },
// GNU-NEXT:                                   declarator: Abstract,
// GNU-NEXT:                               },
// GNU-NEXT:                           ],
// GNU-NEXT:                       },
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   Overloadable,
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "overloaded",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Prototype {
// GNU-NEXT:                           parameters: [
// GNU-NEXT:                               ParameterDeclarationKind {
// GNU-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU-NEXT:                                       ty: Floating(
// GNU-NEXT:                                           Double,
// GNU-NEXT:                                       ),
// GNU-NEXT:                                   },
// GNU-NEXT:                                   declarator: Abstract,
// GNU-NEXT:                               },
// GNU-NEXT:                           ],
// GNU-NEXT:                       },
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   OptimizeNone,
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "unoptimized",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Prototype {
// GNU-NEXT:                           parameters: [
// GNU-NEXT:                               ParameterDeclarationKind {
// GNU-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU-NEXT:                                       ty: Integer(
// GNU-NEXT:                                           Ranked {
// GNU-NEXT:                                               rank: Int,
// GNU-NEXT:                                               signed: true,
// GNU-NEXT:                                           },
// GNU-NEXT:                                       ),
// GNU-NEXT:                                   },
// GNU-NEXT:                                   declarator: Abstract,
// GNU-NEXT:                               },
// GNU-NEXT:                           ],
// GNU-NEXT:                       },
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   Annotate(
// GNU-NEXT:                       "marker",
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Name(
// GNU-NEXT:                       "annotated",
// GNU-NEXT:                   ),
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   Availability(
// GNU-NEXT:                       [
// GNU-NEXT:                           "macos",
// GNU-NEXT:                           "introduced = 12.0",
// GNU-NEXT:                       ],
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "platform_api",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   CpuDispatch(
// GNU-NEXT:                       [
// GNU-NEXT:                           "generic",
// GNU-NEXT:                           "haswell",
// GNU-NEXT:                       ],
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "dispatch",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   CpuSpecific(
// GNU-NEXT:                       [
// GNU-NEXT:                           "haswell",
// GNU-NEXT:                       ],
// GNU-NEXT:                   ),
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "specialized",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Vector(
// GNU-NEXT:                   VectorType {
// GNU-NEXT:                       element: Integer(
// GNU-NEXT:                           Ranked {
// GNU-NEXT:                               rank: Int,
// GNU-NEXT:                               signed: true,
// GNU-NEXT:                           },
// GNU-NEXT:                       ),
// GNU-NEXT:                       size: Lanes(
// GNU-NEXT:                           IntegerLiteral(
// GNU-NEXT:                               IntegerLiteral {
// GNU-NEXT:                                   value: 2,
// GNU-NEXT:                                   radix: Decimal,
// GNU-NEXT:                                   suffix: IntegerSuffix {
// GNU-NEXT:                                       unsigned: false,
// GNU-NEXT:                                       size: None,
// GNU-NEXT:                                   },
// GNU-NEXT:                                   spelling: "2",
// GNU-NEXT:                               },
// GNU-NEXT:                           ),
// GNU-NEXT:                       ),
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               storage: Typedef,
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Name(
// GNU-NEXT:                       "int2",
// GNU-NEXT:                   ),
// GNU-NEXT:                   attributes: [
// GNU-NEXT:                       ExtVectorType(
// GNU-NEXT:                           IntegerLiteral(
// GNU-NEXT:                               IntegerLiteral {
// GNU-NEXT:                                   value: 2,
// GNU-NEXT:                                   radix: Decimal,
// GNU-NEXT:                                   suffix: IntegerSuffix {
// GNU-NEXT:                                       unsigned: false,
// GNU-NEXT:                                       size: None,
// GNU-NEXT:                                   },
// GNU-NEXT:                                   spelling: "2",
// GNU-NEXT:                               },
// GNU-NEXT:                           ),
// GNU-NEXT:                       ),
// GNU-NEXT:                   ],
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Integer(
// GNU-NEXT:                   Ranked {
// GNU-NEXT:                       rank: Int,
// GNU-NEXT:                       signed: true,
// GNU-NEXT:                   },
// GNU-NEXT:               ),
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   WeakImport,
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Name(
// GNU-NEXT:                       "weak_platform",
// GNU-NEXT:                   ),
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Void,
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   NoInline,
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "noinline_fn",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// GNU-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU-NEXT:       Declaration {
// GNU-NEXT:           specifiers: DeclarationSpecifiers {
// GNU-NEXT:               ty: Void,
// GNU-NEXT:               is_inline: true,
// GNU-NEXT:               attributes: [
// GNU-NEXT:                   AlwaysInline,
// GNU-NEXT:               ],
// GNU-NEXT:           },
// GNU-NEXT:           declarators: [
// GNU-NEXT:               InitDeclaratorKind {
// GNU-NEXT:                   declarator: Function {
// GNU-NEXT:                       inner: Name(
// GNU-NEXT:                           "inline_fn",
// GNU-NEXT:                       ),
// GNU-NEXT:                       parameters: Void,
// GNU-NEXT:                   },
// GNU-NEXT:               },
// GNU-NEXT:           ],
// GNU-NEXT:       },
// GNU-NEXT:   )
// SLATE-FILECHECK-END GNU
