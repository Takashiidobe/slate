// SLATE-FILECHECK-DEFINES DEFAULT

struct A
{
  long a1;
  double *a2;
};

struct B
{
  void *b1;
  double b2, b3;
  struct
  {
    int d1;
    double d2;
  } b4;
};

struct C
{
  struct A *c1;
  void *c2;
};

long fn1 (struct A *, double);
void fn2 (void *, const char *);
double fn3 (double);
double fn4 (double);
int fn5 (void *, double, double);

int
foo (struct B *x)
{
  struct C *e = x->b1;
  struct A *f = e->c1;
  long g, h, i;
  double *j, k;
  g = fn1 (f, 0.5 * (x->b2 + x->b3)), h = g + 1, i = f->a1;
  j = f->a2, k = x->b4.d2;
  fn2 (x, "something");
  if (g <= 0)
    {
      double l = j[2] - j[1];
      if (l > 0.0 && l <= 0.02)
        k = (x->b4.d1 == 1
             ? ((1.0 / l) < 25 ? 25 : (1.0 / l))
             : fn3 ((1.0 / l) < 25 ? 25 : (1.0 / l)));
    }
  else
    {
      double m = j[h] - j[g], n = 0.0, l = 0.0;
      if (g > 1)
        n = j[g] - j[g - 1];
      if (h < i)
        l = j[h + 1] - j[h];
      if (n > 0.02)
        n = 0;
      if (m > 0.02)
        m = 0;
      if (l > 0.02)
        l = 0;
      if (m < n)
        {
          double o = m;
          m = n;
          n = o;
        }
      if (l < n)
        {
          double o = l;
          l = n;
          n = o;
        }
      if (l < m)
        {
          double o = l;
          l = m;
          m = o;
        }
      if (n != 0.0)
        k = (x->b4.d1 == 1
             ? ((1 / m) < 25 ? 25 : (1 / m))
             : fn3 ((1 / m) < 25 ? 25 : (1 / m)));
      else if (m != 0.0)
        k = (x->b4.d1 == 1
             ? ((2 / (m + l)) < 25 ? 25 : (2 / (m + l)))
             : fn3 ((2 / (m + l)) < 25 ? 25 : (2 / (m + l))));
      else if (l != 0.0)
        k = (x->b4.d1 == 1
             ? ((1 / l) < 25 ? 25 : (1 / l))
             : fn3 ((1 / l) < 25 ? 25 : (1 / l)));
    }
  fn5 (e->c2, 0.5 * (x->b2 + x->b3), (x->b4.d1 == 1 ? k : fn4 (k)));
  return 1;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "A",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "a1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 3,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "a2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 4,
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
// DEFAULT-NEXT:               line: 1,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "B",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "b1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 9,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "b2",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "b3",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 10,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: None,
// DEFAULT-NEXT:                               body: Some(
// DEFAULT-NEXT:                                   Fields(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Int,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "d1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Floating(
// DEFAULT-NEXT:                                                       Double,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "d2",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "b4",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 11,
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
// DEFAULT-NEXT:               line: 7,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "C",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "A",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "c1",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "c2",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 18,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "fn1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "A",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 24,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "fn2",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Double,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "fn3",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Double,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "fn4",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 27,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
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
// DEFAULT-NEXT:                           "fn5",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
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
// DEFAULT-NEXT: decl[8]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "foo",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Tagged {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: Some(
// DEFAULT-NEXT:                           "B",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "C",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "e",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b1",
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
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "A",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "e",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "c1",
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
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "g",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "h",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "j",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "k",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Comma(
// DEFAULT-NEXT:                           Comma(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "g",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "fn1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "f",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Float(
// DEFAULT-NEXT:                                                   FloatLiteral {
// DEFAULT-NEXT:                                                       value: Double(
// DEFAULT-NEXT:                                                           0.5,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Arrow {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "x",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "b2",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Arrow {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "x",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "b3",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "h",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "g",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "a1",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Comma(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "j",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "f",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "a2",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "k",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: Member {
// DEFAULT-NEXT:                                   base: Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "x",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "b4",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "d2",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "fn2",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "x",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "something",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: LessEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "g",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Floating(
// DEFAULT-NEXT:                                       Double,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "l",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Sub,
// DEFAULT-NEXT:                                                       left: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "j",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               2,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "j",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: And,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Greater,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "l",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: LessEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "l",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.02,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "k",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Ternary {
// DEFAULT-NEXT:                                               condition: Binary {
// DEFAULT-NEXT:                                                   op: Equal,
// DEFAULT-NEXT:                                                   left: Member {
// DEFAULT-NEXT:                                                       base: Arrow {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "x",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "b4",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       field: "d1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               then_value: Ternary {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: Less,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Div,
// DEFAULT-NEXT:                                                           left: Float(
// DEFAULT-NEXT:                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                   value: Double(
// DEFAULT-NEXT:                                                                       1.0,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "l",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           25,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Integer(
// DEFAULT-NEXT:                                                       25,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Binary {
// DEFAULT-NEXT:                                                       op: Div,
// DEFAULT-NEXT:                                                       left: Float(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               value: Double(
// DEFAULT-NEXT:                                                                   1.0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "l",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               else_value: Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "fn3",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Ternary {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: Less,
// DEFAULT-NEXT:                                                               left: Binary {
// DEFAULT-NEXT:                                                                   op: Div,
// DEFAULT-NEXT:                                                                   left: Float(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           value: Double(
// DEFAULT-NEXT:                                                                               1.0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                       "l",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   25,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           then_value: Integer(
// DEFAULT-NEXT:                                                               25,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           else_value: Binary {
// DEFAULT-NEXT:                                                               op: Div,
// DEFAULT-NEXT:                                                               left: Float(
// DEFAULT-NEXT:                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                       value: Double(
// DEFAULT-NEXT:                                                                           1.0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "l",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           Decl(
// DEFAULT-NEXT:                               Declaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Floating(
// DEFAULT-NEXT:                                           Double,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarators: [
// DEFAULT-NEXT:                                       InitDeclarator {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "m",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           initializer: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "j",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "h",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Index {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "j",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               index: Identifier(
// DEFAULT-NEXT:                                                                   "g",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       InitDeclarator {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "n",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           initializer: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Float(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               value: Double(
// DEFAULT-NEXT:                                                                   0.0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       InitDeclarator {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "l",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           initializer: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Float(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               value: Double(
// DEFAULT-NEXT:                                                                   0.0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Greater,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "g",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "j",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "g",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "j",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Binary {
// DEFAULT-NEXT:                                                           op: Sub,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "g",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "h",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "l",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Binary {
// DEFAULT-NEXT:                                                   op: Sub,
// DEFAULT-NEXT:                                                   left: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "j",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "h",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "j",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "h",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Greater,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.02,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Greater,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "m",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.02,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "m",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Greater,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "l",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.02,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "l",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "m",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Decl(
// DEFAULT-NEXT:                                       Declaration {
// DEFAULT-NEXT:                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                               ty: Floating(
// DEFAULT-NEXT:                                                   Double,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           declarators: [
// DEFAULT-NEXT:                                               InitDeclarator {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "o",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   initializer: Some(
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Const(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "m",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "m",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "o",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "l",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Decl(
// DEFAULT-NEXT:                                       Declaration {
// DEFAULT-NEXT:                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                               ty: Floating(
// DEFAULT-NEXT:                                                   Double,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           declarators: [
// DEFAULT-NEXT:                                               InitDeclarator {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "o",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   initializer: Some(
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Const(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "l",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "l",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "n",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "o",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "l",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "m",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Decl(
// DEFAULT-NEXT:                                       Declaration {
// DEFAULT-NEXT:                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                               ty: Floating(
// DEFAULT-NEXT:                                                   Double,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           declarators: [
// DEFAULT-NEXT:                                               InitDeclarator {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "o",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   initializer: Some(
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Const(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "l",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "l",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "m",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "m",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "o",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           If {
// DEFAULT-NEXT:                               condition: Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: NotEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Float(
// DEFAULT-NEXT:                                           FloatLiteral {
// DEFAULT-NEXT:                                               value: Double(
// DEFAULT-NEXT:                                                   0.0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               then_branch: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "k",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Ternary {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: Equal,
// DEFAULT-NEXT:                                                       left: Member {
// DEFAULT-NEXT:                                                           base: Arrow {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "x",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "b4",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           field: "d1",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Ternary {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Div,
// DEFAULT-NEXT:                                                               left: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "m",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               25,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Integer(
// DEFAULT-NEXT:                                                           25,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       else_value: Binary {
// DEFAULT-NEXT:                                                           op: Div,
// DEFAULT-NEXT:                                                           left: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "m",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   else_value: Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "fn3",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [
// DEFAULT-NEXT:                                                           Ternary {
// DEFAULT-NEXT:                                                               condition: Binary {
// DEFAULT-NEXT:                                                                   op: Less,
// DEFAULT-NEXT:                                                                   left: Binary {
// DEFAULT-NEXT:                                                                       op: Div,
// DEFAULT-NEXT:                                                                       left: Integer(
// DEFAULT-NEXT:                                                                           1,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "m",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Integer(
// DEFAULT-NEXT:                                                                       25,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               then_value: Integer(
// DEFAULT-NEXT:                                                                   25,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               else_value: Binary {
// DEFAULT-NEXT:                                                                   op: Div,
// DEFAULT-NEXT:                                                                   left: Integer(
// DEFAULT-NEXT:                                                                       1,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                       "m",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               else_branch: Some(
// DEFAULT-NEXT:                                   [
// DEFAULT-NEXT:                                       If {
// DEFAULT-NEXT:                                           condition: Const(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: NotEqual,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "m",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Float(
// DEFAULT-NEXT:                                                       FloatLiteral {
// DEFAULT-NEXT:                                                           value: Double(
// DEFAULT-NEXT:                                                               0.0,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           then_branch: [
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Const(
// DEFAULT-NEXT:                                                       Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "k",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Ternary {
// DEFAULT-NEXT:                                                               condition: Binary {
// DEFAULT-NEXT:                                                                   op: Equal,
// DEFAULT-NEXT:                                                                   left: Member {
// DEFAULT-NEXT:                                                                       base: Arrow {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "x",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "b4",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       field: "d1",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Integer(
// DEFAULT-NEXT:                                                                       1,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               then_value: Ternary {
// DEFAULT-NEXT:                                                                   condition: Binary {
// DEFAULT-NEXT:                                                                       op: Less,
// DEFAULT-NEXT:                                                                       left: Binary {
// DEFAULT-NEXT:                                                                           op: Div,
// DEFAULT-NEXT:                                                                           left: Integer(
// DEFAULT-NEXT:                                                                               2,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: Identifier(
// DEFAULT-NEXT:                                                                                   "m",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                   "l",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Integer(
// DEFAULT-NEXT:                                                                           25,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   then_value: Integer(
// DEFAULT-NEXT:                                                                       25,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   else_value: Binary {
// DEFAULT-NEXT:                                                                       op: Div,
// DEFAULT-NEXT:                                                                       left: Integer(
// DEFAULT-NEXT:                                                                           2,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                               "m",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                               "l",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               else_value: Call {
// DEFAULT-NEXT:                                                                   callee: Identifier(
// DEFAULT-NEXT:                                                                       "fn3",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   arguments: [
// DEFAULT-NEXT:                                                                       Ternary {
// DEFAULT-NEXT:                                                                           condition: Binary {
// DEFAULT-NEXT:                                                                               op: Less,
// DEFAULT-NEXT:                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                   op: Div,
// DEFAULT-NEXT:                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                       2,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Binary {
// DEFAULT-NEXT:                                                                                       op: Add,
// DEFAULT-NEXT:                                                                                       left: Identifier(
// DEFAULT-NEXT:                                                                                           "m",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                                           "l",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   25,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           then_value: Integer(
// DEFAULT-NEXT:                                                                               25,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           else_value: Binary {
// DEFAULT-NEXT:                                                                               op: Div,
// DEFAULT-NEXT:                                                                               left: Integer(
// DEFAULT-NEXT:                                                                                   2,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                                       "m",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "l",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                           else_branch: Some(
// DEFAULT-NEXT:                                               [
// DEFAULT-NEXT:                                                   If {
// DEFAULT-NEXT:                                                       condition: Const(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: NotEqual,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "l",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Float(
// DEFAULT-NEXT:                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                       value: Double(
// DEFAULT-NEXT:                                                                           0.0,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       then_branch: [
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Const(
// DEFAULT-NEXT:                                                                   Assign {
// DEFAULT-NEXT:                                                                       op: Assign,
// DEFAULT-NEXT:                                                                       target: Identifier(
// DEFAULT-NEXT:                                                                           "k",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       value: Ternary {
// DEFAULT-NEXT:                                                                           condition: Binary {
// DEFAULT-NEXT:                                                                               op: Equal,
// DEFAULT-NEXT:                                                                               left: Member {
// DEFAULT-NEXT:                                                                                   base: Arrow {
// DEFAULT-NEXT:                                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                                           "x",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       field: "b4",
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   field: "d1",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   1,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           then_value: Ternary {
// DEFAULT-NEXT:                                                                               condition: Binary {
// DEFAULT-NEXT:                                                                                   op: Less,
// DEFAULT-NEXT:                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                       op: Div,
// DEFAULT-NEXT:                                                                                       left: Integer(
// DEFAULT-NEXT:                                                                                           1,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                                           "l",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       25,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               then_value: Integer(
// DEFAULT-NEXT:                                                                                   25,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               else_value: Binary {
// DEFAULT-NEXT:                                                                                   op: Div,
// DEFAULT-NEXT:                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "l",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           else_value: Call {
// DEFAULT-NEXT:                                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                                   "fn3",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               arguments: [
// DEFAULT-NEXT:                                                                                   Ternary {
// DEFAULT-NEXT:                                                                                       condition: Binary {
// DEFAULT-NEXT:                                                                                           op: Less,
// DEFAULT-NEXT:                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                               op: Div,
// DEFAULT-NEXT:                                                                                               left: Integer(
// DEFAULT-NEXT:                                                                                                   1,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "l",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                               25,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       then_value: Integer(
// DEFAULT-NEXT:                                                                                           25,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       else_value: Binary {
// DEFAULT-NEXT:                                                                                           op: Div,
// DEFAULT-NEXT:                                                                                           left: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                               "l",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ],
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                       else_branch: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "fn5",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "e",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "c2",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Float(
// DEFAULT-NEXT:                                       FloatLiteral {
// DEFAULT-NEXT:                                           value: Double(
// DEFAULT-NEXT:                                               0.5,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Arrow {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "b2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Arrow {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "b3",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Ternary {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Member {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b4",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           field: "d1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_value: Identifier(
// DEFAULT-NEXT:                                       "k",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   else_value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "fn4",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "k",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 30,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
