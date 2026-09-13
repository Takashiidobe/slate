#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef unsigned uint32_t;
#endif

#define __fake_const_swab32(x)                                                 \
  ((uint32_t)((((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |               \
              (((uint32_t)(x) & (uint32_t)0x0000ff00UL) << 8) |                \
              (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 8) |                \
              (((uint32_t)(x) & (uint32_t)0x0000ff00UL)) |                     \
              (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

/* Previous version of bswap optimization would detect byte swap when none
   happen. This test aims at catching such wrong detection to avoid
   regressions.  */

__attribute__((noinline, noclone)) uint32_t fake_swap32(uint32_t in) {
  return __fake_const_swab32(in);
}

int main(void) {
  if (sizeof(uint32_t) * __CHAR_BIT__ != 32)
    return 0;
  if (fake_swap32(0x12345678UL) != 0x78567E12UL)
    __builtin_abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "uint32_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 1,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* Previous version of bswap optimization would detect byte swap when none\n   happen. This test aims at catching such wrong detection to avoid\n   regressions.  */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 569,
// DEFAULT-NEXT:                       length: 162,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 13,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Named(
// DEFAULT-NEXT:               "uint32_t",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "fake_swap32",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "uint32_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "in",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Cast {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "uint32_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           declarator: Abstract,
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: BitOr,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: BitOr,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: BitOr,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: ShiftLeft,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: BitAnd,
// DEFAULT-NEXT:                                                   left: Cast {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "uint32_t",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "in",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Cast {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "uint32_t",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Integer(
// DEFAULT-NEXT:                                                           255,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   24,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: ShiftLeft,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: BitAnd,
// DEFAULT-NEXT:                                                   left: Cast {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "uint32_t",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "in",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Cast {
// DEFAULT-NEXT:                                                       ty: Named(
// DEFAULT-NEXT:                                                           "uint32_t",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                       value: Integer(
// DEFAULT-NEXT:                                                           65280,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   8,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: ShiftLeft,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: BitAnd,
// DEFAULT-NEXT:                                               left: Cast {
// DEFAULT-NEXT:                                                   ty: Named(
// DEFAULT-NEXT:                                                       "uint32_t",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Identifier(
// DEFAULT-NEXT:                                                       "in",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Cast {
// DEFAULT-NEXT:                                                   ty: Named(
// DEFAULT-NEXT:                                                       "uint32_t",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       255,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               8,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: BitAnd,
// DEFAULT-NEXT:                                       left: Cast {
// DEFAULT-NEXT:                                           ty: Named(
// DEFAULT-NEXT:                                               "uint32_t",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Abstract,
// DEFAULT-NEXT:                                           value: Identifier(
// DEFAULT-NEXT:                                               "in",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Cast {
// DEFAULT-NEXT:                                           ty: Named(
// DEFAULT-NEXT:                                               "uint32_t",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Abstract,
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               65280,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: ShiftRight,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: BitAnd,
// DEFAULT-NEXT:                                       left: Cast {
// DEFAULT-NEXT:                                           ty: Named(
// DEFAULT-NEXT:                                               "uint32_t",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Abstract,
// DEFAULT-NEXT:                                           value: Identifier(
// DEFAULT-NEXT:                                               "in",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Cast {
// DEFAULT-NEXT:                                           ty: Named(
// DEFAULT-NEXT:                                               "uint32_t",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Abstract,
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               4278190080,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       24,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
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
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoInline,
// DEFAULT-NEXT:               NoClone,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Mul,
// DEFAULT-NEXT:                               left: SizeOfType {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "uint32_t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               32,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "fake_swap32",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       305419896,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               2018934290,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:               line: 21,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
