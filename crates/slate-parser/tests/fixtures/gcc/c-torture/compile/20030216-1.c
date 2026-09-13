// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/8086  */

#define P(x) \
        ((((((((((((((((((((((((((((((((        \
         (x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)                 \
         *(x)+a)

int
polynomial(int a)
{
  return P(3);
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR c/8086  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 1,
// DEFAULT-NEXT:           length: 16,
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
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "polynomial",
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
// DEFAULT-NEXT:                           "a",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Add,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Mul,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Mul,
// DEFAULT-NEXT:                                                               left: Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Binary {
// DEFAULT-NEXT:                                                                               op: Mul,
// DEFAULT-NEXT:                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                       3,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                           3,
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                   3,
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                               "a",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                           3,
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                       "a",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                   3,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                               "a",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                           3,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "a",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   3,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                               "a",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Integer(
// DEFAULT-NEXT:                                                                           3,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                       "a",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   3,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "a",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           3,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "a",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   3,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           3,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "a",
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
// DEFAULT-NEXT:               line: 38,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
