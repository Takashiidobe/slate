// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-require-effective-target indirect_calls } */

typedef struct { short x[4]; } S;
typedef struct { unsigned int a, b, c; S *d; } T;

S *(*foo) (T *, int, int, int, int);
unsigned short *(*bar)(const T *);
unsigned short baz(T *,const int);

T *die (void)
{
  typedef struct { unsigned int a, b, e; double f, g; } U;

  char h[8], i[2053], j[2053];
  double k, l, m;
  U n;
  T *o;
  unsigned short p;
  int q, r;
  long s;
  unsigned short *t;
  S *u;
  unsigned char *v, *w;
  unsigned int x;

  o = 0;
  for (x = 0; x < n.e; x++)
    {
      l = 1.0;
      if (n.g - n.f <= 1.0)
	l = ((1 << o->c) - 1) / (n.g - n.f);
      v = w;
      for (r = o->b - 1; r >= 0; r--)
	{
	  u = foo (o, 0, r, o->a, 1);
	  if (!u)
	    break;
	  t = bar (o);
	  for (q = 0; q < (int) o->a; q++)
	    {
	      h[0] = *v;
	      s = *v++;
	      k = (double) s;
	      m = l*k;
	      p = m < 0 ? 0 : m > (1 << o->c) - 1 ? (1 << o->c) - 1 : m + 0.5;
	      p = baz (o,p);
	      t[q] = p;
	      *u++ = o->d[p];
	    }
	}
    }
  return o;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[0]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           0,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Short,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 4,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "4",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[1]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           1,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "S",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "d",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[2]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           2,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "e",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "f",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "S",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "T",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "S",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Grouped(
// DEFAULT-NEXT:                               Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "foo",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Named(
// DEFAULT-NEXT:                                           "T",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Short,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Grouped(
// DEFAULT-NEXT:                               Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "bar",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Named(
// DEFAULT-NEXT:                                           "T",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_const: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Short,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "baz",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Named(
// DEFAULT-NEXT:                                           "T",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_const: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "T",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "die",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Definition(
// DEFAULT-NEXT:                                   TagId(
// DEFAULT-NEXT:                                       2,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           storage: Typedef,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "U",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "h",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 8,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "8",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "i",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 2053,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "2053",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "j",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 2053,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "2053",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "k",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "l",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "m",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "U",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "T",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "o",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Short,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "p",
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
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "q",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "r",
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
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "s",
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
// DEFAULT-NEXT:                                   rank: Short,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "S",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "u",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: Some(
// DEFAULT-NEXT:                                       false,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "v",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "w",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "o",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 0,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "0",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "e",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Postfix {
// DEFAULT-NEXT:                           op: Increment,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "l",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "1.0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Binary {
// DEFAULT-NEXT:                               op: LessEqual,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Sub,
// DEFAULT-NEXT:                                   left: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "g",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "f",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "1.0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "l",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 1,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "1",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "o",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "c",
// DEFAULT-NEXT:                                                               arrow: true,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 1,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "n",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "g",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "n",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "f",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "v",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Identifier(
// DEFAULT-NEXT:                                   "w",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       For {
// DEFAULT-NEXT:                           init: Some(
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "r",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Member {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "o",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                               arrow: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           condition: Some(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: GreaterEqual,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "r",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           increment: Some(
// DEFAULT-NEXT:                               Postfix {
// DEFAULT-NEXT:                                   op: Decrement,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "r",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "u",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "foo",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "o",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 0,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "0",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "r",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "o",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "a",
// DEFAULT-NEXT:                                                   arrow: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       operand: Identifier(
// DEFAULT-NEXT:                                           "u",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Break,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "t",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "bar",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "o",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               For {
// DEFAULT-NEXT:                                   init: Some(
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "q",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 0,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "0",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   condition: Some(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Less,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "q",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Cast {
// DEFAULT-NEXT:                                               ty: TypeName {
// DEFAULT-NEXT:                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                       ty: Integer(
// DEFAULT-NEXT:                                                           Ranked {
// DEFAULT-NEXT:                                                               rank: Int,
// DEFAULT-NEXT:                                                               signed: true,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "o",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "a",
// DEFAULT-NEXT:                                                   arrow: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   increment: Some(
// DEFAULT-NEXT:                                       Postfix {
// DEFAULT-NEXT:                                           op: Increment,
// DEFAULT-NEXT:                                           operand: Identifier(
// DEFAULT-NEXT:                                               "q",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "h",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Unary {
// DEFAULT-NEXT:                                                   op: Deref,
// DEFAULT-NEXT:                                                   operand: Identifier(
// DEFAULT-NEXT:                                                       "v",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "s",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Unary {
// DEFAULT-NEXT:                                                   op: Deref,
// DEFAULT-NEXT:                                                   operand: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "v",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "k",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Cast {
// DEFAULT-NEXT:                                                   ty: TypeName {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Floating(
// DEFAULT-NEXT:                                                               Double,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Identifier(
// DEFAULT-NEXT:                                                       "s",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "m",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "l",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "k",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "p",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: Less,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "m",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Conditional {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: Greater,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "m",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Binary {
// DEFAULT-NEXT:                                                               op: Sub,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 1,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "o",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "c",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 1,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Some(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: Sub,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 1,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "o",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "c",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 1,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "m",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: FloatLiteral(
// DEFAULT-NEXT:                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                   spelling: "0.5",
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "p",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "baz",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "o",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "p",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Index {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "t",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "q",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "p",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Unary {
// DEFAULT-NEXT:                                                   op: Deref,
// DEFAULT-NEXT:                                                   operand: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "u",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Index {
// DEFAULT-NEXT:                                                   base: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "o",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "d",
// DEFAULT-NEXT:                                                       arrow: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Identifier(
// DEFAULT-NEXT:                                                       "p",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "o",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
