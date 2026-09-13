extern void exit(int);
extern void abort(void);

typedef union {
  struct {
    int      f1, f2, f3, f4, f5, f6, f7, f8;
    long int f9, f10;
    int      f11;
  } f;
  char     s[56];
  long int a;
} T;

__attribute__((noinline)) void test(T *t) {
  static int i = 11;
  if (t->f.f1 != i++)
    abort();
  if (t->f.f2 || t->f.f3 || t->f.f4 || t->f.f5 || t->f.f6 || t->f.f7 ||
      t->f.f8 || t->f.f9 || t->f.f10 || t->f.f11)
    abort();
  if (i == 20)
    exit(0);
}

__attribute__((noinline)) void foo(int i) {
  T t;
again:
  t = (T){{++i, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  test(&t);
  goto again;
}

int main(void) {
  T  *t1, *t2;
  int cnt = 0;
  t1      = (T *)0;
loop:
  t2 = t1;
  t1 = &(T){.f.f9 = cnt++};
  if (cnt < 3)
    goto loop;
  if (t1 != t2 || t1->f.f9 != 2)
    abort();
  foo(10);
  return 0;
}


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "exit",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT: decl[2]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Union,
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: None,
// DEFAULT-NEXT:                                   body: Some(
// DEFAULT-NEXT:                                       Fields(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f3",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f4",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f5",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f6",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f7",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f8",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Long,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f9",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Long,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f10",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "f11",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "f",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 4,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Array {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "s",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               size: Expression(
// DEFAULT-NEXT:                                   IntLit(
// DEFAULT-NEXT:                                       56,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 10,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Typedef {
// DEFAULT-NEXT:       name: "T",
// DEFAULT-NEXT:       ty: Tagged {
// DEFAULT-NEXT:           kind: Union,
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "test",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "T",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                           storage: Static,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       11,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "f",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "f1",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Or,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Or,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Or,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Or,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Or,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Or,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Or,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Or,
// DEFAULT-NEXT:                                                           left: Member {
// DEFAULT-NEXT:                                                               base: Arrow {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "t",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "f",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "f2",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Member {
// DEFAULT-NEXT:                                                               base: Arrow {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "t",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "f",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "f3",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Member {
// DEFAULT-NEXT:                                                           base: Arrow {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "t",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "f",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           field: "f4",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Member {
// DEFAULT-NEXT:                                                       base: Arrow {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "f",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       field: "f5",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Member {
// DEFAULT-NEXT:                                                   base: Arrow {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "t",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "f",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   field: "f6",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Member {
// DEFAULT-NEXT:                                               base: Arrow {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "t",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "f",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               field: "f7",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Member {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "f",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           field: "f8",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Member {
// DEFAULT-NEXT:                                       base: Arrow {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "t",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "f",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       field: "f9",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Member {
// DEFAULT-NEXT:                                   base: Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "t",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "f",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "f10",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Member {
// DEFAULT-NEXT:                               base: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "f",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "f11",
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
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               20,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "exit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 13,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoInline,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "foo",
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
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "T",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "t",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Labeled(
// DEFAULT-NEXT:                   "again",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: CompoundLiteral {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "T",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               initializer: [
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: List(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           PreIncrement(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "i",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "test",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Goto(
// DEFAULT-NEXT:                   "again",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 24,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoInline,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "T",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "t1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "T",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "t2",
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
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "cnt",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "t1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Cast {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "T",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Labeled(
// DEFAULT-NEXT:                   "loop",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "t2",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Identifier(
// DEFAULT-NEXT:                               "t1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "t1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: AddrOf(
// DEFAULT-NEXT:                               CompoundLiteral {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "T",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   initializer: [
// DEFAULT-NEXT:                                       InitializerItem {
// DEFAULT-NEXT:                                           designators: [
// DEFAULT-NEXT:                                               Field(
// DEFAULT-NEXT:                                                   "f",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Field(
// DEFAULT-NEXT:                                                   "f9",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                           value: Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   PostIncrement(
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "cnt",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "cnt",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Goto(
// DEFAULT-NEXT:                           "loop",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Or,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "t1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Identifier(
// DEFAULT-NEXT:                                   "t2",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Member {
// DEFAULT-NEXT:                                   base: Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "t1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "f",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "f9",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "foo",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   10,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:               line: 32,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
