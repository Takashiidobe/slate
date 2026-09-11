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
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "USE_INT",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "Value",
// DEFAULT-NEXT:                           ty: Int,
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 1,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "USE_INT",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "Value",
// DEFAULT-NEXT:                           ty: Char,
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 3,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "Value",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "ONLY_LEFT",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "LeftOnly",
// DEFAULT-NEXT:                           ty: Int,
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 9,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "LeftOnly",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "left_value",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 10,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "ONLY_LEFT",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "RightOnly",
// DEFAULT-NEXT:                           ty: Int,
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 12,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "RightOnly",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "right_value",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 13,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: Typedef {
// DEFAULT-NEXT:       name: "Value",
// DEFAULT-NEXT:       ty: Char,
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "Value",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "RightOnly",
// DEFAULT-NEXT:       ty: Int,
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 12,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "RightOnly",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "right_value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 13,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INT
// INT: polyvariant:
// INT-NEXT: decl[0]: Conditional(
// INT-NEXT:       Conditional {
// INT-NEXT:           branches: [
// INT-NEXT:               (
// INT-NEXT:                   Defined(
// INT-NEXT:                       "USE_INT",
// INT-NEXT:                   ),
// INT-NEXT:                   [
// INT-NEXT:                       Typedef {
// INT-NEXT:                           name: "Value",
// INT-NEXT:                           ty: Int,
// INT-NEXT:                           provenance: Provenance {
// INT-NEXT:                               file: FileId(
// INT-NEXT:                                   0,
// INT-NEXT:                               ),
// INT-NEXT:                               kind: User,
// INT-NEXT:                               line: 1,
// INT-NEXT:                           },
// INT-NEXT:                       },
// INT-NEXT:                   ],
// INT-NEXT:               ),
// INT-NEXT:               (
// INT-NEXT:                   Not(
// INT-NEXT:                       Defined(
// INT-NEXT:                           "USE_INT",
// INT-NEXT:                       ),
// INT-NEXT:                   ),
// INT-NEXT:                   [
// INT-NEXT:                       Typedef {
// INT-NEXT:                           name: "Value",
// INT-NEXT:                           ty: Char,
// INT-NEXT:                           provenance: Provenance {
// INT-NEXT:                               file: FileId(
// INT-NEXT:                                   0,
// INT-NEXT:                               ),
// INT-NEXT:                               kind: User,
// INT-NEXT:                               line: 3,
// INT-NEXT:                           },
// INT-NEXT:                       },
// INT-NEXT:                   ],
// INT-NEXT:               ),
// INT-NEXT:           ],
// INT-NEXT:       },
// INT-NEXT:   )
// INT-NEXT: decl[1]: Declaration {
// INT-NEXT:       declaration: Declaration {
// INT-NEXT:           specifiers: DeclarationSpecifiers {
// INT-NEXT:               ty: Named(
// INT-NEXT:                   "Value",
// INT-NEXT:               ),
// INT-NEXT:           },
// INT-NEXT:           declarator: Name(
// INT-NEXT:               "value",
// INT-NEXT:           ),
// INT-NEXT:       },
// INT-NEXT:       provenance: Provenance {
// INT-NEXT:           file: FileId(
// INT-NEXT:               0,
// INT-NEXT:           ),
// INT-NEXT:           kind: User,
// INT-NEXT:           line: 6,
// INT-NEXT:       },
// INT-NEXT:   }
// INT-NEXT: decl[2]: Conditional(
// INT-NEXT:       Conditional {
// INT-NEXT:           branches: [
// INT-NEXT:               (
// INT-NEXT:                   Defined(
// INT-NEXT:                       "ONLY_LEFT",
// INT-NEXT:                   ),
// INT-NEXT:                   [
// INT-NEXT:                       Typedef {
// INT-NEXT:                           name: "LeftOnly",
// INT-NEXT:                           ty: Int,
// INT-NEXT:                           provenance: Provenance {
// INT-NEXT:                               file: FileId(
// INT-NEXT:                                   0,
// INT-NEXT:                               ),
// INT-NEXT:                               kind: User,
// INT-NEXT:                               line: 9,
// INT-NEXT:                           },
// INT-NEXT:                       },
// INT-NEXT:                       Declaration {
// INT-NEXT:                           declaration: Declaration {
// INT-NEXT:                               specifiers: DeclarationSpecifiers {
// INT-NEXT:                                   ty: Named(
// INT-NEXT:                                       "LeftOnly",
// INT-NEXT:                                   ),
// INT-NEXT:                               },
// INT-NEXT:                               declarator: Name(
// INT-NEXT:                                   "left_value",
// INT-NEXT:                               ),
// INT-NEXT:                           },
// INT-NEXT:                           provenance: Provenance {
// INT-NEXT:                               file: FileId(
// INT-NEXT:                                   0,
// INT-NEXT:                               ),
// INT-NEXT:                               kind: User,
// INT-NEXT:                               line: 10,
// INT-NEXT:                           },
// INT-NEXT:                       },
// INT-NEXT:                   ],
// INT-NEXT:               ),
// INT-NEXT:               (
// INT-NEXT:                   Not(
// INT-NEXT:                       Defined(
// INT-NEXT:                           "ONLY_LEFT",
// INT-NEXT:                       ),
// INT-NEXT:                   ),
// INT-NEXT:                   [
// INT-NEXT:                       Typedef {
// INT-NEXT:                           name: "RightOnly",
// INT-NEXT:                           ty: Int,
// INT-NEXT:                           provenance: Provenance {
// INT-NEXT:                               file: FileId(
// INT-NEXT:                                   0,
// INT-NEXT:                               ),
// INT-NEXT:                               kind: User,
// INT-NEXT:                               line: 12,
// INT-NEXT:                           },
// INT-NEXT:                       },
// INT-NEXT:                       Declaration {
// INT-NEXT:                           declaration: Declaration {
// INT-NEXT:                               specifiers: DeclarationSpecifiers {
// INT-NEXT:                                   ty: Named(
// INT-NEXT:                                       "RightOnly",
// INT-NEXT:                                   ),
// INT-NEXT:                               },
// INT-NEXT:                               declarator: Name(
// INT-NEXT:                                   "right_value",
// INT-NEXT:                               ),
// INT-NEXT:                           },
// INT-NEXT:                           provenance: Provenance {
// INT-NEXT:                               file: FileId(
// INT-NEXT:                                   0,
// INT-NEXT:                               ),
// INT-NEXT:                               kind: User,
// INT-NEXT:                               line: 13,
// INT-NEXT:                           },
// INT-NEXT:                       },
// INT-NEXT:                   ],
// INT-NEXT:               ),
// INT-NEXT:           ],
// INT-NEXT:       },
// INT-NEXT:   )
// INT-NEXT: concrete:
// INT-NEXT: decl[0]: Typedef {
// INT-NEXT:       name: "Value",
// INT-NEXT:       ty: Int,
// INT-NEXT:       provenance: Provenance {
// INT-NEXT:           file: FileId(
// INT-NEXT:               0,
// INT-NEXT:           ),
// INT-NEXT:           kind: User,
// INT-NEXT:           line: 1,
// INT-NEXT:       },
// INT-NEXT:   }
// INT-NEXT: decl[1]: Declaration {
// INT-NEXT:       declaration: Declaration {
// INT-NEXT:           specifiers: DeclarationSpecifiers {
// INT-NEXT:               ty: Named(
// INT-NEXT:                   "Value",
// INT-NEXT:               ),
// INT-NEXT:           },
// INT-NEXT:           declarator: Name(
// INT-NEXT:               "value",
// INT-NEXT:           ),
// INT-NEXT:       },
// INT-NEXT:       provenance: Provenance {
// INT-NEXT:           file: FileId(
// INT-NEXT:               0,
// INT-NEXT:           ),
// INT-NEXT:           kind: User,
// INT-NEXT:           line: 6,
// INT-NEXT:       },
// INT-NEXT:   }
// INT-NEXT: decl[2]: Typedef {
// INT-NEXT:       name: "RightOnly",
// INT-NEXT:       ty: Int,
// INT-NEXT:       provenance: Provenance {
// INT-NEXT:           file: FileId(
// INT-NEXT:               0,
// INT-NEXT:           ),
// INT-NEXT:           kind: User,
// INT-NEXT:           line: 12,
// INT-NEXT:       },
// INT-NEXT:   }
// INT-NEXT: decl[3]: Declaration {
// INT-NEXT:       declaration: Declaration {
// INT-NEXT:           specifiers: DeclarationSpecifiers {
// INT-NEXT:               ty: Named(
// INT-NEXT:                   "RightOnly",
// INT-NEXT:               ),
// INT-NEXT:           },
// INT-NEXT:           declarator: Name(
// INT-NEXT:               "right_value",
// INT-NEXT:           ),
// INT-NEXT:       },
// INT-NEXT:       provenance: Provenance {
// INT-NEXT:           file: FileId(
// INT-NEXT:               0,
// INT-NEXT:           ),
// INT-NEXT:           kind: User,
// INT-NEXT:           line: 13,
// INT-NEXT:       },
// INT-NEXT:   }
// SLATE-FILECHECK-END INT
// SLATE-FILECHECK-BEGIN LEFT
// LEFT: polyvariant:
// LEFT-NEXT: decl[0]: Conditional(
// LEFT-NEXT:       Conditional {
// LEFT-NEXT:           branches: [
// LEFT-NEXT:               (
// LEFT-NEXT:                   Defined(
// LEFT-NEXT:                       "USE_INT",
// LEFT-NEXT:                   ),
// LEFT-NEXT:                   [
// LEFT-NEXT:                       Typedef {
// LEFT-NEXT:                           name: "Value",
// LEFT-NEXT:                           ty: Int,
// LEFT-NEXT:                           provenance: Provenance {
// LEFT-NEXT:                               file: FileId(
// LEFT-NEXT:                                   0,
// LEFT-NEXT:                               ),
// LEFT-NEXT:                               kind: User,
// LEFT-NEXT:                               line: 1,
// LEFT-NEXT:                           },
// LEFT-NEXT:                       },
// LEFT-NEXT:                   ],
// LEFT-NEXT:               ),
// LEFT-NEXT:               (
// LEFT-NEXT:                   Not(
// LEFT-NEXT:                       Defined(
// LEFT-NEXT:                           "USE_INT",
// LEFT-NEXT:                       ),
// LEFT-NEXT:                   ),
// LEFT-NEXT:                   [
// LEFT-NEXT:                       Typedef {
// LEFT-NEXT:                           name: "Value",
// LEFT-NEXT:                           ty: Char,
// LEFT-NEXT:                           provenance: Provenance {
// LEFT-NEXT:                               file: FileId(
// LEFT-NEXT:                                   0,
// LEFT-NEXT:                               ),
// LEFT-NEXT:                               kind: User,
// LEFT-NEXT:                               line: 3,
// LEFT-NEXT:                           },
// LEFT-NEXT:                       },
// LEFT-NEXT:                   ],
// LEFT-NEXT:               ),
// LEFT-NEXT:           ],
// LEFT-NEXT:       },
// LEFT-NEXT:   )
// LEFT-NEXT: decl[1]: Declaration {
// LEFT-NEXT:       declaration: Declaration {
// LEFT-NEXT:           specifiers: DeclarationSpecifiers {
// LEFT-NEXT:               ty: Named(
// LEFT-NEXT:                   "Value",
// LEFT-NEXT:               ),
// LEFT-NEXT:           },
// LEFT-NEXT:           declarator: Name(
// LEFT-NEXT:               "value",
// LEFT-NEXT:           ),
// LEFT-NEXT:       },
// LEFT-NEXT:       provenance: Provenance {
// LEFT-NEXT:           file: FileId(
// LEFT-NEXT:               0,
// LEFT-NEXT:           ),
// LEFT-NEXT:           kind: User,
// LEFT-NEXT:           line: 6,
// LEFT-NEXT:       },
// LEFT-NEXT:   }
// LEFT-NEXT: decl[2]: Conditional(
// LEFT-NEXT:       Conditional {
// LEFT-NEXT:           branches: [
// LEFT-NEXT:               (
// LEFT-NEXT:                   Defined(
// LEFT-NEXT:                       "ONLY_LEFT",
// LEFT-NEXT:                   ),
// LEFT-NEXT:                   [
// LEFT-NEXT:                       Typedef {
// LEFT-NEXT:                           name: "LeftOnly",
// LEFT-NEXT:                           ty: Int,
// LEFT-NEXT:                           provenance: Provenance {
// LEFT-NEXT:                               file: FileId(
// LEFT-NEXT:                                   0,
// LEFT-NEXT:                               ),
// LEFT-NEXT:                               kind: User,
// LEFT-NEXT:                               line: 9,
// LEFT-NEXT:                           },
// LEFT-NEXT:                       },
// LEFT-NEXT:                       Declaration {
// LEFT-NEXT:                           declaration: Declaration {
// LEFT-NEXT:                               specifiers: DeclarationSpecifiers {
// LEFT-NEXT:                                   ty: Named(
// LEFT-NEXT:                                       "LeftOnly",
// LEFT-NEXT:                                   ),
// LEFT-NEXT:                               },
// LEFT-NEXT:                               declarator: Name(
// LEFT-NEXT:                                   "left_value",
// LEFT-NEXT:                               ),
// LEFT-NEXT:                           },
// LEFT-NEXT:                           provenance: Provenance {
// LEFT-NEXT:                               file: FileId(
// LEFT-NEXT:                                   0,
// LEFT-NEXT:                               ),
// LEFT-NEXT:                               kind: User,
// LEFT-NEXT:                               line: 10,
// LEFT-NEXT:                           },
// LEFT-NEXT:                       },
// LEFT-NEXT:                   ],
// LEFT-NEXT:               ),
// LEFT-NEXT:               (
// LEFT-NEXT:                   Not(
// LEFT-NEXT:                       Defined(
// LEFT-NEXT:                           "ONLY_LEFT",
// LEFT-NEXT:                       ),
// LEFT-NEXT:                   ),
// LEFT-NEXT:                   [
// LEFT-NEXT:                       Typedef {
// LEFT-NEXT:                           name: "RightOnly",
// LEFT-NEXT:                           ty: Int,
// LEFT-NEXT:                           provenance: Provenance {
// LEFT-NEXT:                               file: FileId(
// LEFT-NEXT:                                   0,
// LEFT-NEXT:                               ),
// LEFT-NEXT:                               kind: User,
// LEFT-NEXT:                               line: 12,
// LEFT-NEXT:                           },
// LEFT-NEXT:                       },
// LEFT-NEXT:                       Declaration {
// LEFT-NEXT:                           declaration: Declaration {
// LEFT-NEXT:                               specifiers: DeclarationSpecifiers {
// LEFT-NEXT:                                   ty: Named(
// LEFT-NEXT:                                       "RightOnly",
// LEFT-NEXT:                                   ),
// LEFT-NEXT:                               },
// LEFT-NEXT:                               declarator: Name(
// LEFT-NEXT:                                   "right_value",
// LEFT-NEXT:                               ),
// LEFT-NEXT:                           },
// LEFT-NEXT:                           provenance: Provenance {
// LEFT-NEXT:                               file: FileId(
// LEFT-NEXT:                                   0,
// LEFT-NEXT:                               ),
// LEFT-NEXT:                               kind: User,
// LEFT-NEXT:                               line: 13,
// LEFT-NEXT:                           },
// LEFT-NEXT:                       },
// LEFT-NEXT:                   ],
// LEFT-NEXT:               ),
// LEFT-NEXT:           ],
// LEFT-NEXT:       },
// LEFT-NEXT:   )
// LEFT-NEXT: concrete:
// LEFT-NEXT: decl[0]: Typedef {
// LEFT-NEXT:       name: "Value",
// LEFT-NEXT:       ty: Char,
// LEFT-NEXT:       provenance: Provenance {
// LEFT-NEXT:           file: FileId(
// LEFT-NEXT:               0,
// LEFT-NEXT:           ),
// LEFT-NEXT:           kind: User,
// LEFT-NEXT:           line: 3,
// LEFT-NEXT:       },
// LEFT-NEXT:   }
// LEFT-NEXT: decl[1]: Declaration {
// LEFT-NEXT:       declaration: Declaration {
// LEFT-NEXT:           specifiers: DeclarationSpecifiers {
// LEFT-NEXT:               ty: Named(
// LEFT-NEXT:                   "Value",
// LEFT-NEXT:               ),
// LEFT-NEXT:           },
// LEFT-NEXT:           declarator: Name(
// LEFT-NEXT:               "value",
// LEFT-NEXT:           ),
// LEFT-NEXT:       },
// LEFT-NEXT:       provenance: Provenance {
// LEFT-NEXT:           file: FileId(
// LEFT-NEXT:               0,
// LEFT-NEXT:           ),
// LEFT-NEXT:           kind: User,
// LEFT-NEXT:           line: 6,
// LEFT-NEXT:       },
// LEFT-NEXT:   }
// LEFT-NEXT: decl[2]: Typedef {
// LEFT-NEXT:       name: "LeftOnly",
// LEFT-NEXT:       ty: Int,
// LEFT-NEXT:       provenance: Provenance {
// LEFT-NEXT:           file: FileId(
// LEFT-NEXT:               0,
// LEFT-NEXT:           ),
// LEFT-NEXT:           kind: User,
// LEFT-NEXT:           line: 9,
// LEFT-NEXT:       },
// LEFT-NEXT:   }
// LEFT-NEXT: decl[3]: Declaration {
// LEFT-NEXT:       declaration: Declaration {
// LEFT-NEXT:           specifiers: DeclarationSpecifiers {
// LEFT-NEXT:               ty: Named(
// LEFT-NEXT:                   "LeftOnly",
// LEFT-NEXT:               ),
// LEFT-NEXT:           },
// LEFT-NEXT:           declarator: Name(
// LEFT-NEXT:               "left_value",
// LEFT-NEXT:           ),
// LEFT-NEXT:       },
// LEFT-NEXT:       provenance: Provenance {
// LEFT-NEXT:           file: FileId(
// LEFT-NEXT:               0,
// LEFT-NEXT:           ),
// LEFT-NEXT:           kind: User,
// LEFT-NEXT:           line: 10,
// LEFT-NEXT:       },
// LEFT-NEXT:   }
// SLATE-FILECHECK-END LEFT
