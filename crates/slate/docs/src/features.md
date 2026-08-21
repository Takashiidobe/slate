# Features

Slate has full C23 support. If it doesn't support C23, it's a bug to be
fixed.

## C89

- [ ] Trigraphs
- [ ] Escape sequences
- [ ] Integer constants: decimal, octal, hexadecimal
- [ ] Floating constants
- [ ] Character constants
- [ ] String literals
- [ ] Adjacent string-literal concatenation
- [ ] Comments `/* ... */`
- [ ] Preprocessor object-like macros
- [ ] Preprocessor function-like macros
- [ ] Macro argument substitution
- [ ] Stringification operator #
- [ ] Token-pasting operator ##
- [ ] #include
- [ ] #define
- [ ] #undef
- [ ] #if
- [ ] #ifdef
- [ ] #ifndef
- [ ] #elif
- [ ] #else
- [ ] #endif
- [ ] defined
- [ ] #line
- [ ] #error
- [ ] #pragma
- [ ] Predefined macros: **LINE**
- [ ] **FILE**
- [ ] **DATE**
- [ ] **TIME**
- [ ] **STDC**
- [ ] Types
- [ ] char
- [ ] signed char
- [ ] unsigned char
- [ ] short
- [ ] unsigned short
- [ ] int
- [ ] unsigned int
- [ ] long
- [ ] unsigned long
- [ ] float
- [ ] double
- [ ] long double
- [ ] void
- [ ] pointers
- [ ] arrays
- [ ] multidimensional arrays
- [ ] functions
- [ ] structures
- [ ] unions
- [ ] enumerations
- [ ] bit-fields
- [ ] typedef
- [ ] incomplete types
- [ ] compatible/composite types
- [ ] Type qualifiers
- [ ] const
- [ ] volatile
- [ ] qualified pointers
- [ ] qualifier propagation rules
- [ ] Storage / linkage
- [ ] auto
- [ ] register
- [ ] static
- [ ] extern
- [ ] automatic storage duration
- [ ] static storage duration
- [ ] external linkage
- [ ] internal linkage
- [ ] no linkage
- [ ] tentative definitions
- [ ] Declarations / functions
- [ ] Function prototypes
- [ ] Prototype parameter type checking
- [ ] Old-style/K&R function declarations
- [ ] Old-style/K&R function definitions
- [ ] Variadic functions via ...
- [ ] Array declarators
- [ ] Function-pointer declarators
- [ ] Abstract declarators
- [ ] Initializers
- [ ] Aggregate initialization
- [ ] Expressions
- [ ] Integer promotions
- [ ] Usual arithmetic conversions
- [ ] Lvalue semantics
- [ ] Array-to-pointer conversion
- [ ] Function-to-pointer conversion
- [ ] Pointer arithmetic
- [ ] Pointer comparison
- [ ] Structure/union member access
- [ ] Cast expressions
- [ ] sizeof
- [ ] Unary operators
- [ ] Arithmetic operators
- [ ] Relational/equality operators
- [ ] Logical operators
- [ ] Bitwise operators
- [ ] Shift operators
- [ ] Assignment operators
- [ ] Conditional ?:
- [ ] Comma operator
- [ ] Function calls
- [ ] Increment/decrement
- [ ] Statements
- [ ] Compound statements
- [ ] if
- [ ] else
- [ ] switch
- [ ] case
- [ ] default
- [ ] while
- [ ] do
- [ ] for
- [ ] goto
- [ ] labels
- [ ] continue
- [ ] break
- [ ] return

### C89 library

- [ ] <assert.h>
- [ ] <ctype.h>
- [ ] <errno.h>
- [ ] <float.h>
- [ ] <limits.h>
- [ ] <locale.h>
- [ ] <math.h>
- [ ] <setjmp.h>
- [ ] <signal.h>
- [ ] <stdarg.h>
- [ ] <stddef.h>
- [ ] <stdio.h>
- [ ] <stdlib.h>
- [ ] <string.h>
- [ ] <time.h>

## C95

- [ ] Digraph <: -> [
- [ ] Digraph :> -> ]
- [ ] Digraph <% -> {
- [ ] Digraph %> -> }
- [ ] Digraph %: -> #
- [ ] Digraph %:%: -> ##
- [ ] **STDC_VERSION**
- [ ] **STDC_VERSION** == 199409L

### Library

- [ ] <iso646.h>
- [ ] Alternative operator macros such as and, or, not, xor
- [ ] <wchar.h>
- [ ] <wctype.h>
- [ ] Expanded wchar_t support
- [ ] Wide-character I/O
- [ ] Wide-string functions
- [ ] Multibyte <-> wide-character conversions
- [ ] Wide-character classification
- [ ] Wide-character case conversion

## C99

- [ ] // line comments
- [ ] Universal character names \uXXXX
- [ ] Universal character names \UXXXXXXXX
- [ ] Universal character names in identifiers
- [ ] Increased minimum translation limits
- [ ] Integer / arithmetic types
- [ ] \_Bool
- [ ] long long
- [ ] unsigned long long
- [ ] Extended signed integer types
- [ ] Extended unsigned integer types
- [ ] Revised integer conversion rules
- [ ] Signed integer division truncates toward zero
- [ ] Corresponding remainder semantics
- [ ] Complex arithmetic
- [ ] \_Complex
- [ ] float \_Complex
- [ ] double \_Complex
- [ ] long double \_Complex
- [ ] \_Imaginary where supported by the standard's model
- [ ] Complex literals through library macros/functions
- [ ] Complex arithmetic semantics
- [ ] Floating point
- [ ] Hexadecimal floating constants, e.g. 0x1.8p+2
- [ ] FLT_EVAL_METHOD
- [ ] Floating-point environment semantics
- [ ] Floating-point contraction rules
- [ ] IEC 60559/IEEE-754 feature macros where applicable
- [ ] Qualifiers / declarations
- [ ] restrict
- [ ] Idempotent qualifiers, e.g. repeated const
- [ ] inline
- [ ] Mixed declarations and statements
- [ ] Declaration in for initializer
- [ ] for (int i = 0; ...)
- [ ] Arrays
- [ ] Variable-length arrays
- [ ] Variably-modified types
- [ ] VLA runtime sizeof
- [ ] Function prototype [*] VLA notation
- [ ] static inside array function parameters
- [ ] const/volatile/restrict inside array parameter brackets
- [ ] Flexible array members
- [ ] Initialization
- [ ] Nonconstant initializers for automatic aggregate objects
- [ ] Array designated initializers [index] = value
- [ ] Structure designated initializers .member = value
- [ ] Nested designated initializers
- [ ] Compound literals (T){...}
- [ ] Enumerations
- [ ] Trailing comma in enumerator list
- [ ] Functions
- [ ] **func**
- [ ] Implicit return 0 from main
- [ ] C99 inline linkage semantics
- [ ] Removed C89 behavior
- [ ] Reject implicit int
- [ ] Reject implicit function declarations

### Preprocessor

- [ ] Variadic macros ...
- [ ] **VA_ARGS**
- [ ] Empty arguments to function-like macros
- [ ] \_Pragma
- [ ] #pragma STDC FENV_ACCESS
- [ ] #pragma STDC FP_CONTRACT
- [ ] #pragma STDC CX_LIMITED_RANGE

### C99 version / feature macros

- [ ] **STDC_VERSION** == 199901L
- [ ] **STDC_HOSTED**
- [ ] **STDC_IEC_559** when applicable
- [ ] **STDC_IEC_559_COMPLEX** when applicable
- [ ] **STDC_ISO_10646** when applicable

### C99 library

#### Headers:

- [ ] <stdbool.h>
- [ ] <stdint.h>
- [ ] <inttypes.h>
- [ ] <complex.h>
- [ ] <fenv.h>
- [ ] <tgmath.h>
- [ ] Integer library
- [ ] int8_t, int16_t, etc. where exact-width types exist
- [ ] uint8_t, uint16_t, etc.
- [ ] int_leastN_t / uint_leastN_t
- [ ] int_fastN_t / uint_fastN_t
- [ ] intptr_t / uintptr_t where provided
- [ ] intmax_t
- [ ] uintmax_t
- [ ] INTN*\* / UINTN*\* limits
- [ ] PRI\* format macros
- [ ] SCN\* format macros
- [ ] General library additions
- [ ] llabs
- [ ] lldiv
- [ ] strtoll
- [ ] strtoull
- [ ] strtoimax
- [ ] strtoumax
- [ ] imaxabs
- [ ] imaxdiv
- [ ] \_Exit
- [ ] va_copy
- [ ] snprintf
- [ ] vsnprintf
- [ ] isblank
- [ ] strtof
- [ ] strtold
- [ ] Wide-character integer conversion counterparts

#### Formatted I/O

- [ ] %a / %A hexadecimal floating format
- [ ] hh length modifier
- [ ] ll length modifier
- [ ] j length modifier
- [ ] z length modifier
- [ ] t length modifier
- [ ] C99 scanf/printf family extensions

#### Math

- [ ] float variants of math functions
- [ ] long double variants
- [ ] New C99 mathematical functions
- [ ] fma
- [ ] remainder / remquo
- [ ] nextafter / nexttoward
- [ ] classification macros such as isfinite
- [ ] comparison macros such as isgreater
- [ ] floating-point environment functions
- [ ] complex math functions
- [ ] <tgmath.h> type-generic dispatch

## C11

### Compiler

- [ ] Memory model / concurrency
- [ ] C11 memory model
- [ ] Sequenced-before relation
- [ ] Data-race rules
- [ ] Happens-before rules
- [ ] Atomic objects
- [ ] \_Atomic(type)
- [ ] \_Atomic qualifier/specifier
- [ ] Atomic initialization semantics
- [ ] Atomic load/store semantics
- [ ] Atomic read-modify-write semantics
- [ ] Memory-order model
- [ ] Thread-local storage
- [ ] \_Thread_local
- [ ] TLS with static
- [ ] TLS with extern
- [ ] Thread storage duration
- [ ] Alignment
- [ ] \_Alignof
- [ ] \_Alignas
- [ ] Extended/over-alignment
- [ ] Fundamental alignment
- [ ] Valid alignment constraints
- [ ] Generic programming
- [ ] \_Generic
- [ ] Generic association lists
- [ ] default generic association
- [ ] Type-based selection semantics
- [ ] Compile-time assertions
- [ ] \_Static_assert(expr, "message")
- [ ] Function properties
- [ ] \_Noreturn
- [ ] Structures / unions
- [ ] Anonymous structure members
- [ ] Anonymous union members
- [ ] Unicode literals
- [ ] u'…'
- [ ] U'…'
- [ ] u8"…"
- [ ] u"…"
- [ ] U"…"
- [ ] Expression / object semantic changes
- [ ] Finer-grained evaluation/sequencing rules
- [ ] Temporary-object lifetime rules
- [ ] Updated effective-type / memory-model interactions
- [ ] Analyzability
- [ ] Analyzability specification
- [ ] **STDC_ANALYZABLE** when supported
- [ ] Conditional feature macros
- [ ] **STDC_NO_ATOMICS**
- [ ] **STDC_NO_THREADS**
- [ ] **STDC_NO_VLA**
- [ ] **STDC_NO_COMPLEX**
- [ ] Version
- [ ] **STDC_VERSION** == 201112L
- [ ] C11 library
- [ ] New headers
- [ ] <stdalign.h>
- [ ] <stdatomic.h>
- [ ] <stdnoreturn.h>
- [ ] <threads.h>
- [ ] <uchar.h>
- [ ] Atomics library
- [ ] atomic\_\* typedefs
- [ ] atomic_init
- [ ] atomic_store
- [ ] atomic_load
- [ ] atomic_exchange
- [ ] atomic*compare_exchange*\*
- [ ] atomic_fetch_add
- [ ] atomic_fetch_sub
- [ ] atomic_fetch_or
- [ ] atomic_fetch_xor
- [ ] atomic_fetch_and
- [ ] atomic_flag
- [ ] atomic fences
- [ ] memory*order*\*
- [ ] lock-free query facilities
- [ ] Threads library
- [ ] thrd_t
- [ ] thread creation
- [ ] thread joining
- [ ] thread detaching
- [ ] thrd_current
- [ ] thrd_equal
- [ ] thrd_sleep
- [ ] mutexes mtx\_\*
- [ ] condition variables cnd\_\*
- [ ] thread-specific storage tss\_\*
- [ ] once_flag
- [ ] call_once
- [ ] Unicode
- [ ] char16_t
- [ ] char32_t
- [ ] mbrtoc16
- [ ] mbrtoc32
- [ ] c16rtomb
- [ ] c32rtomb
- [ ] Runtime / allocation
- [ ] aligned_alloc
- [ ] quick_exit
- [ ] at_quick_exit
- [ ] timespec
- [ ] timespec_get
- [ ] Exclusive "x" mode for fopen/freopen
- [ ] Thread-local errno
- [ ] Complex / floating support
- [ ] CMPLX
- [ ] CMPLXF
- [ ] CMPLXL
- [ ] FLT_DECIMAL_DIG
- [ ] DBL_DECIMAL_DIG
- [ ] LDBL_DECIMAL_DIG
- [ ] \*\_TRUE_MIN
- [ ] \*\_HAS_SUBNORM
- [ ] Annex K — optional
- [ ]
- [ ] Annex K is not required for ordinary conformance.
- [ ]
- [ ] **STDC_LIB_EXT1**, if implemented
- [ ] Bounds-checking interfaces
- [ ] \_s functions
- [ ] runtime-constraint handlers
- [ ] Removed
- [ ] gets() removed
- [ ] C17 / C18
- [ ]
- [ ] ISO/IEC 9899:2018 is usually called C17 or C18. It introduced no major new language or library facility; its purpose was primarily to roll C11 defect resolutions into the standard. cppreference counts 54 defect reports incorporated into C17.
- [ ]
- [ ] **STDC_VERSION** == 201710L
- [ ] All C11 features
- [ ] All applicable C11 defect-report corrections
- [ ] Updated atomic semantics
- [ ] Updated effective-type/object semantics where corrected
- [ ] Updated library wording from C11 defect reports
- [ ] Updated threading wording from C11 defect reports
- [ ] Updated complex/floating-point wording from C11 defect reports
- [ ] Updated generic-selection wording from C11 defect reports
- [ ]
- [ ] For a real C17 conformance suite, the DRs need individual regression tests. Simply accepting C11 syntax is not sufficient.
- [ ]

## C23

### C23 new types

- [ ] \_BitInt(N)
- [ ] signed \_BitInt(N)
- [ ] unsigned \_BitInt(N)
- [ ] Distinct type for each supported width
- [ ] BITINT_MAXWIDTH
- [ ] Bit-precise integer promotions/conversions
- [ ] Bit-precise integer arithmetic
- [ ] Bit-precise integer constant suffix wb
- [ ] Bit-precise unsigned constant suffix uwb
- [ ] Conditional decimal floating point

### Decimal types

- only when the implementation advertises decimal IEC 60559 support through **STDC_IEC_60559_DFP**.
- [ ] \_Decimal32
- [ ] \_Decimal64
- [ ] \_Decimal128
- [ ] Decimal arithmetic
- [ ] Decimal constants/suffixes
- [ ] Decimal conversions
- [ ] Decimal floating environment where advertised
- [ ] C23 integer representation
- [ ] Two's-complement signed integers are mandatory
- [ ] Sign-and-magnitude representation no longer supported
- [ ] Ones'-complement representation no longer supported
- [ ] Corresponding signed integer minimum values updated

### C23 literals

- [ ] Binary literals 0b...
- [ ] Binary literals 0B...
- [ ] Digit separators ', e.g. 1'000'000
- [ ] Digit separators in integer constants
- [ ] Digit separators in floating constants
- [ ] u8'…' UTF-8 character constants
- [ ] u8"…" has type char8_t[N]
- [ ] UTF-16 semantics required for u"…"
- [ ] UTF-32 semantics required for U"…"
- [ ] char8_t is a typedef of unsigned char in <uchar.h>, and C23 changes UTF-8 string literals from char[N] to char8_t[N].

### C23 keywords

- [ ] bool is a keyword
- [ ] true is a keyword
- [ ] false is a keyword
- [ ] alignas is a keyword
- [ ] alignof is a keyword
- [ ] static_assert is a keyword
- [ ] thread_local is a keyword
- [ ] constexpr is a keyword
- [ ] typeof
- [ ] typeof_unqual
- [ ] nullptr
- [ ] The underscore-prefixed historical forms remain relevant for compatibility, although \_Noreturn is deprecated.
- [ ] C23 nullptr
- [ ] nullptr null pointer constant
- [ ] nullptr_t
- [ ] Correct conversions from nullptr
- [ ] Correct comparisons involving nullptr
- [ ] \_Generic interaction with nullptr_t
- [ ] C23 type inference
- [ ] auto object type inference
- [ ] Inference from initializer
- [ ] Correct interaction between old storage-class use of auto and inferred-type use
- [ ] Reject unsupported C++-style return-type inference
- [ ] Reject unsupported parameter type inference
- [ ] C23 typeof
- [ ] typeof(expression)
- [ ] typeof(type-name)
- [ ] typeof_unqual(expression)
- [ ] typeof_unqual(type-name)
- [ ] Qualifier-preserving typeof
- [ ] Qualifier-removing typeof_unqual
- [ ] Unevaluated operand semantics where required
- [ ] C23 constexpr
- [ ] constexpr object declarations
- [ ] Constant initialization requirements
- [ ] Compile-time value semantics
- [ ] Correct linkage/storage restrictions
- [ ] Use in integer constant expressions where permitted
- [ ] C23 constexpr
- [ ] C23 initialization
- [ ] Empty initializer {}
- [ ] T x = {};
- [ ] Zero-initialization semantics for empty initializer
- [ ] Empty initialization of arrays
- [ ] Empty initialization of structures
- [ ] Empty initialization of unions
- [ ] C23 compound literals
- [ ] Storage-class specifier on compound literal
- [ ] static compound literal where allowed
- [ ] register compound literal where allowed
- [ ] thread_local compound literal where allowed
- [ ] constexpr compound literal where allowed by the grammar/rules
- [ ] C23 attributes
- [ ] General attribute system
- [ ] [[...]] syntax
- [ ] Attribute namespaces
- [ ] Unknown attributes handled according to C23 rules
- [ ] Duplicate attributes
- [ ] Attribute placement rules
- [ ] \_\_has_c_attribute
- [ ] Standard attributes
- [ ] [[deprecated]]
- [ ] [[deprecated("reason")]]
- [ ] [[fallthrough]]
- [ ] [[maybe_unused]]
- [ ] [[maybe_unused]] on labels
- [ ] [[nodiscard]]
- [ ] [[nodiscard("reason")]]
- [ ] [[noreturn]]
- [ ] [[reproducible]]
- [ ] [[unsequenced]]
- [ ] C23 static assertions
- [ ] static_assert(expr)
- [ ] static_assert(expr, "message")
- [ ] \_Static_assert(expr)
- [ ] \_Static_assert(expr, "message")
- [ ] static_assert no longer depends on <assert.h>
- [ ] C23 function declarations
- [ ] Old-style/K&R function definitions removed
- [ ] Old-style function declarations without prototypes removed
- [ ] f() means a function with no parameters
- [ ] Unnamed parameters permitted in function definitions
- [ ] Relaxed variadic parameter lists
- [ ] void f(...); where permitted by C23
- [ ] Updated compatibility rules
- [ ] C23 labels / statements
- [ ] Label immediately before a declaration
- [ ] Label immediately before }
- [ ] [[maybe_unused]] label
- [ ] C23 arrays / qualifiers
- [ ] Array type and element type have consistent const qualification
- [ ] Array type and element type have consistent volatile qualification
- [ ] Pointer-to-array qualifier compatibility changes
- [ ] Updated \_Generic behavior resulting from array qualification rules
- [ ] C23 VLA / variably-modified types
- [ ] Variably-modified types are mandatory
- [ ] Allocated-storage VLA types are supported
- [ ] Automatic VLA objects may remain optional
- [ ] **STDC_NO_VLA** now indicates absence of automatic VLA objects rather than all VM types
- [ ] WG14 explicitly made VM types mandatory while leaving automatic-storage VLAs conditional.
- [ ] C23 enumerations
- [ ] Fixed underlying enum type
- [ ] enum E : unsigned char
- [ ] Forward declaration where permitted with fixed type
- [ ] Enumerator values larger than int
- [ ] Improved selection of enumerator types
- [ ] Improved ordinary enumeration semantics
- [ ] C23 tagged types
- [ ] Revised struct compatibility rules
- [ ] Revised union compatibility rules
- [ ] Revised enum compatibility rules
- [ ] Compatible same-tag redeclarations in permitted cases
- [ ] C23 identifiers / Unicode
- [ ] Identifier rules based on Unicode Standard Annex #31
- [ ] Updated permitted Unicode identifier characters
- [ ] Correct normalization/identifier handling required by the standard
- [ ] C23 preprocessor
- [ ] #elifdef
- [ ] #elifndef
- [ ] #warning
- [ ] #embed
- [ ] \_\_has_include
- [ ] \_\_has_c_attribute
- [ ] **VA_OPT**
- [ ] Empty variadic macro argument handling
- [ ] Updated variadic macro replacement rules
- [ ] #embed
- [ ] Basic #embed "file"
- [ ] Binary resource expansion
- [ ] limit(...)
- [ ] prefix(...)
- [ ] suffix(...)
- [ ] if_empty(...)
- [ ] implementation-defined/vendor embed parameters handled correctly
- [ ] \_\_has_embed support where required by C23's preprocessing facilities
- [ ] C23 floating-point pragmas
- [ ] #pragma STDC FENV_ROUND
- [ ] #pragma STDC FENV_DEC_ROUND
- [ ] Updated IEC 60559 feature detection
- [ ] **STDC_IEC_60559_BFP**
- [ ] **STDC_IEC_60559_DFP**
- [ ] **STDC_IEC_60559_COMPLEX**
- [ ] C23 version
- [ ] **STDC_VERSION** == 202311L

## C23 library

## Headers

- [ ] <stdbit.h>
- [ ] <stdckdint.h>
- [ ] Checked arithmetic
- [ ] ckd_add
- [ ] ckd_sub
- [ ] ckd_mul
- [ ] Generic integer operand support
- [ ] Overflow reporting semantics
- [ ] Bit utilities
- [ ] Implement all appropriate signed/unsigned variants/macros in <stdbit.h>:
- [ ] leading-zero count
- [ ] leading-one count
- [ ] trailing-zero count
- [ ] trailing-one count
- [ ] first leading zero
- [ ] first leading one
- [ ] first trailing zero
- [ ] first trailing one
- [ ] population count / count ones
- [ ] zero-bit count
- [ ] single-bit test
- [ ] bit width
- [ ] bit floor
- [ ] bit ceil
- [ ] byte-order macros
- [ ] UTF-8 library support
- [ ] char8_t
- [ ] mbrtoc8
- [ ] c8rtomb
- [ ] atomic_char8_t
- [ ] ATOMIC_CHAR8_T_LOCK_FREE
- [ ] Memory management
- [ ] free_sized
- [ ] free_aligned_sized
- [ ] memalignment
- [ ] Updated allocation/deallocation synchronization semantics
- [ ] C23 realloc(ptr, 0) behavior
- [ ] free_sized, free_aligned_sized, and memalignment are C23 additions.
- [ ] Memory/string functions
- [ ] memset_explicit
- [ ] memccpy
- [ ] strdup
- [ ] strndup
- [ ] qualifier-preserving memchr
- [ ] qualifier-preserving strchr
- [ ] qualifier-preserving strpbrk
- [ ] qualifier-preserving strrchr
- [ ] qualifier-preserving strstr
- [ ] corresponding qualifier-preserving wide-character operations
- [ ] Program support
- [ ] unreachable()
- [ ] C23 provides unreachable through <stddef.h>.

#### Time

- [ ] gmtime_r
- [ ] localtime_r
- [ ] timespec_getres
- [ ] C23 strftime extensions
- [ ] C23 wcsftime extensions
- [ ] updated time-related macros
- [ ] asctime deprecated
- [ ] ctime deprecated
- [ ] Formatted I/O
- [ ] Binary %b conversion
- [ ] wN length modifiers
- [ ] wfN length modifiers
- [ ] H decimal-float length modifier
- [ ] D decimal-float length modifier
- [ ] DD decimal-float length modifier
- [ ] corresponding scanf-family support
- [ ] corresponding printf-family support
- [ ] Floating point
- [ ] New IEC 60559 binary functions where supported
- [ ] Decimal math functions where DFP is supported
- [ ] decimal dN function variants
- [ ] quantizedN
- [ ] samequantumdN
- [ ] quantumdN
- [ ] llquantexpdN
- [ ] decimal encode/decode functions
- [ ] floating-to-string formatting functions
- [ ] additional <float.h> macros
- [ ] revised IEC 60559 feature macros
- [ ] Integer limits
- [ ] Integer width macros
- [ ] CHAR_WIDTH
- [ ] SCHAR_WIDTH
- [ ] UCHAR_WIDTH
- [ ] SHRT_WIDTH
- [ ] USHRT_WIDTH
- [ ] INT_WIDTH
- [ ] UINT_WIDTH
- [ ] LONG_WIDTH
- [ ] ULONG_WIDTH
- [ ] LLONG_WIDTH
- [ ] ULLONG_WIDTH
- [ ] width macros for standard typedefs where specified
- [ ] exact-width integer rules updated
- [ ] [u]intN_t consistency with [u]int_leastN_t
- [ ] Library version macros
- [ ] **STDC_VERSION_FENV_H**
- [ ] **STDC_VERSION_MATH_H**
- [ ] **STDC_VERSION_STDINT_H**
- [ ] **STDC_VERSION_STDLIB_H**
- [ ] **STDC_VERSION_TGMATH_H**
- [ ] **STDC_VERSION_TIME_H**
- [ ] **STDC_VERSION_STDCKDINT_H**
- [ ] **STDC_VERSION_STDBIT_H**

# GNU extensions

## GNU89

- [ ] Statement expressions
- [ ] Locally declared labels with `__label__`
- [ ] Label addresses with `&&label`
- [ ] Computed goto
- [ ] Nested functions
- [ ] Nested-function trampolines / static chains
- [ ] Omitted middle operand of `?:`
- [ ] Case ranges
- [ ] `typeof`
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] `__alignof__`
- [ ] `__int128`
- [ ] `unsigned __int128`
- [ ] `_Float16` where supported
- [ ] `_Float32` where supported
- [ ] `_Float64` where supported
- [ ] `_Float128` where supported
- [ ] `_Float32x` where supported
- [ ] `_Float64x` where supported
- [ ] `_Float128x` where supported
- [ ] `_Decimal32` where supported
- [ ] `_Decimal64` where supported
- [ ] `_Decimal128` where supported
- [ ] GNU fixed-point types
- [ ] GNU saturating fixed-point types
- [ ] `__real__`
- [ ] `__imag__`
- [ ] Variable-length arrays in GNU90
- [ ] VLA members in local structures
- [ ] VLA members in local unions
- [ ] Parameter forward declarations
- [ ] Zero-length arrays
- [ ] Flexible array members in unions
- [ ] Structures containing only a flexible array member
- [ ] Nested flexible-array extensions
- [ ] Static initialization of flexible arrays
- [ ] Empty structures
- [ ] Union casts
- [ ] Designated initializers in GNU90
- [ ] Range designators
- [ ] Historical `[index] value` initializer syntax
- [ ] Historical `field: value` initializer syntax
- [ ] Incomplete enum forward declarations
- [ ] Fixed enum underlying types
- [ ] Enum forward declarations with fixed underlying type
- [ ] Basic `asm`
- [ ] `__asm__`
- [ ] Extended asm
- [ ] Asm input operands
- [ ] Asm output operands
- [ ] Asm constraints
- [ ] Matching asm constraints
- [ ] Early-clobber asm operands
- [ ] Read/write asm operands
- [ ] Asm clobber lists
- [ ] `"memory"` asm clobber
- [ ] `"cc"` asm clobber
- [ ] Symbolic asm operand names
- [ ] Asm operand modifiers
- [ ] `asm volatile`
- [ ] `asm inline`
- [ ] `asm goto`
- [ ] Top-level asm
- [ ] Target-specific asm constraints
- [ ] `__thread`
- [ ] `extern __thread`
- [ ] `static __thread`
- [ ] GNU TLS models
- [ ] Named variadic macro arguments
- [ ] Omitted variadic macro arguments
- [ ] `, ##__VA_ARGS__` comma elision
- [ ] `#include_next`
- [ ] GNU line markers
- [ ] `#pragma GCC diagnostic`
- [ ] `#pragma GCC diagnostic push`
- [ ] `#pragma GCC diagnostic pop`
- [ ] `#pragma GCC diagnostic warning`
- [ ] `#pragma GCC diagnostic error`
- [ ] `#pragma GCC poison`
- [ ] `#pragma GCC system_header`
- [ ] `#pragma GCC dependency`
- [ ] GNU macro push/pop pragmas
- [ ] GNU visibility pragmas
- [ ] GNU weak pragmas
- [ ] GNU structure-layout pragmas
- [ ] GNU function-specific optimization pragmas
- [ ] GNU target-specific pragmas
- [ ] `__GNUC__`
- [ ] `__GNUC_MINOR__`
- [ ] `__GNUC_PATCHLEVEL__`
- [ ] `__VERSION__`
- [ ] `__BASE_FILE__`
- [ ] `__INCLUDE_LEVEL__`
- [ ] `__COUNTER__`
- [ ] `__TIMESTAMP__`
- [ ] Target-specific predefined macros
- [ ] ABI-specific predefined macros
- [ ] `$` in identifiers where supported
- [ ] GNU extended escape handling
- [ ] `__FUNCTION__`
- [ ] `__PRETTY_FUNCTION__`
- [ ] `__inline__`
- [ ] `__extension__`
- [ ] `__restrict__`
- [ ] Arithmetic on `void *`
- [ ] `sizeof(void)` GNU semantics
- [ ] Arithmetic on function pointers
- [ ] `sizeof(function-type)` GNU semantics
- [ ] GNU prototype / old-style definition compatibility rules
- [ ] GNU array-pointer qualifier extensions
- [ ] GNU qualified function-type extensions

## GNU99

- [ ] All GNU90 extensions
- [ ] Statement expressions
- [ ] Local labels
- [ ] Computed goto
- [ ] Nested functions
- [ ] Omitted-middle `?:`
- [ ] Case ranges
- [ ] `typeof`
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] `__alignof__`
- [ ] `__int128`
- [ ] Additional GNU floating types
- [ ] Decimal floating types where supported
- [ ] GNU fixed-point types
- [ ] `__real__`
- [ ] `__imag__`
- [ ] VLA structure / union members
- [ ] Parameter forward declarations
- [ ] Zero-length arrays
- [ ] Extended flexible-array semantics
- [ ] Empty structures
- [ ] Union casts
- [ ] Range designators
- [ ] GNU enum extensions
- [ ] GNU inline assembly
- [ ] `__thread`
- [ ] Named variadic macro arguments
- [ ] `, ##__VA_ARGS__`
- [ ] GNU preprocessor extensions
- [ ] GNU pragmas
- [ ] GNU predefined macros
- [ ] GNU alternate keywords
- [ ] GNU pointer arithmetic extensions
- [ ] GNU attributes
- [ ] GCC builtins
- [ ] GNU vector extensions

## GNU11

- [ ] All GNU99 extensions
- [ ] `__thread` in addition to standard `_Thread_local`
- [ ] GNU atomic builtins in addition to C11 atomics
- [ ] `__atomic_*` builtins
- [ ] Legacy `__sync_*` atomic builtins
- [ ] GNU attributes not represented by C11 syntax
- [ ] GNU vector extensions
- [ ] GNU inline assembly
- [ ] GNU statement expressions
- [ ] GNU nested functions
- [ ] GNU computed goto
- [ ] GNU case ranges
- [ ] GNU `typeof`
- [ ] GNU `__auto_type`
- [ ] GNU enum extensions
- [ ] GNU flexible-array extensions
- [ ] GNU zero-length arrays
- [ ] GNU preprocessor extensions

## GNU17 / GNU18

- [ ] All GNU11 extensions
- [ ] Statement expressions
- [ ] Local labels
- [ ] Computed goto
- [ ] Nested functions
- [ ] Omitted-middle `?:`
- [ ] Case ranges
- [ ] `typeof`
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] GNU numeric types
- [ ] GNU VLA extensions
- [ ] Zero-length arrays
- [ ] Extended flexible-array semantics
- [ ] Empty structures
- [ ] Union casts
- [ ] Range designators
- [ ] GNU enum extensions
- [ ] GNU inline assembly
- [ ] GNU TLS
- [ ] GNU variadic macro extensions
- [ ] GNU preprocessor extensions
- [ ] GNU attributes
- [ ] GCC builtins
- [ ] GNU vector extensions

## GNU23

- [ ] Statement expressions
- [ ] Locally declared labels
- [ ] Label addresses
- [ ] Computed goto
- [ ] Nested functions
- [ ] Nested-function trampolines / static chains
- [ ] Omitted middle operand of `?:`
- [ ] Case ranges
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] `__alignof__`
- [ ] `__int128`
- [ ] GNU additional floating types
- [ ] GNU fixed-point types
- [ ] `__real__`
- [ ] `__imag__`
- [ ] VLA members in local aggregates
- [ ] Parameter forward declarations
- [ ] Zero-length arrays
- [ ] Extended flexible-array semantics
- [ ] Empty structures
- [ ] Union casts
- [ ] Range designators
- [ ] Incomplete enum forward declarations without fixed type
- [ ] GNU inline assembly
- [ ] `__thread`
- [ ] Named variadic macro arguments
- [ ] `, ##__VA_ARGS__`
- [ ] GNU preprocessor extensions
- [ ] GNU pragmas
- [ ] GNU predefined macros
- [ ] GNU alternate keywords
- [ ] GNU pointer arithmetic extensions
- [ ] GNU attributes
- [ ] GCC builtins
- [ ] GNU vector extensions

# GNU Attributes

## General Syntax

- [ ] `__attribute__((...))`
- [ ] `[[gnu::...]]`
- [ ] Multiple attributes
- [ ] Attributes on declarations
- [ ] Attributes on types
- [ ] Attributes on functions
- [ ] Attributes on variables
- [ ] Attributes on labels
- [ ] Attributes on statements

## Type and Layout Attributes

- [ ] `aligned`
- [ ] `packed`
- [ ] `mode`
- [ ] `vector_size`
- [ ] `transparent_union`
- [ ] `warn_if_not_aligned`
- [ ] `strict_flex_array`
- [ ] `counted_by`
- [ ] `hardbool`

## Function and Optimization Attributes

- [ ] `always_inline`
- [ ] `noinline`
- [ ] `gnu_inline`
- [ ] `flatten`
- [ ] `hot`
- [ ] `cold`
- [ ] `pure`
- [ ] `const`
- [ ] `malloc`
- [ ] `alloc_size`
- [ ] `alloc_align`
- [ ] `assume_aligned`
- [ ] `returns_nonnull`
- [ ] `nonnull`
- [ ] `noreturn`
- [ ] `nothrow`
- [ ] `warn_unused_result`
- [ ] `used`
- [ ] `unused`
- [ ] `retain`
- [ ] `leaf`
- [ ] `noclone`
- [ ] `no_icf`
- [ ] `no_instrument_function`
- [ ] `no_profile_instrument_function`
- [ ] `no_sanitize`
- [ ] `optimize`
- [ ] `target`
- [ ] `target_clones`
- [ ] `target_version`

## Linking and Object Attributes

- [ ] `alias`
- [ ] `weak`
- [ ] `weakref`
- [ ] `section`
- [ ] `visibility`
- [ ] `externally_visible`
- [ ] `ifunc`
- [ ] `noplt`
- [ ] `common`
- [ ] `nocommon`
- [ ] `constructor`
- [ ] `destructor`
- [ ] Constructor priority
- [ ] Destructor priority
- [ ] `copy`

## Diagnostic and Contract Attributes

- [ ] `deprecated`
- [ ] `warning`
- [ ] `error`
- [ ] `format`
- [ ] `format_arg`
- [ ] `access`
- [ ] `nonnull`
- [ ] `null_terminated_string_arg`
- [ ] `sentinel`

## Variable and Statement Attributes

- [ ] `cleanup`
- [ ] `fallthrough`
- [ ] `assume`
- [ ] `musttail`

# GCC Builtins

## Compile-Time Introspection

- [ ] `__builtin_constant_p`
- [ ] `__builtin_types_compatible_p`
- [ ] `__builtin_choose_expr`
- [ ] `__builtin_offsetof`
- [ ] `__builtin_object_size`
- [ ] `__builtin_dynamic_object_size`
- [ ] `__builtin_has_attribute`

## Control Flow and Optimization

- [ ] `__builtin_expect`
- [ ] `__builtin_expect_with_probability`
- [ ] `__builtin_unreachable`
- [ ] `__builtin_trap`
- [ ] `__builtin_assume_aligned`

## Bit and Arithmetic Builtins

- [ ] `__builtin_clz`
- [ ] `__builtin_clzl`
- [ ] `__builtin_clzll`
- [ ] `__builtin_ctz`
- [ ] `__builtin_ctzl`
- [ ] `__builtin_ctzll`
- [ ] `__builtin_popcount`
- [ ] `__builtin_popcountl`
- [ ] `__builtin_popcountll`
- [ ] `__builtin_parity`
- [ ] `__builtin_parityl`
- [ ] `__builtin_parityll`
- [ ] `__builtin_bswap16`
- [ ] `__builtin_bswap32`
- [ ] `__builtin_bswap64`
- [ ] `__builtin_bswap128` where supported
- [ ] Signed overflow-checking builtins
- [ ] Unsigned overflow-checking builtins
- [ ] Generic overflow-checking builtins
- [ ] Carry builtins
- [ ] Borrow builtins

## Stack and Frame Builtins

- [ ] `__builtin_alloca`
- [ ] `__builtin_alloca_with_align`
- [ ] `__builtin_frame_address`
- [ ] `__builtin_return_address`

## GNU Atomic Builtins

- [ ] `__atomic_load`
- [ ] `__atomic_load_n`
- [ ] `__atomic_store`
- [ ] `__atomic_store_n`
- [ ] `__atomic_exchange`
- [ ] `__atomic_exchange_n`
- [ ] `__atomic_compare_exchange`
- [ ] `__atomic_compare_exchange_n`
- [ ] `__atomic_add_fetch`
- [ ] `__atomic_sub_fetch`
- [ ] `__atomic_and_fetch`
- [ ] `__atomic_xor_fetch`
- [ ] `__atomic_or_fetch`
- [ ] `__atomic_nand_fetch`
- [ ] `__atomic_fetch_add`
- [ ] `__atomic_fetch_sub`
- [ ] `__atomic_fetch_and`
- [ ] `__atomic_fetch_xor`
- [ ] `__atomic_fetch_or`
- [ ] `__atomic_fetch_nand`
- [ ] `__atomic_test_and_set`
- [ ] `__atomic_clear`
- [ ] `__atomic_thread_fence`
- [ ] `__atomic_signal_fence`
- [ ] `__atomic_always_lock_free`
- [ ] `__atomic_is_lock_free`
- [ ] `__ATOMIC_RELAXED`
- [ ] `__ATOMIC_CONSUME`
- [ ] `__ATOMIC_ACQUIRE`
- [ ] `__ATOMIC_RELEASE`
- [ ] `__ATOMIC_ACQ_REL`
- [ ] `__ATOMIC_SEQ_CST`

## Legacy GNU Atomic Builtins

- [ ] `__sync_fetch_and_add`
- [ ] `__sync_fetch_and_sub`
- [ ] `__sync_fetch_and_or`
- [ ] `__sync_fetch_and_and`
- [ ] `__sync_fetch_and_xor`
- [ ] `__sync_fetch_and_nand`
- [ ] `__sync_add_and_fetch`
- [ ] `__sync_sub_and_fetch`
- [ ] `__sync_or_and_fetch`
- [ ] `__sync_and_and_fetch`
- [ ] `__sync_xor_and_fetch`
- [ ] `__sync_nand_and_fetch`
- [ ] `__sync_bool_compare_and_swap`
- [ ] `__sync_val_compare_and_swap`
- [ ] `__sync_lock_test_and_set`
- [ ] `__sync_lock_release`
- [ ] `__sync_synchronize`

## Variadic ABI Builtins

- [ ] `__builtin_va_list`
- [ ] `__builtin_va_start`
- [ ] `__builtin_va_arg`
- [ ] `__builtin_va_end`
- [ ] `__builtin_va_copy`

## Library Recognition Builtins

- [ ] Memory-operation builtins
- [ ] String-operation builtins
- [ ] Formatted-I/O builtins
- [ ] Math-function builtins
- [ ] Floating-point classification builtins
- [ ] Target-specific intrinsic builtins

# GNU Vector Extensions

- [ ] `vector_size` attribute
- [ ] GNU vector types
- [ ] Vector initialization
- [ ] Vector arithmetic
- [ ] Vector bitwise operations
- [ ] Vector comparisons
- [ ] Vector conversions
- [ ] Vector subscripting
- [ ] Scalar-to-vector operations
- [ ] Vector ABI passing
- [ ] Vector ABI return values
- [ ] Target-specific vector builtins

# GNU libc Feature-Test Modes

## ISO C90 Library

- [ ] ISO C90 library interfaces

## ISO C95 Library

- [ ] ISO C95 library interfaces

## `_ISOC99_SOURCE`

- [ ] ISO C99 library interfaces

## `_ISOC11_SOURCE`

- [ ] ISO C11 library interfaces

## C17 Library

- [ ] ISO C17 library interfaces
- [ ] C11 defect-report corrections
- [ ] No dedicated `_ISOC17_SOURCE` selector

## `_ISOC23_SOURCE`

- [ ] ISO C23 library interfaces

## `_ISOC2X_SOURCE`

- [ ] Legacy selector for developing C23 library interfaces

# `_GNU_SOURCE` Library Extensions

## Dynamic Memory and Allocation

- [ ] GNU allocation extensions
- [ ] `canonicalize_file_name`
- [ ] GNU malloc inspection interfaces
- [ ] GNU malloc debugging interfaces

## String and Memory Functions

- [ ] `mempcpy`
- [ ] `memrchr`
- [ ] `rawmemchr`
- [ ] `strchrnul`
- [ ] `strcasestr`
- [ ] `strverscmp`
- [ ] `strfry`
- [ ] `memfrob`
- [ ] `strdupa`
- [ ] `strndupa`

## Formatted Output and Streams

- [ ] `asprintf`
- [ ] `vasprintf`
- [ ] `fopencookie`
- [ ] `cookie_io_functions_t`
- [ ] `open_memstream`

## Line Input

- [ ] `getline`
- [ ] `getdelim`

## Program and Process Interfaces

- [ ] `on_exit`
- [ ] `secure_getenv`
- [ ] `get_current_dir_name`
- [ ] `execvpe`
- [ ] `program_invocation_name`
- [ ] `program_invocation_short_name`

## Argument Parsing

- [ ] `getopt_long`
- [ ] `getopt_long_only`
- [ ] `argp_parse`
- [ ] GNU `argp_*` interfaces

## Error Reporting

- [ ] `error`
- [ ] `error_at_line`

## Sorting

- [ ] GNU `qsort_r`

## Auxiliary Vector

- [ ] `getauxval`

## Backtrace Support

- [ ] `backtrace`
- [ ] `backtrace_symbols`
- [ ] `backtrace_symbols_fd`

## Obstacks

- [ ] `obstack_init`
- [ ] `obstack_alloc`
- [ ] `obstack_copy`
- [ ] `obstack_copy0`
- [ ] `obstack_blank`
- [ ] `obstack_grow`
- [ ] `obstack_grow0`
- [ ] `obstack_finish`
- [ ] `obstack_free`
- [ ] GNU `obstack_*` interfaces

## Dynamic Linking

- [ ] GNU dynamic-linker extensions
- [ ] GNU `dl*` extensions
- [ ] GNU symbol lookup extensions
- [ ] GNU loader namespace extensions

# `_DEFAULT_SOURCE`

- [ ] Default glibc extension namespace
- [ ] BSD-derived interfaces
- [ ] SVID-derived interfaces
- [ ] Historical miscellaneous interfaces

# POSIX Feature-Test Levels

- [ ] `_POSIX_SOURCE`
- [ ] `_POSIX_C_SOURCE=1`
- [ ] `_POSIX_C_SOURCE=2`
- [ ] `_POSIX_C_SOURCE=199309L`
- [ ] `_POSIX_C_SOURCE=199506L`
- [ ] `_POSIX_C_SOURCE=200112L`
- [ ] `_POSIX_C_SOURCE=200809L`
- [ ] `_POSIX_C_SOURCE=202405L`

# X/Open Feature-Test Levels

- [ ] `_XOPEN_SOURCE`
- [ ] `_XOPEN_SOURCE=500`
- [ ] `_XOPEN_SOURCE=600`
- [ ] `_XOPEN_SOURCE=700`
- [ ] `_XOPEN_SOURCE=800`

# Large-File Support

- [ ] `_LARGEFILE_SOURCE`
- [ ] `_LARGEFILE64_SOURCE`
- [ ] `_FILE_OFFSET_BITS=64`
- [ ] 64-bit `off_t` API mappings
- [ ] Explicit `*64` large-file interfaces

# 64-Bit Time Support

- [ ] `_TIME_BITS=64`
- [ ] `_TIME_BITS=64` interaction with `_FILE_OFFSET_BITS=64`
- [ ] 64-bit `time_t` ABI mappings

# glibc Fortification

- [ ] `_FORTIFY_SOURCE=1`
- [ ] `_FORTIFY_SOURCE=2`
- [ ] `_FORTIFY_SOURCE=3`
- [ ] `__builtin_object_size` integration
- [ ] `__builtin_dynamic_object_size` integration
- [ ] Fortified `__*_chk` interfaces
- [ ] Compile-time bounds diagnostics
- [ ] Runtime bounds checking
