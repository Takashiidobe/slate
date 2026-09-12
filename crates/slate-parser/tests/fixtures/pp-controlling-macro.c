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
// DEFAULT: decl[0]: Typedef {
// DEFAULT-NEXT:       name: "guarded_t",
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
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:               5,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   5,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
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
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: decl[0]: Typedef {
// A-NEXT:       name: "guarded_t",
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
// A-NEXT:           line: 3,
// A-NEXT:           header: Some(
// A-NEXT:               FileId(
// A-NEXT:                   4,
// A-NEXT:               ),
// A-NEXT:           ),
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
// A-NEXT:               5,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 4,
// A-NEXT:           header: Some(
// A-NEXT:               FileId(
// A-NEXT:                   5,
// A-NEXT:               ),
// A-NEXT:           ),
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
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 6,
// A-NEXT:           header: None,
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
// A-NEXT:               3,
// A-NEXT:           ),
// A-NEXT:           kind: User,
// A-NEXT:           line: 7,
// A-NEXT:           header: None,
// A-NEXT:       },
// A-NEXT:   }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN SKIP
// SKIP: decl[0]: Typedef {
// SKIP-NEXT:       name: "guarded_t",
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
// SKIP-NEXT:           line: 3,
// SKIP-NEXT:           header: Some(
// SKIP-NEXT:               FileId(
// SKIP-NEXT:                   4,
// SKIP-NEXT:               ),
// SKIP-NEXT:           ),
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
// SKIP-NEXT:               5,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 4,
// SKIP-NEXT:           header: Some(
// SKIP-NEXT:               FileId(
// SKIP-NEXT:                   5,
// SKIP-NEXT:               ),
// SKIP-NEXT:           ),
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
// SKIP-NEXT:               3,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 6,
// SKIP-NEXT:           header: None,
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
// SKIP-NEXT:               3,
// SKIP-NEXT:           ),
// SKIP-NEXT:           kind: User,
// SKIP-NEXT:           line: 7,
// SKIP-NEXT:           header: None,
// SKIP-NEXT:       },
// SKIP-NEXT:   }
// SLATE-FILECHECK-END SKIP
