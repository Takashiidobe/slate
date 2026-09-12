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
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Conditional(
// DEFAULT-NEXT:                   Conditional {
// DEFAULT-NEXT:                       branches: [
// DEFAULT-NEXT:                           (
// DEFAULT-NEXT:                               Defined(
// DEFAULT-NEXT:                                   "_WIN32",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   Return(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           (
// DEFAULT-NEXT:                               Not(
// DEFAULT-NEXT:                                   Defined(
// DEFAULT-NEXT:                                       "_WIN32",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   Return(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   1,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Typedef {
// DEFAULT-NEXT:       name: "HANDLE",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               1,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "_WIN32",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "Socket",
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "HANDLE",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 11,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "_WIN32",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "Socket",
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   1,
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
// DEFAULT-NEXT: decl[0]: Function(
// DEFAULT-NEXT:       ConcreteFunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   1,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Typedef {
// DEFAULT-NEXT:       name: "HANDLE",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               1,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "Socket",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               1,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 13,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN WIN32
// WIN32: polyvariant:
// WIN32-NEXT: decl[0]: Function(
// WIN32-NEXT:       FunctionDecl {
// WIN32-NEXT:           ret_type: Integer(
// WIN32-NEXT:               Ranked {
// WIN32-NEXT:                   rank: Int,
// WIN32-NEXT:                   signed: true,
// WIN32-NEXT:               },
// WIN32-NEXT:           ),
// WIN32-NEXT:           name: "main",
// WIN32-NEXT:           body: [
// WIN32-NEXT:               Conditional(
// WIN32-NEXT:                   Conditional {
// WIN32-NEXT:                       branches: [
// WIN32-NEXT:                           (
// WIN32-NEXT:                               Defined(
// WIN32-NEXT:                                   "_WIN32",
// WIN32-NEXT:                               ),
// WIN32-NEXT:                               [
// WIN32-NEXT:                                   Return(
// WIN32-NEXT:                                       Const(
// WIN32-NEXT:                                           Integer(
// WIN32-NEXT:                                               2,
// WIN32-NEXT:                                           ),
// WIN32-NEXT:                                       ),
// WIN32-NEXT:                                   ),
// WIN32-NEXT:                               ],
// WIN32-NEXT:                           ),
// WIN32-NEXT:                           (
// WIN32-NEXT:                               Not(
// WIN32-NEXT:                                   Defined(
// WIN32-NEXT:                                       "_WIN32",
// WIN32-NEXT:                                   ),
// WIN32-NEXT:                               ),
// WIN32-NEXT:                               [
// WIN32-NEXT:                                   Return(
// WIN32-NEXT:                                       Const(
// WIN32-NEXT:                                           Integer(
// WIN32-NEXT:                                               3,
// WIN32-NEXT:                                           ),
// WIN32-NEXT:                                       ),
// WIN32-NEXT:                                   ),
// WIN32-NEXT:                               ],
// WIN32-NEXT:                           ),
// WIN32-NEXT:                       ],
// WIN32-NEXT:                   },
// WIN32-NEXT:               ),
// WIN32-NEXT:           ],
// WIN32-NEXT:           provenance: Provenance {
// WIN32-NEXT:               file: FileId(
// WIN32-NEXT:                   1,
// WIN32-NEXT:               ),
// WIN32-NEXT:               kind: User,
// WIN32-NEXT:               line: 0,
// WIN32-NEXT:           },
// WIN32-NEXT:       },
// WIN32-NEXT:   )
// WIN32-NEXT: decl[1]: Typedef {
// WIN32-NEXT:       name: "HANDLE",
// WIN32-NEXT:       ty: Integer(
// WIN32-NEXT:           Ranked {
// WIN32-NEXT:               rank: Int,
// WIN32-NEXT:               signed: true,
// WIN32-NEXT:           },
// WIN32-NEXT:       ),
// WIN32-NEXT:       provenance: Provenance {
// WIN32-NEXT:           file: FileId(
// WIN32-NEXT:               1,
// WIN32-NEXT:           ),
// WIN32-NEXT:           kind: User,
// WIN32-NEXT:           line: 8,
// WIN32-NEXT:       },
// WIN32-NEXT:   }
// WIN32-NEXT: decl[2]: Conditional(
// WIN32-NEXT:       Conditional {
// WIN32-NEXT:           branches: [
// WIN32-NEXT:               (
// WIN32-NEXT:                   Defined(
// WIN32-NEXT:                       "_WIN32",
// WIN32-NEXT:                   ),
// WIN32-NEXT:                   [
// WIN32-NEXT:                       Typedef {
// WIN32-NEXT:                           name: "Socket",
// WIN32-NEXT:                           ty: Named(
// WIN32-NEXT:                               "HANDLE",
// WIN32-NEXT:                           ),
// WIN32-NEXT:                           provenance: Provenance {
// WIN32-NEXT:                               file: FileId(
// WIN32-NEXT:                                   1,
// WIN32-NEXT:                               ),
// WIN32-NEXT:                               kind: User,
// WIN32-NEXT:                               line: 11,
// WIN32-NEXT:                           },
// WIN32-NEXT:                       },
// WIN32-NEXT:                   ],
// WIN32-NEXT:               ),
// WIN32-NEXT:               (
// WIN32-NEXT:                   Not(
// WIN32-NEXT:                       Defined(
// WIN32-NEXT:                           "_WIN32",
// WIN32-NEXT:                       ),
// WIN32-NEXT:                   ),
// WIN32-NEXT:                   [
// WIN32-NEXT:                       Typedef {
// WIN32-NEXT:                           name: "Socket",
// WIN32-NEXT:                           ty: Integer(
// WIN32-NEXT:                               Ranked {
// WIN32-NEXT:                                   rank: Int,
// WIN32-NEXT:                                   signed: true,
// WIN32-NEXT:                               },
// WIN32-NEXT:                           ),
// WIN32-NEXT:                           provenance: Provenance {
// WIN32-NEXT:                               file: FileId(
// WIN32-NEXT:                                   1,
// WIN32-NEXT:                               ),
// WIN32-NEXT:                               kind: User,
// WIN32-NEXT:                               line: 13,
// WIN32-NEXT:                           },
// WIN32-NEXT:                       },
// WIN32-NEXT:                   ],
// WIN32-NEXT:               ),
// WIN32-NEXT:           ],
// WIN32-NEXT:       },
// WIN32-NEXT:   )
// WIN32-NEXT: concrete:
// WIN32-NEXT: decl[0]: Function(
// WIN32-NEXT:       ConcreteFunctionDecl {
// WIN32-NEXT:           ret_type: Integer(
// WIN32-NEXT:               Ranked {
// WIN32-NEXT:                   rank: Int,
// WIN32-NEXT:                   signed: true,
// WIN32-NEXT:               },
// WIN32-NEXT:           ),
// WIN32-NEXT:           name: "main",
// WIN32-NEXT:           body: [
// WIN32-NEXT:               Return(
// WIN32-NEXT:                   Const(
// WIN32-NEXT:                       Integer(
// WIN32-NEXT:                           2,
// WIN32-NEXT:                       ),
// WIN32-NEXT:                   ),
// WIN32-NEXT:               ),
// WIN32-NEXT:           ],
// WIN32-NEXT:           provenance: Provenance {
// WIN32-NEXT:               file: FileId(
// WIN32-NEXT:                   1,
// WIN32-NEXT:               ),
// WIN32-NEXT:               kind: User,
// WIN32-NEXT:               line: 0,
// WIN32-NEXT:           },
// WIN32-NEXT:       },
// WIN32-NEXT:   )
// WIN32-NEXT: decl[1]: Typedef {
// WIN32-NEXT:       name: "HANDLE",
// WIN32-NEXT:       ty: Integer(
// WIN32-NEXT:           Ranked {
// WIN32-NEXT:               rank: Int,
// WIN32-NEXT:               signed: true,
// WIN32-NEXT:           },
// WIN32-NEXT:       ),
// WIN32-NEXT:       provenance: Provenance {
// WIN32-NEXT:           file: FileId(
// WIN32-NEXT:               1,
// WIN32-NEXT:           ),
// WIN32-NEXT:           kind: User,
// WIN32-NEXT:           line: 8,
// WIN32-NEXT:       },
// WIN32-NEXT:   }
// WIN32-NEXT: decl[2]: Typedef {
// WIN32-NEXT:       name: "Socket",
// WIN32-NEXT:       ty: Named(
// WIN32-NEXT:           "HANDLE",
// WIN32-NEXT:       ),
// WIN32-NEXT:       provenance: Provenance {
// WIN32-NEXT:           file: FileId(
// WIN32-NEXT:               1,
// WIN32-NEXT:           ),
// WIN32-NEXT:           kind: User,
// WIN32-NEXT:           line: 11,
// WIN32-NEXT:       },
// WIN32-NEXT:   }
// SLATE-FILECHECK-END WIN32
