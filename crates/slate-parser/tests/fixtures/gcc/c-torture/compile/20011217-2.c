// SLATE-FILECHECK-DEFINES DEFAULT

/* Test that the initializer of a compound literal is properly walked
   when tree inlining.  */
/* Origin: glibc (as reported in PR c/5105) from <aj@suse.de>.  */

inline int
finite (double __x)
{
  return (__extension__
	  (((((union { double __d; int __i[2]; }) {__d: __x}).__i[1]
	     | 0x800fffffu) + 1) >> 31));
}

int
main (void)
{
  double x = 1.0;
  
  return finite (x);
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* Test that the initializer of a compound literal is properly walked\n   when tree inlining.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 1,
// DEFAULT-NEXT:           length: 96,
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
// DEFAULT-NEXT:       text: "/* Origin: glibc (as reported in PR c/5105) from <aj@suse.de>.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 98,
// DEFAULT-NEXT:           length: 66,
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
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "finite",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Floating(
// DEFAULT-NEXT:                       Double,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "__x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: ShiftRight,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: BitOr,
// DEFAULT-NEXT:                                   left: Index {
// DEFAULT-NEXT:                                       base: Member {
// DEFAULT-NEXT:                                           base: CompoundLiteral {
// DEFAULT-NEXT:                                               ty: Tagged {
// DEFAULT-NEXT:                                                   kind: Union,
// DEFAULT-NEXT:                                                   name: None,
// DEFAULT-NEXT:                                                   body: Some(
// DEFAULT-NEXT:                                                       Fields(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               FieldDecl {
// DEFAULT-NEXT:                                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Floating(
// DEFAULT-NEXT:                                                                               Double,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarator: Name(
// DEFAULT-NEXT:                                                                           "__d",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: 0,
// DEFAULT-NEXT:                                                                       header: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               FieldDecl {
// DEFAULT-NEXT:                                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Ranked {
// DEFAULT-NEXT:                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                   signed: true,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarator: Array {
// DEFAULT-NEXT:                                                                           inner: Name(
// DEFAULT-NEXT:                                                                               "__i",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           size: Expression(
// DEFAULT-NEXT:                                                                               IntLit(
// DEFAULT-NEXT:                                                                                   2,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: 0,
// DEFAULT-NEXT:                                                                       header: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               initializer: [
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [
// DEFAULT-NEXT:                                                           Field(
// DEFAULT-NEXT:                                                               "__d",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                       value: Expr(
// DEFAULT-NEXT:                                                           Const(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "__x",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           field: "__i",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       2148532223,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               31,
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
// DEFAULT-NEXT:               line: 5,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           is_inline: true,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Function(
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
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               1.0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "finite",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "x",
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
// DEFAULT-NEXT:               line: 13,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
