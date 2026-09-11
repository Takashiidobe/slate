int generic_value() {
  return _Generic(1, int: 1, default: 0);
}

int statement_value() {
  ({ 1; });
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: function name=generic_value return=int
// DEFAULT-NEXT:   stmt[0]: return _Generic(1, 2 )
// DEFAULT-NEXT: decl[1]: function name=statement_value return=int
// DEFAULT-NEXT:   stmt[0]: expression=StatementExpression([Expr(IntLit(1))])
// DEFAULT-NEXT:   stmt[1]: return 0
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: function name=generic_value return=int
// DEFAULT-NEXT:   stmt[0]: return _Generic(1, 2 )
// DEFAULT-NEXT: decl[1]: function name=statement_value return=int
// DEFAULT-NEXT:   stmt[0]: expression=StatementExpression([Expr(IntLit(1))])
// DEFAULT-NEXT:   stmt[1]: return 0
// SLATE-FILECHECK-END DEFAULT
