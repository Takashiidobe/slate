typedef unsigned long word;
typedef unsigned _BitInt(575) huge;
#define LIMIT(op, type) op(type)
#define _Maxof_ALIAS _Maxof

int largest = _Maxof(int);

void limits(int n) {
    int bound[_Maxof(unsigned char)];
    _Maxof(char);
    _Minof(const volatile unsigned short);
    _Maxof(unsigned long long);
    _Minof(word);
    _Maxof(_BitInt(5));
    _Minof(unsigned _BitInt(1));
    _Maxof(huge);
    _Minof(_BitInt(575));
    _Maxof(typeof(n));
    LIMIT(_Minof, unsigned int);
    _Maxof_ALIAS(int);
    _Maxof(int) + _Minof(int);
    _Generic(_Maxof(int), int: 1, default: 0);
}

#define _Maxof(T) 7
int replaced = _Maxof(int);

// SLATE-FILECHECK-AST
// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES GNU2Y
// SLATE-FILECHECK-STD GNU2Y gnu2y
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ERROR C23
// SLATE-FILECHECK-DEFINES GNU23
// SLATE-FILECHECK-STD GNU23 gnu23
// SLATE-FILECHECK-ERROR GNU23
// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-PREFIX-ARGS IR --dump-ir
// SLATE-FILECHECK-STD IR c2y

// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × expected `)`, found `char`
// C23: ╰─▶ expected `)`, found `char`
// C23: ╭─[tests/fixtures/clang/linux/x86_64/c2y_integer_type_limits.c:9:36]
// C23: 8 │ void limits(int n) {
// C23: 9 │     int bound[_Maxof(unsigned char)];
// C23: ·                                    ─
// C23: 10 │     _Maxof(char);
// C23: ╰────
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU23
// GNU23: Error:   × expected `)`, found `char`
// GNU23: ╰─▶ expected `)`, found `char`
// GNU23: ╭─[tests/fixtures/clang/linux/x86_64/c2y_integer_type_limits.c:9:36]
// GNU23: 8 │ void limits(int n) {
// GNU23: 9 │     int bound[_Maxof(unsigned char)];
// GNU23: ·                                    ─
// GNU23: 10 │     _Maxof(char);
// GNU23: ╰────
// SLATE-FILECHECK-END GNU23
// SLATE-FILECHECK-BEGIN C2Y
// C2Y: decl[{{[0-9]+}}]: Declaration(
// C2Y-NEXT:       Declaration {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Integer(
// C2Y-NEXT:                   Ranked {
// C2Y-NEXT:                       rank: Long,
// C2Y-NEXT:                       signed: false,
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               storage: Typedef,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarators: [
// C2Y-NEXT:               InitDeclaratorKind {
// C2Y-NEXT:                   declarator: Name(
// C2Y-NEXT:                       "word",
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// C2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// C2Y-NEXT:       Declaration {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Integer(
// C2Y-NEXT:                   BitInt {
// C2Y-NEXT:                       width: IntegerLiteral(
// C2Y-NEXT:                           IntegerLiteral {
// C2Y-NEXT:                               value: 575,
// C2Y-NEXT:                               radix: Decimal,
// C2Y-NEXT:                               suffix: IntegerSuffix {
// C2Y-NEXT:                                   unsigned: false,
// C2Y-NEXT:                                   size: None,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               spelling: "575",
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       signed: false,
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               storage: Typedef,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarators: [
// C2Y-NEXT:               InitDeclaratorKind {
// C2Y-NEXT:                   declarator: Name(
// C2Y-NEXT:                       "huge",
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
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
// C2Y-NEXT:                   declarator: Name(
// C2Y-NEXT:                       "largest",
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   initializer: Some(
// C2Y-NEXT:                       Expr(
// C2Y-NEXT:                           MaxOf {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Abstract,
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
// C2Y-NEXT:                   "limits",
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
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ],
// C2Y-NEXT:               },
// C2Y-NEXT:           },
// C2Y-NEXT:           body: [
// C2Y-NEXT:               Decl(
// C2Y-NEXT:                   Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Name(
// C2Y-NEXT:                                       "bound",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   size: Expression(
// C2Y-NEXT:                                       MaxOf {
// C2Y-NEXT:                                           ty: TypeName {
// C2Y-NEXT:                                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                                   ty: Integer(
// C2Y-NEXT:                                                       Char {
// C2Y-NEXT:                                                           signed: Some(
// C2Y-NEXT:                                                               false,
// C2Y-NEXT:                                                           ),
// C2Y-NEXT:                                                       },
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               declarator: Abstract,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MaxOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Char {
// C2Y-NEXT:                                       signed: None,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MinOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Short,
// C2Y-NEXT:                                       signed: false,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               qualifiers: Qualifiers {
// C2Y-NEXT:                                   is_const: true,
// C2Y-NEXT:                                   is_volatile: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MaxOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: LongLong,
// C2Y-NEXT:                                       signed: false,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MinOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Named(
// C2Y-NEXT:                                   "word",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MaxOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   BitInt {
// C2Y-NEXT:                                       width: IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 5,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "5",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MinOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   BitInt {
// C2Y-NEXT:                                       width: IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 1,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "1",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       signed: false,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MaxOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Named(
// C2Y-NEXT:                                   "huge",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MinOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   BitInt {
// C2Y-NEXT:                                       width: IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 575,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "575",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MaxOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: TypeOf(
// C2Y-NEXT:                                   Expression(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MinOf {
// C2Y-NEXT:                       ty: TypeName {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: false,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Abstract,
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   MaxOf {
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
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Binary {
// C2Y-NEXT:                       op: Add,
// C2Y-NEXT:                       left: MaxOf {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                       right: MinOf {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Expr(
// C2Y-NEXT:                           MaxOf {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Abstract,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Abstract,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 1,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "1",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Default(
// C2Y-NEXT:                               IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           ],
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
// C2Y-NEXT:                   declarator: Name(
// C2Y-NEXT:                       "replaced",
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   initializer: Some(
// C2Y-NEXT:                       Expr(
// C2Y-NEXT:                           IntegerLiteral(
// C2Y-NEXT:                               IntegerLiteral {
// C2Y-NEXT:                                   value: 7,
// C2Y-NEXT:                                   radix: Decimal,
// C2Y-NEXT:                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                       unsigned: false,
// C2Y-NEXT:                                       size: None,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   spelling: "7",
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN GNU2Y
// GNU2Y: decl[{{[0-9]+}}]: Declaration(
// GNU2Y-NEXT:       Declaration {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Integer(
// GNU2Y-NEXT:                   Ranked {
// GNU2Y-NEXT:                       rank: Long,
// GNU2Y-NEXT:                       signed: false,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               storage: Typedef,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarators: [
// GNU2Y-NEXT:               InitDeclaratorKind {
// GNU2Y-NEXT:                   declarator: Name(
// GNU2Y-NEXT:                       "word",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU2Y-NEXT:       Declaration {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Integer(
// GNU2Y-NEXT:                   BitInt {
// GNU2Y-NEXT:                       width: IntegerLiteral(
// GNU2Y-NEXT:                           IntegerLiteral {
// GNU2Y-NEXT:                               value: 575,
// GNU2Y-NEXT:                               radix: Decimal,
// GNU2Y-NEXT:                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                   unsigned: false,
// GNU2Y-NEXT:                                   size: None,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               spelling: "575",
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       signed: false,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               storage: Typedef,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarators: [
// GNU2Y-NEXT:               InitDeclaratorKind {
// GNU2Y-NEXT:                   declarator: Name(
// GNU2Y-NEXT:                       "huge",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
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
// GNU2Y-NEXT:                   declarator: Name(
// GNU2Y-NEXT:                       "largest",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   initializer: Some(
// GNU2Y-NEXT:                       Expr(
// GNU2Y-NEXT:                           MaxOf {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Abstract,
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
// GNU2Y-NEXT:                   "limits",
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
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ],
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           body: [
// GNU2Y-NEXT:               Decl(
// GNU2Y-NEXT:                   Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Name(
// GNU2Y-NEXT:                                       "bound",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   size: Expression(
// GNU2Y-NEXT:                                       MaxOf {
// GNU2Y-NEXT:                                           ty: TypeName {
// GNU2Y-NEXT:                                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                                   ty: Integer(
// GNU2Y-NEXT:                                                       Char {
// GNU2Y-NEXT:                                                           signed: Some(
// GNU2Y-NEXT:                                                               false,
// GNU2Y-NEXT:                                                           ),
// GNU2Y-NEXT:                                                       },
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               declarator: Abstract,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MaxOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Char {
// GNU2Y-NEXT:                                       signed: None,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MinOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Short,
// GNU2Y-NEXT:                                       signed: false,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               qualifiers: Qualifiers {
// GNU2Y-NEXT:                                   is_const: true,
// GNU2Y-NEXT:                                   is_volatile: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MaxOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: LongLong,
// GNU2Y-NEXT:                                       signed: false,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MinOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Named(
// GNU2Y-NEXT:                                   "word",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MaxOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   BitInt {
// GNU2Y-NEXT:                                       width: IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 5,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "5",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MinOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   BitInt {
// GNU2Y-NEXT:                                       width: IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 1,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "1",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       signed: false,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MaxOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Named(
// GNU2Y-NEXT:                                   "huge",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MinOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   BitInt {
// GNU2Y-NEXT:                                       width: IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 575,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "575",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MaxOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: TypeOf(
// GNU2Y-NEXT:                                   Expression(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MinOf {
// GNU2Y-NEXT:                       ty: TypeName {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: false,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Abstract,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   MaxOf {
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
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Binary {
// GNU2Y-NEXT:                       op: Add,
// GNU2Y-NEXT:                       left: MaxOf {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       right: MinOf {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Expr(
// GNU2Y-NEXT:                           MaxOf {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Abstract,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Abstract,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 1,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "1",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Default(
// GNU2Y-NEXT:                               IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           ],
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
// GNU2Y-NEXT:                   declarator: Name(
// GNU2Y-NEXT:                       "replaced",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   initializer: Some(
// GNU2Y-NEXT:                       Expr(
// GNU2Y-NEXT:                           IntegerLiteral(
// GNU2Y-NEXT:                               IntegerLiteral {
// GNU2Y-NEXT:                                   value: 7,
// GNU2Y-NEXT:                                   radix: Decimal,
// GNU2Y-NEXT:                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                       unsigned: false,
// GNU2Y-NEXT:                                       size: None,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   spelling: "7",
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// SLATE-FILECHECK-END GNU2Y
// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_word:[0-9]+]] word = u64;
// IR-NEXT:     type @type[[TYPE_huge:[0-9]+]] huge = u575b;
// IR-NEXT:     global %[[VALUE_largest:[0-9]+]] largest: i32 [storage=static] = const<i32>(2147483647) [linkage=external];
// IR-NEXT:     global %[[VALUE_replaced:[0-9]+]] replaced: i32 [storage=static] = const<i32>(7) [linkage=external];
// IR-NEXT:     fn %[[VALUE_limits:[0-9]+]] @limits(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_bound:[0-9]+]] bound: array<i32, 255> [storage=automatic] [align=16];
// IR-NEXT:         const<i8>(127);
// IR-NEXT:         const<u16>(0);
// IR-NEXT:         const<u64>(18446744073709551615);
// IR-NEXT:         const<u64>(0);
// IR-NEXT:         const<i5b>(15);
// IR-NEXT:         const<u1b>(0);
// IR-NEXT:         const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567);
// IR-NEXT:         const<i575b>(-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174784);
// IR-NEXT:         const<i32>(2147483647);
// IR-NEXT:         const<u32>(0);
// IR-NEXT:         const<i32>(2147483647);
// IR-NEXT:         add<i32, overflow=ub>(const<i32>(2147483647), const<i32>(-2147483648));
// IR-NEXT:         const<i32>(1);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
