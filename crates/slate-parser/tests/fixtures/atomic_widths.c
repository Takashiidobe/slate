#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_uchar u8  = 250;
  atomic_schar i8  = -5;
  atomic_uint  u32 = 1000u;
  atomic_llong i64 = -10000000000LL;

  unsigned char old_u8 =
      atomic_fetch_add_explicit(&u8, 3, memory_order_relaxed);
  signed char  old_i8 = atomic_fetch_sub_explicit(&i8, 7, memory_order_acq_rel);
  unsigned int old_u32 =
      atomic_fetch_xor_explicit(&u32, 0x00FFu, memory_order_release);
  long long old_i64 =
      atomic_exchange_explicit(&i64, 1234567890123LL, memory_order_acquire);

  printf("%u %d %u %lld %u %d %u %lld\n", old_u8, old_i8, old_u32, old_i64,
         (unsigned char)u8, (signed char)i8, (unsigned int)u32, (long long)i64);
  return 0;
}

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Enum(
// DEFAULT-NEXT:       EnumDecl {
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "memory_order",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "memory_order_relaxed",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "memory_order_consume",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "memory_order_acquire",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           2,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "memory_order_release",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "memory_order_acq_rel",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           4,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "memory_order_seq_cst",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           5,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: System,
// DEFAULT-NEXT:               line: 67,
// DEFAULT-NEXT:               header: Some(
// DEFAULT-NEXT:                   FileId(
// DEFAULT-NEXT:                       4,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Typedef {
// DEFAULT-NEXT:       name: "atomic_schar",
// DEFAULT-NEXT:       ty: Atomic(
// DEFAULT-NEXT:           Integer(
// DEFAULT-NEXT:               Char {
// DEFAULT-NEXT:                   signed: Some(
// DEFAULT-NEXT:                       true,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 99,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Typedef {
// DEFAULT-NEXT:       name: "atomic_uchar",
// DEFAULT-NEXT:       ty: Atomic(
// DEFAULT-NEXT:           Integer(
// DEFAULT-NEXT:               Char {
// DEFAULT-NEXT:                   signed: Some(
// DEFAULT-NEXT:                       false,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 100,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Typedef {
// DEFAULT-NEXT:       name: "atomic_uint",
// DEFAULT-NEXT:       ty: Atomic(
// DEFAULT-NEXT:           Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: false,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 104,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Typedef {
// DEFAULT-NEXT:       name: "atomic_llong",
// DEFAULT-NEXT:       ty: Atomic(
// DEFAULT-NEXT:           Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: LongLong,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 107,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:                   "printf",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Qualified {
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers {
// DEFAULT-NEXT:                                   is_restrict: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               inner: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:               variadic: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               21,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   21,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "atomic_uchar",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "u8",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       250,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "atomic_schar",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i8",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "atomic_uint",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "u32",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       1000,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "atomic_llong",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "i64",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           10000000000,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: Some(
// DEFAULT-NEXT:                                       false,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "old_u8",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__c11_atomic_fetch_add",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "u8",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "memory_order_relaxed",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: Some(
// DEFAULT-NEXT:                                       true,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "old_i8",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__c11_atomic_fetch_sub",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "i8",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               7,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "memory_order_acq_rel",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "old_u32",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__c11_atomic_fetch_xor",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "u32",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               255,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "memory_order_release",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "old_i64",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__c11_atomic_exchange",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           AddrOf(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "i64",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               1234567890123,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "memory_order_acquire",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%u %d %u %lld %u %d %u %lld\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "old_u8",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "old_i8",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "old_u32",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "old_i64",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: Some(
// DEFAULT-NEXT:                                               false,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "u8",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: Some(
// DEFAULT-NEXT:                                               true,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "i8",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "u32",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: LongLong,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "i64",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
