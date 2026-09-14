// SLATE-FILECHECK-DEFINES DEFAULT

// Contributed by Alexandre Oliva <aoliva@redhat.com>
// From Red Hat case 106165.

/* { dg-require-effective-target indirect_calls } */

typedef struct s1
{
  unsigned short v1;
  unsigned char *v2;
} S1;

extern void bar(const struct s1 *const hdb);
extern unsigned char* foo ();

unsigned int sn;
S1 *hdb;
S1 *pb;
unsigned short len;

unsigned int crashIt()
{
  unsigned char *p;
  unsigned int nsn;
  unsigned short cnt;

  if (sn != 0) return 1;

  if ((len < 12) || ((p = (((pb->v1) >= 8) ? pb->v2 : foo() )) == 0))
    return 1;

  nsn = (
	 (((*(unsigned int*)p) & 0x000000ff) << 24) |
	 (((*(unsigned int*)p) & 0x0000ff00) << 8)  |
	 (((*(unsigned int*)p) & 0x00ff0000) >> 8)  |
	 (((*(unsigned int*)p) & 0xff000000) >> 24)  );
  p += 4;

  cnt = (unsigned short) ((
			   (((*(unsigned int*)p) & 0x000000ff) << 24) |
			   (((*(unsigned int*)p) & 0x0000ff00) << 8)  |
			   (((*(unsigned int*)p) & 0x00ff0000) >> 8)  |
			   (((*(unsigned int*)p) & 0xff000000) >> 24)  ) &
			  0xffff);

  if ((len != 12 + (cnt * 56)) || (nsn == 0))
    {
      bar(hdb);
      return 1;
    }

  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[0]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           0,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "s1",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Short,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "v1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 8,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: Some(
// DEFAULT-NEXT:                                       false,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "v2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 9,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "S1",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "bar",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Tag(
// DEFAULT-NEXT:                                           Reference {
// DEFAULT-NEXT:                                               kind: Struct,
// DEFAULT-NEXT:                                               name: "s1",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_const: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_const: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "hdb",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 12,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: Some(
// DEFAULT-NEXT:                           false,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "foo",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: Empty,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 13,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "sn",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 15,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "S1",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "hdb",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 16,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "S1",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "pb",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Short,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "len",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 18,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "crashIt",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: Some(
// DEFAULT-NEXT:                                       false,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "p",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "nsn",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Short,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "cnt",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Binary {
// DEFAULT-NEXT:                       op: NotEqual,
// DEFAULT-NEXT:                       left: Identifier(
// DEFAULT-NEXT:                           "sn",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       right: IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 0,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "0",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Binary {
// DEFAULT-NEXT:                       op: Or,
// DEFAULT-NEXT:                       left: Paren(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Less,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "len",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 12,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "12",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       right: Paren(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Paren(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "p",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Conditional {
// DEFAULT-NEXT:                                               condition: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "pb",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "v1",
// DEFAULT-NEXT:                                                               arrow: true,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 8,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "8",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               then_value: Some(
// DEFAULT-NEXT:                                                   Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "pb",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "v2",
// DEFAULT-NEXT:                                                       arrow: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               else_value: Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "foo",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "nsn",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Paren(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: BitOr,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: BitOr,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: BitOr,
// DEFAULT-NEXT:                                       left: Paren(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: ShiftLeft,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: BitAnd,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Unary {
// DEFAULT-NEXT:                                                               op: Deref,
// DEFAULT-NEXT:                                                               operand: Cast {
// DEFAULT-NEXT:                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Ranked {
// DEFAULT-NEXT:                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                   signed: false,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarator: Pointer {
// DEFAULT-NEXT:                                                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                           inner: Abstract,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                       "p",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 255,
// DEFAULT-NEXT:                                                               radix: Hex,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0x000000ff",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 24,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "24",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Paren(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: ShiftLeft,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: BitAnd,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Unary {
// DEFAULT-NEXT:                                                               op: Deref,
// DEFAULT-NEXT:                                                               operand: Cast {
// DEFAULT-NEXT:                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Ranked {
// DEFAULT-NEXT:                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                   signed: false,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarator: Pointer {
// DEFAULT-NEXT:                                                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                           inner: Abstract,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                       "p",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 65280,
// DEFAULT-NEXT:                                                               radix: Hex,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0x0000ff00",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 8,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "8",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: ShiftRight,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: BitAnd,
// DEFAULT-NEXT:                                                   left: Paren(
// DEFAULT-NEXT:                                                       Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Cast {
// DEFAULT-NEXT:                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                           Ranked {
// DEFAULT-NEXT:                                                                               rank: Int,
// DEFAULT-NEXT:                                                                               signed: false,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               value: Identifier(
// DEFAULT-NEXT:                                                                   "p",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 16711680,
// DEFAULT-NEXT:                                                           radix: Hex,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0x00ff0000",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 8,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "8",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Paren(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: ShiftRight,
// DEFAULT-NEXT:                                       left: Paren(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: BitAnd,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Unary {
// DEFAULT-NEXT:                                                       op: Deref,
// DEFAULT-NEXT:                                                       operand: Cast {
// DEFAULT-NEXT:                                                           ty: TypeName {
// DEFAULT-NEXT:                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                       Ranked {
// DEFAULT-NEXT:                                                                           rank: Int,
// DEFAULT-NEXT:                                                                           signed: false,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               declarator: Pointer {
// DEFAULT-NEXT:                                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                   inner: Abstract,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           value: Identifier(
// DEFAULT-NEXT:                                                               "p",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 4278190080,
// DEFAULT-NEXT:                                                       radix: Hex,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "0xff000000",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 24,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "24",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "p",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 4,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "4",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "cnt",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Cast {
// DEFAULT-NEXT:                           ty: TypeName {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Short,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: BitAnd,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: BitOr,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: BitOr,
// DEFAULT-NEXT:                                                   left: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: BitAnd,
// DEFAULT-NEXT:                                                                   left: Paren(
// DEFAULT-NEXT:                                                                       Unary {
// DEFAULT-NEXT:                                                                           op: Deref,
// DEFAULT-NEXT:                                                                           operand: Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Ranked {
// DEFAULT-NEXT:                                                                                               rank: Int,
// DEFAULT-NEXT:                                                                                               signed: false,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "p",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 255,
// DEFAULT-NEXT:                                                                           radix: Hex,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "0x000000ff",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 24,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "24",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: BitAnd,
// DEFAULT-NEXT:                                                                   left: Paren(
// DEFAULT-NEXT:                                                                       Unary {
// DEFAULT-NEXT:                                                                           op: Deref,
// DEFAULT-NEXT:                                                                           operand: Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Ranked {
// DEFAULT-NEXT:                                                                                               rank: Int,
// DEFAULT-NEXT:                                                                                               signed: false,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "p",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 65280,
// DEFAULT-NEXT:                                                                           radix: Hex,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "0x0000ff00",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 8,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "8",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: ShiftRight,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: BitAnd,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   Unary {
// DEFAULT-NEXT:                                                                       op: Deref,
// DEFAULT-NEXT:                                                                       operand: Cast {
// DEFAULT-NEXT:                                                                           ty: TypeName {
// DEFAULT-NEXT:                                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                                       Ranked {
// DEFAULT-NEXT:                                                                                           rank: Int,
// DEFAULT-NEXT:                                                                                           signed: false,
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               declarator: Pointer {
// DEFAULT-NEXT:                                                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                   inner: Abstract,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           value: Identifier(
// DEFAULT-NEXT:                                                                               "p",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 16711680,
// DEFAULT-NEXT:                                                                       radix: Hex,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "0x00ff0000",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 8,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "8",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                   left: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: BitAnd,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Unary {
// DEFAULT-NEXT:                                                                   op: Deref,
// DEFAULT-NEXT:                                                                   operand: Cast {
// DEFAULT-NEXT:                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                       rank: Int,
// DEFAULT-NEXT:                                                                                       signed: false,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           declarator: Pointer {
// DEFAULT-NEXT:                                                                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                               inner: Abstract,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                           "p",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 4278190080,
// DEFAULT-NEXT:                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0xff000000",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 24,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "24",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 65535,
// DEFAULT-NEXT:                                           radix: Hex,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0xffff",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Binary {
// DEFAULT-NEXT:                       op: Or,
// DEFAULT-NEXT:                       left: Paren(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "len",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 12,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "12",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "cnt",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 56,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "56",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       right: Paren(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "nsn",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "bar",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "hdb",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
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
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 20,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
