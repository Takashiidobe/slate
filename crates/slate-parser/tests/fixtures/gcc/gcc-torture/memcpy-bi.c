/* Test builtin-memcpy (which may emit different code for different N).  */
#include <string.h>

void abort(void);

#define TESTSIZE 80

char src[TESTSIZE] __attribute__((aligned));
char dst[TESTSIZE] __attribute__((aligned));

void check(char *test, char *match, int n) {
  if (memcmp(test, match, n))
    abort();
}

#define TN(n)                                                                  \
  {                                                                            \
    memset(dst, 0, n);                                                         \
    memcpy(dst, src, n);                                                       \
    check(dst, src, n);                                                        \
  }
#define T(n)                                                                   \
  TN(n)                                                                        \
  TN((n) + 1)                                                                  \
  TN((n) + 2)                                                                  \
  TN((n) + 3)

int main(void) {
  int i, j;

  for (i = 0; i < sizeof(src); ++i)
    src[i] = 'a' + i % 26;

  T(0);
  T(4);
  T(8);
  T(12);
  T(16);
  T(20);
  T(24);
  T(28);
  T(32);
  T(36);
  T(40);
  T(44);
  T(48);
  T(52);
  T(56);
  T(60);
  T(64);
  T(68);
  T(72);
  T(76);

  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* Test builtin-memcpy (which may emit different code for different N).  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 75,
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
// DEFAULT-NEXT: decl[1]: Typedef {
// DEFAULT-NEXT:       name: "size_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Long,
// DEFAULT-NEXT:               signed: false,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "__size_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Long,
// DEFAULT-NEXT:               signed: false,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 258,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Typedef {
// DEFAULT-NEXT:       name: "size_t",
// DEFAULT-NEXT:       ty: Named(
// DEFAULT-NEXT:           "__size_t",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 795,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
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
// DEFAULT-NEXT:                               inner: Abstract,
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
// DEFAULT-NEXT:                               inner: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Named(
// DEFAULT-NEXT:                           "size_t",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 14,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "memset",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Void,
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Named(
// DEFAULT-NEXT:                           "size_t",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 16,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:                   "memcmp",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Qualified {
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Abstract,
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
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Named(
// DEFAULT-NEXT:                           "size_t",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 20,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
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
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "src",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       80,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Aligned(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       16,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "dst",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       80,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Aligned(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       16,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "check",
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
// DEFAULT-NEXT:                               "test",
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
// DEFAULT-NEXT:                               "match",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "n",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "memcmp",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "test",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "match",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
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
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 10,
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
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "j",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "i",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Less,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: SizeOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "src",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           PreIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Integer(
// DEFAULT-NEXT:                                           97,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Rem,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "i",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               26,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           12,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           12,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           12,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               12,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           16,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           16,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           16,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               16,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           20,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           20,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           20,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               20,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           24,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           24,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           24,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               24,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           28,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           28,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           28,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               28,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           32,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           32,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           32,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               32,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           36,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           36,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           36,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               36,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           40,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           40,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           40,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               40,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           44,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           44,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           44,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               44,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           48,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           48,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           48,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               48,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           52,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           52,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           52,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               52,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           56,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           56,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           56,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               56,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           60,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           60,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           60,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               60,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           64,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           64,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           64,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               64,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           68,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           68,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           68,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               68,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           72,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           72,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           72,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               72,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           76,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           76,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           76,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memset",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "check",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "dst",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "src",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Integer(
// DEFAULT-NEXT:                                               76,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
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
// DEFAULT-NEXT:               line: 27,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
