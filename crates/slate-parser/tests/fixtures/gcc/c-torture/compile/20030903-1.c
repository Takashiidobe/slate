// SLATE-FILECHECK-DEFINES DEFAULT

/* Derived from PR optimization/11700.  */
/* The compiler used to ICE during reload for m68k targets.  */

/* { dg-skip-if "exceeds eBPF stack limit" { bpf-*-* } } */

void check_complex (__complex__ double, __complex__ double,
                    __complex__ double, __complex__ int);
void check_float (double, double, double, int);
extern double _Complex conj (double _Complex);
extern double carg (double _Complex __z);

static double minus_zero;

void
conj_test (void)
{
  check_complex (conj (({ __complex__ double __retval;
			  __real__ __retval = (0.0);
			  __imag__ __retval = (0.0);
			  __retval; })),
		 ({ __complex__ double __retval;
		    __real__ __retval = (0.0);
		    __imag__ __retval = (minus_zero);
		    __retval; }), 0, 0);
}

void
carg_test (void)
{
  check_float (carg (({ __complex__ double __retval;
			__real__ __retval = (2.0);
			__imag__ __retval = (0);
			__retval; })), 0, 0, 0);
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* Derived from PR optimization/11700.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 1,
// DEFAULT-NEXT:           length: 42,
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
// DEFAULT-NEXT: decl[1]: Comment {
// DEFAULT-NEXT:       text: "/* The compiler used to ICE during reload for m68k targets.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 44,
// DEFAULT-NEXT:           length: 63,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 2,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Comment {
// DEFAULT-NEXT:       text: "/* { dg-skip-if \"exceeds eBPF stack limit\" { bpf-*-* } } */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 109,
// DEFAULT-NEXT:           length: 59,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "check_complex",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Complex(
// DEFAULT-NEXT:                           Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Complex(
// DEFAULT-NEXT:                           Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Complex(
// DEFAULT-NEXT:                           Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Complex(
// DEFAULT-NEXT:                           Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "check_float",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Floating(
// DEFAULT-NEXT:                           Double,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Floating(
// DEFAULT-NEXT:                           Double,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Floating(
// DEFAULT-NEXT:                           Double,
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
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Complex(
// DEFAULT-NEXT:                   Floating(
// DEFAULT-NEXT:                       Double,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "conj",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Complex(
// DEFAULT-NEXT:                           Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Double,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "carg",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Complex(
// DEFAULT-NEXT:                           Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Name(
// DEFAULT-NEXT:                               "__z",
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:           line: 10,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Double,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "minus_zero",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 12,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "conj_test",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "check_complex",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "conj",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StatementExpression(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               Keyword(
// DEFAULT-NEXT:                                                   Complex,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Keyword(
// DEFAULT-NEXT:                                                   Double,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__real__",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Equal,
// DEFAULT-NEXT:                                               LParen,
// DEFAULT-NEXT:                                               FloatLit(
// DEFAULT-NEXT:                                                   "0.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               RParen,
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__imag__",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Equal,
// DEFAULT-NEXT:                                               LParen,
// DEFAULT-NEXT:                                               FloatLit(
// DEFAULT-NEXT:                                                   "0.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               RParen,
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               StatementExpression(
// DEFAULT-NEXT:                                   [
// DEFAULT-NEXT:                                       Keyword(
// DEFAULT-NEXT:                                           Complex,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Keyword(
// DEFAULT-NEXT:                                           Double,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "__retval",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "__real__",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "__retval",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Equal,
// DEFAULT-NEXT:                                       LParen,
// DEFAULT-NEXT:                                       FloatLit(
// DEFAULT-NEXT:                                           "0.0",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       RParen,
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "__imag__",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "__retval",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Equal,
// DEFAULT-NEXT:                                       LParen,
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "minus_zero",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       RParen,
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "__retval",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 14,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[9]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "carg_test",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "check_float",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "carg",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StatementExpression(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               Keyword(
// DEFAULT-NEXT:                                                   Complex,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Keyword(
// DEFAULT-NEXT:                                                   Double,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__real__",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Equal,
// DEFAULT-NEXT:                                               LParen,
// DEFAULT-NEXT:                                               FloatLit(
// DEFAULT-NEXT:                                                   "2.0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               RParen,
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__imag__",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Equal,
// DEFAULT-NEXT:                                               LParen,
// DEFAULT-NEXT:                                               IntLit(
// DEFAULT-NEXT:                                                   "0",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               RParen,
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                               Ident(
// DEFAULT-NEXT:                                                   "__retval",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Semi,
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
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
