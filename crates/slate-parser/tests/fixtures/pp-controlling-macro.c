#ifdef A
#include "controlling-macro.h"
#endif
#include "controlling-macro.h"
#include "controlling-macro.h"
#include "partially-guarded.h"
guarded_t value;
trailing_t trailing;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A
// SLATE-FILECHECK-DEFINES SKIP PARTIALLY_GUARDED_H

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "A",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "guarded_t",
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   3,
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
// DEFAULT-NEXT: decl[1]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "guarded_t",
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   3,
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
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "trailing_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "guarded_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "trailing_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "trailing",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: Typedef {
// DEFAULT-NEXT:       name: "guarded_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Typedef {
// DEFAULT-NEXT:       name: "trailing_t",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "guarded_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "trailing_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "trailing",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               2,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: polyvariant:
// A-NEXT: decl[0]: Conditional(
// A-NEXT:       Conditional {
// A-NEXT:           branches: [
// A-NEXT:               (
// A-NEXT:                   Defined(
// A-NEXT:                       "A",
// A-NEXT:                   ),
// A-NEXT:                   [
// A-NEXT:                       Typedef {
// A-NEXT:                           name: "guarded_t",
// A-NEXT:                           ty: Integer(
// A-NEXT:                               Ranked {
// A-NEXT:                                   rank: Int,
// A-NEXT:                                   signed: true,
// A-NEXT:                               },
// A-NEXT:                           ),
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   3,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 3,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[1]: Conditional(
// A-NEXT:       Conditional {
// A-NEXT:           branches: [
// A-NEXT:               (
// A-NEXT:                   Not(
// A-NEXT:                       Defined(
// A-NEXT:                           "A",
// A-NEXT:                       ),
// A-NEXT:                   ),
// A-NEXT:                   [
// A-NEXT:                       Typedef {
// A-NEXT:                           name: "guarded_t",
// A-NEXT:                           ty: Integer(
// A-NEXT:                               Ranked {
// A-NEXT:                                   rank: Int,
// A-NEXT:                                   signed: true,
// A-NEXT:                               },
// A-NEXT:                           ),
// A-NEXT:                           provenance: Provenance {
// A-NEXT:                               file: FileId(
// A-NEXT:                                   3,
// A-NEXT:                               ),
// A-NEXT:                               kind: User,
// A-NEXT:                               line: 3,
// A-NEXT:                           },
// A-NEXT:                       },
// A-NEXT:                   ],
// A-NEXT:               ),
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[2]: Typedef {
// A-NEXT:       name: "trailing_t",
// A-NEXT:       ty: Integer(
// A-NEXT:           Ranked {
// A-NEXT:               rank: Int,
// A-NEXT:               signed: true,
// A-NEXT:           },
// A-NEXT:       ),
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               4,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 4,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[3]: Declaration {
// A-NEXT:       declaration: Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Named(
// A-NEXT:                   "guarded_t",
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarator: Name(
// A-NEXT:               "value",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 6,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[4]: Declaration {
// A-NEXT:       declaration: Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Named(
// A-NEXT:                   "trailing_t",
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarator: Name(
// A-NEXT:               "trailing",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 7,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: concrete:
// A-NEXT: decl[0]: Typedef {
// A-NEXT:       name: "guarded_t",
// A-NEXT:       ty: Integer(
// A-NEXT:           Ranked {
// A-NEXT:               rank: Int,
// A-NEXT:               signed: true,
// A-NEXT:           },
// A-NEXT:       ),
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 3,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[1]: Typedef {
// A-NEXT:       name: "trailing_t",
// A-NEXT:       ty: Integer(
// A-NEXT:           Ranked {
// A-NEXT:               rank: Int,
// A-NEXT:               signed: true,
// A-NEXT:           },
// A-NEXT:       ),
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               4,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 4,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[2]: Declaration {
// A-NEXT:       declaration: Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Named(
// A-NEXT:                   "guarded_t",
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarator: Name(
// A-NEXT:               "value",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 6,
// A-NEXT:       },
// A-NEXT:   }
// A-NEXT: decl[3]: Declaration {
// A-NEXT:       declaration: Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Named(
// A-NEXT:                   "trailing_t",
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarator: Name(
// A-NEXT:               "trailing",
// A-NEXT:           ),
// A-NEXT:       },
// A-NEXT:       provenance: Provenance {
// A-NEXT:           file: FileId(
// A-NEXT:               2,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 7,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN SKIP
// SKIP: polyvariant:
// SKIP-NEXT: decl[0]: Conditional(
// SKIP-NEXT:       Conditional {
// SKIP-NEXT:           branches: [
// SKIP-NEXT:               (
// SKIP-NEXT:                   Defined(
// SKIP-NEXT:                       "A",
// SKIP-NEXT:                   ),
// SKIP-NEXT:                   [
// SKIP-NEXT:                       Typedef {
// SKIP-NEXT:                           name: "guarded_t",
// SKIP-NEXT:                           ty: Integer(
// SKIP-NEXT:                               Ranked {
// SKIP-NEXT:                                   rank: Int,
// SKIP-NEXT:                                   signed: true,
// SKIP-NEXT:                               },
// SKIP-NEXT:                           ),
// SKIP-NEXT:                           provenance: Provenance {
// SKIP-NEXT:                               file: FileId(
// SKIP-NEXT:                                   3,
// SKIP-NEXT:                               ),
// SKIP-NEXT:                               kind: User,
// SKIP-NEXT:                               line: 3,
// SKIP-NEXT:                           },
// SKIP-NEXT:                       },
// SKIP-NEXT:                   ],
// SKIP-NEXT:               ),
// SKIP-NEXT:           ],
// SKIP-NEXT:       },
// SKIP-NEXT:   )
// SKIP-NEXT: decl[1]: Conditional(
// SKIP-NEXT:       Conditional {
// SKIP-NEXT:           branches: [
// SKIP-NEXT:               (
// SKIP-NEXT:                   Not(
// SKIP-NEXT:                       Defined(
// SKIP-NEXT:                           "A",
// SKIP-NEXT:                       ),
// SKIP-NEXT:                   ),
// SKIP-NEXT:                   [
// SKIP-NEXT:                       Typedef {
// SKIP-NEXT:                           name: "guarded_t",
// SKIP-NEXT:                           ty: Integer(
// SKIP-NEXT:                               Ranked {
// SKIP-NEXT:                                   rank: Int,
// SKIP-NEXT:                                   signed: true,
// SKIP-NEXT:                               },
// SKIP-NEXT:                           ),
// SKIP-NEXT:                           provenance: Provenance {
// SKIP-NEXT:                               file: FileId(
// SKIP-NEXT:                                   3,
// SKIP-NEXT:                               ),
// SKIP-NEXT:                               kind: User,
// SKIP-NEXT:                               line: 3,
// SKIP-NEXT:                           },
// SKIP-NEXT:                       },
// SKIP-NEXT:                   ],
// SKIP-NEXT:               ),
// SKIP-NEXT:           ],
// SKIP-NEXT:       },
// SKIP-NEXT:   )
// SKIP-NEXT: decl[2]: Typedef {
// SKIP-NEXT:       name: "trailing_t",
// SKIP-NEXT:       ty: Integer(
// SKIP-NEXT:           Ranked {
// SKIP-NEXT:               rank: Int,
// SKIP-NEXT:               signed: true,
// SKIP-NEXT:           },
// SKIP-NEXT:       ),
// SKIP-NEXT:       provenance: Provenance {
// SKIP-NEXT:           file: FileId(
// SKIP-NEXT:               4,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 4,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SKIP-NEXT: decl[3]: Declaration {
// SKIP-NEXT:       declaration: Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Named(
// SKIP-NEXT:                   "guarded_t",
// SKIP-NEXT:               ),
// SKIP-NEXT:           },
// SKIP-NEXT:           declarator: Name(
// SKIP-NEXT:               "value",
// SKIP-NEXT:           ),
// SKIP-NEXT:       },
// SKIP-NEXT:       provenance: Provenance {
// SKIP-NEXT:           file: FileId(
// SKIP-NEXT:               2,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 6,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SKIP-NEXT: decl[4]: Declaration {
// SKIP-NEXT:       declaration: Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Named(
// SKIP-NEXT:                   "trailing_t",
// SKIP-NEXT:               ),
// SKIP-NEXT:           },
// SKIP-NEXT:           declarator: Name(
// SKIP-NEXT:               "trailing",
// SKIP-NEXT:           ),
// SKIP-NEXT:       },
// SKIP-NEXT:       provenance: Provenance {
// SKIP-NEXT:           file: FileId(
// SKIP-NEXT:               2,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 7,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SKIP-NEXT: concrete:
// SKIP-NEXT: decl[0]: Typedef {
// SKIP-NEXT:       name: "guarded_t",
// SKIP-NEXT:       ty: Integer(
// SKIP-NEXT:           Ranked {
// SKIP-NEXT:               rank: Int,
// SKIP-NEXT:               signed: true,
// SKIP-NEXT:           },
// SKIP-NEXT:       ),
// SKIP-NEXT:       provenance: Provenance {
// SKIP-NEXT:           file: FileId(
// SKIP-NEXT:               3,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 3,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SKIP-NEXT: decl[1]: Typedef {
// SKIP-NEXT:       name: "trailing_t",
// SKIP-NEXT:       ty: Integer(
// SKIP-NEXT:           Ranked {
// SKIP-NEXT:               rank: Int,
// SKIP-NEXT:               signed: true,
// SKIP-NEXT:           },
// SKIP-NEXT:       ),
// SKIP-NEXT:       provenance: Provenance {
// SKIP-NEXT:           file: FileId(
// SKIP-NEXT:               4,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 4,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SKIP-NEXT: decl[2]: Declaration {
// SKIP-NEXT:       declaration: Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Named(
// SKIP-NEXT:                   "guarded_t",
// SKIP-NEXT:               ),
// SKIP-NEXT:           },
// SKIP-NEXT:           declarator: Name(
// SKIP-NEXT:               "value",
// SKIP-NEXT:           ),
// SKIP-NEXT:       },
// SKIP-NEXT:       provenance: Provenance {
// SKIP-NEXT:           file: FileId(
// SKIP-NEXT:               2,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 6,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SKIP-NEXT: decl[3]: Declaration {
// SKIP-NEXT:       declaration: Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Named(
// SKIP-NEXT:                   "trailing_t",
// SKIP-NEXT:               ),
// SKIP-NEXT:           },
// SKIP-NEXT:           declarator: Name(
// SKIP-NEXT:               "trailing",
// SKIP-NEXT:           ),
// SKIP-NEXT:       },
// SKIP-NEXT:       provenance: Provenance {
// SKIP-NEXT:           file: FileId(
// SKIP-NEXT:               2,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 7,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SLATE-FILECHECK-END SKIP
