#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>

#pragma STDC FENV_ROUND FE_TONEAREST
#ifdef __STDC_IEC_60559_DFP__
#pragma STDC FENV_DEC_ROUND FE_DEC_TONEAREST
#endif

#warning C23 warning directive probe

#define C23_OPTIONAL(base, ...) ((base)__VA_OPT__(+(__VA_ARGS__)))

#define C23_TYPE_KIND(T) _Generic(T, int: 1, double: 2, char *: 3, default: 4)

#define C23_BRANCH_VALUE 1
#if 0
#define C23_ELIFDEF_VALUE 0
#elifdef C23_BRANCH_VALUE
#define C23_ELIFDEF_VALUE 23
#else
#define C23_ELIFDEF_VALUE 0
#endif

#if 0
#define C23_ELIFNDEF_VALUE 0
#elifndef C23_MISSING_VALUE
#define C23_ELIFNDEF_VALUE 29
#else
#define C23_ELIFNDEF_VALUE 0
#endif

#if __has_include(<stdint.h>)
#define C23_HAS_INCLUDE_VALUE 1
#else
#define C23_HAS_INCLUDE_VALUE 0
#endif

#if __has_embed("c23_embed.bin") == __STDC_EMBED_FOUND__
#define C23_HAS_EMBED_VALUE 1
#else
#define C23_HAS_EMBED_VALUE 0
#endif

#if __has_c_attribute(reproducible)
#define C23_REPRODUCIBLE_VALUE 1
#else
#define C23_REPRODUCIBLE_VALUE 0
#endif

#if __has_c_attribute(unsequenced)
#define C23_UNSEQUENCED_VALUE 1
#else
#define C23_UNSEQUENCED_VALUE 0
#endif

#ifdef __STDC_IEC_60559_DFP__
#define C23_DECIMAL_VALUE 1
#else
#define C23_DECIMAL_VALUE 0
#endif

#define C23_STORAGE_COMPOUND_VALUE 0

static const unsigned char c23_embedded[] = {
#embed "c23_embed.bin" limit(3)
};

enum C23Fixed : unsigned short { C23_FIXED_FIRST = 31, C23_FIXED_SECOND = 37 };

enum C23Wide { C23_WIDE_VALUE = 0x1ffffffff };

struct C23Empty {
  int first;
  int second;
};

typedef int C23Array[3];

[[deprecated("C23 deprecated attribute")]]
static int c23_deprecated_value = 41;

[[maybe_unused, maybe_unused]]
static int c23_maybe_unused_value = 43;

[[nodiscard("C23 nodiscard attribute")]]
static int c23_nodiscard_value(void) {
  return 47;
}

[[noreturn]]
static void c23_never_return(void) {
  exit(99);
}

static thread_local int c23_thread_value = 53;
static volatile int     c23_never_flag;

constexpr int c23_file_constant = 59;

static int c23_unnamed_parameter(int, int value) { return value; }

static int c23_label_declaration(int value) {
  goto c23_label;
c23_label:
  int result = value + 1;
  return result;
}

static void c23_label_before_brace(void) {
  goto c23_end;
c23_end:
}

static int c23_switch_fallthrough(int value) {
  int result = 0;
  switch (value) {
  case 1:
    result += 3;
    [[fallthrough]];
  case 2:
    result += 5;
    break;
  default:
    break;
  }
  return result;
}

static int c23_relaxed_variadic(...) {
  va_list arguments;
  int     first;
  int     second;
  va_start(arguments);
  first  = va_arg(arguments, int);
  second = va_arg(arguments, int);
  va_end(arguments);
  return first + second;
}

int main(void) {
  constexpr int          local_constant            = 61;
  alignas(32) int        aligned_value             = 3;
  auto                   inferred_value            = 67;
  typeof(inferred_value) same_type_value           = 71;
  const int              qualified_value           = 73;
  typeof_unqual(qualified_value) unqualified_value = 79;
  int \u03b1                                       = 5;
  signed _BitInt(17) signed_precise                = -12345;
  unsigned _BitInt(17) unsigned_precise            = 100000uwb;
  int                  binary_value                = 0b1010'0101;
  char8_t              utf8_character              = u8'Z';
  static const char8_t utf8_text[]                 = u8"\u03a9";
  struct C23Empty      empty_struct                = {};
  int                  empty_array[3]              = {};
  const C23Array       qualified_array             = {2, 3, 5};
  enum C23Fixed        fixed_value                 = C23_FIXED_SECOND;
  enum C23Wide         wide_value                  = C23_WIDE_VALUE;
  nullptr_t            null_value                  = nullptr;
  int                 *null_pointer                = nullptr;
  bool                 boolean_value               = true;
  bool                 false_value                 = false;
  int                  static_compound_value       = 83;
  int                  language_total;
  int                  attribute_total;
  int                  preprocessor_total;
  int                  type_total;
  int                  control_total;
  int                  removal_total;
#ifdef __STDC_IEC_60559_DFP__
  _Decimal32 decimal_value = 1.5df;
#endif

  static_assert(sizeof(binary_value) == sizeof(int));
  _Static_assert(sizeof(signed_precise) >= 3);
  static_assert(-1 == ~0);

  language_total =
      c23_file_constant + local_constant + inferred_value + same_type_value +
      unqualified_value + (int)signed_precise + (int)unsigned_precise +
      binary_value + aligned_value + alignof(int) + \u03b1 + utf8_character +
      (unsigned char)utf8_text[0] + (unsigned char)utf8_text[1] +
      empty_struct.first + empty_array[0] + qualified_array[0] +
      qualified_array[2] + fixed_value + (int)(wide_value == C23_WIDE_VALUE) +
      static_compound_value + C23_OPTIONAL(7) + C23_OPTIONAL(11, 13) +
      C23_TYPE_KIND(int) + C23_TYPE_KIND(double) + C23_TYPE_KIND(char *) +
      C23_TYPE_KIND(long);

  attribute_total    = c23_nodiscard_value();
  preprocessor_total = C23_ELIFDEF_VALUE + C23_ELIFNDEF_VALUE +
                       C23_HAS_INCLUDE_VALUE + C23_HAS_EMBED_VALUE +
                       C23_REPRODUCIBLE_VALUE + C23_UNSEQUENCED_VALUE +
                       C23_DECIMAL_VALUE + C23_STORAGE_COMPOUND_VALUE +
                       c23_embedded[0] + c23_embedded[1] + c23_embedded[2];
  type_total         = (null_value == nullptr) + (null_pointer == nullptr) +
                       boolean_value + !false_value + c23_thread_value;
  control_total = c23_unnamed_parameter(89, 97) + c23_label_declaration(101) +
                  c23_switch_fallthrough(1) + c23_relaxed_variadic(103, 107);
  removal_total = (__STDC_VERSION__ == 202311L);

  if (c23_never_flag) {
    c23_never_return();
  }
  c23_label_before_brace();

  printf("%d %d %d %d %d %d\n", language_total, attribute_total,
         preprocessor_total, type_total, control_total, removal_total);
  return 0;
}

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Typedef {
// DEFAULT-NEXT:       name: "__va_list",
// DEFAULT-NEXT:       ty: TargetBuiltin(
// DEFAULT-NEXT:           "__builtin_va_list",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               26,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 527,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   25,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
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
// DEFAULT-NEXT:               27,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   27,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               is_noreturn: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "exit",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Name(
// DEFAULT-NEXT:                               "status",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               28,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 66,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   28,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: Some(
// DEFAULT-NEXT:                           false,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Array {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "c23_embedded",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               size: Unspecified,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               List(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       67,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       50,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       InitializerItem {
// DEFAULT-NEXT:                           designators: [],
// DEFAULT-NEXT:                           value: Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       51,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 67,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Enum(
// DEFAULT-NEXT:       EnumDecl {
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "C23Fixed",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "C23_FIXED_FIRST",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           31,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "C23_FIXED_SECOND",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           37,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 71,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Enum(
// DEFAULT-NEXT:       EnumDecl {
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "C23Wide",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "C23_WIDE_VALUE",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       IntLit(
// DEFAULT-NEXT:                           8589934591,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 73,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "C23Empty",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
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
// DEFAULT-NEXT:                               "first",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 76,
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
// DEFAULT-NEXT:                               "second",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 77,
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
// DEFAULT-NEXT:               line: 75,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: Typedef {
// DEFAULT-NEXT:       name: "C23Array",
// DEFAULT-NEXT:       ty: Integer(
// DEFAULT-NEXT:           Ranked {
// DEFAULT-NEXT:               rank: Int,
// DEFAULT-NEXT:               signed: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 80,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "c23_deprecated_value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           41,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               Deprecated(
// DEFAULT-NEXT:                   Some(
// DEFAULT-NEXT:                       "C23 deprecated attribute",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 82,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "c23_maybe_unused_value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           43,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               MaybeUnused,
// DEFAULT-NEXT:               MaybeUnused,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 85,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "c23_nodiscard_value",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           47,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 88,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoDiscard(
// DEFAULT-NEXT:                   Some(
// DEFAULT-NEXT:                       "C23 nodiscard attribute",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[11]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "c23_never_return",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "exit",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   99,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 93,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoReturn,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[12]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:               is_thread_local: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "c23_thread_value",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           53,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 98,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_volatile: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "c23_never_flag",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 99,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[14]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               is_constexpr: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "c23_file_constant",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           59,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 101,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[15]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "c23_unnamed_parameter",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 103,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[16]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "c23_label_declaration",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Goto(
// DEFAULT-NEXT:                   "c23_label",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Labeled(
// DEFAULT-NEXT:                   "c23_label",
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "result",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "value",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "result",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 105,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[17]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "c23_label_before_brace",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Goto(
// DEFAULT-NEXT:                   "c23_end",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Labeled(
// DEFAULT-NEXT:                   "c23_end",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 112,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[18]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "c23_switch_fallthrough",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "result",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Switch {
// DEFAULT-NEXT:                   discriminant: Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Case(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: AddAssign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "result",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Attribute(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Fallthrough,
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Case(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: AddAssign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "result",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       5,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Break,
// DEFAULT-NEXT:                       Default,
// DEFAULT-NEXT:                       Break,
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "result",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 117,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[19]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "c23_relaxed_variadic",
// DEFAULT-NEXT:           variadic: true,
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "__va_list",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "arguments",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "first",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "second",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "__builtin_c23_va_start",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "arguments",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "first",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: VaArg {
// DEFAULT-NEXT:                               ap: Identifier(
// DEFAULT-NEXT:                                   "arguments",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "second",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: VaArg {
// DEFAULT-NEXT:                               ap: Identifier(
// DEFAULT-NEXT:                                   "arguments",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "__builtin_va_end",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "arguments",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Add,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "first",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "second",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 132,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[20]: Function(
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
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           is_constexpr: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "local_constant",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       61,
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "aligned_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       3,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       attributes: [
// DEFAULT-NEXT:                           Aligned(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   32,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: TargetBuiltin(
// DEFAULT-NEXT:                               "__auto_type",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           storage: Auto,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "inferred_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       67,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: TypeOf(
// DEFAULT-NEXT:                               Expression(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "inferred_value",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "same_type_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       71,
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "qualified_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       73,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: TypeOfUnqual(
// DEFAULT-NEXT:                               Expression(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "qualified_value",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "unqualified_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       79,
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "\\u03b1",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       5,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               BitInt {
// DEFAULT-NEXT:                                   width: Integer(
// DEFAULT-NEXT:                                       17,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "signed_precise",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Minus,
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           12345,
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
// DEFAULT-NEXT:                               BitInt {
// DEFAULT-NEXT:                                   width: Integer(
// DEFAULT-NEXT:                                       17,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "unsigned_precise",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       100000,
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "binary_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       165,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: TargetBuiltin(
// DEFAULT-NEXT:                               "char8_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "utf8_character",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       90,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: TargetBuiltin(
// DEFAULT-NEXT:                               "char8_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           storage: Static,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "utf8_text",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Unspecified,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Utf8StringLit(
// DEFAULT-NEXT:                                       "\\u03a9",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "C23Empty",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "empty_struct",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [],
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "empty_array",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               IntLit(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "C23Array",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "qualified_array",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   3,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   5,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Enum,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "C23Fixed",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "fixed_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "C23_FIXED_SECOND",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Enum,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "C23Wide",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "wide_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "C23_WIDE_VALUE",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: TargetBuiltin(
// DEFAULT-NEXT:                               "nullptr_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "null_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "nullptr",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "null_pointer",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "nullptr",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Bool,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "boolean_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "true",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Bool,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "false_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "false",
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "static_compound_value",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       83,
// DEFAULT-NEXT:                                   ),
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
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "language_total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "attribute_total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "preprocessor_total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "type_total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "control_total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "removal_total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               StaticAssert(
// DEFAULT-NEXT:                   StaticAssert {
// DEFAULT-NEXT:                       condition: Const(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: SizeOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "binary_value",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: SizeOfType {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               StaticAssert(
// DEFAULT-NEXT:                   StaticAssert {
// DEFAULT-NEXT:                       condition: Const(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: GreaterEqual,
// DEFAULT-NEXT:                               left: SizeOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "signed_precise",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   3,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               StaticAssert(
// DEFAULT-NEXT:                   StaticAssert {
// DEFAULT-NEXT:                       condition: Const(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Unary {
// DEFAULT-NEXT:                                   op: BitNot,
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "language_total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Add,
// DEFAULT-NEXT:                                                               left: Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: Binary {
// DEFAULT-NEXT:                                                                       op: Add,
// DEFAULT-NEXT:                                                                       left: Binary {
// DEFAULT-NEXT:                                                                           op: Add,
// DEFAULT-NEXT:                                                                           left: Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                       op: Add,
// DEFAULT-NEXT:                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                               op: Add,
// DEFAULT-NEXT:                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                       op: Add,
// DEFAULT-NEXT:                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                               op: Add,
// DEFAULT-NEXT:                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                                       op: Add,
// DEFAULT-NEXT:                                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                                               op: Add,
// DEFAULT-NEXT:                                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                                                                                       "c23_file_constant",
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                                                       "local_constant",
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                                                   "inferred_value",
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                                               "same_type_value",
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                                                                           "unqualified_value",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   right: Cast {
// DEFAULT-NEXT:                                                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                                                           Ranked {
// DEFAULT-NEXT:                                                                                                                               rank: Int,
// DEFAULT-NEXT:                                                                                                                               signed: true,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                       value: Identifier(
// DEFAULT-NEXT:                                                                                                                           "signed_precise",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               right: Cast {
// DEFAULT-NEXT:                                                                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                                                                       Ranked {
// DEFAULT-NEXT:                                                                                                                           rank: Int,
// DEFAULT-NEXT:                                                                                                                           signed: true,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                                                                       "unsigned_precise",
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                                               "binary_value",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                                                           "aligned_value",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   right: Call {
// DEFAULT-NEXT:                                                                                                       callee: Identifier(
// DEFAULT-NEXT:                                                                                                           "alignof",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       arguments: [
// DEFAULT-NEXT:                                                                                                           Identifier(
// DEFAULT-NEXT:                                                                                                               "int",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ],
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "\\u03b1",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           right: Identifier(
// DEFAULT-NEXT:                                                                                               "utf8_character",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Cast {
// DEFAULT-NEXT:                                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                                               Char {
// DEFAULT-NEXT:                                                                                                   signed: Some(
// DEFAULT-NEXT:                                                                                                       false,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                                           value: Index {
// DEFAULT-NEXT:                                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                                   "utf8_text",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               index: Integer(
// DEFAULT-NEXT:                                                                                                   0,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Cast {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: Some(
// DEFAULT-NEXT:                                                                                                   false,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                       value: Index {
// DEFAULT-NEXT:                                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                                               "utf8_text",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           index: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               right: Member {
// DEFAULT-NEXT:                                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                                       "empty_struct",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   field: "first",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: Index {
// DEFAULT-NEXT:                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                   "empty_array",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               index: Integer(
// DEFAULT-NEXT:                                                                                   0,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Index {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "qualified_array",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           index: Integer(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Index {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "qualified_array",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       index: Integer(
// DEFAULT-NEXT:                                                                           2,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "fixed_value",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Cast {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Ranked {
// DEFAULT-NEXT:                                                                       rank: Int,
// DEFAULT-NEXT:                                                                       signed: true,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: Equal,
// DEFAULT-NEXT:                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                       "wide_value",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                       "C23_WIDE_VALUE",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Identifier(
// DEFAULT-NEXT:                                                           "static_compound_value",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       7,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Integer(
// DEFAULT-NEXT:                                                       11,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       13,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Generic {
// DEFAULT-NEXT:                                               controlling: Identifier(
// DEFAULT-NEXT:                                                   "<type-name>",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               associations: [
// DEFAULT-NEXT:                                                   ConstGenericAssociation {
// DEFAULT-NEXT:                                                       type_name: Some(
// DEFAULT-NEXT:                                                           "int",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       expression: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   ConstGenericAssociation {
// DEFAULT-NEXT:                                                       type_name: Some(
// DEFAULT-NEXT:                                                           "double",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       expression: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   ConstGenericAssociation {
// DEFAULT-NEXT:                                                       type_name: Some(
// DEFAULT-NEXT:                                                           "char *",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       expression: Integer(
// DEFAULT-NEXT:                                                           3,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   ConstGenericAssociation {
// DEFAULT-NEXT:                                                       type_name: None,
// DEFAULT-NEXT:                                                       expression: Integer(
// DEFAULT-NEXT:                                                           4,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Generic {
// DEFAULT-NEXT:                                           controlling: Identifier(
// DEFAULT-NEXT:                                               "<type-name>",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           associations: [
// DEFAULT-NEXT:                                               ConstGenericAssociation {
// DEFAULT-NEXT:                                                   type_name: Some(
// DEFAULT-NEXT:                                                       "int",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   expression: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               ConstGenericAssociation {
// DEFAULT-NEXT:                                                   type_name: Some(
// DEFAULT-NEXT:                                                       "double",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   expression: Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               ConstGenericAssociation {
// DEFAULT-NEXT:                                                   type_name: Some(
// DEFAULT-NEXT:                                                       "char *",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   expression: Integer(
// DEFAULT-NEXT:                                                       3,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               ConstGenericAssociation {
// DEFAULT-NEXT:                                                   type_name: None,
// DEFAULT-NEXT:                                                   expression: Integer(
// DEFAULT-NEXT:                                                       4,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Generic {
// DEFAULT-NEXT:                                       controlling: Identifier(
// DEFAULT-NEXT:                                           "<type-name>",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       associations: [
// DEFAULT-NEXT:                                           ConstGenericAssociation {
// DEFAULT-NEXT:                                               type_name: Some(
// DEFAULT-NEXT:                                                   "int",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               expression: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           ConstGenericAssociation {
// DEFAULT-NEXT:                                               type_name: Some(
// DEFAULT-NEXT:                                                   "double",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               expression: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           ConstGenericAssociation {
// DEFAULT-NEXT:                                               type_name: Some(
// DEFAULT-NEXT:                                                   "char *",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               expression: Integer(
// DEFAULT-NEXT:                                                   3,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           ConstGenericAssociation {
// DEFAULT-NEXT:                                               type_name: None,
// DEFAULT-NEXT:                                               expression: Integer(
// DEFAULT-NEXT:                                                   4,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Generic {
// DEFAULT-NEXT:                                   controlling: Identifier(
// DEFAULT-NEXT:                                       "<type-name>",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   associations: [
// DEFAULT-NEXT:                                       ConstGenericAssociation {
// DEFAULT-NEXT:                                           type_name: Some(
// DEFAULT-NEXT:                                               "int",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           expression: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       ConstGenericAssociation {
// DEFAULT-NEXT:                                           type_name: Some(
// DEFAULT-NEXT:                                               "double",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           expression: Integer(
// DEFAULT-NEXT:                                               2,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       ConstGenericAssociation {
// DEFAULT-NEXT:                                           type_name: Some(
// DEFAULT-NEXT:                                               "char *",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           expression: Integer(
// DEFAULT-NEXT:                                               3,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       ConstGenericAssociation {
// DEFAULT-NEXT:                                           type_name: None,
// DEFAULT-NEXT:                                           expression: Integer(
// DEFAULT-NEXT:                                               4,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "attribute_total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "c23_nodiscard_value",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "preprocessor_total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Add,
// DEFAULT-NEXT:                                                               left: Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: Integer(
// DEFAULT-NEXT:                                                                       23,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Integer(
// DEFAULT-NEXT:                                                                       29,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Index {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "c23_embedded",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Index {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "c23_embedded",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Index {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "c23_embedded",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       2,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "type_total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "null_value",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "nullptr",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "null_pointer",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "nullptr",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "boolean_value",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Identifier(
// DEFAULT-NEXT:                                           "false_value",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Identifier(
// DEFAULT-NEXT:                                   "c23_thread_value",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "control_total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "c23_unnamed_parameter",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   89,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   97,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "c23_label_declaration",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   101,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "c23_switch_fallthrough",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "c23_relaxed_variadic",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           103,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           107,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "removal_total",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Integer(
// DEFAULT-NEXT:                                   202311,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   202311,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "c23_never_flag",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "c23_never_return",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "c23_label_before_brace",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "printf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "%d %d %d %d %d %d\\n",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "language_total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "attribute_total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "preprocessor_total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "type_total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "control_total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "removal_total",
// DEFAULT-NEXT:                               ),
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
// DEFAULT-NEXT:               line: 143,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
