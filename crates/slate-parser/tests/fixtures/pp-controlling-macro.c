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
// DEFAULT: decl[0]: Declaration(
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
// DEFAULT-NEXT:                       "guarded_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration(
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
// DEFAULT-NEXT:                       "trailing_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "guarded_t",
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
// DEFAULT-NEXT: decl[3]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "trailing_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "trailing",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: decl[0]: Declaration(
// A-NEXT:       Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Integer(
// A-NEXT:                   Ranked {
// A-NEXT:                       rank: Int,
// A-NEXT:                       signed: true,
// A-NEXT:                   },
// A-NEXT:               ),
// A-NEXT:               storage: Typedef,
// A-NEXT:           },
// A-NEXT:           declarators: [
// A-NEXT:               InitDeclaratorKind {
// A-NEXT:                   declarator: Name(
// A-NEXT:                       "guarded_t",
// A-NEXT:                   ),
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[1]: Declaration(
// A-NEXT:       Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Integer(
// A-NEXT:                   Ranked {
// A-NEXT:                       rank: Int,
// A-NEXT:                       signed: true,
// A-NEXT:                   },
// A-NEXT:               ),
// A-NEXT:               storage: Typedef,
// A-NEXT:           },
// A-NEXT:           declarators: [
// A-NEXT:               InitDeclaratorKind {
// A-NEXT:                   declarator: Name(
// A-NEXT:                       "trailing_t",
// A-NEXT:                   ),
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[2]: Declaration(
// A-NEXT:       Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Named(
// A-NEXT:                   "guarded_t",
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarators: [
// A-NEXT:               InitDeclaratorKind {
// A-NEXT:                   declarator: Name(
// A-NEXT:                       "value",
// A-NEXT:                   ),
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// A-NEXT: decl[3]: Declaration(
// A-NEXT:       Declaration {
// A-NEXT:           specifiers: DeclarationSpecifiers {
// A-NEXT:               ty: Named(
// A-NEXT:                   "trailing_t",
// A-NEXT:               ),
// A-NEXT:           },
// A-NEXT:           declarators: [
// A-NEXT:               InitDeclaratorKind {
// A-NEXT:                   declarator: Name(
// A-NEXT:                       "trailing",
// A-NEXT:                   ),
// A-NEXT:               },
// A-NEXT:           ],
// A-NEXT:       },
// A-NEXT:   )
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN SKIP
// SKIP: decl[0]: Declaration(
// SKIP-NEXT:       Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Integer(
// SKIP-NEXT:                   Ranked {
// SKIP-NEXT:                       rank: Int,
// SKIP-NEXT:                       signed: true,
// SKIP-NEXT:                   },
// SKIP-NEXT:               ),
// SKIP-NEXT:               storage: Typedef,
// SKIP-NEXT:           },
// SKIP-NEXT:           declarators: [
// SKIP-NEXT:               InitDeclaratorKind {
// SKIP-NEXT:                   declarator: Name(
// SKIP-NEXT:                       "guarded_t",
// SKIP-NEXT:                   ),
// SKIP-NEXT:               },
// SKIP-NEXT:           ],
// SKIP-NEXT:       },
// SKIP-NEXT:   )
// SKIP-NEXT: decl[1]: Declaration(
// SKIP-NEXT:       Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Integer(
// SKIP-NEXT:                   Ranked {
// SKIP-NEXT:                       rank: Int,
// SKIP-NEXT:                       signed: true,
// SKIP-NEXT:                   },
// SKIP-NEXT:               ),
// SKIP-NEXT:               storage: Typedef,
// SKIP-NEXT:           },
// SKIP-NEXT:           declarators: [
// SKIP-NEXT:               InitDeclaratorKind {
// SKIP-NEXT:                   declarator: Name(
// SKIP-NEXT:                       "trailing_t",
// SKIP-NEXT:                   ),
// SKIP-NEXT:               },
// SKIP-NEXT:           ],
// SKIP-NEXT:       },
// SKIP-NEXT:   )
// SKIP-NEXT: decl[2]: Declaration(
// SKIP-NEXT:       Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Named(
// SKIP-NEXT:                   "guarded_t",
// SKIP-NEXT:               ),
// SKIP-NEXT:           },
// SKIP-NEXT:           declarators: [
// SKIP-NEXT:               InitDeclaratorKind {
// SKIP-NEXT:                   declarator: Name(
// SKIP-NEXT:                       "value",
// SKIP-NEXT:                   ),
// SKIP-NEXT:               },
// SKIP-NEXT:           ],
// SKIP-NEXT:       },
// SKIP-NEXT:   )
// SKIP-NEXT: decl[3]: Declaration(
// SKIP-NEXT:       Declaration {
// SKIP-NEXT:           specifiers: DeclarationSpecifiers {
// SKIP-NEXT:               ty: Named(
// SKIP-NEXT:                   "trailing_t",
// SKIP-NEXT:               ),
// SKIP-NEXT:           },
// SKIP-NEXT:           declarators: [
// SKIP-NEXT:               InitDeclaratorKind {
// SKIP-NEXT:                   declarator: Name(
// SKIP-NEXT:                       "trailing",
// SKIP-NEXT:                   ),
// SKIP-NEXT:               },
// SKIP-NEXT:           ],
// SKIP-NEXT:       },
// SKIP-NEXT:   )
// SLATE-FILECHECK-END SKIP
