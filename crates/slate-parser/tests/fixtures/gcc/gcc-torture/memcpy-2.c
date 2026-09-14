/* Copyright (C) 2002  Free Software Foundation.

   Test memcpy with various combinations of pointer alignments and lengths to
   make sure any optimizations in the library are correct.

   Written by Michael Meissner, March 9, 2002.  */

#include <string.h>

void abort(void);
void exit(int);

#ifndef MAX_OFFSET
#define MAX_OFFSET (sizeof(long long))
#endif

#ifndef MAX_COPY
#define MAX_COPY (10 * sizeof(long long))
#endif

#ifndef MAX_EXTRA
#define MAX_EXTRA (sizeof(long long))
#endif

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + MAX_EXTRA)

/* Use a sequence length that is not divisible by two, to make it more
   likely to detect when words are mixed up.  */
#define SEQUENCE_LENGTH 31

static union {
  char        buf[MAX_LENGTH];
  long long   align_int;
  long double align_fp;
} u1, u2;

int main(void) {
  int   off1, off2, len, i;
  char *p, *q, c;

  for (off1 = 0; off1 < MAX_OFFSET; off1++)
    for (off2 = 0; off2 < MAX_OFFSET; off2++)
      for (len = 1; len < MAX_COPY; len++) {
        for (i = 0, c = 'A'; i < MAX_LENGTH; i++, c++) {
          u1.buf[i] = 'a';
          if (c >= 'A' + SEQUENCE_LENGTH)
            c = 'A';
          u2.buf[i] = c;
        }

        p = memcpy(u1.buf + off1, u2.buf + off2, len);
        if (p != u1.buf + off1)
          abort();

        q = u1.buf;
        for (i = 0; i < off1; i++, q++)
          if (*q != 'a')
            abort();

        for (i = 0, c = 'A' + off2; i < len; i++, q++, c++) {
          if (c >= 'A' + SEQUENCE_LENGTH)
            c = 'A';
          if (*q != c)
            abort();
        }

        for (i = 0; i < MAX_EXTRA; i++, q++)
          if (*q != 'a')
            abort();
      }

  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[15]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           15,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Union,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
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
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "buf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Paren(
// DEFAULT-NEXT:                                                       SizeOfType {
// DEFAULT-NEXT:                                                           ty: TypeName {
// DEFAULT-NEXT:                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                       Ranked {
// DEFAULT-NEXT:                                                                           rank: LongLong,
// DEFAULT-NEXT:                                                                           signed: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 10,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "10",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: SizeOfType {
// DEFAULT-NEXT:                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                           Ranked {
// DEFAULT-NEXT:                                                                               rank: LongLong,
// DEFAULT-NEXT:                                                                               signed: true,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Paren(
// DEFAULT-NEXT:                                                   SizeOfType {
// DEFAULT-NEXT:                                                       ty: TypeName {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Ranked {
// DEFAULT-NEXT:                                                                       rank: LongLong,
// DEFAULT-NEXT:                                                                       signed: true,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "align_int",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               LongDouble,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "align_fp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Spanned {
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
// DEFAULT-NEXT:                               "size_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               12,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 17,
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
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 17,
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
// DEFAULT-NEXT:                               "__size_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               19,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 258,
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
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 258,
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
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "__size_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Typedef,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "size_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               19,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 795,
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
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 795,
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
// DEFAULT-NEXT:                   ty: Void,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Function {
// DEFAULT-NEXT:                               inner: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "memcpy",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               parameters: Prototype {
// DEFAULT-NEXT:                                   parameters: [
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Void,
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
// DEFAULT-NEXT:                                               line: 14,
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       4,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Void,
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
// DEFAULT-NEXT:                                               line: 14,
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       4,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Named(
// DEFAULT-NEXT:                                                       "size_t",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: 14,
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       4,
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
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 14,
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
// DEFAULT-NEXT:           line: 14,
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "abort",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Void,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclarationKind {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           15,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "u1",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "u2",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: Function(
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
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "off1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "off2",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "len",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "p",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "q",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "off1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "off1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               SizeOfType {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: LongLong,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Postfix {
// DEFAULT-NEXT:                           op: Increment,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "off1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       For {
// DEFAULT-NEXT:                           init: Some(
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "off2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 0,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "0",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           condition: Some(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Less,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "off2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       SizeOfType {
// DEFAULT-NEXT:                                           ty: TypeName {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: LongLong,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           increment: Some(
// DEFAULT-NEXT:                               Postfix {
// DEFAULT-NEXT:                                   op: Increment,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "off2",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               For {
// DEFAULT-NEXT:                                   init: Some(
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "len",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 1,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "1",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   condition: Some(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Less,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "len",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Mul,
// DEFAULT-NEXT:                                                   left: IntegerLiteral(
// DEFAULT-NEXT:                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                           value: 10,
// DEFAULT-NEXT:                                                           radix: Decimal,
// DEFAULT-NEXT:                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                               unsigned: false,
// DEFAULT-NEXT:                                                               size: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           spelling: "10",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: SizeOfType {
// DEFAULT-NEXT:                                                       ty: TypeName {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Ranked {
// DEFAULT-NEXT:                                                                       rank: LongLong,
// DEFAULT-NEXT:                                                                       signed: true,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   increment: Some(
// DEFAULT-NEXT:                                       Postfix {
// DEFAULT-NEXT:                                           op: Increment,
// DEFAULT-NEXT:                                           operand: Identifier(
// DEFAULT-NEXT:                                               "len",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       For {
// DEFAULT-NEXT:                                           init: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Comma {
// DEFAULT-NEXT:                                                       left: Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "i",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "c",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: CharLiteral(
// DEFAULT-NEXT:                                                               CharLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       65,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   spelling: "A",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           condition: Some(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Less,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "i",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Add,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   SizeOfType {
// DEFAULT-NEXT:                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                       rank: LongLong,
// DEFAULT-NEXT:                                                                                       signed: true,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Paren(
// DEFAULT-NEXT:                                                                   Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 10,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "10",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: SizeOfType {
// DEFAULT-NEXT:                                                                           ty: TypeName {
// DEFAULT-NEXT:                                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                                       Ranked {
// DEFAULT-NEXT:                                                                                           rank: LongLong,
// DEFAULT-NEXT:                                                                                           signed: true,
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Paren(
// DEFAULT-NEXT:                                                               SizeOfType {
// DEFAULT-NEXT:                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Ranked {
// DEFAULT-NEXT:                                                                                   rank: LongLong,
// DEFAULT-NEXT:                                                                                   signed: true,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           increment: Some(
// DEFAULT-NEXT:                                               Comma {
// DEFAULT-NEXT:                                                   left: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "i",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "c",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           body: [
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Index {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "u1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "buf",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       value: CharLiteral(
// DEFAULT-NEXT:                                                           CharLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   97,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               spelling: "a",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "c",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: CharLiteral(
// DEFAULT-NEXT:                                                               CharLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       65,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   spelling: "A",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 31,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "31",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Identifier(
// DEFAULT-NEXT:                                                                   "c",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               value: CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           65,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "A",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Index {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "u2",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "buf",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       value: Identifier(
// DEFAULT-NEXT:                                                           "c",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "p",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "memcpy",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "u1",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "buf",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "off1",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "u2",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "buf",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "off2",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       Identifier(
// DEFAULT-NEXT:                                                           "len",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       If {
// DEFAULT-NEXT:                                           condition: Binary {
// DEFAULT-NEXT:                                               op: NotEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "p",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "u1",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "buf",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "off1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           then_branch: [
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Call {
// DEFAULT-NEXT:                                                       callee: Identifier(
// DEFAULT-NEXT:                                                           "abort",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       arguments: [],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                           else_branch: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Assign {
// DEFAULT-NEXT:                                               op: Assign,
// DEFAULT-NEXT:                                               target: Identifier(
// DEFAULT-NEXT:                                                   "q",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               value: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "u1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "buf",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       For {
// DEFAULT-NEXT:                                           init: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "i",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           condition: Some(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Less,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "i",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "off1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           increment: Some(
// DEFAULT-NEXT:                                               Comma {
// DEFAULT-NEXT:                                                   left: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "i",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "q",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           body: [
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: NotEqual,
// DEFAULT-NEXT:                                                       left: Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "q",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: CharLiteral(
// DEFAULT-NEXT:                                                           CharLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   97,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               spelling: "a",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Call {
// DEFAULT-NEXT:                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                   "abort",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               arguments: [],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       For {
// DEFAULT-NEXT:                                           init: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Comma {
// DEFAULT-NEXT:                                                       left: Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "i",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Assign {
// DEFAULT-NEXT:                                                           op: Assign,
// DEFAULT-NEXT:                                                           target: Identifier(
// DEFAULT-NEXT:                                                               "c",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           value: Binary {
// DEFAULT-NEXT:                                                               op: Add,
// DEFAULT-NEXT:                                                               left: CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           65,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "A",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "off2",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           condition: Some(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Less,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "i",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Identifier(
// DEFAULT-NEXT:                                                       "len",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           increment: Some(
// DEFAULT-NEXT:                                               Comma {
// DEFAULT-NEXT:                                                   left: Comma {
// DEFAULT-NEXT:                                                       left: Postfix {
// DEFAULT-NEXT:                                                           op: Increment,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "i",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Postfix {
// DEFAULT-NEXT:                                                           op: Increment,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "q",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "c",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           body: [
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                       left: Identifier(
// DEFAULT-NEXT:                                                           "c",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: CharLiteral(
// DEFAULT-NEXT:                                                               CharLiteral {
// DEFAULT-NEXT:                                                                   encoding: Plain,
// DEFAULT-NEXT:                                                                   code_units: [
// DEFAULT-NEXT:                                                                       65,
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   spelling: "A",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 31,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "31",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Identifier(
// DEFAULT-NEXT:                                                                   "c",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               value: CharLiteral(
// DEFAULT-NEXT:                                                                   CharLiteral {
// DEFAULT-NEXT:                                                                       encoding: Plain,
// DEFAULT-NEXT:                                                                       code_units: [
// DEFAULT-NEXT:                                                                           65,
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       spelling: "A",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: NotEqual,
// DEFAULT-NEXT:                                                       left: Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "q",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "c",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Call {
// DEFAULT-NEXT:                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                   "abort",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               arguments: [],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       For {
// DEFAULT-NEXT:                                           init: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Identifier(
// DEFAULT-NEXT:                                                           "i",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           condition: Some(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: Less,
// DEFAULT-NEXT:                                                   left: Identifier(
// DEFAULT-NEXT:                                                       "i",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       SizeOfType {
// DEFAULT-NEXT:                                                           ty: TypeName {
// DEFAULT-NEXT:                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                       Ranked {
// DEFAULT-NEXT:                                                                           rank: LongLong,
// DEFAULT-NEXT:                                                                           signed: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           increment: Some(
// DEFAULT-NEXT:                                               Comma {
// DEFAULT-NEXT:                                                   left: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "i",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Postfix {
// DEFAULT-NEXT:                                                       op: Increment,
// DEFAULT-NEXT:                                                       operand: Identifier(
// DEFAULT-NEXT:                                                           "q",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           body: [
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: NotEqual,
// DEFAULT-NEXT:                                                       left: Unary {
// DEFAULT-NEXT:                                                           op: Deref,
// DEFAULT-NEXT:                                                           operand: Identifier(
// DEFAULT-NEXT:                                                               "q",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: CharLiteral(
// DEFAULT-NEXT:                                                           CharLiteral {
// DEFAULT-NEXT:                                                               encoding: Plain,
// DEFAULT-NEXT:                                                               code_units: [
// DEFAULT-NEXT:                                                                   97,
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                               spelling: "a",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: [
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Call {
// DEFAULT-NEXT:                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                   "abort",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               arguments: [],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 0,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
