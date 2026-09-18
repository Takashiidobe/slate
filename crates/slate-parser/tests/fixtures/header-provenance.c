#include "header-provenance-headers/local.h"

outer_int from_outer;
nested_int from_nested;
local_int from_local;

int use_wrapper(void) {
    return local_wrapper(1);
}

// SLATE-FILECHECK-ISYSTEM tests/fixtures/header-provenance-headers/system

// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:                       "local_int",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Spanned {
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
// DEFAULT-NEXT:                               "nested_int",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               6,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               6,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   5,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "nested_int",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "outer_int",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               5,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               5,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   5,
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
// DEFAULT-NEXT:                                   "system_call",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               parameters: Prototype {
// DEFAULT-NEXT:                                   parameters: [
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Int,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "value",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       5,
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
// DEFAULT-NEXT:                               5,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   5,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               5,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   5,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:               is_inline: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "local_wrapper",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclarationKind {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "value",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "system_call",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "value",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "outer_int",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "from_outer",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "nested_int",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "from_nested",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "local_int",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "from_local",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[8]: Function(
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
// DEFAULT-NEXT:                   "use_wrapper",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "local_wrapper",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
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
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
