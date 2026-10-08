typedef int T;
struct pair { int values[2]; };
int *global = &(static int){1};

void literals(int i) {
    (register int[2]){2, 3}[i];
    (register struct pair){.values = {4, 5}}.values[i];
    &(static T){6};
    (constexpr int){7};
    (register constexpr int){8};
    &(static constexpr int){9};
    &(static thread_local int){10};
    &(thread_local static int){11};
    &(static _Thread_local int){12};
    (static _Alignas(16) int){14};
    (int){15};
}

// SLATE-FILECHECK-AST
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-DEFINES GNU23
// SLATE-FILECHECK-STD GNU23 gnu23
// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES GNU2Y
// SLATE-FILECHECK-STD GNU2Y gnu2y
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-ERROR C17
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-ERROR GNU17
// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-PREFIX-ARGS IR --dump-ir
// SLATE-FILECHECK-IR-ERROR IR
// SLATE-FILECHECK-STD IR c23

// SLATE-FILECHECK-BEGIN C17
// C17: Error:   × storage-class specifiers in compound literals require C23
// C17: ╰─▶ storage-class specifiers in compound literals require C23
// C17: ╭─[tests/fixtures/msvc/windows/x86_64/c23_compound_literal_storage.c:3:15]
// C17: 2 │ struct pair { int values[2]; };
// C17: 3 │ int *global = &(static int){1};
// C17: ·               ─
// C17: 4 │
// C17: ╰────
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: Error:   × storage-class specifiers in compound literals require C23
// GNU17: ╰─▶ storage-class specifiers in compound literals require C23
// GNU17: ╭─[tests/fixtures/msvc/windows/x86_64/c23_compound_literal_storage.c:3:15]
// GNU17: 2 │ struct pair { int values[2]; };
// GNU17: 3 │ int *global = &(static int){1};
// GNU17: ·               ─
// GNU17: 4 │
// GNU17: ╰────
// SLATE-FILECHECK-END GNU17
// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × not implemented: compound literal storage-class specifiers
// IR: ╭─[tests/fixtures/msvc/windows/x86_64/c23_compound_literal_storage.c:3:1]
// IR: 2 │ struct pair { int values[2]; };
// IR: 3 │ int *global = &(static int){1};
// IR: · ───────────────────────────────
// IR: 4 │
// IR: ╰────
// IR: Error:
// IR: × not implemented: compound literal storage-class specifiers
// IR: ╭─[tests/fixtures/msvc/windows/x86_64/c23_compound_literal_storage.c:6:5]
// IR: 5 │ void literals(int i) {
// IR: 6 │     (register int[2]){2, 3}[i];
// IR: ·     ───────────────────────
// IR: 7 │     (register struct pair){.values = {4, 5{{[}][}]}}.values[i];
// IR: ╰────
// SLATE-FILECHECK-END IR
// SLATE-FILECHECK-BEGIN C23
// C23: tag[{{[0-9]+}}]: TagDefinition {
// C23-NEXT:       id: TagId(
// C23-NEXT:           [[#TAG0:]],
// C23-NEXT:       ),
// C23-NEXT:       kind: Struct,
// C23-NEXT:       name: Some(
// C23-NEXT:           "pair",
// C23-NEXT:       ),
// C23-NEXT:       body: Record(
// C23-NEXT:           [
// C23-NEXT:               Field(
// C23-NEXT:                   FieldDecl {
// C23-NEXT:                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                           ty: Integer(
// C23-NEXT:                               Ranked {
// C23-NEXT:                                   rank: Int,
// C23-NEXT:                                   signed: true,
// C23-NEXT:                               },
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                       declarators: [
// C23-NEXT:                           FieldDeclaratorKind {
// C23-NEXT:                               declarator: Array {
// C23-NEXT:                                   inner: Name(
// C23-NEXT:                                       "values",
// C23-NEXT:                                   ),
// C23-NEXT:                                   size: Expression(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 2,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "2",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       ),
// C23-NEXT:   }
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Integer(
// C23-NEXT:                   Ranked {
// C23-NEXT:                       rank: Int,
// C23-NEXT:                       signed: true,
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               storage: Typedef,
// C23-NEXT:           },
// C23-NEXT:           declarators: [
// C23-NEXT:               InitDeclaratorKind {
// C23-NEXT:                   declarator: Name(
// C23-NEXT:                       "T",
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Declaration(
// C23-NEXT:       Declaration {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Tag(
// C23-NEXT:                   Definition(
// C23-NEXT:                       TagId(
// C23-NEXT:                           [[#TAG0]],
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               ),
// C23-NEXT:           },
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
// C23-NEXT:                   declarator: Pointer {
// C23-NEXT:                       qualifiers: Qualifiers,
// C23-NEXT:                       inner: Name(
// C23-NEXT:                           "global",
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:                   initializer: Some(
// C23-NEXT:                       Expr(
// C23-NEXT:                           Unary {
// C23-NEXT:                               op: AddrOf,
// C23-NEXT:                               operand: CompoundLiteral {
// C23-NEXT:                                   ty: TypeName {
// C23-NEXT:                                       specifiers: DeclarationSpecifiers {
// C23-NEXT:                                           ty: Integer(
// C23-NEXT:                                               Ranked {
// C23-NEXT:                                                   rank: Int,
// C23-NEXT:                                                   signed: true,
// C23-NEXT:                                               },
// C23-NEXT:                                           ),
// C23-NEXT:                                           storage: Static,
// C23-NEXT:                                       },
// C23-NEXT:                                       declarator: Abstract,
// C23-NEXT:                                   },
// C23-NEXT:                                   initializer: [
// C23-NEXT:                                       InitializerItem {
// C23-NEXT:                                           designators: [],
// C23-NEXT:                                           value: Expr(
// C23-NEXT:                                               IntegerLiteral(
// C23-NEXT:                                                   IntegerLiteral {
// C23-NEXT:                                                       value: 1,
// C23-NEXT:                                                       radix: Decimal,
// C23-NEXT:                                                       suffix: IntegerSuffix {
// C23-NEXT:                                                           unsigned: false,
// C23-NEXT:                                                           size: None,
// C23-NEXT:                                                       },
// C23-NEXT:                                                       spelling: "1",
// C23-NEXT:                                                   },
// C23-NEXT:                                               ),
// C23-NEXT:                                           ),
// C23-NEXT:                                       },
// C23-NEXT:                                   ],
// C23-NEXT:                               },
// C23-NEXT:                           },
// C23-NEXT:                       ),
// C23-NEXT:                   ),
// C23-NEXT:               },
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// C23-NEXT: decl[{{[0-9]+}}]: Function(
// C23-NEXT:       FunctionDefinition {
// C23-NEXT:           specifiers: DeclarationSpecifiers {
// C23-NEXT:               ty: Void,
// C23-NEXT:           },
// C23-NEXT:           declarator: Function {
// C23-NEXT:               inner: Name(
// C23-NEXT:                   "literals",
// C23-NEXT:               ),
// C23-NEXT:               parameters: Prototype {
// C23-NEXT:                   parameters: [
// C23-NEXT:                       ParameterDeclarationKind {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Name(
// C23-NEXT:                               "i",
// C23-NEXT:                           ),
// C23-NEXT:                       },
// C23-NEXT:                   ],
// C23-NEXT:               },
// C23-NEXT:           },
// C23-NEXT:           body: [
// C23-NEXT:               Expr(
// C23-NEXT:                   Index {
// C23-NEXT:                       base: CompoundLiteral {
// C23-NEXT:                           ty: TypeName {
// C23-NEXT:                               specifiers: DeclarationSpecifiers {
// C23-NEXT:                                   ty: Integer(
// C23-NEXT:                                       Ranked {
// C23-NEXT:                                           rank: Int,
// C23-NEXT:                                           signed: true,
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                                   storage: Register,
// C23-NEXT:                               },
// C23-NEXT:                               declarator: Array {
// C23-NEXT:                                   inner: Abstract,
// C23-NEXT:                                   size: Expression(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 2,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "2",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           },
// C23-NEXT:                           initializer: [
// C23-NEXT:                               InitializerItem {
// C23-NEXT:                                   designators: [],
// C23-NEXT:                                   value: Expr(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 2,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "2",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                               InitializerItem {
// C23-NEXT:                                   designators: [],
// C23-NEXT:                                   value: Expr(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 3,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "3",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                       index: Identifier(
// C23-NEXT:                           "i",
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   Index {
// C23-NEXT:                       base: Member {
// C23-NEXT:                           base: CompoundLiteral {
// C23-NEXT:                               ty: TypeName {
// C23-NEXT:                                   specifiers: DeclarationSpecifiers {
// C23-NEXT:                                       ty: Tag(
// C23-NEXT:                                           Reference {
// C23-NEXT:                                               kind: Struct,
// C23-NEXT:                                               name: "pair",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                       storage: Register,
// C23-NEXT:                                   },
// C23-NEXT:                                   declarator: Abstract,
// C23-NEXT:                               },
// C23-NEXT:                               initializer: [
// C23-NEXT:                                   InitializerItem {
// C23-NEXT:                                       designators: [
// C23-NEXT:                                           Field(
// C23-NEXT:                                               "values",
// C23-NEXT:                                           ),
// C23-NEXT:                                       ],
// C23-NEXT:                                       value: List(
// C23-NEXT:                                           [
// C23-NEXT:                                               InitializerItem {
// C23-NEXT:                                                   designators: [],
// C23-NEXT:                                                   value: Expr(
// C23-NEXT:                                                       IntegerLiteral(
// C23-NEXT:                                                           IntegerLiteral {
// C23-NEXT:                                                               value: 4,
// C23-NEXT:                                                               radix: Decimal,
// C23-NEXT:                                                               suffix: IntegerSuffix {
// C23-NEXT:                                                                   unsigned: false,
// C23-NEXT:                                                                   size: None,
// C23-NEXT:                                                               },
// C23-NEXT:                                                               spelling: "4",
// C23-NEXT:                                                           },
// C23-NEXT:                                                       ),
// C23-NEXT:                                                   ),
// C23-NEXT:                                               },
// C23-NEXT:                                               InitializerItem {
// C23-NEXT:                                                   designators: [],
// C23-NEXT:                                                   value: Expr(
// C23-NEXT:                                                       IntegerLiteral(
// C23-NEXT:                                                           IntegerLiteral {
// C23-NEXT:                                                               value: 5,
// C23-NEXT:                                                               radix: Decimal,
// C23-NEXT:                                                               suffix: IntegerSuffix {
// C23-NEXT:                                                                   unsigned: false,
// C23-NEXT:                                                                   size: None,
// C23-NEXT:                                                               },
// C23-NEXT:                                                               spelling: "5",
// C23-NEXT:                                                           },
// C23-NEXT:                                                       ),
// C23-NEXT:                                                   ),
// C23-NEXT:                                               },
// C23-NEXT:                                           ],
// C23-NEXT:                                       ),
// C23-NEXT:                                   },
// C23-NEXT:                               ],
// C23-NEXT:                           },
// C23-NEXT:                           field: "values",
// C23-NEXT:                       },
// C23-NEXT:                       index: Identifier(
// C23-NEXT:                           "i",
// C23-NEXT:                       ),
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   Unary {
// C23-NEXT:                       op: AddrOf,
// C23-NEXT:                       operand: CompoundLiteral {
// C23-NEXT:                           ty: TypeName {
// C23-NEXT:                               specifiers: DeclarationSpecifiers {
// C23-NEXT:                                   ty: Named(
// C23-NEXT:                                       "T",
// C23-NEXT:                                   ),
// C23-NEXT:                                   storage: Static,
// C23-NEXT:                               },
// C23-NEXT:                               declarator: Abstract,
// C23-NEXT:                           },
// C23-NEXT:                           initializer: [
// C23-NEXT:                               InitializerItem {
// C23-NEXT:                                   designators: [],
// C23-NEXT:                                   value: Expr(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 6,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "6",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   CompoundLiteral {
// C23-NEXT:                       ty: TypeName {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                               is_constexpr: true,
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Abstract,
// C23-NEXT:                       },
// C23-NEXT:                       initializer: [
// C23-NEXT:                           InitializerItem {
// C23-NEXT:                               designators: [],
// C23-NEXT:                               value: Expr(
// C23-NEXT:                                   IntegerLiteral(
// C23-NEXT:                                       IntegerLiteral {
// C23-NEXT:                                           value: 7,
// C23-NEXT:                                           radix: Decimal,
// C23-NEXT:                                           suffix: IntegerSuffix {
// C23-NEXT:                                               unsigned: false,
// C23-NEXT:                                               size: None,
// C23-NEXT:                                           },
// C23-NEXT:                                           spelling: "7",
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   CompoundLiteral {
// C23-NEXT:                       ty: TypeName {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                               storage: Register,
// C23-NEXT:                               is_constexpr: true,
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Abstract,
// C23-NEXT:                       },
// C23-NEXT:                       initializer: [
// C23-NEXT:                           InitializerItem {
// C23-NEXT:                               designators: [],
// C23-NEXT:                               value: Expr(
// C23-NEXT:                                   IntegerLiteral(
// C23-NEXT:                                       IntegerLiteral {
// C23-NEXT:                                           value: 8,
// C23-NEXT:                                           radix: Decimal,
// C23-NEXT:                                           suffix: IntegerSuffix {
// C23-NEXT:                                               unsigned: false,
// C23-NEXT:                                               size: None,
// C23-NEXT:                                           },
// C23-NEXT:                                           spelling: "8",
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   Unary {
// C23-NEXT:                       op: AddrOf,
// C23-NEXT:                       operand: CompoundLiteral {
// C23-NEXT:                           ty: TypeName {
// C23-NEXT:                               specifiers: DeclarationSpecifiers {
// C23-NEXT:                                   ty: Integer(
// C23-NEXT:                                       Ranked {
// C23-NEXT:                                           rank: Int,
// C23-NEXT:                                           signed: true,
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                                   storage: Static,
// C23-NEXT:                                   is_constexpr: true,
// C23-NEXT:                               },
// C23-NEXT:                               declarator: Abstract,
// C23-NEXT:                           },
// C23-NEXT:                           initializer: [
// C23-NEXT:                               InitializerItem {
// C23-NEXT:                                   designators: [],
// C23-NEXT:                                   value: Expr(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 9,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "9",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   Unary {
// C23-NEXT:                       op: AddrOf,
// C23-NEXT:                       operand: CompoundLiteral {
// C23-NEXT:                           ty: TypeName {
// C23-NEXT:                               specifiers: DeclarationSpecifiers {
// C23-NEXT:                                   ty: Integer(
// C23-NEXT:                                       Ranked {
// C23-NEXT:                                           rank: Int,
// C23-NEXT:                                           signed: true,
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                                   storage: Static,
// C23-NEXT:                                   is_thread_local: true,
// C23-NEXT:                               },
// C23-NEXT:                               declarator: Abstract,
// C23-NEXT:                           },
// C23-NEXT:                           initializer: [
// C23-NEXT:                               InitializerItem {
// C23-NEXT:                                   designators: [],
// C23-NEXT:                                   value: Expr(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 10,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "10",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   Unary {
// C23-NEXT:                       op: AddrOf,
// C23-NEXT:                       operand: CompoundLiteral {
// C23-NEXT:                           ty: TypeName {
// C23-NEXT:                               specifiers: DeclarationSpecifiers {
// C23-NEXT:                                   ty: Integer(
// C23-NEXT:                                       Ranked {
// C23-NEXT:                                           rank: Int,
// C23-NEXT:                                           signed: true,
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                                   storage: Static,
// C23-NEXT:                                   is_thread_local: true,
// C23-NEXT:                               },
// C23-NEXT:                               declarator: Abstract,
// C23-NEXT:                           },
// C23-NEXT:                           initializer: [
// C23-NEXT:                               InitializerItem {
// C23-NEXT:                                   designators: [],
// C23-NEXT:                                   value: Expr(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 11,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "11",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   Unary {
// C23-NEXT:                       op: AddrOf,
// C23-NEXT:                       operand: CompoundLiteral {
// C23-NEXT:                           ty: TypeName {
// C23-NEXT:                               specifiers: DeclarationSpecifiers {
// C23-NEXT:                                   ty: Integer(
// C23-NEXT:                                       Ranked {
// C23-NEXT:                                           rank: Int,
// C23-NEXT:                                           signed: true,
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                                   storage: Static,
// C23-NEXT:                                   is_thread_local: true,
// C23-NEXT:                               },
// C23-NEXT:                               declarator: Abstract,
// C23-NEXT:                           },
// C23-NEXT:                           initializer: [
// C23-NEXT:                               InitializerItem {
// C23-NEXT:                                   designators: [],
// C23-NEXT:                                   value: Expr(
// C23-NEXT:                                       IntegerLiteral(
// C23-NEXT:                                           IntegerLiteral {
// C23-NEXT:                                               value: 12,
// C23-NEXT:                                               radix: Decimal,
// C23-NEXT:                                               suffix: IntegerSuffix {
// C23-NEXT:                                                   unsigned: false,
// C23-NEXT:                                                   size: None,
// C23-NEXT:                                               },
// C23-NEXT:                                               spelling: "12",
// C23-NEXT:                                           },
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               },
// C23-NEXT:                           ],
// C23-NEXT:                       },
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   CompoundLiteral {
// C23-NEXT:                       ty: TypeName {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                               storage: Static,
// C23-NEXT:                               attributes: [
// C23-NEXT:                                   AlignAs(
// C23-NEXT:                                       Expr(
// C23-NEXT:                                           IntegerLiteral(
// C23-NEXT:                                               IntegerLiteral {
// C23-NEXT:                                                   value: 16,
// C23-NEXT:                                                   radix: Decimal,
// C23-NEXT:                                                   suffix: IntegerSuffix {
// C23-NEXT:                                                       unsigned: false,
// C23-NEXT:                                                       size: None,
// C23-NEXT:                                                   },
// C23-NEXT:                                                   spelling: "16",
// C23-NEXT:                                               },
// C23-NEXT:                                           ),
// C23-NEXT:                                       ),
// C23-NEXT:                                   ),
// C23-NEXT:                               ],
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Abstract,
// C23-NEXT:                       },
// C23-NEXT:                       initializer: [
// C23-NEXT:                           InitializerItem {
// C23-NEXT:                               designators: [],
// C23-NEXT:                               value: Expr(
// C23-NEXT:                                   IntegerLiteral(
// C23-NEXT:                                       IntegerLiteral {
// C23-NEXT:                                           value: 14,
// C23-NEXT:                                           radix: Decimal,
// C23-NEXT:                                           suffix: IntegerSuffix {
// C23-NEXT:                                               unsigned: false,
// C23-NEXT:                                               size: None,
// C23-NEXT:                                           },
// C23-NEXT:                                           spelling: "14",
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:               Expr(
// C23-NEXT:                   CompoundLiteral {
// C23-NEXT:                       ty: TypeName {
// C23-NEXT:                           specifiers: DeclarationSpecifiers {
// C23-NEXT:                               ty: Integer(
// C23-NEXT:                                   Ranked {
// C23-NEXT:                                       rank: Int,
// C23-NEXT:                                       signed: true,
// C23-NEXT:                                   },
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                           declarator: Abstract,
// C23-NEXT:                       },
// C23-NEXT:                       initializer: [
// C23-NEXT:                           InitializerItem {
// C23-NEXT:                               designators: [],
// C23-NEXT:                               value: Expr(
// C23-NEXT:                                   IntegerLiteral(
// C23-NEXT:                                       IntegerLiteral {
// C23-NEXT:                                           value: 15,
// C23-NEXT:                                           radix: Decimal,
// C23-NEXT:                                           suffix: IntegerSuffix {
// C23-NEXT:                                               unsigned: false,
// C23-NEXT:                                               size: None,
// C23-NEXT:                                           },
// C23-NEXT:                                           spelling: "15",
// C23-NEXT:                                       },
// C23-NEXT:                                   ),
// C23-NEXT:                               ),
// C23-NEXT:                           },
// C23-NEXT:                       ],
// C23-NEXT:                   },
// C23-NEXT:               ),
// C23-NEXT:           ],
// C23-NEXT:       },
// C23-NEXT:   )
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU23
// GNU23: tag[{{[0-9]+}}]: TagDefinition {
// GNU23-NEXT:       id: TagId(
// GNU23-NEXT:           [[#TAG0:]],
// GNU23-NEXT:       ),
// GNU23-NEXT:       kind: Struct,
// GNU23-NEXT:       name: Some(
// GNU23-NEXT:           "pair",
// GNU23-NEXT:       ),
// GNU23-NEXT:       body: Record(
// GNU23-NEXT:           [
// GNU23-NEXT:               Field(
// GNU23-NEXT:                   FieldDecl {
// GNU23-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                           ty: Integer(
// GNU23-NEXT:                               Ranked {
// GNU23-NEXT:                                   rank: Int,
// GNU23-NEXT:                                   signed: true,
// GNU23-NEXT:                               },
// GNU23-NEXT:                           ),
// GNU23-NEXT:                       },
// GNU23-NEXT:                       declarators: [
// GNU23-NEXT:                           FieldDeclaratorKind {
// GNU23-NEXT:                               declarator: Array {
// GNU23-NEXT:                                   inner: Name(
// GNU23-NEXT:                                       "values",
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                                   size: Expression(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 2,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "2",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           },
// GNU23-NEXT:                       ],
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           ],
// GNU23-NEXT:       ),
// GNU23-NEXT:   }
// GNU23-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU23-NEXT:       Declaration {
// GNU23-NEXT:           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:               ty: Integer(
// GNU23-NEXT:                   Ranked {
// GNU23-NEXT:                       rank: Int,
// GNU23-NEXT:                       signed: true,
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               storage: Typedef,
// GNU23-NEXT:           },
// GNU23-NEXT:           declarators: [
// GNU23-NEXT:               InitDeclaratorKind {
// GNU23-NEXT:                   declarator: Name(
// GNU23-NEXT:                       "T",
// GNU23-NEXT:                   ),
// GNU23-NEXT:               },
// GNU23-NEXT:           ],
// GNU23-NEXT:       },
// GNU23-NEXT:   )
// GNU23-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU23-NEXT:       Declaration {
// GNU23-NEXT:           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:               ty: Tag(
// GNU23-NEXT:                   Definition(
// GNU23-NEXT:                       TagId(
// GNU23-NEXT:                           [[#TAG0]],
// GNU23-NEXT:                       ),
// GNU23-NEXT:                   ),
// GNU23-NEXT:               ),
// GNU23-NEXT:           },
// GNU23-NEXT:       },
// GNU23-NEXT:   )
// GNU23-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU23-NEXT:       Declaration {
// GNU23-NEXT:           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:               ty: Integer(
// GNU23-NEXT:                   Ranked {
// GNU23-NEXT:                       rank: Int,
// GNU23-NEXT:                       signed: true,
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           },
// GNU23-NEXT:           declarators: [
// GNU23-NEXT:               InitDeclaratorKind {
// GNU23-NEXT:                   declarator: Pointer {
// GNU23-NEXT:                       qualifiers: Qualifiers,
// GNU23-NEXT:                       inner: Name(
// GNU23-NEXT:                           "global",
// GNU23-NEXT:                       ),
// GNU23-NEXT:                   },
// GNU23-NEXT:                   initializer: Some(
// GNU23-NEXT:                       Expr(
// GNU23-NEXT:                           Unary {
// GNU23-NEXT:                               op: AddrOf,
// GNU23-NEXT:                               operand: CompoundLiteral {
// GNU23-NEXT:                                   ty: TypeName {
// GNU23-NEXT:                                       specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                           ty: Integer(
// GNU23-NEXT:                                               Ranked {
// GNU23-NEXT:                                                   rank: Int,
// GNU23-NEXT:                                                   signed: true,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                           ),
// GNU23-NEXT:                                           storage: Static,
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                       declarator: Abstract,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                                   initializer: [
// GNU23-NEXT:                                       InitializerItem {
// GNU23-NEXT:                                           designators: [],
// GNU23-NEXT:                                           value: Expr(
// GNU23-NEXT:                                               IntegerLiteral(
// GNU23-NEXT:                                                   IntegerLiteral {
// GNU23-NEXT:                                                       value: 1,
// GNU23-NEXT:                                                       radix: Decimal,
// GNU23-NEXT:                                                       suffix: IntegerSuffix {
// GNU23-NEXT:                                                           unsigned: false,
// GNU23-NEXT:                                                           size: None,
// GNU23-NEXT:                                                       },
// GNU23-NEXT:                                                       spelling: "1",
// GNU23-NEXT:                                                   },
// GNU23-NEXT:                                               ),
// GNU23-NEXT:                                           ),
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ],
// GNU23-NEXT:                               },
// GNU23-NEXT:                           },
// GNU23-NEXT:                       ),
// GNU23-NEXT:                   ),
// GNU23-NEXT:               },
// GNU23-NEXT:           ],
// GNU23-NEXT:       },
// GNU23-NEXT:   )
// GNU23-NEXT: decl[{{[0-9]+}}]: Function(
// GNU23-NEXT:       FunctionDefinition {
// GNU23-NEXT:           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:               ty: Void,
// GNU23-NEXT:           },
// GNU23-NEXT:           declarator: Function {
// GNU23-NEXT:               inner: Name(
// GNU23-NEXT:                   "literals",
// GNU23-NEXT:               ),
// GNU23-NEXT:               parameters: Prototype {
// GNU23-NEXT:                   parameters: [
// GNU23-NEXT:                       ParameterDeclarationKind {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Name(
// GNU23-NEXT:                               "i",
// GNU23-NEXT:                           ),
// GNU23-NEXT:                       },
// GNU23-NEXT:                   ],
// GNU23-NEXT:               },
// GNU23-NEXT:           },
// GNU23-NEXT:           body: [
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   Index {
// GNU23-NEXT:                       base: CompoundLiteral {
// GNU23-NEXT:                           ty: TypeName {
// GNU23-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                   ty: Integer(
// GNU23-NEXT:                                       Ranked {
// GNU23-NEXT:                                           rank: Int,
// GNU23-NEXT:                                           signed: true,
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                                   storage: Register,
// GNU23-NEXT:                               },
// GNU23-NEXT:                               declarator: Array {
// GNU23-NEXT:                                   inner: Abstract,
// GNU23-NEXT:                                   size: Expression(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 2,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "2",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           },
// GNU23-NEXT:                           initializer: [
// GNU23-NEXT:                               InitializerItem {
// GNU23-NEXT:                                   designators: [],
// GNU23-NEXT:                                   value: Expr(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 2,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "2",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                               InitializerItem {
// GNU23-NEXT:                                   designators: [],
// GNU23-NEXT:                                   value: Expr(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 3,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "3",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                       index: Identifier(
// GNU23-NEXT:                           "i",
// GNU23-NEXT:                       ),
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   Index {
// GNU23-NEXT:                       base: Member {
// GNU23-NEXT:                           base: CompoundLiteral {
// GNU23-NEXT:                               ty: TypeName {
// GNU23-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                       ty: Tag(
// GNU23-NEXT:                                           Reference {
// GNU23-NEXT:                                               kind: Struct,
// GNU23-NEXT:                                               name: "pair",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                       storage: Register,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                                   declarator: Abstract,
// GNU23-NEXT:                               },
// GNU23-NEXT:                               initializer: [
// GNU23-NEXT:                                   InitializerItem {
// GNU23-NEXT:                                       designators: [
// GNU23-NEXT:                                           Field(
// GNU23-NEXT:                                               "values",
// GNU23-NEXT:                                           ),
// GNU23-NEXT:                                       ],
// GNU23-NEXT:                                       value: List(
// GNU23-NEXT:                                           [
// GNU23-NEXT:                                               InitializerItem {
// GNU23-NEXT:                                                   designators: [],
// GNU23-NEXT:                                                   value: Expr(
// GNU23-NEXT:                                                       IntegerLiteral(
// GNU23-NEXT:                                                           IntegerLiteral {
// GNU23-NEXT:                                                               value: 4,
// GNU23-NEXT:                                                               radix: Decimal,
// GNU23-NEXT:                                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                                   unsigned: false,
// GNU23-NEXT:                                                                   size: None,
// GNU23-NEXT:                                                               },
// GNU23-NEXT:                                                               spelling: "4",
// GNU23-NEXT:                                                           },
// GNU23-NEXT:                                                       ),
// GNU23-NEXT:                                                   ),
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               InitializerItem {
// GNU23-NEXT:                                                   designators: [],
// GNU23-NEXT:                                                   value: Expr(
// GNU23-NEXT:                                                       IntegerLiteral(
// GNU23-NEXT:                                                           IntegerLiteral {
// GNU23-NEXT:                                                               value: 5,
// GNU23-NEXT:                                                               radix: Decimal,
// GNU23-NEXT:                                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                                   unsigned: false,
// GNU23-NEXT:                                                                   size: None,
// GNU23-NEXT:                                                               },
// GNU23-NEXT:                                                               spelling: "5",
// GNU23-NEXT:                                                           },
// GNU23-NEXT:                                                       ),
// GNU23-NEXT:                                                   ),
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                           ],
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ],
// GNU23-NEXT:                           },
// GNU23-NEXT:                           field: "values",
// GNU23-NEXT:                       },
// GNU23-NEXT:                       index: Identifier(
// GNU23-NEXT:                           "i",
// GNU23-NEXT:                       ),
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   Unary {
// GNU23-NEXT:                       op: AddrOf,
// GNU23-NEXT:                       operand: CompoundLiteral {
// GNU23-NEXT:                           ty: TypeName {
// GNU23-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                   ty: Named(
// GNU23-NEXT:                                       "T",
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                                   storage: Static,
// GNU23-NEXT:                               },
// GNU23-NEXT:                               declarator: Abstract,
// GNU23-NEXT:                           },
// GNU23-NEXT:                           initializer: [
// GNU23-NEXT:                               InitializerItem {
// GNU23-NEXT:                                   designators: [],
// GNU23-NEXT:                                   value: Expr(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 6,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "6",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   CompoundLiteral {
// GNU23-NEXT:                       ty: TypeName {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                               is_constexpr: true,
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Abstract,
// GNU23-NEXT:                       },
// GNU23-NEXT:                       initializer: [
// GNU23-NEXT:                           InitializerItem {
// GNU23-NEXT:                               designators: [],
// GNU23-NEXT:                               value: Expr(
// GNU23-NEXT:                                   IntegerLiteral(
// GNU23-NEXT:                                       IntegerLiteral {
// GNU23-NEXT:                                           value: 7,
// GNU23-NEXT:                                           radix: Decimal,
// GNU23-NEXT:                                           suffix: IntegerSuffix {
// GNU23-NEXT:                                               unsigned: false,
// GNU23-NEXT:                                               size: None,
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                           spelling: "7",
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                       ],
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   CompoundLiteral {
// GNU23-NEXT:                       ty: TypeName {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                               storage: Register,
// GNU23-NEXT:                               is_constexpr: true,
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Abstract,
// GNU23-NEXT:                       },
// GNU23-NEXT:                       initializer: [
// GNU23-NEXT:                           InitializerItem {
// GNU23-NEXT:                               designators: [],
// GNU23-NEXT:                               value: Expr(
// GNU23-NEXT:                                   IntegerLiteral(
// GNU23-NEXT:                                       IntegerLiteral {
// GNU23-NEXT:                                           value: 8,
// GNU23-NEXT:                                           radix: Decimal,
// GNU23-NEXT:                                           suffix: IntegerSuffix {
// GNU23-NEXT:                                               unsigned: false,
// GNU23-NEXT:                                               size: None,
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                           spelling: "8",
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                       ],
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   Unary {
// GNU23-NEXT:                       op: AddrOf,
// GNU23-NEXT:                       operand: CompoundLiteral {
// GNU23-NEXT:                           ty: TypeName {
// GNU23-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                   ty: Integer(
// GNU23-NEXT:                                       Ranked {
// GNU23-NEXT:                                           rank: Int,
// GNU23-NEXT:                                           signed: true,
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                                   storage: Static,
// GNU23-NEXT:                                   is_constexpr: true,
// GNU23-NEXT:                               },
// GNU23-NEXT:                               declarator: Abstract,
// GNU23-NEXT:                           },
// GNU23-NEXT:                           initializer: [
// GNU23-NEXT:                               InitializerItem {
// GNU23-NEXT:                                   designators: [],
// GNU23-NEXT:                                   value: Expr(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 9,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "9",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   Unary {
// GNU23-NEXT:                       op: AddrOf,
// GNU23-NEXT:                       operand: CompoundLiteral {
// GNU23-NEXT:                           ty: TypeName {
// GNU23-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                   ty: Integer(
// GNU23-NEXT:                                       Ranked {
// GNU23-NEXT:                                           rank: Int,
// GNU23-NEXT:                                           signed: true,
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                                   storage: Static,
// GNU23-NEXT:                                   is_thread_local: true,
// GNU23-NEXT:                               },
// GNU23-NEXT:                               declarator: Abstract,
// GNU23-NEXT:                           },
// GNU23-NEXT:                           initializer: [
// GNU23-NEXT:                               InitializerItem {
// GNU23-NEXT:                                   designators: [],
// GNU23-NEXT:                                   value: Expr(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 10,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "10",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   Unary {
// GNU23-NEXT:                       op: AddrOf,
// GNU23-NEXT:                       operand: CompoundLiteral {
// GNU23-NEXT:                           ty: TypeName {
// GNU23-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                   ty: Integer(
// GNU23-NEXT:                                       Ranked {
// GNU23-NEXT:                                           rank: Int,
// GNU23-NEXT:                                           signed: true,
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                                   storage: Static,
// GNU23-NEXT:                                   is_thread_local: true,
// GNU23-NEXT:                               },
// GNU23-NEXT:                               declarator: Abstract,
// GNU23-NEXT:                           },
// GNU23-NEXT:                           initializer: [
// GNU23-NEXT:                               InitializerItem {
// GNU23-NEXT:                                   designators: [],
// GNU23-NEXT:                                   value: Expr(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 11,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "11",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   Unary {
// GNU23-NEXT:                       op: AddrOf,
// GNU23-NEXT:                       operand: CompoundLiteral {
// GNU23-NEXT:                           ty: TypeName {
// GNU23-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                                   ty: Integer(
// GNU23-NEXT:                                       Ranked {
// GNU23-NEXT:                                           rank: Int,
// GNU23-NEXT:                                           signed: true,
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                                   storage: Static,
// GNU23-NEXT:                                   is_thread_local: true,
// GNU23-NEXT:                               },
// GNU23-NEXT:                               declarator: Abstract,
// GNU23-NEXT:                           },
// GNU23-NEXT:                           initializer: [
// GNU23-NEXT:                               InitializerItem {
// GNU23-NEXT:                                   designators: [],
// GNU23-NEXT:                                   value: Expr(
// GNU23-NEXT:                                       IntegerLiteral(
// GNU23-NEXT:                                           IntegerLiteral {
// GNU23-NEXT:                                               value: 12,
// GNU23-NEXT:                                               radix: Decimal,
// GNU23-NEXT:                                               suffix: IntegerSuffix {
// GNU23-NEXT:                                                   unsigned: false,
// GNU23-NEXT:                                                   size: None,
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                               spelling: "12",
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               },
// GNU23-NEXT:                           ],
// GNU23-NEXT:                       },
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   CompoundLiteral {
// GNU23-NEXT:                       ty: TypeName {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                               storage: Static,
// GNU23-NEXT:                               attributes: [
// GNU23-NEXT:                                   AlignAs(
// GNU23-NEXT:                                       Expr(
// GNU23-NEXT:                                           IntegerLiteral(
// GNU23-NEXT:                                               IntegerLiteral {
// GNU23-NEXT:                                                   value: 16,
// GNU23-NEXT:                                                   radix: Decimal,
// GNU23-NEXT:                                                   suffix: IntegerSuffix {
// GNU23-NEXT:                                                       unsigned: false,
// GNU23-NEXT:                                                       size: None,
// GNU23-NEXT:                                                   },
// GNU23-NEXT:                                                   spelling: "16",
// GNU23-NEXT:                                               },
// GNU23-NEXT:                                           ),
// GNU23-NEXT:                                       ),
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               ],
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Abstract,
// GNU23-NEXT:                       },
// GNU23-NEXT:                       initializer: [
// GNU23-NEXT:                           InitializerItem {
// GNU23-NEXT:                               designators: [],
// GNU23-NEXT:                               value: Expr(
// GNU23-NEXT:                                   IntegerLiteral(
// GNU23-NEXT:                                       IntegerLiteral {
// GNU23-NEXT:                                           value: 14,
// GNU23-NEXT:                                           radix: Decimal,
// GNU23-NEXT:                                           suffix: IntegerSuffix {
// GNU23-NEXT:                                               unsigned: false,
// GNU23-NEXT:                                               size: None,
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                           spelling: "14",
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                       ],
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:               Expr(
// GNU23-NEXT:                   CompoundLiteral {
// GNU23-NEXT:                       ty: TypeName {
// GNU23-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU23-NEXT:                               ty: Integer(
// GNU23-NEXT:                                   Ranked {
// GNU23-NEXT:                                       rank: Int,
// GNU23-NEXT:                                       signed: true,
// GNU23-NEXT:                                   },
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                           declarator: Abstract,
// GNU23-NEXT:                       },
// GNU23-NEXT:                       initializer: [
// GNU23-NEXT:                           InitializerItem {
// GNU23-NEXT:                               designators: [],
// GNU23-NEXT:                               value: Expr(
// GNU23-NEXT:                                   IntegerLiteral(
// GNU23-NEXT:                                       IntegerLiteral {
// GNU23-NEXT:                                           value: 15,
// GNU23-NEXT:                                           radix: Decimal,
// GNU23-NEXT:                                           suffix: IntegerSuffix {
// GNU23-NEXT:                                               unsigned: false,
// GNU23-NEXT:                                               size: None,
// GNU23-NEXT:                                           },
// GNU23-NEXT:                                           spelling: "15",
// GNU23-NEXT:                                       },
// GNU23-NEXT:                                   ),
// GNU23-NEXT:                               ),
// GNU23-NEXT:                           },
// GNU23-NEXT:                       ],
// GNU23-NEXT:                   },
// GNU23-NEXT:               ),
// GNU23-NEXT:           ],
// GNU23-NEXT:       },
// GNU23-NEXT:   )
// SLATE-FILECHECK-END GNU23
// SLATE-FILECHECK-BEGIN C2Y
// C2Y: tag[{{[0-9]+}}]: TagDefinition {
// C2Y-NEXT:       id: TagId(
// C2Y-NEXT:           [[#TAG0:]],
// C2Y-NEXT:       ),
// C2Y-NEXT:       kind: Struct,
// C2Y-NEXT:       name: Some(
// C2Y-NEXT:           "pair",
// C2Y-NEXT:       ),
// C2Y-NEXT:       body: Record(
// C2Y-NEXT:           [
// C2Y-NEXT:               Field(
// C2Y-NEXT:                   FieldDecl {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           FieldDeclaratorKind {
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Name(
// C2Y-NEXT:                                       "values",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   size: Expression(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 2,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "2",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           ],
// C2Y-NEXT:       ),
// C2Y-NEXT:   }
// C2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// C2Y-NEXT:       Declaration {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Integer(
// C2Y-NEXT:                   Ranked {
// C2Y-NEXT:                       rank: Int,
// C2Y-NEXT:                       signed: true,
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               storage: Typedef,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarators: [
// C2Y-NEXT:               InitDeclaratorKind {
// C2Y-NEXT:                   declarator: Name(
// C2Y-NEXT:                       "T",
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// C2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// C2Y-NEXT:       Declaration {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Tag(
// C2Y-NEXT:                   Definition(
// C2Y-NEXT:                       TagId(
// C2Y-NEXT:                           [[#TAG0]],
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:               ),
// C2Y-NEXT:           },
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// C2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// C2Y-NEXT:       Declaration {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Integer(
// C2Y-NEXT:                   Ranked {
// C2Y-NEXT:                       rank: Int,
// C2Y-NEXT:                       signed: true,
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           },
// C2Y-NEXT:           declarators: [
// C2Y-NEXT:               InitDeclaratorKind {
// C2Y-NEXT:                   declarator: Pointer {
// C2Y-NEXT:                       qualifiers: Qualifiers,
// C2Y-NEXT:                       inner: Name(
// C2Y-NEXT:                           "global",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   },
// C2Y-NEXT:                   initializer: Some(
// C2Y-NEXT:                       Expr(
// C2Y-NEXT:                           Unary {
// C2Y-NEXT:                               op: AddrOf,
// C2Y-NEXT:                               operand: CompoundLiteral {
// C2Y-NEXT:                                   ty: TypeName {
// C2Y-NEXT:                                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                           ty: Integer(
// C2Y-NEXT:                                               Ranked {
// C2Y-NEXT:                                                   rank: Int,
// C2Y-NEXT:                                                   signed: true,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           storage: Static,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       declarator: Abstract,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   initializer: [
// C2Y-NEXT:                                       InitializerItem {
// C2Y-NEXT:                                           designators: [],
// C2Y-NEXT:                                           value: Expr(
// C2Y-NEXT:                                               IntegerLiteral(
// C2Y-NEXT:                                                   IntegerLiteral {
// C2Y-NEXT:                                                       value: 1,
// C2Y-NEXT:                                                       radix: Decimal,
// C2Y-NEXT:                                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                                           unsigned: false,
// C2Y-NEXT:                                                           size: None,
// C2Y-NEXT:                                                       },
// C2Y-NEXT:                                                       spelling: "1",
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ],
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// C2Y-NEXT: decl[{{[0-9]+}}]: Function(
// C2Y-NEXT:       FunctionDefinition {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Void,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarator: Function {
// C2Y-NEXT:               inner: Name(
// C2Y-NEXT:                   "literals",
// C2Y-NEXT:               ),
// C2Y-NEXT:               parameters: Prototype {
// C2Y-NEXT:                   parameters: [
// C2Y-NEXT:                       ParameterDeclarationKind {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Name(
// C2Y-NEXT:                               "i",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ],
// C2Y-NEXT:               },
// C2Y-NEXT:           },
// C2Y-NEXT:           body: [
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Index {
// C2Y-NEXT:                       base: CompoundLiteral {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   storage: Register,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Abstract,
// C2Y-NEXT:                                   size: Expression(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 2,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "2",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                           initializer: [
// C2Y-NEXT:                               InitializerItem {
// C2Y-NEXT:                                   designators: [],
// C2Y-NEXT:                                   value: Expr(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 2,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "2",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               InitializerItem {
// C2Y-NEXT:                                   designators: [],
// C2Y-NEXT:                                   value: Expr(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 3,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "3",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                       index: Identifier(
// C2Y-NEXT:                           "i",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Index {
// C2Y-NEXT:                       base: Member {
// C2Y-NEXT:                           base: CompoundLiteral {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Tag(
// C2Y-NEXT:                                           Reference {
// C2Y-NEXT:                                               kind: Struct,
// C2Y-NEXT:                                               name: "pair",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       storage: Register,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Abstract,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               initializer: [
// C2Y-NEXT:                                   InitializerItem {
// C2Y-NEXT:                                       designators: [
// C2Y-NEXT:                                           Field(
// C2Y-NEXT:                                               "values",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ],
// C2Y-NEXT:                                       value: List(
// C2Y-NEXT:                                           [
// C2Y-NEXT:                                               InitializerItem {
// C2Y-NEXT:                                                   designators: [],
// C2Y-NEXT:                                                   value: Expr(
// C2Y-NEXT:                                                       IntegerLiteral(
// C2Y-NEXT:                                                           IntegerLiteral {
// C2Y-NEXT:                                                               value: 4,
// C2Y-NEXT:                                                               radix: Decimal,
// C2Y-NEXT:                                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                                   unsigned: false,
// C2Y-NEXT:                                                                   size: None,
// C2Y-NEXT:                                                               },
// C2Y-NEXT:                                                               spelling: "4",
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                       ),
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               InitializerItem {
// C2Y-NEXT:                                                   designators: [],
// C2Y-NEXT:                                                   value: Expr(
// C2Y-NEXT:                                                       IntegerLiteral(
// C2Y-NEXT:                                                           IntegerLiteral {
// C2Y-NEXT:                                                               value: 5,
// C2Y-NEXT:                                                               radix: Decimal,
// C2Y-NEXT:                                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                                   unsigned: false,
// C2Y-NEXT:                                                                   size: None,
// C2Y-NEXT:                                                               },
// C2Y-NEXT:                                                               spelling: "5",
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                       ),
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                           ],
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ],
// C2Y-NEXT:                           },
// C2Y-NEXT:                           field: "values",
// C2Y-NEXT:                       },
// C2Y-NEXT:                       index: Identifier(
// C2Y-NEXT:                           "i",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Unary {
// C2Y-NEXT:                       op: AddrOf,
// C2Y-NEXT:                       operand: CompoundLiteral {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Named(
// C2Y-NEXT:                                       "T",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   storage: Static,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                           initializer: [
// C2Y-NEXT:                               InitializerItem {
// C2Y-NEXT:                                   designators: [],
// C2Y-NEXT:                                   value: Expr(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 6,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "6",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   CompoundLiteral {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               is_constexpr: true,
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                       initializer: [
// C2Y-NEXT:                           InitializerItem {
// C2Y-NEXT:                               designators: [],
// C2Y-NEXT:                               value: Expr(
// C2Y-NEXT:                                   IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 7,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "7",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   CompoundLiteral {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               storage: Register,
// C2Y-NEXT:                               is_constexpr: true,
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                       initializer: [
// C2Y-NEXT:                           InitializerItem {
// C2Y-NEXT:                               designators: [],
// C2Y-NEXT:                               value: Expr(
// C2Y-NEXT:                                   IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 8,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "8",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Unary {
// C2Y-NEXT:                       op: AddrOf,
// C2Y-NEXT:                       operand: CompoundLiteral {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   storage: Static,
// C2Y-NEXT:                                   is_constexpr: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                           initializer: [
// C2Y-NEXT:                               InitializerItem {
// C2Y-NEXT:                                   designators: [],
// C2Y-NEXT:                                   value: Expr(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 9,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "9",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Unary {
// C2Y-NEXT:                       op: AddrOf,
// C2Y-NEXT:                       operand: CompoundLiteral {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   storage: Static,
// C2Y-NEXT:                                   is_thread_local: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                           initializer: [
// C2Y-NEXT:                               InitializerItem {
// C2Y-NEXT:                                   designators: [],
// C2Y-NEXT:                                   value: Expr(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 10,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "10",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Unary {
// C2Y-NEXT:                       op: AddrOf,
// C2Y-NEXT:                       operand: CompoundLiteral {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   storage: Static,
// C2Y-NEXT:                                   is_thread_local: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                           initializer: [
// C2Y-NEXT:                               InitializerItem {
// C2Y-NEXT:                                   designators: [],
// C2Y-NEXT:                                   value: Expr(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 11,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "11",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Unary {
// C2Y-NEXT:                       op: AddrOf,
// C2Y-NEXT:                       operand: CompoundLiteral {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   storage: Static,
// C2Y-NEXT:                                   is_thread_local: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                           initializer: [
// C2Y-NEXT:                               InitializerItem {
// C2Y-NEXT:                                   designators: [],
// C2Y-NEXT:                                   value: Expr(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 12,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "12",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   CompoundLiteral {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               storage: Static,
// C2Y-NEXT:                               attributes: [
// C2Y-NEXT:                                   AlignAs(
// C2Y-NEXT:                                       Expr(
// C2Y-NEXT:                                           IntegerLiteral(
// C2Y-NEXT:                                               IntegerLiteral {
// C2Y-NEXT:                                                   value: 16,
// C2Y-NEXT:                                                   radix: Decimal,
// C2Y-NEXT:                                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                                       unsigned: false,
// C2Y-NEXT:                                                       size: None,
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                                   spelling: "16",
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ],
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                       initializer: [
// C2Y-NEXT:                           InitializerItem {
// C2Y-NEXT:                               designators: [],
// C2Y-NEXT:                               value: Expr(
// C2Y-NEXT:                                   IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 14,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "14",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   CompoundLiteral {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                       initializer: [
// C2Y-NEXT:                           InitializerItem {
// C2Y-NEXT:                               designators: [],
// C2Y-NEXT:                               value: Expr(
// C2Y-NEXT:                                   IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 15,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "15",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN GNU2Y
// GNU2Y: tag[{{[0-9]+}}]: TagDefinition {
// GNU2Y-NEXT:       id: TagId(
// GNU2Y-NEXT:           [[#TAG0:]],
// GNU2Y-NEXT:       ),
// GNU2Y-NEXT:       kind: Struct,
// GNU2Y-NEXT:       name: Some(
// GNU2Y-NEXT:           "pair",
// GNU2Y-NEXT:       ),
// GNU2Y-NEXT:       body: Record(
// GNU2Y-NEXT:           [
// GNU2Y-NEXT:               Field(
// GNU2Y-NEXT:                   FieldDecl {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           FieldDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Name(
// GNU2Y-NEXT:                                       "values",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   size: Expression(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 2,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "2",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       ),
// GNU2Y-NEXT:   }
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU2Y-NEXT:       Declaration {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Integer(
// GNU2Y-NEXT:                   Ranked {
// GNU2Y-NEXT:                       rank: Int,
// GNU2Y-NEXT:                       signed: true,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               storage: Typedef,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarators: [
// GNU2Y-NEXT:               InitDeclaratorKind {
// GNU2Y-NEXT:                   declarator: Name(
// GNU2Y-NEXT:                       "T",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU2Y-NEXT:       Declaration {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Tag(
// GNU2Y-NEXT:                   Definition(
// GNU2Y-NEXT:                       TagId(
// GNU2Y-NEXT:                           [[#TAG0]],
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU2Y-NEXT:       Declaration {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Integer(
// GNU2Y-NEXT:                   Ranked {
// GNU2Y-NEXT:                       rank: Int,
// GNU2Y-NEXT:                       signed: true,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarators: [
// GNU2Y-NEXT:               InitDeclaratorKind {
// GNU2Y-NEXT:                   declarator: Pointer {
// GNU2Y-NEXT:                       qualifiers: Qualifiers,
// GNU2Y-NEXT:                       inner: Name(
// GNU2Y-NEXT:                           "global",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   initializer: Some(
// GNU2Y-NEXT:                       Expr(
// GNU2Y-NEXT:                           Unary {
// GNU2Y-NEXT:                               op: AddrOf,
// GNU2Y-NEXT:                               operand: CompoundLiteral {
// GNU2Y-NEXT:                                   ty: TypeName {
// GNU2Y-NEXT:                                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                           ty: Integer(
// GNU2Y-NEXT:                                               Ranked {
// GNU2Y-NEXT:                                                   rank: Int,
// GNU2Y-NEXT:                                                   signed: true,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           storage: Static,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       declarator: Abstract,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   initializer: [
// GNU2Y-NEXT:                                       InitializerItem {
// GNU2Y-NEXT:                                           designators: [],
// GNU2Y-NEXT:                                           value: Expr(
// GNU2Y-NEXT:                                               IntegerLiteral(
// GNU2Y-NEXT:                                                   IntegerLiteral {
// GNU2Y-NEXT:                                                       value: 1,
// GNU2Y-NEXT:                                                       radix: Decimal,
// GNU2Y-NEXT:                                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                           unsigned: false,
// GNU2Y-NEXT:                                                           size: None,
// GNU2Y-NEXT:                                                       },
// GNU2Y-NEXT:                                                       spelling: "1",
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ],
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Function(
// GNU2Y-NEXT:       FunctionDefinition {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Void,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarator: Function {
// GNU2Y-NEXT:               inner: Name(
// GNU2Y-NEXT:                   "literals",
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               parameters: Prototype {
// GNU2Y-NEXT:                   parameters: [
// GNU2Y-NEXT:                       ParameterDeclarationKind {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Name(
// GNU2Y-NEXT:                               "i",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ],
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           body: [
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Index {
// GNU2Y-NEXT:                       base: CompoundLiteral {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   storage: Register,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Abstract,
// GNU2Y-NEXT:                                   size: Expression(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 2,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "2",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           initializer: [
// GNU2Y-NEXT:                               InitializerItem {
// GNU2Y-NEXT:                                   designators: [],
// GNU2Y-NEXT:                                   value: Expr(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 2,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "2",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               InitializerItem {
// GNU2Y-NEXT:                                   designators: [],
// GNU2Y-NEXT:                                   value: Expr(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 3,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "3",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       index: Identifier(
// GNU2Y-NEXT:                           "i",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Index {
// GNU2Y-NEXT:                       base: Member {
// GNU2Y-NEXT:                           base: CompoundLiteral {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Tag(
// GNU2Y-NEXT:                                           Reference {
// GNU2Y-NEXT:                                               kind: Struct,
// GNU2Y-NEXT:                                               name: "pair",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       storage: Register,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Abstract,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               initializer: [
// GNU2Y-NEXT:                                   InitializerItem {
// GNU2Y-NEXT:                                       designators: [
// GNU2Y-NEXT:                                           Field(
// GNU2Y-NEXT:                                               "values",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ],
// GNU2Y-NEXT:                                       value: List(
// GNU2Y-NEXT:                                           [
// GNU2Y-NEXT:                                               InitializerItem {
// GNU2Y-NEXT:                                                   designators: [],
// GNU2Y-NEXT:                                                   value: Expr(
// GNU2Y-NEXT:                                                       IntegerLiteral(
// GNU2Y-NEXT:                                                           IntegerLiteral {
// GNU2Y-NEXT:                                                               value: 4,
// GNU2Y-NEXT:                                                               radix: Decimal,
// GNU2Y-NEXT:                                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                                   unsigned: false,
// GNU2Y-NEXT:                                                                   size: None,
// GNU2Y-NEXT:                                                               },
// GNU2Y-NEXT:                                                               spelling: "4",
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                       ),
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               InitializerItem {
// GNU2Y-NEXT:                                                   designators: [],
// GNU2Y-NEXT:                                                   value: Expr(
// GNU2Y-NEXT:                                                       IntegerLiteral(
// GNU2Y-NEXT:                                                           IntegerLiteral {
// GNU2Y-NEXT:                                                               value: 5,
// GNU2Y-NEXT:                                                               radix: Decimal,
// GNU2Y-NEXT:                                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                                   unsigned: false,
// GNU2Y-NEXT:                                                                   size: None,
// GNU2Y-NEXT:                                                               },
// GNU2Y-NEXT:                                                               spelling: "5",
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                       ),
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                           ],
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ],
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           field: "values",
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       index: Identifier(
// GNU2Y-NEXT:                           "i",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Unary {
// GNU2Y-NEXT:                       op: AddrOf,
// GNU2Y-NEXT:                       operand: CompoundLiteral {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Named(
// GNU2Y-NEXT:                                       "T",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   storage: Static,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           initializer: [
// GNU2Y-NEXT:                               InitializerItem {
// GNU2Y-NEXT:                                   designators: [],
// GNU2Y-NEXT:                                   value: Expr(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 6,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "6",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   CompoundLiteral {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               is_constexpr: true,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       initializer: [
// GNU2Y-NEXT:                           InitializerItem {
// GNU2Y-NEXT:                               designators: [],
// GNU2Y-NEXT:                               value: Expr(
// GNU2Y-NEXT:                                   IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 7,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "7",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   CompoundLiteral {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               storage: Register,
// GNU2Y-NEXT:                               is_constexpr: true,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       initializer: [
// GNU2Y-NEXT:                           InitializerItem {
// GNU2Y-NEXT:                               designators: [],
// GNU2Y-NEXT:                               value: Expr(
// GNU2Y-NEXT:                                   IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 8,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "8",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Unary {
// GNU2Y-NEXT:                       op: AddrOf,
// GNU2Y-NEXT:                       operand: CompoundLiteral {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   storage: Static,
// GNU2Y-NEXT:                                   is_constexpr: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           initializer: [
// GNU2Y-NEXT:                               InitializerItem {
// GNU2Y-NEXT:                                   designators: [],
// GNU2Y-NEXT:                                   value: Expr(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 9,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "9",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Unary {
// GNU2Y-NEXT:                       op: AddrOf,
// GNU2Y-NEXT:                       operand: CompoundLiteral {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   storage: Static,
// GNU2Y-NEXT:                                   is_thread_local: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           initializer: [
// GNU2Y-NEXT:                               InitializerItem {
// GNU2Y-NEXT:                                   designators: [],
// GNU2Y-NEXT:                                   value: Expr(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 10,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "10",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Unary {
// GNU2Y-NEXT:                       op: AddrOf,
// GNU2Y-NEXT:                       operand: CompoundLiteral {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   storage: Static,
// GNU2Y-NEXT:                                   is_thread_local: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           initializer: [
// GNU2Y-NEXT:                               InitializerItem {
// GNU2Y-NEXT:                                   designators: [],
// GNU2Y-NEXT:                                   value: Expr(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 11,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "11",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Unary {
// GNU2Y-NEXT:                       op: AddrOf,
// GNU2Y-NEXT:                       operand: CompoundLiteral {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   storage: Static,
// GNU2Y-NEXT:                                   is_thread_local: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           initializer: [
// GNU2Y-NEXT:                               InitializerItem {
// GNU2Y-NEXT:                                   designators: [],
// GNU2Y-NEXT:                                   value: Expr(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 12,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "12",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   CompoundLiteral {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               storage: Static,
// GNU2Y-NEXT:                               attributes: [
// GNU2Y-NEXT:                                   AlignAs(
// GNU2Y-NEXT:                                       Expr(
// GNU2Y-NEXT:                                           IntegerLiteral(
// GNU2Y-NEXT:                                               IntegerLiteral {
// GNU2Y-NEXT:                                                   value: 16,
// GNU2Y-NEXT:                                                   radix: Decimal,
// GNU2Y-NEXT:                                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                       unsigned: false,
// GNU2Y-NEXT:                                                       size: None,
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                                   spelling: "16",
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ],
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       initializer: [
// GNU2Y-NEXT:                           InitializerItem {
// GNU2Y-NEXT:                               designators: [],
// GNU2Y-NEXT:                               value: Expr(
// GNU2Y-NEXT:                                   IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 14,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "14",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   CompoundLiteral {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       initializer: [
// GNU2Y-NEXT:                           InitializerItem {
// GNU2Y-NEXT:                               designators: [],
// GNU2Y-NEXT:                               value: Expr(
// GNU2Y-NEXT:                                   IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 15,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "15",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// SLATE-FILECHECK-END GNU2Y
