/* Test C23 digit separators.  Valid usages.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_Static_assert(123'45'6 == 123456);
_Static_assert(0'123 == 0123);
_Static_assert(0x1'23 == 0x123);
_Static_assert(0b1'01 == 0b101);

#define m(x) 0

_Static_assert(m(1'2) + (3'4) == 34);

_Static_assert(0x0'e - 0xe == 0);

#define a0      '.' -
#define acat(x) a##x
_Static_assert(acat(0 '.') == 0);

#define c0(x) 0
#define b0 c0 (
#define bcat(x) b##x
_Static_assert (bcat (0'\u00c0')) == 0);

extern void exit(int);
extern void abort(void);

int main(void) {
  if (314'159e-0'5f != 3.14159f)
    abort();
  exit(0);
}

#line 0'123
_Static_assert(__LINE__ == 123);

#line 4'56'7'8'9
_Static_assert(__LINE__ == 456789);



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 123456,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "123'45'6",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 123456,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "123456",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 83,
// DEFAULT-NEXT:                       radix: Octal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0'123",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 83,
// DEFAULT-NEXT:                       radix: Octal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0123",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 291,
// DEFAULT-NEXT:                       radix: Hex,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0x1'23",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 291,
// DEFAULT-NEXT:                       radix: Hex,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0x123",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 5,
// DEFAULT-NEXT:                       radix: Binary,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0b1'01",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 5,
// DEFAULT-NEXT:                       radix: Binary,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0b101",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: Binary {
// DEFAULT-NEXT:                   op: Add,
// DEFAULT-NEXT:                   left: IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 0,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   right: Paren(
// DEFAULT-NEXT:                       IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 34,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "3'4",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 34,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "34",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: Binary {
// DEFAULT-NEXT:                   op: Sub,
// DEFAULT-NEXT:                   left: IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 14,
// DEFAULT-NEXT:                           radix: Hex,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0x0'e",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   right: IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 14,
// DEFAULT-NEXT:                           radix: Hex,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0xe",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 0,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: Binary {
// DEFAULT-NEXT:                   op: Sub,
// DEFAULT-NEXT:                   left: CharLiteral(
// DEFAULT-NEXT:                       CharLiteral {
// DEFAULT-NEXT:                           encoding: Plain,
// DEFAULT-NEXT:                           code_units: [
// DEFAULT-NEXT:                               46,
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           spelling: ".",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   right: CharLiteral(
// DEFAULT-NEXT:                       CharLiteral {
// DEFAULT-NEXT:                           encoding: Plain,
// DEFAULT-NEXT:                           code_units: [
// DEFAULT-NEXT:                               46,
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           spelling: ".",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 0,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 0,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 0,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "0",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[8]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[9]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "abort",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Void,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[10]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Binary {
// DEFAULT-NEXT:                       op: NotEqual,
// DEFAULT-NEXT:                       left: FloatLiteral(
// DEFAULT-NEXT:                           FloatLiteral {
// DEFAULT-NEXT:                               spelling: "314'159e-0'5f",
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: F,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       right: FloatLiteral(
// DEFAULT-NEXT:                           FloatLiteral {
// DEFAULT-NEXT:                               spelling: "3.14159f",
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: F,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: Expr(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "abort",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 0,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[11]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 123,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "123",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 123,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "123",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[12]: StaticAssert(
// DEFAULT-NEXT:       StaticAssert {
// DEFAULT-NEXT:           condition: Binary {
// DEFAULT-NEXT:               op: Equal,
// DEFAULT-NEXT:               left: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 456789,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "456789",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               right: IntegerLiteral(
// DEFAULT-NEXT:                   IntegerLiteral {
// DEFAULT-NEXT:                       value: 456789,
// DEFAULT-NEXT:                       radix: Decimal,
// DEFAULT-NEXT:                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                           unsigned: false,
// DEFAULT-NEXT:                           size: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       spelling: "456789",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
