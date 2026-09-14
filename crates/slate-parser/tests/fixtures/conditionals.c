int main() {
#ifdef _WIN32
  return 2;
#else
  return 3;
#endif
}

typedef int HANDLE;

#ifdef _WIN32
typedef HANDLE Socket;
#else
typedef int Socket;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WIN32 _WIN32

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
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
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 3,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "3",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       "HANDLE",
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
// DEFAULT-NEXT:                       "Socket",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN WIN32
// WIN32: decl[0]: Function(
// WIN32-NEXT:       FunctionDefinition {
// WIN32-NEXT:           specifiers: DeclarationSpecifiers {
// WIN32-NEXT:               ty: Integer(
// WIN32-NEXT:                   Ranked {
// WIN32-NEXT:                       rank: Int,
// WIN32-NEXT:                       signed: true,
// WIN32-NEXT:                   },
// WIN32-NEXT:               ),
// WIN32-NEXT:           },
// WIN32-NEXT:           declarator: Function {
// WIN32-NEXT:               inner: Name(
// WIN32-NEXT:                   "main",
// WIN32-NEXT:               ),
// WIN32-NEXT:               parameters: Empty,
// WIN32-NEXT:           },
// WIN32-NEXT:           body: [
// WIN32-NEXT:               Return(
// WIN32-NEXT:                   IntegerLiteral(
// WIN32-NEXT:                       IntegerLiteral {
// WIN32-NEXT:                           value: 2,
// WIN32-NEXT:                           radix: Decimal,
// WIN32-NEXT:                           suffix: IntegerSuffix {
// WIN32-NEXT:                               unsigned: false,
// WIN32-NEXT:                               size: None,
// WIN32-NEXT:                           },
// WIN32-NEXT:                           spelling: "2",
// WIN32-NEXT:                       },
// WIN32-NEXT:                   ),
// WIN32-NEXT:               ),
// WIN32-NEXT:           ],
// WIN32-NEXT:       },
// WIN32-NEXT:   )
// WIN32-NEXT: decl[1]: Declaration(
// WIN32-NEXT:       Declaration {
// WIN32-NEXT:           specifiers: DeclarationSpecifiers {
// WIN32-NEXT:               ty: Integer(
// WIN32-NEXT:                   Ranked {
// WIN32-NEXT:                       rank: Int,
// WIN32-NEXT:                       signed: true,
// WIN32-NEXT:                   },
// WIN32-NEXT:               ),
// WIN32-NEXT:               storage: Typedef,
// WIN32-NEXT:           },
// WIN32-NEXT:           declarators: [
// WIN32-NEXT:               InitDeclaratorKind {
// WIN32-NEXT:                   declarator: Name(
// WIN32-NEXT:                       "HANDLE",
// WIN32-NEXT:                   ),
// WIN32-NEXT:               },
// WIN32-NEXT:           ],
// WIN32-NEXT:       },
// WIN32-NEXT:   )
// WIN32-NEXT: decl[2]: Declaration(
// WIN32-NEXT:       Declaration {
// WIN32-NEXT:           specifiers: DeclarationSpecifiers {
// WIN32-NEXT:               ty: Named(
// WIN32-NEXT:                   "HANDLE",
// WIN32-NEXT:               ),
// WIN32-NEXT:               storage: Typedef,
// WIN32-NEXT:           },
// WIN32-NEXT:           declarators: [
// WIN32-NEXT:               InitDeclaratorKind {
// WIN32-NEXT:                   declarator: Name(
// WIN32-NEXT:                       "Socket",
// WIN32-NEXT:                   ),
// WIN32-NEXT:               },
// WIN32-NEXT:           ],
// WIN32-NEXT:       },
// WIN32-NEXT:   )
// SLATE-FILECHECK-END WIN32
