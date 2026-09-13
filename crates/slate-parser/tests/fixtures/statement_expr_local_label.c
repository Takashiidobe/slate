#include <stdio.h>

#define CHECK_VALUE(expression)                                                \
  ({                                                                           \
    __label__ failed, done;                                                    \
    int result;                                                                \
    if (!(expression))                                                         \
      goto failed;                                                             \
    result = 17;                                                               \
    goto done;                                                                 \
  failed:                                                                      \
    result = -5;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

int main(void) {
  int value  = 0;
  int first  = CHECK_VALUE(++value == 1);
  int second = CHECK_VALUE(++value == 9);
  printf("%d %d %d\n", first, second, value);
  return 0;
}




// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       variadic: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
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
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "value",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "first",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       StatementExpression(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               LocalLabelDecl(
// DEFAULT-NEXT:                                                   [
// DEFAULT-NEXT:                                                       "failed",
// DEFAULT-NEXT:                                                       "done",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Decl(
// DEFAULT-NEXT:                                                   Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarators: [
// DEFAULT-NEXT:                                                           InitDeclarator {
// DEFAULT-NEXT:                                                               declarator: Name(
// DEFAULT-NEXT:                                                                   "result",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Const(
// DEFAULT-NEXT:                                                       Unary {
// DEFAULT-NEXT:                                                           op: Not,
// DEFAULT-NEXT:                                                           value: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: PreIncrement(
// DEFAULT-NEXT:                                                                   Identifier(
// DEFAULT-NEXT:                                                                       "value",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Goto(
// DEFAULT-NEXT:                                                           "failed",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "result",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Integer(
// DEFAULT-NEXT:                                                               17,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Goto(
// DEFAULT-NEXT:                                                   "done",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Labeled(
// DEFAULT-NEXT:                                                   "failed",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "result",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Unary {
// DEFAULT-NEXT:                                                               op: Minus,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   5,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Labeled(
// DEFAULT-NEXT:                                                   "done",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "result",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "second",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       StatementExpression(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               LocalLabelDecl(
// DEFAULT-NEXT:                                                   [
// DEFAULT-NEXT:                                                       "failed",
// DEFAULT-NEXT:                                                       "done",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Decl(
// DEFAULT-NEXT:                                                   Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarators: [
// DEFAULT-NEXT:                                                           InitDeclarator {
// DEFAULT-NEXT:                                                               declarator: Name(
// DEFAULT-NEXT:                                                                   "result",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Const(
// DEFAULT-NEXT:                                                       Unary {
// DEFAULT-NEXT:                                                           op: Not,
// DEFAULT-NEXT:                                                           value: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: PreIncrement(
// DEFAULT-NEXT:                                                                   Identifier(
// DEFAULT-NEXT:                                                                       "value",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   9,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Goto(
// DEFAULT-NEXT:                                                           "failed",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "result",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Integer(
// DEFAULT-NEXT:                                                               17,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Goto(
// DEFAULT-NEXT:                                                   "done",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Labeled(
// DEFAULT-NEXT:                                                   "failed",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "result",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Unary {
// DEFAULT-NEXT:                                                               op: Minus,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   5,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Labeled(
// DEFAULT-NEXT:                                                   "done",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "result",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                                   "%d %d %d\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "first",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "second",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "value",
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
// DEFAULT-NEXT:               line: 16,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
