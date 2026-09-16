#if STYLE == 1
typedef
struct
Payload
{
  int
  value
  ;
  int
  (*callback)
  (int)
  ;
}
Payload
;
enum
Mode
:
unsigned
int
{
  FIRST = 1,
  LAST = FIRST + 2
}
;
int
read_value
(
  Payload *p
)
{
  if (p)
  {
    return p->callback(p->value);
  }
  return LAST;
}
int
after
;
#elif STYLE == 2
#define RECORD(name, fields) typedef struct name { fields } name;
#define ENUM(name, ...) enum name : unsigned int { __VA_ARGS__ };
#define FUNCTION(name, args, body) int name args body
RECORD(Payload, int value; int (*callback)(int);)
ENUM(Mode, FIRST = 1, LAST = FIRST + 2)
FUNCTION(read_value, (Payload *p), { if (p) { return p->callback(p->value); } return LAST; })
int after;
#else
typedef struct Payload { int value; int (*callback)(int); } Payload; enum Mode : unsigned int { FIRST = 1, LAST = FIRST + 2 }; int read_value(Payload *p) { if (p) { return p->callback(p->value); } return LAST; } int after;
#endif

// SLATE-FILECHECK-DEFINES COMPACT STYLE=0
// SLATE-FILECHECK-DEFINES SPLIT STYLE=1
// SLATE-FILECHECK-DEFINES MACRO STYLE=2

// SLATE-FILECHECK-BEGIN COMPACT
// COMPACT: tag[0]: TagDefinition {
// COMPACT-NEXT:       id: TagId(
// COMPACT-NEXT:           0,
// COMPACT-NEXT:       ),
// COMPACT-NEXT:       kind: Struct,
// COMPACT-NEXT:       name: Some(
// COMPACT-NEXT:           "Payload",
// COMPACT-NEXT:       ),
// COMPACT-NEXT:       body: Record(
// COMPACT-NEXT:           [
// COMPACT-NEXT:               Field(
// COMPACT-NEXT:                   FieldDecl {
// COMPACT-NEXT:                       specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:                           ty: Integer(
// COMPACT-NEXT:                               Ranked {
// COMPACT-NEXT:                                   rank: Int,
// COMPACT-NEXT:                                   signed: true,
// COMPACT-NEXT:                               },
// COMPACT-NEXT:                           ),
// COMPACT-NEXT:                       },
// COMPACT-NEXT:                       declarators: [
// COMPACT-NEXT:                           FieldDeclaratorKind {
// COMPACT-NEXT:                               declarator: Name(
// COMPACT-NEXT:                                   "value",
// COMPACT-NEXT:                               ),
// COMPACT-NEXT:                           },
// COMPACT-NEXT:                       ],
// COMPACT-NEXT:                   },
// COMPACT-NEXT:               ),
// COMPACT-NEXT:               Field(
// COMPACT-NEXT:                   FieldDecl {
// COMPACT-NEXT:                       specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:                           ty: Integer(
// COMPACT-NEXT:                               Ranked {
// COMPACT-NEXT:                                   rank: Int,
// COMPACT-NEXT:                                   signed: true,
// COMPACT-NEXT:                               },
// COMPACT-NEXT:                           ),
// COMPACT-NEXT:                       },
// COMPACT-NEXT:                       declarators: [
// COMPACT-NEXT:                           FieldDeclaratorKind {
// COMPACT-NEXT:                               declarator: Function {
// COMPACT-NEXT:                                   inner: Grouped(
// COMPACT-NEXT:                                       Pointer {
// COMPACT-NEXT:                                           qualifiers: Qualifiers,
// COMPACT-NEXT:                                           inner: Name(
// COMPACT-NEXT:                                               "callback",
// COMPACT-NEXT:                                           ),
// COMPACT-NEXT:                                       },
// COMPACT-NEXT:                                   ),
// COMPACT-NEXT:                                   parameters: Prototype {
// COMPACT-NEXT:                                       parameters: [
// COMPACT-NEXT:                                           ParameterDeclarationKind {
// COMPACT-NEXT:                                               specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:                                                   ty: Integer(
// COMPACT-NEXT:                                                       Ranked {
// COMPACT-NEXT:                                                           rank: Int,
// COMPACT-NEXT:                                                           signed: true,
// COMPACT-NEXT:                                                       },
// COMPACT-NEXT:                                                   ),
// COMPACT-NEXT:                                               },
// COMPACT-NEXT:                                               declarator: Abstract,
// COMPACT-NEXT:                                           },
// COMPACT-NEXT:                                       ],
// COMPACT-NEXT:                                   },
// COMPACT-NEXT:                               },
// COMPACT-NEXT:                           },
// COMPACT-NEXT:                       ],
// COMPACT-NEXT:                   },
// COMPACT-NEXT:               ),
// COMPACT-NEXT:           ],
// COMPACT-NEXT:       ),
// COMPACT-NEXT:   }
// COMPACT-NEXT: tag[1]: TagDefinition {
// COMPACT-NEXT:       id: TagId(
// COMPACT-NEXT:           1,
// COMPACT-NEXT:       ),
// COMPACT-NEXT:       kind: Enum,
// COMPACT-NEXT:       name: Some(
// COMPACT-NEXT:           "Mode",
// COMPACT-NEXT:       ),
// COMPACT-NEXT:       body: Enum {
// COMPACT-NEXT:           fixed_type: Some(
// COMPACT-NEXT:               TypeName {
// COMPACT-NEXT:                   specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:                       ty: Integer(
// COMPACT-NEXT:                           Ranked {
// COMPACT-NEXT:                               rank: Int,
// COMPACT-NEXT:                               signed: false,
// COMPACT-NEXT:                           },
// COMPACT-NEXT:                       ),
// COMPACT-NEXT:                   },
// COMPACT-NEXT:                   declarator: Abstract,
// COMPACT-NEXT:               },
// COMPACT-NEXT:           ),
// COMPACT-NEXT:           enumerators: [
// COMPACT-NEXT:               Enumerator(
// COMPACT-NEXT:                   Enumerator {
// COMPACT-NEXT:                       name: "FIRST",
// COMPACT-NEXT:                       value: Some(
// COMPACT-NEXT:                           IntegerLiteral(
// COMPACT-NEXT:                               IntegerLiteral {
// COMPACT-NEXT:                                   value: 1,
// COMPACT-NEXT:                                   radix: Decimal,
// COMPACT-NEXT:                                   suffix: IntegerSuffix {
// COMPACT-NEXT:                                       unsigned: false,
// COMPACT-NEXT:                                       size: None,
// COMPACT-NEXT:                                   },
// COMPACT-NEXT:                                   spelling: "1",
// COMPACT-NEXT:                               },
// COMPACT-NEXT:                           ),
// COMPACT-NEXT:                       ),
// COMPACT-NEXT:                   },
// COMPACT-NEXT:               ),
// COMPACT-NEXT:               Enumerator(
// COMPACT-NEXT:                   Enumerator {
// COMPACT-NEXT:                       name: "LAST",
// COMPACT-NEXT:                       value: Some(
// COMPACT-NEXT:                           Binary {
// COMPACT-NEXT:                               op: Add,
// COMPACT-NEXT:                               left: Identifier(
// COMPACT-NEXT:                                   "FIRST",
// COMPACT-NEXT:                               ),
// COMPACT-NEXT:                               right: IntegerLiteral(
// COMPACT-NEXT:                                   IntegerLiteral {
// COMPACT-NEXT:                                       value: 2,
// COMPACT-NEXT:                                       radix: Decimal,
// COMPACT-NEXT:                                       suffix: IntegerSuffix {
// COMPACT-NEXT:                                           unsigned: false,
// COMPACT-NEXT:                                           size: None,
// COMPACT-NEXT:                                       },
// COMPACT-NEXT:                                       spelling: "2",
// COMPACT-NEXT:                                   },
// COMPACT-NEXT:                               ),
// COMPACT-NEXT:                           },
// COMPACT-NEXT:                       ),
// COMPACT-NEXT:                   },
// COMPACT-NEXT:               ),
// COMPACT-NEXT:           ],
// COMPACT-NEXT:       },
// COMPACT-NEXT:   }
// COMPACT-NEXT: decl[0]: Declaration(
// COMPACT-NEXT:       Declaration {
// COMPACT-NEXT:           specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:               ty: Tag(
// COMPACT-NEXT:                   Definition(
// COMPACT-NEXT:                       TagId(
// COMPACT-NEXT:                           0,
// COMPACT-NEXT:                       ),
// COMPACT-NEXT:                   ),
// COMPACT-NEXT:               ),
// COMPACT-NEXT:               storage: Typedef,
// COMPACT-NEXT:           },
// COMPACT-NEXT:           declarators: [
// COMPACT-NEXT:               InitDeclaratorKind {
// COMPACT-NEXT:                   declarator: Name(
// COMPACT-NEXT:                       "Payload",
// COMPACT-NEXT:                   ),
// COMPACT-NEXT:               },
// COMPACT-NEXT:           ],
// COMPACT-NEXT:       },
// COMPACT-NEXT:   )
// COMPACT-NEXT: decl[1]: Declaration(
// COMPACT-NEXT:       Declaration {
// COMPACT-NEXT:           specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:               ty: Tag(
// COMPACT-NEXT:                   Definition(
// COMPACT-NEXT:                       TagId(
// COMPACT-NEXT:                           1,
// COMPACT-NEXT:                       ),
// COMPACT-NEXT:                   ),
// COMPACT-NEXT:               ),
// COMPACT-NEXT:           },
// COMPACT-NEXT:       },
// COMPACT-NEXT:   )
// COMPACT-NEXT: decl[2]: Function(
// COMPACT-NEXT:       FunctionDefinition {
// COMPACT-NEXT:           specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:               ty: Integer(
// COMPACT-NEXT:                   Ranked {
// COMPACT-NEXT:                       rank: Int,
// COMPACT-NEXT:                       signed: true,
// COMPACT-NEXT:                   },
// COMPACT-NEXT:               ),
// COMPACT-NEXT:           },
// COMPACT-NEXT:           declarator: Function {
// COMPACT-NEXT:               inner: Name(
// COMPACT-NEXT:                   "read_value",
// COMPACT-NEXT:               ),
// COMPACT-NEXT:               parameters: Prototype {
// COMPACT-NEXT:                   parameters: [
// COMPACT-NEXT:                       ParameterDeclarationKind {
// COMPACT-NEXT:                           specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:                               ty: Named(
// COMPACT-NEXT:                                   "Payload",
// COMPACT-NEXT:                               ),
// COMPACT-NEXT:                           },
// COMPACT-NEXT:                           declarator: Pointer {
// COMPACT-NEXT:                               qualifiers: Qualifiers,
// COMPACT-NEXT:                               inner: Name(
// COMPACT-NEXT:                                   "p",
// COMPACT-NEXT:                               ),
// COMPACT-NEXT:                           },
// COMPACT-NEXT:                       },
// COMPACT-NEXT:                   ],
// COMPACT-NEXT:               },
// COMPACT-NEXT:           },
// COMPACT-NEXT:           body: [
// COMPACT-NEXT:               If {
// COMPACT-NEXT:                   condition: Identifier(
// COMPACT-NEXT:                       "p",
// COMPACT-NEXT:                   ),
// COMPACT-NEXT:                   then_branch: Block(
// COMPACT-NEXT:                       [
// COMPACT-NEXT:                           Return(
// COMPACT-NEXT:                               Call {
// COMPACT-NEXT:                                   callee: Member {
// COMPACT-NEXT:                                       base: Identifier(
// COMPACT-NEXT:                                           "p",
// COMPACT-NEXT:                                       ),
// COMPACT-NEXT:                                       field: "callback",
// COMPACT-NEXT:                                       arrow: true,
// COMPACT-NEXT:                                   },
// COMPACT-NEXT:                                   arguments: [
// COMPACT-NEXT:                                       Member {
// COMPACT-NEXT:                                           base: Identifier(
// COMPACT-NEXT:                                               "p",
// COMPACT-NEXT:                                           ),
// COMPACT-NEXT:                                           field: "value",
// COMPACT-NEXT:                                           arrow: true,
// COMPACT-NEXT:                                       },
// COMPACT-NEXT:                                   ],
// COMPACT-NEXT:                               },
// COMPACT-NEXT:                           ),
// COMPACT-NEXT:                       ],
// COMPACT-NEXT:                   ),
// COMPACT-NEXT:                   else_branch: None,
// COMPACT-NEXT:               },
// COMPACT-NEXT:               Return(
// COMPACT-NEXT:                   Identifier(
// COMPACT-NEXT:                       "LAST",
// COMPACT-NEXT:                   ),
// COMPACT-NEXT:               ),
// COMPACT-NEXT:           ],
// COMPACT-NEXT:       },
// COMPACT-NEXT:   )
// COMPACT-NEXT: decl[3]: Declaration(
// COMPACT-NEXT:       Declaration {
// COMPACT-NEXT:           specifiers: DeclarationSpecifiers {
// COMPACT-NEXT:               ty: Integer(
// COMPACT-NEXT:                   Ranked {
// COMPACT-NEXT:                       rank: Int,
// COMPACT-NEXT:                       signed: true,
// COMPACT-NEXT:                   },
// COMPACT-NEXT:               ),
// COMPACT-NEXT:           },
// COMPACT-NEXT:           declarators: [
// COMPACT-NEXT:               InitDeclaratorKind {
// COMPACT-NEXT:                   declarator: Name(
// COMPACT-NEXT:                       "after",
// COMPACT-NEXT:                   ),
// COMPACT-NEXT:               },
// COMPACT-NEXT:           ],
// COMPACT-NEXT:       },
// COMPACT-NEXT:   )
// SLATE-FILECHECK-END COMPACT
// SLATE-FILECHECK-BEGIN SPLIT
// SPLIT: tag[0]: TagDefinition {
// SPLIT-NEXT:       id: TagId(
// SPLIT-NEXT:           0,
// SPLIT-NEXT:       ),
// SPLIT-NEXT:       kind: Struct,
// SPLIT-NEXT:       name: Some(
// SPLIT-NEXT:           "Payload",
// SPLIT-NEXT:       ),
// SPLIT-NEXT:       body: Record(
// SPLIT-NEXT:           [
// SPLIT-NEXT:               Field(
// SPLIT-NEXT:                   FieldDecl {
// SPLIT-NEXT:                       specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:                           ty: Integer(
// SPLIT-NEXT:                               Ranked {
// SPLIT-NEXT:                                   rank: Int,
// SPLIT-NEXT:                                   signed: true,
// SPLIT-NEXT:                               },
// SPLIT-NEXT:                           ),
// SPLIT-NEXT:                       },
// SPLIT-NEXT:                       declarators: [
// SPLIT-NEXT:                           FieldDeclaratorKind {
// SPLIT-NEXT:                               declarator: Name(
// SPLIT-NEXT:                                   "value",
// SPLIT-NEXT:                               ),
// SPLIT-NEXT:                           },
// SPLIT-NEXT:                       ],
// SPLIT-NEXT:                   },
// SPLIT-NEXT:               ),
// SPLIT-NEXT:               Field(
// SPLIT-NEXT:                   FieldDecl {
// SPLIT-NEXT:                       specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:                           ty: Integer(
// SPLIT-NEXT:                               Ranked {
// SPLIT-NEXT:                                   rank: Int,
// SPLIT-NEXT:                                   signed: true,
// SPLIT-NEXT:                               },
// SPLIT-NEXT:                           ),
// SPLIT-NEXT:                       },
// SPLIT-NEXT:                       declarators: [
// SPLIT-NEXT:                           FieldDeclaratorKind {
// SPLIT-NEXT:                               declarator: Function {
// SPLIT-NEXT:                                   inner: Grouped(
// SPLIT-NEXT:                                       Pointer {
// SPLIT-NEXT:                                           qualifiers: Qualifiers,
// SPLIT-NEXT:                                           inner: Name(
// SPLIT-NEXT:                                               "callback",
// SPLIT-NEXT:                                           ),
// SPLIT-NEXT:                                       },
// SPLIT-NEXT:                                   ),
// SPLIT-NEXT:                                   parameters: Prototype {
// SPLIT-NEXT:                                       parameters: [
// SPLIT-NEXT:                                           ParameterDeclarationKind {
// SPLIT-NEXT:                                               specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:                                                   ty: Integer(
// SPLIT-NEXT:                                                       Ranked {
// SPLIT-NEXT:                                                           rank: Int,
// SPLIT-NEXT:                                                           signed: true,
// SPLIT-NEXT:                                                       },
// SPLIT-NEXT:                                                   ),
// SPLIT-NEXT:                                               },
// SPLIT-NEXT:                                               declarator: Abstract,
// SPLIT-NEXT:                                           },
// SPLIT-NEXT:                                       ],
// SPLIT-NEXT:                                   },
// SPLIT-NEXT:                               },
// SPLIT-NEXT:                           },
// SPLIT-NEXT:                       ],
// SPLIT-NEXT:                   },
// SPLIT-NEXT:               ),
// SPLIT-NEXT:           ],
// SPLIT-NEXT:       ),
// SPLIT-NEXT:   }
// SPLIT-NEXT: tag[1]: TagDefinition {
// SPLIT-NEXT:       id: TagId(
// SPLIT-NEXT:           1,
// SPLIT-NEXT:       ),
// SPLIT-NEXT:       kind: Enum,
// SPLIT-NEXT:       name: Some(
// SPLIT-NEXT:           "Mode",
// SPLIT-NEXT:       ),
// SPLIT-NEXT:       body: Enum {
// SPLIT-NEXT:           fixed_type: Some(
// SPLIT-NEXT:               TypeName {
// SPLIT-NEXT:                   specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:                       ty: Integer(
// SPLIT-NEXT:                           Ranked {
// SPLIT-NEXT:                               rank: Int,
// SPLIT-NEXT:                               signed: false,
// SPLIT-NEXT:                           },
// SPLIT-NEXT:                       ),
// SPLIT-NEXT:                   },
// SPLIT-NEXT:                   declarator: Abstract,
// SPLIT-NEXT:               },
// SPLIT-NEXT:           ),
// SPLIT-NEXT:           enumerators: [
// SPLIT-NEXT:               Enumerator(
// SPLIT-NEXT:                   Enumerator {
// SPLIT-NEXT:                       name: "FIRST",
// SPLIT-NEXT:                       value: Some(
// SPLIT-NEXT:                           IntegerLiteral(
// SPLIT-NEXT:                               IntegerLiteral {
// SPLIT-NEXT:                                   value: 1,
// SPLIT-NEXT:                                   radix: Decimal,
// SPLIT-NEXT:                                   suffix: IntegerSuffix {
// SPLIT-NEXT:                                       unsigned: false,
// SPLIT-NEXT:                                       size: None,
// SPLIT-NEXT:                                   },
// SPLIT-NEXT:                                   spelling: "1",
// SPLIT-NEXT:                               },
// SPLIT-NEXT:                           ),
// SPLIT-NEXT:                       ),
// SPLIT-NEXT:                   },
// SPLIT-NEXT:               ),
// SPLIT-NEXT:               Enumerator(
// SPLIT-NEXT:                   Enumerator {
// SPLIT-NEXT:                       name: "LAST",
// SPLIT-NEXT:                       value: Some(
// SPLIT-NEXT:                           Binary {
// SPLIT-NEXT:                               op: Add,
// SPLIT-NEXT:                               left: Identifier(
// SPLIT-NEXT:                                   "FIRST",
// SPLIT-NEXT:                               ),
// SPLIT-NEXT:                               right: IntegerLiteral(
// SPLIT-NEXT:                                   IntegerLiteral {
// SPLIT-NEXT:                                       value: 2,
// SPLIT-NEXT:                                       radix: Decimal,
// SPLIT-NEXT:                                       suffix: IntegerSuffix {
// SPLIT-NEXT:                                           unsigned: false,
// SPLIT-NEXT:                                           size: None,
// SPLIT-NEXT:                                       },
// SPLIT-NEXT:                                       spelling: "2",
// SPLIT-NEXT:                                   },
// SPLIT-NEXT:                               ),
// SPLIT-NEXT:                           },
// SPLIT-NEXT:                       ),
// SPLIT-NEXT:                   },
// SPLIT-NEXT:               ),
// SPLIT-NEXT:           ],
// SPLIT-NEXT:       },
// SPLIT-NEXT:   }
// SPLIT-NEXT: decl[0]: Declaration(
// SPLIT-NEXT:       Declaration {
// SPLIT-NEXT:           specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:               ty: Tag(
// SPLIT-NEXT:                   Definition(
// SPLIT-NEXT:                       TagId(
// SPLIT-NEXT:                           0,
// SPLIT-NEXT:                       ),
// SPLIT-NEXT:                   ),
// SPLIT-NEXT:               ),
// SPLIT-NEXT:               storage: Typedef,
// SPLIT-NEXT:           },
// SPLIT-NEXT:           declarators: [
// SPLIT-NEXT:               InitDeclaratorKind {
// SPLIT-NEXT:                   declarator: Name(
// SPLIT-NEXT:                       "Payload",
// SPLIT-NEXT:                   ),
// SPLIT-NEXT:               },
// SPLIT-NEXT:           ],
// SPLIT-NEXT:       },
// SPLIT-NEXT:   )
// SPLIT-NEXT: decl[1]: Declaration(
// SPLIT-NEXT:       Declaration {
// SPLIT-NEXT:           specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:               ty: Tag(
// SPLIT-NEXT:                   Definition(
// SPLIT-NEXT:                       TagId(
// SPLIT-NEXT:                           1,
// SPLIT-NEXT:                       ),
// SPLIT-NEXT:                   ),
// SPLIT-NEXT:               ),
// SPLIT-NEXT:           },
// SPLIT-NEXT:       },
// SPLIT-NEXT:   )
// SPLIT-NEXT: decl[2]: Function(
// SPLIT-NEXT:       FunctionDefinition {
// SPLIT-NEXT:           specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:               ty: Integer(
// SPLIT-NEXT:                   Ranked {
// SPLIT-NEXT:                       rank: Int,
// SPLIT-NEXT:                       signed: true,
// SPLIT-NEXT:                   },
// SPLIT-NEXT:               ),
// SPLIT-NEXT:           },
// SPLIT-NEXT:           declarator: Function {
// SPLIT-NEXT:               inner: Name(
// SPLIT-NEXT:                   "read_value",
// SPLIT-NEXT:               ),
// SPLIT-NEXT:               parameters: Prototype {
// SPLIT-NEXT:                   parameters: [
// SPLIT-NEXT:                       ParameterDeclarationKind {
// SPLIT-NEXT:                           specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:                               ty: Named(
// SPLIT-NEXT:                                   "Payload",
// SPLIT-NEXT:                               ),
// SPLIT-NEXT:                           },
// SPLIT-NEXT:                           declarator: Pointer {
// SPLIT-NEXT:                               qualifiers: Qualifiers,
// SPLIT-NEXT:                               inner: Name(
// SPLIT-NEXT:                                   "p",
// SPLIT-NEXT:                               ),
// SPLIT-NEXT:                           },
// SPLIT-NEXT:                       },
// SPLIT-NEXT:                   ],
// SPLIT-NEXT:               },
// SPLIT-NEXT:           },
// SPLIT-NEXT:           body: [
// SPLIT-NEXT:               If {
// SPLIT-NEXT:                   condition: Identifier(
// SPLIT-NEXT:                       "p",
// SPLIT-NEXT:                   ),
// SPLIT-NEXT:                   then_branch: Block(
// SPLIT-NEXT:                       [
// SPLIT-NEXT:                           Return(
// SPLIT-NEXT:                               Call {
// SPLIT-NEXT:                                   callee: Member {
// SPLIT-NEXT:                                       base: Identifier(
// SPLIT-NEXT:                                           "p",
// SPLIT-NEXT:                                       ),
// SPLIT-NEXT:                                       field: "callback",
// SPLIT-NEXT:                                       arrow: true,
// SPLIT-NEXT:                                   },
// SPLIT-NEXT:                                   arguments: [
// SPLIT-NEXT:                                       Member {
// SPLIT-NEXT:                                           base: Identifier(
// SPLIT-NEXT:                                               "p",
// SPLIT-NEXT:                                           ),
// SPLIT-NEXT:                                           field: "value",
// SPLIT-NEXT:                                           arrow: true,
// SPLIT-NEXT:                                       },
// SPLIT-NEXT:                                   ],
// SPLIT-NEXT:                               },
// SPLIT-NEXT:                           ),
// SPLIT-NEXT:                       ],
// SPLIT-NEXT:                   ),
// SPLIT-NEXT:                   else_branch: None,
// SPLIT-NEXT:               },
// SPLIT-NEXT:               Return(
// SPLIT-NEXT:                   Identifier(
// SPLIT-NEXT:                       "LAST",
// SPLIT-NEXT:                   ),
// SPLIT-NEXT:               ),
// SPLIT-NEXT:           ],
// SPLIT-NEXT:       },
// SPLIT-NEXT:   )
// SPLIT-NEXT: decl[3]: Declaration(
// SPLIT-NEXT:       Declaration {
// SPLIT-NEXT:           specifiers: DeclarationSpecifiers {
// SPLIT-NEXT:               ty: Integer(
// SPLIT-NEXT:                   Ranked {
// SPLIT-NEXT:                       rank: Int,
// SPLIT-NEXT:                       signed: true,
// SPLIT-NEXT:                   },
// SPLIT-NEXT:               ),
// SPLIT-NEXT:           },
// SPLIT-NEXT:           declarators: [
// SPLIT-NEXT:               InitDeclaratorKind {
// SPLIT-NEXT:                   declarator: Name(
// SPLIT-NEXT:                       "after",
// SPLIT-NEXT:                   ),
// SPLIT-NEXT:               },
// SPLIT-NEXT:           ],
// SPLIT-NEXT:       },
// SPLIT-NEXT:   )
// SLATE-FILECHECK-END SPLIT
// SLATE-FILECHECK-BEGIN MACRO
// MACRO: tag[0]: TagDefinition {
// MACRO-NEXT:       id: TagId(
// MACRO-NEXT:           0,
// MACRO-NEXT:       ),
// MACRO-NEXT:       kind: Struct,
// MACRO-NEXT:       name: Some(
// MACRO-NEXT:           "Payload",
// MACRO-NEXT:       ),
// MACRO-NEXT:       body: Record(
// MACRO-NEXT:           [
// MACRO-NEXT:               Field(
// MACRO-NEXT:                   FieldDecl {
// MACRO-NEXT:                       specifiers: DeclarationSpecifiers {
// MACRO-NEXT:                           ty: Integer(
// MACRO-NEXT:                               Ranked {
// MACRO-NEXT:                                   rank: Int,
// MACRO-NEXT:                                   signed: true,
// MACRO-NEXT:                               },
// MACRO-NEXT:                           ),
// MACRO-NEXT:                       },
// MACRO-NEXT:                       declarators: [
// MACRO-NEXT:                           FieldDeclaratorKind {
// MACRO-NEXT:                               declarator: Name(
// MACRO-NEXT:                                   "value",
// MACRO-NEXT:                               ),
// MACRO-NEXT:                           },
// MACRO-NEXT:                       ],
// MACRO-NEXT:                   },
// MACRO-NEXT:               ),
// MACRO-NEXT:               Field(
// MACRO-NEXT:                   FieldDecl {
// MACRO-NEXT:                       specifiers: DeclarationSpecifiers {
// MACRO-NEXT:                           ty: Integer(
// MACRO-NEXT:                               Ranked {
// MACRO-NEXT:                                   rank: Int,
// MACRO-NEXT:                                   signed: true,
// MACRO-NEXT:                               },
// MACRO-NEXT:                           ),
// MACRO-NEXT:                       },
// MACRO-NEXT:                       declarators: [
// MACRO-NEXT:                           FieldDeclaratorKind {
// MACRO-NEXT:                               declarator: Function {
// MACRO-NEXT:                                   inner: Grouped(
// MACRO-NEXT:                                       Pointer {
// MACRO-NEXT:                                           qualifiers: Qualifiers,
// MACRO-NEXT:                                           inner: Name(
// MACRO-NEXT:                                               "callback",
// MACRO-NEXT:                                           ),
// MACRO-NEXT:                                       },
// MACRO-NEXT:                                   ),
// MACRO-NEXT:                                   parameters: Prototype {
// MACRO-NEXT:                                       parameters: [
// MACRO-NEXT:                                           ParameterDeclarationKind {
// MACRO-NEXT:                                               specifiers: DeclarationSpecifiers {
// MACRO-NEXT:                                                   ty: Integer(
// MACRO-NEXT:                                                       Ranked {
// MACRO-NEXT:                                                           rank: Int,
// MACRO-NEXT:                                                           signed: true,
// MACRO-NEXT:                                                       },
// MACRO-NEXT:                                                   ),
// MACRO-NEXT:                                               },
// MACRO-NEXT:                                               declarator: Abstract,
// MACRO-NEXT:                                           },
// MACRO-NEXT:                                       ],
// MACRO-NEXT:                                   },
// MACRO-NEXT:                               },
// MACRO-NEXT:                           },
// MACRO-NEXT:                       ],
// MACRO-NEXT:                   },
// MACRO-NEXT:               ),
// MACRO-NEXT:           ],
// MACRO-NEXT:       ),
// MACRO-NEXT:   }
// MACRO-NEXT: tag[1]: TagDefinition {
// MACRO-NEXT:       id: TagId(
// MACRO-NEXT:           1,
// MACRO-NEXT:       ),
// MACRO-NEXT:       kind: Enum,
// MACRO-NEXT:       name: Some(
// MACRO-NEXT:           "Mode",
// MACRO-NEXT:       ),
// MACRO-NEXT:       body: Enum {
// MACRO-NEXT:           fixed_type: Some(
// MACRO-NEXT:               TypeName {
// MACRO-NEXT:                   specifiers: DeclarationSpecifiers {
// MACRO-NEXT:                       ty: Integer(
// MACRO-NEXT:                           Ranked {
// MACRO-NEXT:                               rank: Int,
// MACRO-NEXT:                               signed: false,
// MACRO-NEXT:                           },
// MACRO-NEXT:                       ),
// MACRO-NEXT:                   },
// MACRO-NEXT:                   declarator: Abstract,
// MACRO-NEXT:               },
// MACRO-NEXT:           ),
// MACRO-NEXT:           enumerators: [
// MACRO-NEXT:               Enumerator(
// MACRO-NEXT:                   Enumerator {
// MACRO-NEXT:                       name: "FIRST",
// MACRO-NEXT:                       value: Some(
// MACRO-NEXT:                           IntegerLiteral(
// MACRO-NEXT:                               IntegerLiteral {
// MACRO-NEXT:                                   value: 1,
// MACRO-NEXT:                                   radix: Decimal,
// MACRO-NEXT:                                   suffix: IntegerSuffix {
// MACRO-NEXT:                                       unsigned: false,
// MACRO-NEXT:                                       size: None,
// MACRO-NEXT:                                   },
// MACRO-NEXT:                                   spelling: "1",
// MACRO-NEXT:                               },
// MACRO-NEXT:                           ),
// MACRO-NEXT:                       ),
// MACRO-NEXT:                   },
// MACRO-NEXT:               ),
// MACRO-NEXT:               Enumerator(
// MACRO-NEXT:                   Enumerator {
// MACRO-NEXT:                       name: "LAST",
// MACRO-NEXT:                       value: Some(
// MACRO-NEXT:                           Binary {
// MACRO-NEXT:                               op: Add,
// MACRO-NEXT:                               left: Identifier(
// MACRO-NEXT:                                   "FIRST",
// MACRO-NEXT:                               ),
// MACRO-NEXT:                               right: IntegerLiteral(
// MACRO-NEXT:                                   IntegerLiteral {
// MACRO-NEXT:                                       value: 2,
// MACRO-NEXT:                                       radix: Decimal,
// MACRO-NEXT:                                       suffix: IntegerSuffix {
// MACRO-NEXT:                                           unsigned: false,
// MACRO-NEXT:                                           size: None,
// MACRO-NEXT:                                       },
// MACRO-NEXT:                                       spelling: "2",
// MACRO-NEXT:                                   },
// MACRO-NEXT:                               ),
// MACRO-NEXT:                           },
// MACRO-NEXT:                       ),
// MACRO-NEXT:                   },
// MACRO-NEXT:               ),
// MACRO-NEXT:           ],
// MACRO-NEXT:       },
// MACRO-NEXT:   }
// MACRO-NEXT: decl[0]: Declaration(
// MACRO-NEXT:       Declaration {
// MACRO-NEXT:           specifiers: DeclarationSpecifiers {
// MACRO-NEXT:               ty: Tag(
// MACRO-NEXT:                   Definition(
// MACRO-NEXT:                       TagId(
// MACRO-NEXT:                           0,
// MACRO-NEXT:                       ),
// MACRO-NEXT:                   ),
// MACRO-NEXT:               ),
// MACRO-NEXT:               storage: Typedef,
// MACRO-NEXT:           },
// MACRO-NEXT:           declarators: [
// MACRO-NEXT:               InitDeclaratorKind {
// MACRO-NEXT:                   declarator: Name(
// MACRO-NEXT:                       "Payload",
// MACRO-NEXT:                   ),
// MACRO-NEXT:               },
// MACRO-NEXT:           ],
// MACRO-NEXT:       },
// MACRO-NEXT:   )
// MACRO-NEXT: decl[1]: Declaration(
// MACRO-NEXT:       Declaration {
// MACRO-NEXT:           specifiers: DeclarationSpecifiers {
// MACRO-NEXT:               ty: Tag(
// MACRO-NEXT:                   Definition(
// MACRO-NEXT:                       TagId(
// MACRO-NEXT:                           1,
// MACRO-NEXT:                       ),
// MACRO-NEXT:                   ),
// MACRO-NEXT:               ),
// MACRO-NEXT:           },
// MACRO-NEXT:       },
// MACRO-NEXT:   )
// MACRO-NEXT: decl[2]: Function(
// MACRO-NEXT:       FunctionDefinition {
// MACRO-NEXT:           specifiers: DeclarationSpecifiers {
// MACRO-NEXT:               ty: Integer(
// MACRO-NEXT:                   Ranked {
// MACRO-NEXT:                       rank: Int,
// MACRO-NEXT:                       signed: true,
// MACRO-NEXT:                   },
// MACRO-NEXT:               ),
// MACRO-NEXT:           },
// MACRO-NEXT:           declarator: Function {
// MACRO-NEXT:               inner: Name(
// MACRO-NEXT:                   "read_value",
// MACRO-NEXT:               ),
// MACRO-NEXT:               parameters: Prototype {
// MACRO-NEXT:                   parameters: [
// MACRO-NEXT:                       ParameterDeclarationKind {
// MACRO-NEXT:                           specifiers: DeclarationSpecifiers {
// MACRO-NEXT:                               ty: Named(
// MACRO-NEXT:                                   "Payload",
// MACRO-NEXT:                               ),
// MACRO-NEXT:                           },
// MACRO-NEXT:                           declarator: Pointer {
// MACRO-NEXT:                               qualifiers: Qualifiers,
// MACRO-NEXT:                               inner: Name(
// MACRO-NEXT:                                   "p",
// MACRO-NEXT:                               ),
// MACRO-NEXT:                           },
// MACRO-NEXT:                       },
// MACRO-NEXT:                   ],
// MACRO-NEXT:               },
// MACRO-NEXT:           },
// MACRO-NEXT:           body: [
// MACRO-NEXT:               If {
// MACRO-NEXT:                   condition: Identifier(
// MACRO-NEXT:                       "p",
// MACRO-NEXT:                   ),
// MACRO-NEXT:                   then_branch: Block(
// MACRO-NEXT:                       [
// MACRO-NEXT:                           Return(
// MACRO-NEXT:                               Call {
// MACRO-NEXT:                                   callee: Member {
// MACRO-NEXT:                                       base: Identifier(
// MACRO-NEXT:                                           "p",
// MACRO-NEXT:                                       ),
// MACRO-NEXT:                                       field: "callback",
// MACRO-NEXT:                                       arrow: true,
// MACRO-NEXT:                                   },
// MACRO-NEXT:                                   arguments: [
// MACRO-NEXT:                                       Member {
// MACRO-NEXT:                                           base: Identifier(
// MACRO-NEXT:                                               "p",
// MACRO-NEXT:                                           ),
// MACRO-NEXT:                                           field: "value",
// MACRO-NEXT:                                           arrow: true,
// MACRO-NEXT:                                       },
// MACRO-NEXT:                                   ],
// MACRO-NEXT:                               },
// MACRO-NEXT:                           ),
// MACRO-NEXT:                       ],
// MACRO-NEXT:                   ),
// MACRO-NEXT:                   else_branch: None,
// MACRO-NEXT:               },
// MACRO-NEXT:               Return(
// MACRO-NEXT:                   Identifier(
// MACRO-NEXT:                       "LAST",
// MACRO-NEXT:                   ),
// MACRO-NEXT:               ),
// MACRO-NEXT:           ],
// MACRO-NEXT:       },
// MACRO-NEXT:   )
// MACRO-NEXT: decl[3]: Declaration(
// MACRO-NEXT:       Declaration {
// MACRO-NEXT:           specifiers: DeclarationSpecifiers {
// MACRO-NEXT:               ty: Integer(
// MACRO-NEXT:                   Ranked {
// MACRO-NEXT:                       rank: Int,
// MACRO-NEXT:                       signed: true,
// MACRO-NEXT:                   },
// MACRO-NEXT:               ),
// MACRO-NEXT:           },
// MACRO-NEXT:           declarators: [
// MACRO-NEXT:               InitDeclaratorKind {
// MACRO-NEXT:                   declarator: Name(
// MACRO-NEXT:                       "after",
// MACRO-NEXT:                   ),
// MACRO-NEXT:               },
// MACRO-NEXT:           ],
// MACRO-NEXT:       },
// MACRO-NEXT:   )
// SLATE-FILECHECK-END MACRO
