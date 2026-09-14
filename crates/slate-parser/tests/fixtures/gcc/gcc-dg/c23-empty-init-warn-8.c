/* Test that no C23 warnings are produced (by default) about
 * initializers that might not zero padding bits (when configured not to zero
 * padding bits unless mandated by the language standard).
 */
/* { dg-do run } */
/* { dg-options "-fzero-init-padding-bits=standard" } */

#include "c23-empty-init-warn-4.c"



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comment: Comment {
// DEFAULT-NEXT:               text: [
// DEFAULT-NEXT:                   "/* Test that no C23 warnings are produced (by default) about\n * initializers that might not zero padding bits (when configured not to zero\n * padding bits unless mandated by the language standard).\n */",
// DEFAULT-NEXT:                   "/* { dg-do run } */",
// DEFAULT-NEXT:                   "/* { dg-options \"-fzero-init-padding-bits=standard\" } */",
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:               loc: Loc {
// DEFAULT-NEXT:                   file: FileId(
// DEFAULT-NEXT:                       3,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   offset: 0,
// DEFAULT-NEXT:                   length: 278,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
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
