/* PR tree-optimization/51315 */
/* Reported by Jurij Smakov <jurij@wooyd.org> */

typedef __SIZE_TYPE__ size_t;

extern void *memcpy(void *__restrict __dest, __const void *__restrict __src,
                    size_t __n) __attribute__((__nothrow__))
__attribute__((__nonnull__(1, 2)));

extern size_t strlen(__const char *__s) __attribute__((__nothrow__))
__attribute__((__pure__)) __attribute__((__nonnull__(1)));

typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;

extern void abort(void);

int a;

static void __attribute__((noinline, noclone)) do_something(int item) {
  a = item;
}

int pack_unpack(char *s, char *p) {
  char *send, *pend;
  char  type;
  int   integer_size;

  send = s + strlen(s);
  pend = p + strlen(p);

  while (p < pend) {
    type = *p++;

    switch (type) {
    case 's':
      integer_size = 2;
      goto unpack_integer;

    case 'l':
      integer_size = 4;
      goto unpack_integer;

    unpack_integer:
      switch (integer_size) {
      case 2: {
        union {
          int16_t i;
          char    a[sizeof(int16_t)];
        } v;
        memcpy(v.a, s, sizeof(int16_t));
        s += sizeof(int16_t);
        do_something(v.i);
      } break;

      case 4: {
        union {
          int32_t i;
          char    a[sizeof(int32_t)];
        } v;
        memcpy(v.a, s, sizeof(int32_t));
        s += sizeof(int32_t);
        do_something(v.i);
      } break;
      }
      break;
    }
  }
  return (int)*s;
}

int main(void) {
  int n = pack_unpack("\200\001\377\376\035\300", "sl");
  if (n != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR tree-optimization/51315 */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 32,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Comment {
// DEFAULT-NEXT:       text: "/* Reported by Jurij Smakov <jurij@wooyd.org> */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 33,
// DEFAULT-NEXT:           length: 48,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 1,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "size_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Long,
// DEFAULT-NEXT:               signed: false,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "memcpy",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Void,
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers {
// DEFAULT-NEXT:                                   is_restrict: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "__dest",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Qualified {
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers {
// DEFAULT-NEXT:                                   is_restrict: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "__src",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Named(
// DEFAULT-NEXT:                           "size_t",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Name(
// DEFAULT-NEXT:                               "__n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Unknown {
// DEFAULT-NEXT:                   name: "__nothrow__",
// DEFAULT-NEXT:                   arguments: [],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               NonNull(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       1,
// DEFAULT-NEXT:                       2,
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "size_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "strlen",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Qualified {
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "__s",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Unknown {
// DEFAULT-NEXT:                   name: "__nothrow__",
// DEFAULT-NEXT:                   arguments: [],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Pure,
// DEFAULT-NEXT:               NonNull(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       1,
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Typedef {
// DEFAULT-NEXT:       name: "int16_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Short,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 12,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Typedef {
// DEFAULT-NEXT:       name: "int32_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 13,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "abort",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "a",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT: decl[9]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "do_something",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "item",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Identifier(
// DEFAULT-NEXT:                               "item",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 19,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoInline,
// DEFAULT-NEXT:               NoClone,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[10]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "pack_unpack",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Char {
// DEFAULT-NEXT:                           signed: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "s",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Char {
// DEFAULT-NEXT:                           signed: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "p",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "send",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "pend",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "type",
// DEFAULT-NEXT:                       ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "integer_size",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "send",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "s",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "s",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "pend",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strlen",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "p",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               While {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "p",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "pend",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "type",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Deref(
// DEFAULT-NEXT:                                       PostIncrement(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "p",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Switch {
// DEFAULT-NEXT:                           discriminant: Const(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               Case(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           115,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "integer_size",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Goto(
// DEFAULT-NEXT:                                   "unpack_integer",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Case(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           108,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "integer_size",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Goto(
// DEFAULT-NEXT:                                   "unpack_integer",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Labeled(
// DEFAULT-NEXT:                                   "unpack_integer",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Switch {
// DEFAULT-NEXT:                                   discriminant: Const(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "integer_size",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       Case(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Block(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               Decl(
// DEFAULT-NEXT:                                                   Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Tagged {
// DEFAULT-NEXT:                                                               kind: Union,
// DEFAULT-NEXT:                                                               name: None,
// DEFAULT-NEXT:                                                               body: Some(
// DEFAULT-NEXT:                                                                   Fields(
// DEFAULT-NEXT:                                                                       [
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Named(
// DEFAULT-NEXT:                                                                                           "int16_t",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                                       "i",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: None,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Array {
// DEFAULT-NEXT:                                                                                       inner: Name(
// DEFAULT-NEXT:                                                                                           "a",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       size: Expression(
// DEFAULT-NEXT:                                                                                           Const(
// DEFAULT-NEXT:                                                                                               SizeOf(
// DEFAULT-NEXT:                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                       "int16_t",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "v",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Call {
// DEFAULT-NEXT:                                                           callee: Identifier(
// DEFAULT-NEXT:                                                               "memcpy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           arguments: [
// DEFAULT-NEXT:                                                               Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "v",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "a",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "s",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               SizeOfType {
// DEFAULT-NEXT:                                                                   ty: Named(
// DEFAULT-NEXT:                                                                       "int16_t",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: AddAssign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "s",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: SizeOfType {
// DEFAULT-NEXT:                                                               ty: Named(
// DEFAULT-NEXT:                                                                   "int16_t",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Call {
// DEFAULT-NEXT:                                                           callee: Identifier(
// DEFAULT-NEXT:                                                               "do_something",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           arguments: [
// DEFAULT-NEXT:                                                               Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "v",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "i",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Break,
// DEFAULT-NEXT:                                       Case(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Block(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               Decl(
// DEFAULT-NEXT:                                                   Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Tagged {
// DEFAULT-NEXT:                                                               kind: Union,
// DEFAULT-NEXT:                                                               name: None,
// DEFAULT-NEXT:                                                               body: Some(
// DEFAULT-NEXT:                                                                   Fields(
// DEFAULT-NEXT:                                                                       [
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Named(
// DEFAULT-NEXT:                                                                                           "int32_t",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                                       "i",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: None,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Array {
// DEFAULT-NEXT:                                                                                       inner: Name(
// DEFAULT-NEXT:                                                                                           "a",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       size: Expression(
// DEFAULT-NEXT:                                                                                           Const(
// DEFAULT-NEXT:                                                                                               SizeOf(
// DEFAULT-NEXT:                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                       "int32_t",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "v",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Call {
// DEFAULT-NEXT:                                                           callee: Identifier(
// DEFAULT-NEXT:                                                               "memcpy",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           arguments: [
// DEFAULT-NEXT:                                                               Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "v",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "a",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "s",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               SizeOfType {
// DEFAULT-NEXT:                                                                   ty: Named(
// DEFAULT-NEXT:                                                                       "int32_t",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: AddAssign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "s",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: SizeOfType {
// DEFAULT-NEXT:                                                               ty: Named(
// DEFAULT-NEXT:                                                                   "int32_t",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Call {
// DEFAULT-NEXT:                                                           callee: Identifier(
// DEFAULT-NEXT:                                                               "do_something",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           arguments: [
// DEFAULT-NEXT:                                                               Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "v",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "i",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Break,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Break,
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Cast {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           declarator: Abstract,
// DEFAULT-NEXT:                           value: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "s",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 23,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[11]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "n",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "pack_unpack",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "\\200\\001\\377\\376\\035\\300",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "sl",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 71,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
