#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info = {0};
  if (stat("/dev/null", &info) != 0)
    return 1;
  printf("%lld\n", (long long)info.st_mtime);
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[15]: Spanned {
// DEFAULT-NEXT:       value: TagDefinition {
// DEFAULT-NEXT:           id: TagId(
// DEFAULT-NEXT:               15,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "timespec",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           body: Record(
// DEFAULT-NEXT:               [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "__time_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "tv_sec",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 719,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 719,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Long,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "tv_nsec",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 723,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 723,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 718,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[16]: Spanned {
// DEFAULT-NEXT:       value: TagDefinition {
// DEFAULT-NEXT:           id: TagId(
// DEFAULT-NEXT:               16,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "stat",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           body: Record(
// DEFAULT-NEXT:               [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "dev_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_dev",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 5,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 5,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "ino_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_ino",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 6,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 6,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "nlink_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_nlink",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 7,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 7,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "mode_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_mode",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 9,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 9,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "uid_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_uid",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 10,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 10,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "gid_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_gid",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 11,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 11,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "__pad0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 12,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 12,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "dev_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_rdev",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 13,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 13,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "off_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_size",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 14,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 14,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "blksize_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_blksize",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 15,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 15,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "blkcnt_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_blocks",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 16,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 16,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Tag(
// DEFAULT-NEXT:                                       Reference {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: "timespec",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_atim",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 18,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 18,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Tag(
// DEFAULT-NEXT:                                       Reference {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: "timespec",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_mtim",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 19,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 19,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Tag(
// DEFAULT-NEXT:                                       Reference {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: "timespec",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "st_ctim",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 20,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 20,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Field(
// DEFAULT-NEXT:                           FieldDecl {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Long,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: FieldDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Array {
// DEFAULT-NEXT:                                               inner: Name(
// DEFAULT-NEXT:                                                   "__unused",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               size: Expression(
// DEFAULT-NEXT:                                                   Spanned {
// DEFAULT-NEXT:                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 3,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "3",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               11,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: 21,
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   9,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               11,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: 21,
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 21,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               11,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__mode_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 53,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 53,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__uid_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 55,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 55,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__gid_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 56,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 56,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: LongLong,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__dev_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 57,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 57,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__nlink_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 59,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 59,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__off_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 260,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 260,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__time_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 262,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 262,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__ino_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 264,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 264,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__blksize_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 267,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 267,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Long,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "__blkcnt_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 271,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 271,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__off_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "off_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 816,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 816,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[11]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Function {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "printf",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               parameters: Prototype {
// DEFAULT-NEXT:                                   parameters: [
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_const: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Pointer {
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_restrict: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   inner: Abstract,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: 170,
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       4,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   variadic: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 170,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[12]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Tag(
// DEFAULT-NEXT:                       Definition(
// DEFAULT-NEXT:                           TagId(
// DEFAULT-NEXT:                               15,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 718,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__mode_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "mode_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 828,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 828,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[14]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__uid_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "uid_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 840,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 840,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[15]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__gid_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "gid_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 846,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 846,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[16]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__ino_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "ino_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 870,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 870,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[17]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__dev_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "dev_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 882,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 882,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[18]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__nlink_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "nlink_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 888,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 888,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[19]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__blksize_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "blksize_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 894,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 894,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[20]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__blkcnt_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "blkcnt_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               8,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 900,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 900,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[21]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Tag(
// DEFAULT-NEXT:                       Definition(
// DEFAULT-NEXT:                           TagId(
// DEFAULT-NEXT:                               16,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               11,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[22]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Function {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "stat",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               parameters: Prototype {
// DEFAULT-NEXT:                                   parameters: [
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_const: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Pointer {
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_restrict: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   inner: Abstract,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: 99,
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       9,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tag(
// DEFAULT-NEXT:                                                       Reference {
// DEFAULT-NEXT:                                                           kind: Struct,
// DEFAULT-NEXT:                                                           name: "stat",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Pointer {
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_restrict: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   inner: Abstract,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   9,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: 99,
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       9,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               9,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 99,
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               9,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 99,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[23]: Function(
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
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "stat",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "info",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 0,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "0",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Binary {
// DEFAULT-NEXT:                       op: NotEqual,
// DEFAULT-NEXT:                       left: Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "stat",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLiteral(
// DEFAULT-NEXT:                                   StringLiteral {
// DEFAULT-NEXT:                                       encoding: Plain,
// DEFAULT-NEXT:                                       code_units: [
// DEFAULT-NEXT:                                           47,
// DEFAULT-NEXT:                                           100,
// DEFAULT-NEXT:                                           101,
// DEFAULT-NEXT:                                           118,
// DEFAULT-NEXT:                                           47,
// DEFAULT-NEXT:                                           110,
// DEFAULT-NEXT:                                           117,
// DEFAULT-NEXT:                                           108,
// DEFAULT-NEXT:                                           108,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       pieces: [
// DEFAULT-NEXT:                                           "/dev/null",
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "info",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       right: IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 0,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "0",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       108,
// DEFAULT-NEXT:                                       108,
// DEFAULT-NEXT:                                       100,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "%lld\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Cast {
// DEFAULT-NEXT:                               ty: TypeName {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: LongLong,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "info",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "st_mtim",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "tv_sec",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
