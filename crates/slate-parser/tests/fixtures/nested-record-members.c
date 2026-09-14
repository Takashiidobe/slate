struct outer {
  unsigned long bits[128 / sizeof(unsigned long)];
  union {
    int i;
    char c;
  } value;
#ifdef WITH_EXTRA
  int extra;
#else
  int fallback;
#endif
};

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES EXTRA WITH_EXTRA

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[0]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           0,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Union,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 3,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 4,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 2,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[1]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           1,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "outer",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "bits",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Div,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 128,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "128",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: SizeOfType {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Long,
// DEFAULT-NEXT:                                                       signed: false,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 1,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Definition(
// DEFAULT-NEXT:                                   TagId(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "value",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 2,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "fallback",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 9,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN EXTRA
// EXTRA: tag[0]: TagDefinition {
// EXTRA-NEXT:       id: TagId(
// EXTRA-NEXT:           0,
// EXTRA-NEXT:       ),
// EXTRA-NEXT:       kind: Union,
// EXTRA-NEXT:       name: None,
// EXTRA-NEXT:       body: Record(
// EXTRA-NEXT:           [
// EXTRA-NEXT:               Field(
// EXTRA-NEXT:                   FieldDecl {
// EXTRA-NEXT:                       specifiers: DeclarationSpecifiers {
// EXTRA-NEXT:                           ty: Integer(
// EXTRA-NEXT:                               Ranked {
// EXTRA-NEXT:                                   rank: Int,
// EXTRA-NEXT:                                   signed: true,
// EXTRA-NEXT:                               },
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                       declarators: [
// EXTRA-NEXT:                           FieldDeclarator {
// EXTRA-NEXT:                               declarator: Name(
// EXTRA-NEXT:                                   "i",
// EXTRA-NEXT:                               ),
// EXTRA-NEXT:                           },
// EXTRA-NEXT:                       ],
// EXTRA-NEXT:                       provenance: Provenance {
// EXTRA-NEXT:                           file: FileId(
// EXTRA-NEXT:                               3,
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                           kind: User,
// EXTRA-NEXT:                           line: 3,
// EXTRA-NEXT:                           header: None,
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                   },
// EXTRA-NEXT:               ),
// EXTRA-NEXT:               Field(
// EXTRA-NEXT:                   FieldDecl {
// EXTRA-NEXT:                       specifiers: DeclarationSpecifiers {
// EXTRA-NEXT:                           ty: Integer(
// EXTRA-NEXT:                               Char {
// EXTRA-NEXT:                                   signed: None,
// EXTRA-NEXT:                               },
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                       declarators: [
// EXTRA-NEXT:                           FieldDeclarator {
// EXTRA-NEXT:                               declarator: Name(
// EXTRA-NEXT:                                   "c",
// EXTRA-NEXT:                               ),
// EXTRA-NEXT:                           },
// EXTRA-NEXT:                       ],
// EXTRA-NEXT:                       provenance: Provenance {
// EXTRA-NEXT:                           file: FileId(
// EXTRA-NEXT:                               3,
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                           kind: User,
// EXTRA-NEXT:                           line: 4,
// EXTRA-NEXT:                           header: None,
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                   },
// EXTRA-NEXT:               ),
// EXTRA-NEXT:           ],
// EXTRA-NEXT:       ),
// EXTRA-NEXT:       provenance: Provenance {
// EXTRA-NEXT:           file: FileId(
// EXTRA-NEXT:               3,
// EXTRA-NEXT:           ),
// EXTRA-NEXT:           kind: User,
// EXTRA-NEXT:           line: 2,
// EXTRA-NEXT:           header: None,
// EXTRA-NEXT:       },
// EXTRA-NEXT:   }
// EXTRA-NEXT: tag[1]: TagDefinition {
// EXTRA-NEXT:       id: TagId(
// EXTRA-NEXT:           1,
// EXTRA-NEXT:       ),
// EXTRA-NEXT:       kind: Struct,
// EXTRA-NEXT:       name: Some(
// EXTRA-NEXT:           "outer",
// EXTRA-NEXT:       ),
// EXTRA-NEXT:       body: Record(
// EXTRA-NEXT:           [
// EXTRA-NEXT:               Field(
// EXTRA-NEXT:                   FieldDecl {
// EXTRA-NEXT:                       specifiers: DeclarationSpecifiers {
// EXTRA-NEXT:                           ty: Integer(
// EXTRA-NEXT:                               Ranked {
// EXTRA-NEXT:                                   rank: Long,
// EXTRA-NEXT:                                   signed: false,
// EXTRA-NEXT:                               },
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                       declarators: [
// EXTRA-NEXT:                           FieldDeclarator {
// EXTRA-NEXT:                               declarator: Array {
// EXTRA-NEXT:                                   inner: Name(
// EXTRA-NEXT:                                       "bits",
// EXTRA-NEXT:                                   ),
// EXTRA-NEXT:                                   size: Expression(
// EXTRA-NEXT:                                       Binary {
// EXTRA-NEXT:                                           op: Div,
// EXTRA-NEXT:                                           left: IntegerLiteral(
// EXTRA-NEXT:                                               IntegerLiteral {
// EXTRA-NEXT:                                                   value: 128,
// EXTRA-NEXT:                                                   radix: Decimal,
// EXTRA-NEXT:                                                   suffix: IntegerSuffix {
// EXTRA-NEXT:                                                       unsigned: false,
// EXTRA-NEXT:                                                       size: None,
// EXTRA-NEXT:                                                   },
// EXTRA-NEXT:                                                   spelling: "128",
// EXTRA-NEXT:                                               },
// EXTRA-NEXT:                                           ),
// EXTRA-NEXT:                                           right: SizeOfType {
// EXTRA-NEXT:                                               ty: Integer(
// EXTRA-NEXT:                                                   Ranked {
// EXTRA-NEXT:                                                       rank: Long,
// EXTRA-NEXT:                                                       signed: false,
// EXTRA-NEXT:                                                   },
// EXTRA-NEXT:                                               ),
// EXTRA-NEXT:                                               declarator: Abstract,
// EXTRA-NEXT:                                           },
// EXTRA-NEXT:                                       },
// EXTRA-NEXT:                                   ),
// EXTRA-NEXT:                               },
// EXTRA-NEXT:                           },
// EXTRA-NEXT:                       ],
// EXTRA-NEXT:                       provenance: Provenance {
// EXTRA-NEXT:                           file: FileId(
// EXTRA-NEXT:                               3,
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                           kind: User,
// EXTRA-NEXT:                           line: 1,
// EXTRA-NEXT:                           header: None,
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                   },
// EXTRA-NEXT:               ),
// EXTRA-NEXT:               Field(
// EXTRA-NEXT:                   FieldDecl {
// EXTRA-NEXT:                       specifiers: DeclarationSpecifiers {
// EXTRA-NEXT:                           ty: Tag(
// EXTRA-NEXT:                               Definition(
// EXTRA-NEXT:                                   TagId(
// EXTRA-NEXT:                                       0,
// EXTRA-NEXT:                                   ),
// EXTRA-NEXT:                               ),
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                       declarators: [
// EXTRA-NEXT:                           FieldDeclarator {
// EXTRA-NEXT:                               declarator: Name(
// EXTRA-NEXT:                                   "value",
// EXTRA-NEXT:                               ),
// EXTRA-NEXT:                           },
// EXTRA-NEXT:                       ],
// EXTRA-NEXT:                       provenance: Provenance {
// EXTRA-NEXT:                           file: FileId(
// EXTRA-NEXT:                               3,
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                           kind: User,
// EXTRA-NEXT:                           line: 2,
// EXTRA-NEXT:                           header: None,
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                   },
// EXTRA-NEXT:               ),
// EXTRA-NEXT:               Field(
// EXTRA-NEXT:                   FieldDecl {
// EXTRA-NEXT:                       specifiers: DeclarationSpecifiers {
// EXTRA-NEXT:                           ty: Integer(
// EXTRA-NEXT:                               Ranked {
// EXTRA-NEXT:                                   rank: Int,
// EXTRA-NEXT:                                   signed: true,
// EXTRA-NEXT:                               },
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                       declarators: [
// EXTRA-NEXT:                           FieldDeclarator {
// EXTRA-NEXT:                               declarator: Name(
// EXTRA-NEXT:                                   "extra",
// EXTRA-NEXT:                               ),
// EXTRA-NEXT:                           },
// EXTRA-NEXT:                       ],
// EXTRA-NEXT:                       provenance: Provenance {
// EXTRA-NEXT:                           file: FileId(
// EXTRA-NEXT:                               3,
// EXTRA-NEXT:                           ),
// EXTRA-NEXT:                           kind: User,
// EXTRA-NEXT:                           line: 7,
// EXTRA-NEXT:                           header: None,
// EXTRA-NEXT:                       },
// EXTRA-NEXT:                   },
// EXTRA-NEXT:               ),
// EXTRA-NEXT:           ],
// EXTRA-NEXT:       ),
// EXTRA-NEXT:       provenance: Provenance {
// EXTRA-NEXT:           file: FileId(
// EXTRA-NEXT:               3,
// EXTRA-NEXT:           ),
// EXTRA-NEXT:           kind: User,
// EXTRA-NEXT:           line: 0,
// EXTRA-NEXT:           header: None,
// EXTRA-NEXT:       },
// EXTRA-NEXT:   }
// EXTRA-NEXT: decl[0]: Declaration {
// EXTRA-NEXT:       declaration: Declaration {
// EXTRA-NEXT:           specifiers: DeclarationSpecifiers {
// EXTRA-NEXT:               ty: Tag(
// EXTRA-NEXT:                   Definition(
// EXTRA-NEXT:                       TagId(
// EXTRA-NEXT:                           1,
// EXTRA-NEXT:                       ),
// EXTRA-NEXT:                   ),
// EXTRA-NEXT:               ),
// EXTRA-NEXT:           },
// EXTRA-NEXT:       },
// EXTRA-NEXT:       provenance: Provenance {
// EXTRA-NEXT:           file: FileId(
// EXTRA-NEXT:               3,
// EXTRA-NEXT:           ),
// EXTRA-NEXT:           kind: User,
// EXTRA-NEXT:           line: 0,
// EXTRA-NEXT:           header: None,
// EXTRA-NEXT:       },
// EXTRA-NEXT:   }
// SLATE-FILECHECK-END EXTRA
