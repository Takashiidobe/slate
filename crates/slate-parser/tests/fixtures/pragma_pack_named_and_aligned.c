#include <stddef.h>
#include <stdio.h>

#pragma pack(3)
struct IgnoredBadAlign {
  char a;
  int  b;
};
#pragma pack()

#pragma pack(push, lbl, 1)
struct NamedPushed {
  char a;
  int  b;
};
#pragma pack(pop, lbl)

struct AfterNamedPop {
  char a;
  int  b;
};

#pragma pack(pop)
struct AfterExtraPop {
  char a;
  int  b;
};

#pragma pack(push, 1)
struct __attribute__((aligned(16))) PackedButAligned {
  char a;
  int  b;
  char c;
};
#pragma pack(pop)

int main(void) {
  printf("%d %d %d %d %d %d %d\n", (int)sizeof(struct IgnoredBadAlign),
         (int)sizeof(struct NamedPushed), (int)sizeof(struct AfterNamedPop),
         (int)sizeof(struct AfterExtraPop),
         (int)sizeof(struct PackedButAligned),
         (int)_Alignof(struct PackedButAligned),
         (int)offsetof(struct PackedButAligned, b));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "IgnoredBadAlign",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 5,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 6,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 4,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "NamedPushed",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 12,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 13,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 11,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "AfterNamedPop",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 18,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 19,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 17,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "AfterExtraPop",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 24,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 25,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 23,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "PackedButAligned",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 30,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 31,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 32,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 29,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Aligned(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       16,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%d %d %d %d %d %d %d\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: SizeOfType {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: Some(
// DEFAULT-NEXT:                                               "IgnoredBadAlign",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: SizeOfType {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: Some(
// DEFAULT-NEXT:                                               "NamedPushed",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: SizeOfType {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: Some(
// DEFAULT-NEXT:                                               "AfterNamedPop",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: SizeOfType {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: Some(
// DEFAULT-NEXT:                                               "AfterExtraPop",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: SizeOfType {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: Some(
// DEFAULT-NEXT:                                               "PackedButAligned",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: AlignOf {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: Some(
// DEFAULT-NEXT:                                               "PackedButAligned",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: OffsetOf {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: Some(
// DEFAULT-NEXT:                                               "PackedButAligned",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                       member: Identifier(
// DEFAULT-NEXT:                                           "b",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 36,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
