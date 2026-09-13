#define DECLARE_ARRAY(A)   array##A[0x10]
#define DECLARE_COUNTER(A) counter##A = 0
#define DECLARE_POINTER(A) *pointer##A = array##A + x
/* Create a loop that allows post-modification of pointerA, followed by
   a use of the post-modified address.  */
#define BEFORE(A)          counter##A += *pointer##A, pointer##A += 3
#define AFTER(A)           counter##A += pointer##A[x]

/* Set up the arrays so that one iteration of the loop sets the counter
   to 3.0f.  */
#define INIT_ARRAY(A) array##A[1] = 1.0f, array##A[5] = 2.0f

/* Check that the loop worked correctly for all values.  */
#define CHECK_ARRAY(A) exit_code |= (counter##A != 3.0f)

/* Having 6 copies triggered the bug for ARM and Thumb.  */
#define MANY(A) A(0), A(1), A(2), A(3), A(4), A(5)

/* Each addendA should be allocated a register.  */
#define INIT_VOLATILE(A) addend##A = vol
#define ADD_VOLATILE(A)  vol += addend##A

/* Having 5 copies triggered the bug for ARM and Thumb.  */
#define MANY2(A) A(0), A(1), A(2), A(3), A(4)

float MANY(DECLARE_ARRAY);
float MANY(DECLARE_COUNTER);

volatile int stop = 1;
volatile int vol;

void __attribute__((noinline)) foo(int x) {
  float MANY(DECLARE_POINTER);
  int   i;

  do {
    MANY(BEFORE);
    MANY(AFTER);
    /* Create an inner loop that should ensure the code above
       has registers free for reload inheritance.  */
    {
      int MANY2(INIT_VOLATILE);
      for (i = 0; i < 10; i++)
        MANY2(ADD_VOLATILE);
    }
  } while (!stop);
}

int main(void) {
  int exit_code = 0;

  MANY(INIT_ARRAY);
  foo(1);
  MANY(CHECK_ARRAY);
  return exit_code;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Create a loop that allows post-modification of pointerA, followed by\n   a use of the post-modified address.  */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 138,
// DEFAULT-NEXT:                       length: 114,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Set up the arrays so that one iteration of the loop sets the counter\n   to 3.0f.  */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 379,
// DEFAULT-NEXT:                       length: 87,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Check that the loop worked correctly for all values.  */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 529,
// DEFAULT-NEXT:                       length: 59,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Having 6 copies triggered the bug for ARM and Thumb.  */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 647,
// DEFAULT-NEXT:                       length: 59,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Each addendA should be allocated a register.  */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 759,
// DEFAULT-NEXT:                       length: 51,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Having 5 copies triggered the bug for ARM and Thumb.  */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 895,
// DEFAULT-NEXT:                       length: 59,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Float,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "array0",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               16,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "array1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               16,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "array2",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               16,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "array3",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               16,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "array4",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               16,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "array5",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           IntLit(
// DEFAULT-NEXT:                               16,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 25,
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
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "counter0",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "counter1",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "counter2",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "counter3",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "counter4",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "counter5",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 26,
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
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "stop",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 28,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "vol",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 29,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
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
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Float,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "pointer0",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "array0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "pointer1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "array1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "pointer2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "array2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "pointer3",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "array3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "pointer4",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "array4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "pointer5",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "array5",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
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
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               DoWhile {
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
// DEFAULT-NEXT:                                                                           Assign {
// DEFAULT-NEXT:                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                   "counter0",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               value: Deref(
// DEFAULT-NEXT:                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                       "pointer0",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           Assign {
// DEFAULT-NEXT:                                                                               op: AddAssign,
// DEFAULT-NEXT:                                                                               target: Identifier(
// DEFAULT-NEXT:                                                                                   "pointer0",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               value: Integer(
// DEFAULT-NEXT:                                                                                   3,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       Assign {
// DEFAULT-NEXT:                                                                           op: AddAssign,
// DEFAULT-NEXT:                                                                           target: Identifier(
// DEFAULT-NEXT:                                                                               "counter1",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           value: Deref(
// DEFAULT-NEXT:                                                                               Identifier(
// DEFAULT-NEXT:                                                                                   "pointer1",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: AddAssign,
// DEFAULT-NEXT:                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                           "pointer1",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       value: Integer(
// DEFAULT-NEXT:                                                                           3,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "counter2",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Deref(
// DEFAULT-NEXT:                                                                       Identifier(
// DEFAULT-NEXT:                                                                           "pointer2",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: AddAssign,
// DEFAULT-NEXT:                                                               target: Identifier(
// DEFAULT-NEXT:                                                                   "pointer2",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   3,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: AddAssign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "counter3",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Deref(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "pointer3",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: AddAssign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "pointer3",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: Integer(
// DEFAULT-NEXT:                                                           3,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: AddAssign,
// DEFAULT-NEXT:                                                   target: Identifier(
// DEFAULT-NEXT:                                                       "counter4",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Deref(
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "pointer4",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: AddAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "pointer4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   3,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: AddAssign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "counter5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Deref(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "pointer5",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: AddAssign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "pointer5",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Comma(
// DEFAULT-NEXT:                                   Comma(
// DEFAULT-NEXT:                                       Comma(
// DEFAULT-NEXT:                                           Comma(
// DEFAULT-NEXT:                                               Comma(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: AddAssign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "counter0",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "pointer0",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: AddAssign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "counter1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "pointer1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: AddAssign,
// DEFAULT-NEXT:                                                   target: Identifier(
// DEFAULT-NEXT:                                                       "counter2",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "pointer2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "x",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: AddAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "counter3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "pointer3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "x",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: AddAssign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "counter4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "pointer4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: AddAssign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "counter5",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "pointer5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Decl(
// DEFAULT-NEXT:                                   Declaration {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Int,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarators: [
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "addend0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "vol",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "addend1",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "vol",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "addend2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "vol",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "addend3",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "vol",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitDeclarator {
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "addend4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               initializer: Some(
// DEFAULT-NEXT:                                                   Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Identifier(
// DEFAULT-NEXT:                                                               "vol",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               For {
// DEFAULT-NEXT:                                   init: Some(
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Identifier(
// DEFAULT-NEXT:                                                       "i",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   condition: Some(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Less,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "i",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   10,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   increment: Some(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           PostIncrement(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "i",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Comma(
// DEFAULT-NEXT:                                                   Comma(
// DEFAULT-NEXT:                                                       Comma(
// DEFAULT-NEXT:                                                           Comma(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "vol",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                       "addend0",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: AddAssign,
// DEFAULT-NEXT:                                                                   target: Identifier(
// DEFAULT-NEXT:                                                                       "vol",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                       "addend1",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: AddAssign,
// DEFAULT-NEXT:                                                               target: Identifier(
// DEFAULT-NEXT:                                                                   "vol",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               value: Identifier(
// DEFAULT-NEXT:                                                                   "addend2",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: AddAssign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "vol",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Identifier(
// DEFAULT-NEXT:                                                               "addend3",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: AddAssign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "vol",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "addend4",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Unary {
// DEFAULT-NEXT:                           op: Not,
// DEFAULT-NEXT:                           value: Identifier(
// DEFAULT-NEXT:                               "stop",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment(
// DEFAULT-NEXT:                   CommentGroup {
// DEFAULT-NEXT:                       comments: [
// DEFAULT-NEXT:                           Comment {
// DEFAULT-NEXT:                               text: "/* Create an inner loop that should ensure the code above\n       has registers free for reload inheritance.  */",
// DEFAULT-NEXT:                               kind: Block,
// DEFAULT-NEXT:                               loc: Loc {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   offset: 1234,
// DEFAULT-NEXT:                                   length: 111,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 38,
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
// DEFAULT-NEXT:               line: 31,
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
// DEFAULT-NEXT:                                   "exit_code",
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
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Index {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "array0",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           index: Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Float(
// DEFAULT-NEXT:                                                                           FloatLiteral {
// DEFAULT-NEXT:                                                                               value: Single(
// DEFAULT-NEXT:                                                                                   1.0,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Index {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "array0",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           index: Integer(
// DEFAULT-NEXT:                                                                               5,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Float(
// DEFAULT-NEXT:                                                                           FloatLiteral {
// DEFAULT-NEXT:                                                                               value: Single(
// DEFAULT-NEXT:                                                                                   2.0,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Index {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "array1",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       index: Integer(
// DEFAULT-NEXT:                                                                           1,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   value: Float(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           value: Single(
// DEFAULT-NEXT:                                                                               1.0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Index {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "array1",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   index: Integer(
// DEFAULT-NEXT:                                                                       5,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               value: Float(
// DEFAULT-NEXT:                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                       value: Single(
// DEFAULT-NEXT:                                                                           2.0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "array2",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           value: Float(
// DEFAULT-NEXT:                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                   value: Single(
// DEFAULT-NEXT:                                                                       1.0,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "array2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               5,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       value: Float(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               value: Single(
// DEFAULT-NEXT:                                                                   2.0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "array3",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Single(
// DEFAULT-NEXT:                                                               1.0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "array3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       5,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Float(
// DEFAULT-NEXT:                                                   FloatLiteral {
// DEFAULT-NEXT:                                                       value: Single(
// DEFAULT-NEXT:                                                           2.0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Index {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "array4",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           value: Float(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   value: Single(
// DEFAULT-NEXT:                                                       1.0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "array4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               5,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   2.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "array5",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               1.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "array5",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       5,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Single(
// DEFAULT-NEXT:                                           2.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "foo",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Comma(
// DEFAULT-NEXT:                           Comma(
// DEFAULT-NEXT:                               Comma(
// DEFAULT-NEXT:                                   Comma(
// DEFAULT-NEXT:                                       Comma(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: BitOrAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "exit_code",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: NotEqual,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "counter0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Single(
// DEFAULT-NEXT:                                                               3.0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: BitOrAssign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "exit_code",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: NotEqual,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "counter1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Single(
// DEFAULT-NEXT:                                                               3.0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: BitOrAssign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "exit_code",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "counter2",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Float(
// DEFAULT-NEXT:                                                   FloatLiteral {
// DEFAULT-NEXT:                                                       value: Single(
// DEFAULT-NEXT:                                                           3.0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: BitOrAssign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "exit_code",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: NotEqual,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "counter3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Float(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   value: Single(
// DEFAULT-NEXT:                                                       3.0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: BitOrAssign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "exit_code",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: NotEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "counter4",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Single(
// DEFAULT-NEXT:                                                   3.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: BitOrAssign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "exit_code",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Binary {
// DEFAULT-NEXT:                                   op: NotEqual,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "counter5",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Single(
// DEFAULT-NEXT:                                               3.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "exit_code",
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
