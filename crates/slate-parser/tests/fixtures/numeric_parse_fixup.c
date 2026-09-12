#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char  whole[]          = "42";
  char  whole_long[]     = "-12345";
  char  whole_unsigned[] = "77";
  char  leading[]        = "  -17tail";
  char  empty[]          = "";
  char  large[]          = "999999999999999999999999999999";
  char  flt[]            = "  -3.5e2rest";
  char  end_source[]     = "12tail";
  char *end              = 0;

  printf("%d %ld %lu %ld %ld %lu %.1f\n", atoi(whole),
         strtol(whole_long, 0, 10), strtoul(whole_unsigned, 0, 10),
         atol(leading), strtol(large, 0, 10), strtoul(empty, 0, 10),
         strtod(flt, 0));

  long raw = strtol(end_source, &end, 10);
  printf("%ld %c\n", raw, *end);
  return 0;
}


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
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
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "whole",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "42",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "whole_long",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "-12345",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "whole_unsigned",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "77",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "leading",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "  -17tail",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "empty",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "large",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "999999999999999999999999999999",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "flt",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "  -3.5e2rest",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "end_source",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "12tail",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                       declarator: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "end",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%d %ld %lu %ld %ld %lu %.1f\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "atoi",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "whole",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strtol",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "whole_long",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           10,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strtoul",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "whole_unsigned",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           10,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "atol",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "leading",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strtol",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "large",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           10,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strtoul",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "empty",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           10,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strtod",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "flt",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "raw",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strtol",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "end_source",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "end",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               10,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%ld %c\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "raw",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Deref(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "end",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
