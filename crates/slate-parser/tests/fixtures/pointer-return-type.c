char *make_greeting(void) { return "hi"; }

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Pointer {
// DEFAULT-NEXT:               pointee: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           name: "make_greeting",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   StringLit(
// DEFAULT-NEXT:                       "hi",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   0,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: Function(
// DEFAULT-NEXT:       ConcreteFunctionDecl {
// DEFAULT-NEXT:           ret_type: Pointer {
// DEFAULT-NEXT:               pointee: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           name: "make_greeting",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   StringLit(
// DEFAULT-NEXT:                       "hi",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   0,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
