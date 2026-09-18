// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-require-effective-target label_values } */

int
main()
{
l1:
  return &&l1-&&l2;
l2:;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[{{[0-9]+}}]: Function(
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
// DEFAULT-NEXT:               Labeled {
// DEFAULT-NEXT:                   label: "l1",
// DEFAULT-NEXT:                   body: Return(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Sub,
// DEFAULT-NEXT:                           left: LabelAddress(
// DEFAULT-NEXT:                               "l1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: LabelAddress(
// DEFAULT-NEXT:                               "l2",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Labeled {
// DEFAULT-NEXT:                   label: "l2",
// DEFAULT-NEXT:                   body: Null,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
