/* PR rtl-optimization/28982.  Function foo() does the equivalent of:

     float tmp_results[NVARS];
     for (int i = 0; i < NVARS; i++)
       {
         int inc = incs[i];
         float *ptr = ptrs[i], result = 0;
         for (int j = 0; j < n; j++)
           result += *ptr, ptr += inc;
         tmp_results[i] = result;
       }
     memcpy (results, tmp_results, sizeof (results));

   but without the outermost loop.  The idea is to create high register
   pressure and ensure that some INC and PTR variables are spilled.

   On ARM targets, sequences like "result += *ptr, ptr += inc" can
   usually be implemented using (mem (post_modify ...)), and we do
   indeed create such MEMs before reload for this testcase.  However,
   (post_modify ...) is not a valid address for coprocessor loads, so
   for -mfloat-abi=softfp, reload reloads the POST_MODIFY into a base
   register.  GCC did not deal correctly with cases where the base and
   index of the POST_MODIFY are themselves reloaded.  */
#define NITER 4
#define NVARS 20
#define MULTI(X)                                                               \
  X(0), X(1), X(2), X(3), X(4), X(5), X(6), X(7), X(8), X(9), X(10), X(11),    \
      X(12), X(13), X(14), X(15), X(16), X(17), X(18), X(19)

#define DECLAREI(INDEX) inc##INDEX = incs[INDEX]
#define DECLAREF(INDEX) *ptr##INDEX = ptrs[INDEX], result##INDEX = 0
#define LOOP(INDEX)     result##INDEX += *ptr##INDEX, ptr##INDEX += inc##INDEX
#define COPYOUT(INDEX)  results[INDEX] = result##INDEX

float *ptrs[NVARS];
float  results[NVARS];
int    incs[NVARS];

void __attribute__((noinline)) foo(int n) {
  int   MULTI(DECLAREI);
  float MULTI(DECLAREF);
  while (n--)
    MULTI(LOOP);
  MULTI(COPYOUT);
}

float input[NITER * NVARS];

int main(void) {
  int i;

  for (i = 0; i < NVARS; i++)
    ptrs[i] = input + i, incs[i] = i;
  for (i = 0; i < NITER * NVARS; i++)
    input[i] = i;
  foo(NITER);
  for (i = 0; i < NVARS; i++)
    if (results[i] != i * NITER * (NITER + 1) / 2)
      return 1;
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR rtl-optimization/28982.  Function foo() does the equivalent of:\n\n     float tmp_results[NVARS];\n     for (int i = 0; i < NVARS; i++)\n       {\n         int inc = incs[i];\n         float *ptr = ptrs[i], result = 0;\n         for (int j = 0; j < n; j++)\n           result += *ptr, ptr += inc;\n         tmp_results[i] = result;\n       }\n     memcpy (results, tmp_results, sizeof (results));\n\n   but without the outermost loop.  The idea is to create high register\n   pressure and ensure that some INC and PTR variables are spilled.\n\n   On ARM targets, sequences like \"result += *ptr, ptr += inc\" can\n   usually be implemented using (mem (post_modify ...)), and we do\n   indeed create such MEMs before reload for this testcase.  However,\n   (post_modify ...) is not a valid address for coprocessor loads, so\n   for -mfloat-abi=softfp, reload reloads the POST_MODIFY into a base\n   register.  GCC did not deal correctly with cases where the base and\n   index of the POST_MODIFY are themselves reloaded.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 1005,
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
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Float,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "ptrs",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       20,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 34,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Float,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "results",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       20,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 35,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "incs",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       20,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 36,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Function(
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
// DEFAULT-NEXT:                           "n",
// DEFAULT-NEXT:                       ),
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
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "inc0",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc2",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc3",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   3,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc4",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc5",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc6",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   6,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc7",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   7,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc8",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   8,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc9",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc10",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   10,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc11",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   11,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc12",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   12,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc13",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   13,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc14",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   14,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc15",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   15,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc16",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   16,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc17",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   17,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc18",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   18,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   "inc19",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "incs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   19,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr0",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result0",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result1",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result2",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr3",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   3,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result3",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr4",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result4",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr5",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result5",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr6",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   6,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result6",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr7",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   7,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result7",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr8",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   8,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result8",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr9",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result9",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr10",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   10,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result10",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr11",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   11,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result11",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr12",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   12,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result12",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr13",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   13,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result13",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr14",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   14,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result14",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr15",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   15,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result15",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr16",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   16,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result16",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr17",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   17,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result17",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr18",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   18,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result18",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "ptr19",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "ptrs",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   19,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Float,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "result19",
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
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               While {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       PostDecrement(
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Comma(
// DEFAULT-NEXT:                                   Comma(
// DEFAULT-NEXT:                                       Comma(
// DEFAULT-NEXT:                                           Comma(
// DEFAULT-NEXT:                                               Comma(
// DEFAULT-NEXT:                                                   Comma(
// DEFAULT-NEXT:                                                       Comma(
// DEFAULT-NEXT:                                                           Comma(
// DEFAULT-NEXT:                                                               Comma(
// DEFAULT-NEXT:                                                                   Comma(
// DEFAULT-NEXT:                                                                       Comma(
// DEFAULT-NEXT:                                                                           Comma(
// DEFAULT-NEXT:                                                                               Comma(
// DEFAULT-NEXT:                                                                                   Comma(
// DEFAULT-NEXT:                                                                                       Comma(
// DEFAULT-NEXT:                                                                                           Comma(
// DEFAULT-NEXT:                                                                                               Comma(
// DEFAULT-NEXT:                                                                                                   Comma(
// DEFAULT-NEXT:                                                                                                       Comma(
// DEFAULT-NEXT:                                                                                                           Comma(
// DEFAULT-NEXT:                                                                                                               Comma(
// DEFAULT-NEXT:                                                                                                                   Comma(
// DEFAULT-NEXT:                                                                                                                       Comma(
// DEFAULT-NEXT:                                                                                                                           Comma(
// DEFAULT-NEXT:                                                                                                                               Comma(
// DEFAULT-NEXT:                                                                                                                                   Comma(
// DEFAULT-NEXT:                                                                                                                                       Comma(
// DEFAULT-NEXT:                                                                                                                                           Comma(
// DEFAULT-NEXT:                                                                                                                                               Comma(
// DEFAULT-NEXT:                                                                                                                                                   Comma(
// DEFAULT-NEXT:                                                                                                                                                       Comma(
// DEFAULT-NEXT:                                                                                                                                                           Comma(
// DEFAULT-NEXT:                                                                                                                                                               Comma(
// DEFAULT-NEXT:                                                                                                                                                                   Comma(
// DEFAULT-NEXT:                                                                                                                                                                       Comma(
// DEFAULT-NEXT:                                                                                                                                                                           Comma(
// DEFAULT-NEXT:                                                                                                                                                                               Comma(
// DEFAULT-NEXT:                                                                                                                                                                                   Comma(
// DEFAULT-NEXT:                                                                                                                                                                                       Comma(
// DEFAULT-NEXT:                                                                                                                                                                                           Assign {
// DEFAULT-NEXT:                                                                                                                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                   "result0",
// DEFAULT-NEXT:                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                               value: Deref(
// DEFAULT-NEXT:                                                                                                                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                       "ptr0",
// DEFAULT-NEXT:                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                           Assign {
// DEFAULT-NEXT:                                                                                                                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                   "ptr0",
// DEFAULT-NEXT:                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                   "inc0",
// DEFAULT-NEXT:                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                       Assign {
// DEFAULT-NEXT:                                                                                                                                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                               "result1",
// DEFAULT-NEXT:                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                           value: Deref(
// DEFAULT-NEXT:                                                                                                                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                   "ptr1",
// DEFAULT-NEXT:                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                                                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                           "ptr1",
// DEFAULT-NEXT:                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                           "inc1",
// DEFAULT-NEXT:                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                               Assign {
// DEFAULT-NEXT:                                                                                                                                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                       "result2",
// DEFAULT-NEXT:                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                   value: Deref(
// DEFAULT-NEXT:                                                                                                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                           "ptr2",
// DEFAULT-NEXT:                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                           Assign {
// DEFAULT-NEXT:                                                                                                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                   "ptr2",
// DEFAULT-NEXT:                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                   "inc2",
// DEFAULT-NEXT:                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                       Assign {
// DEFAULT-NEXT:                                                                                                                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                               "result3",
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                           value: Deref(
// DEFAULT-NEXT:                                                                                                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                   "ptr3",
// DEFAULT-NEXT:                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                           "ptr3",
// DEFAULT-NEXT:                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                           "inc3",
// DEFAULT-NEXT:                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                               Assign {
// DEFAULT-NEXT:                                                                                                                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                       "result4",
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                   value: Deref(
// DEFAULT-NEXT:                                                                                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                                                                                           "ptr4",
// DEFAULT-NEXT:                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                           Assign {
// DEFAULT-NEXT:                                                                                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                   "ptr4",
// DEFAULT-NEXT:                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                   "inc4",
// DEFAULT-NEXT:                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                       Assign {
// DEFAULT-NEXT:                                                                                                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                               "result5",
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                           value: Deref(
// DEFAULT-NEXT:                                                                                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                                                                                   "ptr5",
// DEFAULT-NEXT:                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                           "ptr5",
// DEFAULT-NEXT:                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                                                                           "inc5",
// DEFAULT-NEXT:                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                               Assign {
// DEFAULT-NEXT:                                                                                                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                       "result6",
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                   value: Deref(
// DEFAULT-NEXT:                                                                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                                                                           "ptr6",
// DEFAULT-NEXT:                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                           Assign {
// DEFAULT-NEXT:                                                                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                                                                   "ptr6",
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                                                                   "inc6",
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                       Assign {
// DEFAULT-NEXT:                                                                                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                                                                                               "result7",
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                           value: Deref(
// DEFAULT-NEXT:                                                                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                                                                   "ptr7",
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                                                                                           "ptr7",
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                                                           "inc7",
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                               Assign {
// DEFAULT-NEXT:                                                                                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                                                                                       "result8",
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   value: Deref(
// DEFAULT-NEXT:                                                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                                                           "ptr8",
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           Assign {
// DEFAULT-NEXT:                                                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                                                   "ptr8",
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                                                   "inc8",
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       Assign {
// DEFAULT-NEXT:                                                                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                                                                               "result9",
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           value: Deref(
// DEFAULT-NEXT:                                                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                                                   "ptr9",
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                                                                           "ptr9",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                                           "inc9",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               Assign {
// DEFAULT-NEXT:                                                                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                                                                       "result10",
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   value: Deref(
// DEFAULT-NEXT:                                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                                           "ptr10",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           Assign {
// DEFAULT-NEXT:                                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                                   "ptr10",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                                   "inc10",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       Assign {
// DEFAULT-NEXT:                                                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                                                               "result11",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           value: Deref(
// DEFAULT-NEXT:                                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                                   "ptr11",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                                                           "ptr11",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                           "inc11",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               Assign {
// DEFAULT-NEXT:                                                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                                                       "result12",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   value: Deref(
// DEFAULT-NEXT:                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                           "ptr12",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           Assign {
// DEFAULT-NEXT:                                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                                   "ptr12",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                   "inc12",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       Assign {
// DEFAULT-NEXT:                                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                                               "result13",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           value: Deref(
// DEFAULT-NEXT:                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                   "ptr13",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   Assign {
// DEFAULT-NEXT:                                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                                           "ptr13",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                           "inc13",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               Assign {
// DEFAULT-NEXT:                                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                                       "result14",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   value: Deref(
// DEFAULT-NEXT:                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                           "ptr14",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           Assign {
// DEFAULT-NEXT:                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                   "ptr14",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "inc14",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       Assign {
// DEFAULT-NEXT:                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                               "result15",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           value: Deref(
// DEFAULT-NEXT:                                                                               Identifier(
// DEFAULT-NEXT:                                                                                   "ptr15",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                           "ptr15",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                           "inc15",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "result16",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Deref(
// DEFAULT-NEXT:                                                                       Identifier(
// DEFAULT-NEXT:                                                                           "ptr16",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: AddAssign,
// DEFAULT-NEXT:                                                               target: Identifier(
// DEFAULT-NEXT:                                                                   "ptr16",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               value: Identifier(
// DEFAULT-NEXT:                                                                   "inc16",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: AddAssign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "result17",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Deref(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "ptr17",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: AddAssign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "ptr17",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "inc17",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: AddAssign,
// DEFAULT-NEXT:                                                   target: Identifier(
// DEFAULT-NEXT:                                                       "result18",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Deref(
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "ptr18",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: AddAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "ptr18",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "inc18",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: AddAssign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "result19",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Deref(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "ptr19",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: AddAssign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "ptr19",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "inc19",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Comma(
// DEFAULT-NEXT:                           Comma(
// DEFAULT-NEXT:                               Comma(
// DEFAULT-NEXT:                                   Comma(
// DEFAULT-NEXT:                                       Comma(
// DEFAULT-NEXT:                                           Comma(
// DEFAULT-NEXT:                                               Comma(
// DEFAULT-NEXT:                                                   Comma(
// DEFAULT-NEXT:                                                       Comma(
// DEFAULT-NEXT:                                                           Comma(
// DEFAULT-NEXT:                                                               Comma(
// DEFAULT-NEXT:                                                                   Comma(
// DEFAULT-NEXT:                                                                       Comma(
// DEFAULT-NEXT:                                                                           Comma(
// DEFAULT-NEXT:                                                                               Comma(
// DEFAULT-NEXT:                                                                                   Comma(
// DEFAULT-NEXT:                                                                                       Comma(
// DEFAULT-NEXT:                                                                                           Comma(
// DEFAULT-NEXT:                                                                                               Comma(
// DEFAULT-NEXT:                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                       op: Assign,
// DEFAULT-NEXT:                                                                                                       target: Index {
// DEFAULT-NEXT:                                                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                                                               "results",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           index: Integer(
// DEFAULT-NEXT:                                                                                                               0,
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                           "result0",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   Assign {
// DEFAULT-NEXT:                                                                                                       op: Assign,
// DEFAULT-NEXT:                                                                                                       target: Index {
// DEFAULT-NEXT:                                                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                                                               "results",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           index: Integer(
// DEFAULT-NEXT:                                                                                                               1,
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                           "result1",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               Assign {
// DEFAULT-NEXT:                                                                                                   op: Assign,
// DEFAULT-NEXT:                                                                                                   target: Index {
// DEFAULT-NEXT:                                                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                                                           "results",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       index: Integer(
// DEFAULT-NEXT:                                                                                                           2,
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                                                       "result2",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           Assign {
// DEFAULT-NEXT:                                                                                               op: Assign,
// DEFAULT-NEXT:                                                                                               target: Index {
// DEFAULT-NEXT:                                                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                                                       "results",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   index: Integer(
// DEFAULT-NEXT:                                                                                                       3,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                                   "result3",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       Assign {
// DEFAULT-NEXT:                                                                                           op: Assign,
// DEFAULT-NEXT:                                                                                           target: Index {
// DEFAULT-NEXT:                                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                                   "results",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               index: Integer(
// DEFAULT-NEXT:                                                                                                   4,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           value: Identifier(
// DEFAULT-NEXT:                                                                                               "result4",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   Assign {
// DEFAULT-NEXT:                                                                                       op: Assign,
// DEFAULT-NEXT:                                                                                       target: Index {
// DEFAULT-NEXT:                                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                                               "results",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           index: Integer(
// DEFAULT-NEXT:                                                                                               5,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                           "result5",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               Assign {
// DEFAULT-NEXT:                                                                                   op: Assign,
// DEFAULT-NEXT:                                                                                   target: Index {
// DEFAULT-NEXT:                                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                                           "results",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       index: Integer(
// DEFAULT-NEXT:                                                                                           6,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                                       "result6",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           Assign {
// DEFAULT-NEXT:                                                                               op: Assign,
// DEFAULT-NEXT:                                                                               target: Index {
// DEFAULT-NEXT:                                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                                       "results",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   index: Integer(
// DEFAULT-NEXT:                                                                                       7,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "result7",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       Assign {
// DEFAULT-NEXT:                                                                           op: Assign,
// DEFAULT-NEXT:                                                                           target: Index {
// DEFAULT-NEXT:                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                   "results",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               index: Integer(
// DEFAULT-NEXT:                                                                                   8,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           value: Identifier(
// DEFAULT-NEXT:                                                                               "result8",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Index {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "results",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           index: Integer(
// DEFAULT-NEXT:                                                                               9,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                           "result9",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Index {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "results",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       index: Integer(
// DEFAULT-NEXT:                                                                           10,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                       "result10",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Index {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "results",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   index: Integer(
// DEFAULT-NEXT:                                                                       11,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               value: Identifier(
// DEFAULT-NEXT:                                                                   "result11",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "results",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   12,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           value: Identifier(
// DEFAULT-NEXT:                                                               "result12",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "results",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               13,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "result13",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "results",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           14,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Identifier(
// DEFAULT-NEXT:                                                       "result14",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "results",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       15,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "result15",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "results",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   16,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           value: Identifier(
// DEFAULT-NEXT:                                               "result16",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "results",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               17,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "result17",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "results",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           18,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "result18",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "results",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       19,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Identifier(
// DEFAULT-NEXT:                                   "result19",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 38,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoInline,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Float,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "input",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Expression(
// DEFAULT-NEXT:                   IntLit(
// DEFAULT-NEXT:                       80,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 46,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
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
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   20,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Comma(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "ptrs",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Identifier(
// DEFAULT-NEXT:                                               "i",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "input",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "i",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "incs",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Identifier(
// DEFAULT-NEXT:                                               "i",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Integer(
// DEFAULT-NEXT:                                       4,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       20,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           PostIncrement(
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
// DEFAULT-NEXT:                                           "input",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "i",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "foo",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
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
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   20,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: NotEqual,
// DEFAULT-NEXT:                                   left: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "results",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Div,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "i",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Integer(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Return(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
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
// DEFAULT-NEXT:               line: 48,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
