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
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           2,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INNER
// INNER: decl[0]: Function(
// INNER-NEXT:       FunctionDecl {
// INNER-NEXT:           ret_type: Integer(
// INNER-NEXT:               Ranked {
// INNER-NEXT:                   rank: Int,
// INNER-NEXT:                   signed: true,
// INNER-NEXT:               },
// INNER-NEXT:           ),
// INNER-NEXT:           name: "main",
// INNER-NEXT:           body: [
// INNER-NEXT:               Return(
// INNER-NEXT:                   Const(
// INNER-NEXT:                       Integer(
// INNER-NEXT:                           1,
// INNER-NEXT:                       ),
// INNER-NEXT:                   ),
// INNER-NEXT:               ),
// INNER-NEXT:           ],
// INNER-NEXT:           provenance: Provenance {
// INNER-NEXT:               file: FileId(
// INNER-NEXT:                   3,
// INNER-NEXT:               ),
// INNER-NEXT:               kind: User,
// INNER-NEXT:               line: 0,
// INNER-NEXT:           },
// INNER-NEXT:       },
// INNER-NEXT:   )
// SLATE-FILECHECK-END INNER
