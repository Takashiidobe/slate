// SLATE-FILECHECK-DEFINES DEFAULT

static void
foo ()
{
  long maplength;
  int type;
  {
    const long nibbles = 8;
    char buf1[nibbles + 1];
    char buf2[nibbles + 1];
    char buf3[nibbles + 1];
    buf1[nibbles] = '\0';
    buf2[nibbles] = '\0';
    buf3[nibbles] = '\0';
    ((nibbles) <= 16
     ? (({
       void *__s = (buf1);
       union
	 {
	   unsigned int __ui;
	   unsigned short int __usi;
	   unsigned char __uc;
	 }
       *__u = __s;
       unsigned char __c = (unsigned char)('0');
       switch ((unsigned int) (nibbles))
	 {
	  case 16:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 12:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 0:
	   break;
	 }
       __s;
     }))
     : 0);
    ((nibbles) <= 16
     ? (({
       void *__s = (buf2);
       union
	 {
	   unsigned int __ui;
	   unsigned short int __usi;
	   unsigned char __uc;
	 }
       *__u = __s;
       unsigned char __c = (unsigned char)('0');
       switch ((unsigned int) (nibbles))
	 {
	  case 16:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 12:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 8:
	   __u->__ui = __c * 0x01010101; 
	   __u = __extension__ ((void *) __u + 4);
	  case 4:
	   __u->__ui = __c * 0x01010101;
	  case 0:
	   break;
	 }
       __s;
     }))
     : 0);
  }
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[1]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           1,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Union,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__ui",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 19,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                                   "__usi",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 20,
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
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__uc",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 21,
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
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[3]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           3,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Union,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__ui",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 44,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                                   "__usi",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 45,
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
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__uc",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 46,
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
// DEFAULT-NEXT:           line: 42,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "foo",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "maplength",
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
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Long,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "nibbles",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               IntegerLiteral(
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
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Array {
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "buf1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           size: Expression(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "nibbles",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 1,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Array {
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "buf2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           size: Expression(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "nibbles",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 1,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Array {
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "buf3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           size: Expression(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "nibbles",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 1,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "buf1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: Identifier(
// DEFAULT-NEXT:                                       "nibbles",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: CharLiteral(
// DEFAULT-NEXT:                                   CharLiteral {
// DEFAULT-NEXT:                                       encoding: Plain,
// DEFAULT-NEXT:                                       code_units: [
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       spelling: "\\0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "buf2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: Identifier(
// DEFAULT-NEXT:                                       "nibbles",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: CharLiteral(
// DEFAULT-NEXT:                                   CharLiteral {
// DEFAULT-NEXT:                                       encoding: Plain,
// DEFAULT-NEXT:                                       code_units: [
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       spelling: "\\0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "buf3",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: Identifier(
// DEFAULT-NEXT:                                       "nibbles",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: CharLiteral(
// DEFAULT-NEXT:                                   CharLiteral {
// DEFAULT-NEXT:                                       encoding: Plain,
// DEFAULT-NEXT:                                       code_units: [
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       spelling: "\\0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: LessEqual,
// DEFAULT-NEXT:                                       left: Paren(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "nibbles",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 16,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "16",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_value: Some(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           StatementExpression(
// DEFAULT-NEXT:                                               [
// DEFAULT-NEXT:                                                   Decl(
// DEFAULT-NEXT:                                                       Declaration {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Void,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               InitDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                       inner: Name(
// DEFAULT-NEXT:                                                                           "__s",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   initializer: Some(
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Paren(
// DEFAULT-NEXT:                                                                               Identifier(
// DEFAULT-NEXT:                                                                                   "buf1",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Decl(
// DEFAULT-NEXT:                                                       Declaration {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Tag(
// DEFAULT-NEXT:                                                                   Definition(
// DEFAULT-NEXT:                                                                       TagId(
// DEFAULT-NEXT:                                                                           1,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               InitDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                       inner: Name(
// DEFAULT-NEXT:                                                                           "__u",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   initializer: Some(
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Identifier(
// DEFAULT-NEXT:                                                                               "__s",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Decl(
// DEFAULT-NEXT:                                                       Declaration {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               InitDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                       "__c",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   initializer: Some(
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: Some(
// DEFAULT-NEXT:                                                                                                   false,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Paren(
// DEFAULT-NEXT:                                                                                   CharLiteral(
// DEFAULT-NEXT:                                                                                       CharLiteral {
// DEFAULT-NEXT:                                                                                           encoding: Plain,
// DEFAULT-NEXT:                                                                                           code_units: [
// DEFAULT-NEXT:                                                                                               48,
// DEFAULT-NEXT:                                                                                           ],
// DEFAULT-NEXT:                                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Switch {
// DEFAULT-NEXT:                                                       discriminant: Cast {
// DEFAULT-NEXT:                                                           ty: TypeName {
// DEFAULT-NEXT:                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                       Ranked {
// DEFAULT-NEXT:                                                                           rank: Int,
// DEFAULT-NEXT:                                                                           signed: false,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "nibbles",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       body: [
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 16,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "16",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Expr(
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "__u",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "__ui",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Binary {
// DEFAULT-NEXT:                                                                           op: Mul,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "__c",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 16843009,
// DEFAULT-NEXT:                                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0x01010101",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "__u",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Void,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "__u",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 4,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "4",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 12,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "12",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Expr(
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "__u",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "__ui",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Binary {
// DEFAULT-NEXT:                                                                           op: Mul,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "__c",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 16843009,
// DEFAULT-NEXT:                                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0x01010101",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "__u",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Void,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "__u",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 4,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "4",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 0,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Break,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "__s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   else_value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Conditional {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: LessEqual,
// DEFAULT-NEXT:                                       left: Paren(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "nibbles",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 16,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "16",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_value: Some(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           StatementExpression(
// DEFAULT-NEXT:                                               [
// DEFAULT-NEXT:                                                   Decl(
// DEFAULT-NEXT:                                                       Declaration {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Void,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               InitDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                       inner: Name(
// DEFAULT-NEXT:                                                                           "__s",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   initializer: Some(
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Paren(
// DEFAULT-NEXT:                                                                               Identifier(
// DEFAULT-NEXT:                                                                                   "buf2",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Decl(
// DEFAULT-NEXT:                                                       Declaration {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Tag(
// DEFAULT-NEXT:                                                                   Definition(
// DEFAULT-NEXT:                                                                       TagId(
// DEFAULT-NEXT:                                                                           3,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               InitDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                       inner: Name(
// DEFAULT-NEXT:                                                                           "__u",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   initializer: Some(
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Identifier(
// DEFAULT-NEXT:                                                                               "__s",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Decl(
// DEFAULT-NEXT:                                                       Declaration {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               InitDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                       "__c",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   initializer: Some(
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: Some(
// DEFAULT-NEXT:                                                                                                   false,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Paren(
// DEFAULT-NEXT:                                                                                   CharLiteral(
// DEFAULT-NEXT:                                                                                       CharLiteral {
// DEFAULT-NEXT:                                                                                           encoding: Plain,
// DEFAULT-NEXT:                                                                                           code_units: [
// DEFAULT-NEXT:                                                                                               48,
// DEFAULT-NEXT:                                                                                           ],
// DEFAULT-NEXT:                                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Switch {
// DEFAULT-NEXT:                                                       discriminant: Cast {
// DEFAULT-NEXT:                                                           ty: TypeName {
// DEFAULT-NEXT:                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                       Ranked {
// DEFAULT-NEXT:                                                                           rank: Int,
// DEFAULT-NEXT:                                                                           signed: false,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "nibbles",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       body: [
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 16,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "16",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Expr(
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "__u",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "__ui",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Binary {
// DEFAULT-NEXT:                                                                           op: Mul,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "__c",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 16843009,
// DEFAULT-NEXT:                                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0x01010101",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "__u",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Void,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "__u",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 4,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "4",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 12,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "12",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Expr(
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "__u",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "__ui",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Binary {
// DEFAULT-NEXT:                                                                           op: Mul,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "__c",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 16843009,
// DEFAULT-NEXT:                                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0x01010101",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "__u",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Void,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "__u",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 4,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "4",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 8,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Expr(
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "__u",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "__ui",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Binary {
// DEFAULT-NEXT:                                                                           op: Mul,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "__c",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 16843009,
// DEFAULT-NEXT:                                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0x01010101",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "__u",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Void,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Abstract,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "__u",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 4,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "4",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 4,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "4",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Expr(
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "__u",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "__ui",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Binary {
// DEFAULT-NEXT:                                                                           op: Mul,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "__c",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 16843009,
// DEFAULT-NEXT:                                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0x01010101",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           SwitchLabel {
// DEFAULT-NEXT:                                                               label: Case(
// DEFAULT-NEXT:                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 0,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Break,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "__s",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   else_value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 1,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
