/* { dg-options "-Wno-psabi" } */
extern void abort(void);

typedef int          V2SI __attribute__((vector_size(8)));
typedef unsigned int V2USI __attribute__((vector_size(8)));
typedef short        V2HI __attribute__((vector_size(4)));
typedef unsigned int V2UHI __attribute__((vector_size(4)));

V2USI
test1(V2SI x) { return (V2USI)(V2SI)(long long)x; }

long long test2(V2SI x) { return (long long)(V2USI)(V2SI)(long long)x; }

int main(void) {
  if (sizeof(short) != 2 || sizeof(int) != 4 || sizeof(long long) != 8)
    return 0;

  union {
    V2SI      x;
    int       y[2];
    V2USI     z;
    long long l;
  } u;
  V2SI a = {-3, -3};
  u.z    = test1(a);
  if (u.y[0] != -3 || u.y[1] != -3)
    abort();

  u.l = test2(a);
  if (u.y[0] != -3 || u.y[1] != -3)
    abort();
  return 0;
}


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* { dg-options \"-Wno-psabi\" } */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 33,
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
// DEFAULT-NEXT: decl[1]: Declaration {
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
// DEFAULT-NEXT:           line: 1,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "V2SI",
// DEFAULT-NEXT:       ty: Vector(
// DEFAULT-NEXT:           VectorType {
// DEFAULT-NEXT:               element: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Bytes(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       8,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:       attributes: [
// DEFAULT-NEXT:           VectorSize(
// DEFAULT-NEXT:               Integer(
// DEFAULT-NEXT:                   8,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ],
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Typedef {
// DEFAULT-NEXT:       name: "V2USI",
// DEFAULT-NEXT:       ty: Vector(
// DEFAULT-NEXT:           VectorType {
// DEFAULT-NEXT:               element: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Bytes(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       8,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       attributes: [
// DEFAULT-NEXT:           VectorSize(
// DEFAULT-NEXT:               Integer(
// DEFAULT-NEXT:                   8,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ],
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Typedef {
// DEFAULT-NEXT:       name: "V2HI",
// DEFAULT-NEXT:       ty: Vector(
// DEFAULT-NEXT:           VectorType {
// DEFAULT-NEXT:               element: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Short,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Bytes(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       4,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       attributes: [
// DEFAULT-NEXT:           VectorSize(
// DEFAULT-NEXT:               Integer(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ],
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Typedef {
// DEFAULT-NEXT:       name: "V2UHI",
// DEFAULT-NEXT:       ty: Vector(
// DEFAULT-NEXT:           VectorType {
// DEFAULT-NEXT:               element: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Bytes(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       4,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       attributes: [
// DEFAULT-NEXT:           VectorSize(
// DEFAULT-NEXT:               Integer(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ],
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Named(
// DEFAULT-NEXT:               "V2USI",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "test1",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "V2SI",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Cast {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "V2USI",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           declarator: Abstract,
// DEFAULT-NEXT:                           value: Cast {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "V2SI",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: LongLong,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 8,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: LongLong,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "test2",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "V2SI",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Cast {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           declarator: Abstract,
// DEFAULT-NEXT:                           value: Cast {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "V2USI",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Cast {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "V2SI",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Cast {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: LongLong,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "x",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 11,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[8]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: NotEqual,
// DEFAULT-NEXT:                                   left: SizeOfType {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Short,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       2,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: NotEqual,
// DEFAULT-NEXT:                                   left: SizeOfType {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       4,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: SizeOfType {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: LongLong,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Union,
// DEFAULT-NEXT:                               name: None,
// DEFAULT-NEXT:                               body: Some(
// DEFAULT-NEXT:                                   Fields(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               declaration: Declaration {
// DEFAULT-NEXT:                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "V2SI",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               declaration: Declaration {
// DEFAULT-NEXT:                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: Int,
// DEFAULT-NEXT:                                                               signed: true,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   declarator: Array {
// DEFAULT-NEXT:                                                       inner: Name(
// DEFAULT-NEXT:                                                           "y",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       size: Expression(
// DEFAULT-NEXT:                                                           IntLit(
// DEFAULT-NEXT:                                                               2,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               declaration: Declaration {
// DEFAULT-NEXT:                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "V2USI",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "z",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               declaration: Declaration {
// DEFAULT-NEXT:                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: LongLong,
// DEFAULT-NEXT:                                                               signed: true,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "l",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "u",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "V2SI",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "a",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       3,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       3,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "u",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "z",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "test1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Index {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "u",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "y",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Index {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "u",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "y",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "u",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "l",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "test2",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Index {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "u",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "y",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Index {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "u",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "y",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:               line: 13,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
