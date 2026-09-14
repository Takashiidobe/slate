int main() {
#if 0
  return 0;
#elif 1
#ifdef INNER
  return 1;
#else
  return 2;
#endif
#else
  return 3;
#endif
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES INNER INNER

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 2,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "2",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INNER
// INNER: decl[0]: Function(
// INNER-NEXT:       FunctionDefinition {
// INNER-NEXT:           specifiers: DeclarationSpecifiers {
// INNER-NEXT:               ty: Integer(
// INNER-NEXT:                   Ranked {
// INNER-NEXT:                       rank: Int,
// INNER-NEXT:                       signed: true,
// INNER-NEXT:                   },
// INNER-NEXT:               ),
// INNER-NEXT:           },
// INNER-NEXT:           declarator: Function {
// INNER-NEXT:               inner: Name(
// INNER-NEXT:                   "main",
// INNER-NEXT:               ),
// INNER-NEXT:               parameters: Empty,
// INNER-NEXT:           },
// INNER-NEXT:           body: [
// INNER-NEXT:               Return(
// INNER-NEXT:                   IntegerLiteral(
// INNER-NEXT:                       IntegerLiteral {
// INNER-NEXT:                           value: 1,
// INNER-NEXT:                           radix: Decimal,
// INNER-NEXT:                           suffix: IntegerSuffix {
// INNER-NEXT:                               unsigned: false,
// INNER-NEXT:                               size: None,
// INNER-NEXT:                           },
// INNER-NEXT:                           spelling: "1",
// INNER-NEXT:                       },
// INNER-NEXT:                   ),
// INNER-NEXT:               ),
// INNER-NEXT:           ],
// INNER-NEXT:           provenance: Provenance {
// INNER-NEXT:               file: FileId(
// INNER-NEXT:                   3,
// INNER-NEXT:               ),
// INNER-NEXT:               kind: User,
// INNER-NEXT:               line: 0,
// INNER-NEXT:               header: None,
// INNER-NEXT:           },
// INNER-NEXT:       },
// INNER-NEXT:   )
// SLATE-FILECHECK-END INNER
