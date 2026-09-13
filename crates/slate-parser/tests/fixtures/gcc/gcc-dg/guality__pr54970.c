/* PR debug/54970 */
/* PR debug/54971 */
/* { dg-do run } */
/* { dg-options "-g" } */

#if defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#define NOP "nop 0"
#elif defined(__MMIX__)
#define NOP "swym 0"
#elif defined(__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

int
main() {
  int a[] = {
      1, 2,
      3}; /* { dg-final { gdb-test .+4 "a\[0\]" "1" { xfail { no-opts "-O0" "-Og" } } } } */
  int *p = a + 2; /* { dg-final { gdb-test .+3 "a\[1\]" "2" } } */
  int *q = a + 1; /* { dg-final { gdb-test .+2 "a\[2\]" "3" } } */
  /* { dg-final { gdb-test .+1 "*p" "3" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "2" } } */
  *p +=
      10; /* { dg-final { gdb-test .+4 "a\[0\]" "1" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "2" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "13" } } */
  /* { dg-final { gdb-test .+1 "*p" "13" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "2" } } */
  *q +=
      10; /* { dg-final { gdb-test .+4 "a\[0\]" "1" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "12" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "13" } } */
  /* { dg-final { gdb-test .+1 "*p" "13" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "12" } } */
  __builtin_memcpy(&a, (int[3]){4, 5, 6}, sizeof(a));
  /* { dg-final { gdb-test .+4 "a\[0\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "5" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "6" } } */
  /* { dg-final { gdb-test .+1 "*p" "6" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "5" } } */
  *p +=
      20; /* { dg-final { gdb-test .+4 "a\[0\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "5" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "26" } } */
  /* { dg-final { gdb-test .+1 "*p" "26" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "5" } } */
  *q +=
      20; /* { dg-final { gdb-test .+8 "a\[0\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+7 "a\[1\]" "25" } } */
  /* { dg-final { gdb-test .+6 "a\[2\]" "26" } } */
  /* { dg-final { gdb-test .+5 "*p" "26" } } */
  /* { dg-final { gdb-test .+4 "p\[-1\]" "25" } } */
  /* { dg-final { gdb-test .+3 "p\[-2\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+2 "q\[-1\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+1 "q\[1\]" "26" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "25" } } */
  return 0;
}



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* PR debug/54970 */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 0,
// DEFAULT-NEXT:                       length: 20,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* PR debug/54971 */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 21,
// DEFAULT-NEXT:                       length: 20,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-do run } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 42,
// DEFAULT-NEXT:                       length: 19,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-options \"-g\" } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 62,
// DEFAULT-NEXT:                       length: 25,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Function(
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Unspecified,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Integer(
// DEFAULT-NEXT:                                                           3,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+4 \"a\\[0\\]\" \"1\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 344,
// DEFAULT-NEXT:                                   length: 82,
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+3 \"a\\[1\\]\" \"2\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 445,
// DEFAULT-NEXT:                                   length: 48,
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "p",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+2 \"a\\[2\\]\" \"3\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 512,
// DEFAULT-NEXT:                                   length: 48,
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "q",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+1 \"*p\" \"3\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 563,
// DEFAULT-NEXT:                                   length: 44,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test . \"*q\" \"2\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 629,
// DEFAULT-NEXT:                                   length: 42,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 22,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "nop",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               10,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+4 \"a\\[0\\]\" \"1\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 690,
// DEFAULT-NEXT:                                   length: 82,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 25,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+3 \"a\\[1\\]\" \"2\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 775,
// DEFAULT-NEXT:                                   length: 48,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+2 \"a\\[2\\]\" \"13\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 826,
// DEFAULT-NEXT:                                   length: 49,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+1 \"*p\" \"13\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 878,
// DEFAULT-NEXT:                                   length: 45,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test . \"*q\" \"2\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 945,
// DEFAULT-NEXT:                                   length: 42,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 26,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "nop",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "q",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               10,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+4 \"a\\[0\\]\" \"1\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1006,
// DEFAULT-NEXT:                                   length: 82,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 31,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+3 \"a\\[1\\]\" \"12\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1091,
// DEFAULT-NEXT:                                   length: 49,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+2 \"a\\[2\\]\" \"13\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1143,
// DEFAULT-NEXT:                                   length: 49,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+1 \"*p\" \"13\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1195,
// DEFAULT-NEXT:                                   length: 45,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test . \"*q\" \"12\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1262,
// DEFAULT-NEXT:                                   length: 43,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 32,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "nop",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "__builtin_memcpy",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               CompoundLiteral {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Array {
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                       size: Expression(
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   initializer: [
// DEFAULT-NEXT:                                       InitializerItem {
// DEFAULT-NEXT:                                           designators: [],
// DEFAULT-NEXT:                                           value: Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       4,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       InitializerItem {
// DEFAULT-NEXT:                                           designators: [],
// DEFAULT-NEXT:                                           value: Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       5,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       InitializerItem {
// DEFAULT-NEXT:                                           designators: [],
// DEFAULT-NEXT:                                           value: Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       6,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               SizeOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+4 \"a\\[0\\]\" \"4\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1362,
// DEFAULT-NEXT:                                   length: 82,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+3 \"a\\[1\\]\" \"5\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1447,
// DEFAULT-NEXT:                                   length: 48,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+2 \"a\\[2\\]\" \"6\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1498,
// DEFAULT-NEXT:                                   length: 48,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+1 \"*p\" \"6\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1549,
// DEFAULT-NEXT:                                   length: 44,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test . \"*q\" \"5\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1615,
// DEFAULT-NEXT:                                   length: 42,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 37,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "nop",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               20,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+4 \"a\\[0\\]\" \"4\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1676,
// DEFAULT-NEXT:                                   length: 82,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 43,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+3 \"a\\[1\\]\" \"5\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1761,
// DEFAULT-NEXT:                                   length: 48,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+2 \"a\\[2\\]\" \"26\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1812,
// DEFAULT-NEXT:                                   length: 49,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+1 \"*p\" \"26\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1864,
// DEFAULT-NEXT:                                   length: 45,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test . \"*q\" \"5\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1931,
// DEFAULT-NEXT:                                   length: 42,
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "nop",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: AddAssign,
// DEFAULT-NEXT:                           target: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "q",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               20,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+8 \"a\\[0\\]\" \"4\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1992,
// DEFAULT-NEXT:                                   length: 82,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 49,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+7 \"a\\[1\\]\" \"25\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2077,
// DEFAULT-NEXT:                                   length: 49,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+6 \"a\\[2\\]\" \"26\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2129,
// DEFAULT-NEXT:                                   length: 49,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+5 \"*p\" \"26\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2181,
// DEFAULT-NEXT:                                   length: 45,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+4 \"p\\[-1\\]\" \"25\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2229,
// DEFAULT-NEXT:                                   length: 50,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+3 \"p\\[-2\\]\" \"4\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2282,
// DEFAULT-NEXT:                                   length: 83,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+2 \"q\\[-1\\]\" \"4\" { xfail { no-opts \"-O0\" \"-Og\" } } } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2368,
// DEFAULT-NEXT:                                   length: 83,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test .+1 \"q\\[1\\]\" \"26\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2454,
// DEFAULT-NEXT:                                   length: 49,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* { dg-final { gdb-test . \"*q\" \"25\" } } */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 2525,
// DEFAULT-NEXT:                                   length: 43,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 50,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "nop",
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:               line: 15,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
