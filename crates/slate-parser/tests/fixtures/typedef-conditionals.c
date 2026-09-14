#ifdef USE_INT
typedef int Value;
#else
typedef char Value;
#endif

Value value;

#ifdef ONLY_LEFT
typedef int LeftOnly;
LeftOnly left_value;
#else
typedef int RightOnly;
RightOnly right_value;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES INT USE_INT
// SLATE-FILECHECK-DEFINES LEFT ONLY_LEFT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "Value",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "Value",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "value",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "RightOnly",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "RightOnly",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "right_value",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INT
// INT: decl[0]: Declaration(
// INT-NEXT:       Declaration {
// INT-NEXT:           specifiers: DeclarationSpecifiers {
// INT-NEXT:               ty: Integer(
// INT-NEXT:                   Ranked {
// INT-NEXT:                       rank: Int,
// INT-NEXT:                       signed: true,
// INT-NEXT:                   },
// INT-NEXT:               ),
// INT-NEXT:               storage: Typedef,
// INT-NEXT:           },
// INT-NEXT:           declarators: [
// INT-NEXT:               InitDeclaratorKind {
// INT-NEXT:                   declarator: Name(
// INT-NEXT:                       "Value",
// INT-NEXT:                   ),
// INT-NEXT:               },
// INT-NEXT:           ],
// INT-NEXT:       },
// INT-NEXT:   )
// INT-NEXT: decl[1]: Declaration(
// INT-NEXT:       Declaration {
// INT-NEXT:           specifiers: DeclarationSpecifiers {
// INT-NEXT:               ty: Named(
// INT-NEXT:                   "Value",
// INT-NEXT:               ),
// INT-NEXT:           },
// INT-NEXT:           declarators: [
// INT-NEXT:               InitDeclaratorKind {
// INT-NEXT:                   declarator: Name(
// INT-NEXT:                       "value",
// INT-NEXT:                   ),
// INT-NEXT:               },
// INT-NEXT:           ],
// INT-NEXT:       },
// INT-NEXT:   )
// INT-NEXT: decl[2]: Declaration(
// INT-NEXT:       Declaration {
// INT-NEXT:           specifiers: DeclarationSpecifiers {
// INT-NEXT:               ty: Integer(
// INT-NEXT:                   Ranked {
// INT-NEXT:                       rank: Int,
// INT-NEXT:                       signed: true,
// INT-NEXT:                   },
// INT-NEXT:               ),
// INT-NEXT:               storage: Typedef,
// INT-NEXT:           },
// INT-NEXT:           declarators: [
// INT-NEXT:               InitDeclaratorKind {
// INT-NEXT:                   declarator: Name(
// INT-NEXT:                       "RightOnly",
// INT-NEXT:                   ),
// INT-NEXT:               },
// INT-NEXT:           ],
// INT-NEXT:       },
// INT-NEXT:   )
// INT-NEXT: decl[3]: Declaration(
// INT-NEXT:       Declaration {
// INT-NEXT:           specifiers: DeclarationSpecifiers {
// INT-NEXT:               ty: Named(
// INT-NEXT:                   "RightOnly",
// INT-NEXT:               ),
// INT-NEXT:           },
// INT-NEXT:           declarators: [
// INT-NEXT:               InitDeclaratorKind {
// INT-NEXT:                   declarator: Name(
// INT-NEXT:                       "right_value",
// INT-NEXT:                   ),
// INT-NEXT:               },
// INT-NEXT:           ],
// INT-NEXT:       },
// INT-NEXT:   )
// SLATE-FILECHECK-END INT
// SLATE-FILECHECK-BEGIN LEFT
// LEFT: decl[0]: Declaration(
// LEFT-NEXT:       Declaration {
// LEFT-NEXT:           specifiers: DeclarationSpecifiers {
// LEFT-NEXT:               ty: Integer(
// LEFT-NEXT:                   Char {
// LEFT-NEXT:                       signed: None,
// LEFT-NEXT:                   },
// LEFT-NEXT:               ),
// LEFT-NEXT:               storage: Typedef,
// LEFT-NEXT:           },
// LEFT-NEXT:           declarators: [
// LEFT-NEXT:               InitDeclaratorKind {
// LEFT-NEXT:                   declarator: Name(
// LEFT-NEXT:                       "Value",
// LEFT-NEXT:                   ),
// LEFT-NEXT:               },
// LEFT-NEXT:           ],
// LEFT-NEXT:       },
// LEFT-NEXT:   )
// LEFT-NEXT: decl[1]: Declaration(
// LEFT-NEXT:       Declaration {
// LEFT-NEXT:           specifiers: DeclarationSpecifiers {
// LEFT-NEXT:               ty: Named(
// LEFT-NEXT:                   "Value",
// LEFT-NEXT:               ),
// LEFT-NEXT:           },
// LEFT-NEXT:           declarators: [
// LEFT-NEXT:               InitDeclaratorKind {
// LEFT-NEXT:                   declarator: Name(
// LEFT-NEXT:                       "value",
// LEFT-NEXT:                   ),
// LEFT-NEXT:               },
// LEFT-NEXT:           ],
// LEFT-NEXT:       },
// LEFT-NEXT:   )
// LEFT-NEXT: decl[2]: Declaration(
// LEFT-NEXT:       Declaration {
// LEFT-NEXT:           specifiers: DeclarationSpecifiers {
// LEFT-NEXT:               ty: Integer(
// LEFT-NEXT:                   Ranked {
// LEFT-NEXT:                       rank: Int,
// LEFT-NEXT:                       signed: true,
// LEFT-NEXT:                   },
// LEFT-NEXT:               ),
// LEFT-NEXT:               storage: Typedef,
// LEFT-NEXT:           },
// LEFT-NEXT:           declarators: [
// LEFT-NEXT:               InitDeclaratorKind {
// LEFT-NEXT:                   declarator: Name(
// LEFT-NEXT:                       "LeftOnly",
// LEFT-NEXT:                   ),
// LEFT-NEXT:               },
// LEFT-NEXT:           ],
// LEFT-NEXT:       },
// LEFT-NEXT:   )
// LEFT-NEXT: decl[3]: Declaration(
// LEFT-NEXT:       Declaration {
// LEFT-NEXT:           specifiers: DeclarationSpecifiers {
// LEFT-NEXT:               ty: Named(
// LEFT-NEXT:                   "LeftOnly",
// LEFT-NEXT:               ),
// LEFT-NEXT:           },
// LEFT-NEXT:           declarators: [
// LEFT-NEXT:               InitDeclaratorKind {
// LEFT-NEXT:                   declarator: Name(
// LEFT-NEXT:                       "left_value",
// LEFT-NEXT:                   ),
// LEFT-NEXT:               },
// LEFT-NEXT:           ],
// LEFT-NEXT:       },
// LEFT-NEXT:   )
// SLATE-FILECHECK-END LEFT
