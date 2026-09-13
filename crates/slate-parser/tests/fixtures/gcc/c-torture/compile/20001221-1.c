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
// DEFAULT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "foo",
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
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       8,
// DEFAULT-NEXT:                                                   ),
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
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "nibbles",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "nibbles",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
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
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "nibbles",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "buf1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Identifier(
// DEFAULT-NEXT:                                           "nibbles",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "buf2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Identifier(
// DEFAULT-NEXT:                                           "nibbles",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "buf3",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Identifier(
// DEFAULT-NEXT:                                           "nibbles",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Ternary {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: LessEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "nibbles",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           16,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_value: StatementExpression(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "buf1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Union,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LBrace,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Short,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__usi",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__uc",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           RBrace,
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           CharLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Switch,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "nibbles",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           LBrace,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "16",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0x01010101",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__extension__",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Plus,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "12",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0x01010101",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__extension__",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Plus,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           RBrace,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   else_value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Ternary {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: LessEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "nibbles",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           16,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_value: StatementExpression(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "buf2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Union,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LBrace,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Short,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__usi",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__uc",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           RBrace,
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           CharLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Switch,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Unsigned,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "nibbles",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           LBrace,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "16",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0x01010101",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__extension__",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Plus,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "12",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0x01010101",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__extension__",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Plus,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "8",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0x01010101",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__extension__",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Plus,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__u",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__ui",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0x01010101",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           RBrace,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__s",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   else_value: Integer(
// DEFAULT-NEXT:                                       0,
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
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
