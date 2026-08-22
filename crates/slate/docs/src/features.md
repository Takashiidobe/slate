# Features

Slate has full C23 support. If it doesn't support C23, it's a bug to be
fixed.

## C89

- [ ] Trigraphs
- [x] Escape sequences
- [x] Integer constants: decimal, octal, hexadecimal
- [x] Floating constants
- [x] Character constants
- [x] String literals
- [x] Adjacent string-literal concatenation
- [x] Comments `/* ... */`
- [x] Preprocessor object-like macros
- [x] Preprocessor function-like macros
- [x] Macro argument substitution
- [x] Stringification operator #
- [x] Token-pasting operator ##
- [x] #include
- [x] #define
- [x] #undef
- [x] #if
- [x] #ifdef
- [x] #ifndef
- [x] #elif
- [x] #else
- [x] #endif
- [x] defined
- [ ] #line
- [ ] #error
- [x] #pragma
- [ ] Predefined macros: `__LINE__`
- [ ] `__FILE__`
- [ ] `__DATE__`
- [ ] `__TIME__`
- [x] `__STDC__`
- [x] Types
- [x] char
- [x] signed char
- [x] unsigned char
- [x] short
- [x] unsigned short
- [x] int
- [x] unsigned int
- [x] long
- [x] unsigned long
- [x] float
- [x] double
- [x] long double
- [x] void
- [x] pointers
- [x] arrays
- [x] multidimensional arrays
- [x] functions
- [x] structures
- [x] unions
- [x] enumerations
- [x] bit-fields
- [x] typedef
- [ ] incomplete types
- [ ] compatible/composite types
- [x] Type qualifiers
- [x] const
- [x] volatile
- [x] qualified pointers
- [ ] qualifier propagation rules
- [x] Storage / linkage
- [x] auto
- [x] register
- [x] static
- [x] extern
- [x] automatic storage duration
- [x] static storage duration
- [x] external linkage
- [x] internal linkage
- [x] no linkage
- [x] tentative definitions
- [x] Declarations / functions
- [x] Function prototypes
- [ ] Prototype parameter type checking
- [ ] Old-style/K&R function declarations
- [ ] Old-style/K&R function definitions
- [x] Variadic functions via ...
- [x] Array declarators
- [x] Function-pointer declarators
- [ ] Abstract declarators
- [x] Initializers
- [x] Aggregate initialization
- [x] Expressions
- [x] Integer promotions
- [x] Usual arithmetic conversions
- [x] Lvalue semantics
- [x] Array-to-pointer conversion
- [x] Function-to-pointer conversion
- [x] Pointer arithmetic
- [x] Pointer comparison
- [x] Structure/union member access
- [x] Cast expressions
- [x] sizeof
- [x] Unary operators
- [x] Arithmetic operators
- [x] Relational/equality operators
- [x] Logical operators
- [x] Bitwise operators
- [x] Shift operators
- [x] Assignment operators
- [x] Conditional ?:
- [x] Comma operator
- [x] Function calls
- [x] Increment/decrement
- [x] Statements
- [x] Compound statements
- [x] if
- [x] else
- [x] switch
- [x] case
- [x] default
- [x] while
- [x] do
- [x] for
- [x] goto
- [x] labels
- [x] continue
- [x] break
- [x] return

### C89 library

- [x] <assert.h>
- [x] <ctype.h>
- [x] <errno.h>
- [x] <float.h>
- [x] <limits.h>
- [x] <locale.h>
- [x] <math.h>
- [x] <setjmp.h>
- [x] <signal.h>
- [x] <stdarg.h>
- [x] <stddef.h>
- [x] <stdio.h>
- [x] <stdlib.h>
- [x] <string.h>
- [x] <time.h>

## C95

- [x] Digraph <: -> [
- [x] Digraph :> -> ]
- [x] Digraph <% -> {
- [x] Digraph %> -> }
- [x] Digraph %: -> #
- [x] Digraph %:%: -> ##
- [x] `__STDC_VERSION__`
- [x] `__STDC_VERSION__` == 199409L

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

- [x] // line comments
- [x] Universal character names \uXXXX
- [ ] Universal character names \UXXXXXXXX
- [x] Universal character names in identifiers
- [x] Increased minimum translation limits
- [x] Integer / arithmetic types
- [x] `_Bool`
- [x] long long
- [x] unsigned long long
- [ ] Extended signed integer types
- [ ] Extended unsigned integer types
- [ ] Revised integer conversion rules
- [x] Signed integer division truncates toward zero
- [x] Corresponding remainder semantics
- [x] Complex arithmetic
- [x] `_Complex`
- [x] float `_Complex`
- [x] double `_Complex`
- [x] long double `_Complex`
- [ ] `_Imaginary` where supported by the standard's model
- [x] Complex literals through library macros/functions
- [x] Complex arithmetic semantics
- [x] Floating point
- [x] Hexadecimal floating constants, e.g. 0x1.8p+2
- [ ] FLT_EVAL_METHOD
- [x] Floating-point environment semantics
- [x] Floating-point contraction rules
- [ ] IEC 60559/IEEE-754 feature macros where applicable
- [x] Qualifiers / declarations
- [x] restrict
- [x] Idempotent qualifiers, e.g. repeated const
- [x] inline
- [x] Mixed declarations and statements
- [x] Declaration in for initializer
- [x] for (int i = 0; ...)
- [x] Arrays
- [x] Variable-length arrays
- [x] Variably-modified types
- [ ] VLA runtime sizeof
- [ ] Function prototype [*] VLA notation
- [x] static inside array function parameters
- [x] const/volatile/restrict inside array parameter brackets
- [x] Flexible array members
- [x] Initialization
- [x] Nonconstant initializers for automatic aggregate objects
- [x] Array designated initializers [index] = value
- [x] Structure designated initializers .member = value
- [ ] Nested designated initializers
- [x] Compound literals (T){...}
- [x] Enumerations
- [x] Trailing comma in enumerator list
- [x] Functions
- [x] `__func__`
- [x] Implicit return 0 from main
- [ ] C99 inline linkage semantics
- [x] Removed C89 behavior
- [x] Reject implicit int
- [x] Reject implicit function declarations

### Preprocessor

- [x] Variadic macros ...
- [x] `__VA_ARGS__`
- [ ] Empty arguments to function-like macros
- [ ] `_Pragma`
- [x] #pragma STDC FENV_ACCESS
- [x] #pragma STDC FP_CONTRACT
- [x] #pragma STDC CX_LIMITED_RANGE

### C99 version / feature macros

- [ ] `__STDC_VERSION__`== 199901L
- [ ] `__STDC_HOSTED__`
- [ ] `__STDC_IEC_559__`
- [ ] `__STDC_IEC_559_COMPLEX__`
- [ ] `__STDC_ISO_10646__`

### C99 library

#### Headers:

- [x] <stdbool.h>
- [x] <stdint.h>
- [x] <inttypes.h>
- [x] <complex.h>
- [x] <fenv.h>
- [x] <tgmath.h>
- [x] Integer library
- [x] int8_t, int16_t, etc. where exact-width types exist
- [x] uint8_t, uint16_t, etc.
- [x] int_leastN_t / uint_leastN_t
- [x] int_fastN_t / uint_fastN_t
- [x] intptr_t / uintptr_t where provided
- [x] intmax_t
- [x] uintmax_t
- [x] INTN*\* / UINTN*\* limits
- [x] PRI\* format macros
- [x] SCN\* format macros
- [x] General library additions
- [x] llabs
- [x] lldiv
- [x] strtoll
- [x] strtoull
- [x] strtoimax
- [x] strtoumax
- [x] imaxabs
- [x] imaxdiv
- [x] `_Exit`
- [x] va_copy
- [x] snprintf
- [x] vsnprintf
- [x] isblank
- [x] strtof
- [x] strtold
- [x] Wide-character integer conversion counterparts

#### Formatted I/O

- [x] %a / %A hexadecimal floating format
- [x] hh length modifier
- [x] ll length modifier
- [x] j length modifier
- [x] z length modifier
- [x] t length modifier
- [x] C99 scanf/printf family extensions

#### Math

- [x] float variants of math functions
- [x] long double variants
- [x] New C99 mathematical functions
- [x] fma
- [x] remainder / remquo
- [x] nextafter / nexttoward
- [x] classification macros such as isfinite
- [x] comparison macros such as isgreater
- [x] floating-point environment functions
- [x] complex math functions
- [x] <tgmath.h> type-generic dispatch

## C11

### Compiler

- [ ] Memory model / concurrency
- [ ] C11 memory model
- [ ] Sequenced-before relation
- [ ] Data-race rules
- [ ] Happens-before rules
- [x] Atomic objects
- [x] \_Atomic(type)
- [x] \_Atomic qualifier/specifier
- [x] Atomic initialization semantics
- [x] Atomic load/store semantics
- [x] Atomic read-modify-write semantics
- [x] Memory-order model
- [x] Thread-local storage
- [x] \_Thread_local
- [x] TLS with static
- [ ] TLS with extern
- [x] Thread storage duration
- [x] Alignment
- [x] \_Alignof
- [x] \_Alignas
- [x] Extended/over-alignment
- [ ] Fundamental alignment
- [ ] Valid alignment constraints
- [ ] Generic programming
- [ ] \_Generic
- [ ] Generic association lists
- [ ] default generic association
- [ ] Type-based selection semantics
- [ ] Compile-time assertions
- [ ] \_Static_assert(expr, "message")
- [x] Function properties
- [x] \_Noreturn
- [x] Structures / unions
- [x] Anonymous structure members
- [x] Anonymous union members
- [x] Unicode literals
- [x] u'…'
- [x] U'…'
- [x] u8"…"
- [ ] u"…"
- [ ] U"…"
- [ ] Expression / object semantic changes
- [ ] Finer-grained evaluation/sequencing rules
- [ ] Temporary-object lifetime rules
- [ ] Updated effective-type / memory-model interactions
- [ ] Analyzability
- [ ] Analyzability specification
- [ ] `__STDC_ANALYZABLE__` when supported
- [ ] Conditional feature macros
- [ ] `__STDC_NO_ATOMICS__`- [ ] `__STDC_NO_THREADS__`- [ ] `__STDC_NO_VLA__`- [ ] `__STDC_NO_COMPLEX__`- [ ] Version
- [ ] `__STDC_VERSION__` == 201112L

### C11 library

- [ ] New headers
- [ ] <stdalign.h>
- [ ] <stdatomic.h>
- [x] <stdnoreturn.h>
- [x] <threads.h>
- [x] <uchar.h>
- [x] Atomics library
- [x] `atomic_*` typedefs
- [ ] atomic_init
- [x] atomic_store
- [x] atomic_load
- [x] atomic_exchange
- [x] atomic*compare_exchange*\*
- [x] atomic_fetch_add
- [x] atomic_fetch_sub
- [x] atomic_fetch_or
- [x] atomic_fetch_xor
- [x] atomic_fetch_and
- [x] atomic_flag
- [x] atomic fences
- [x] memory*order*\*
- [ ] lock-free query facilities
- [x] Threads library
- [x] thrd_t
- [x] thread creation
- [x] thread joining
- [x] thread detaching
- [x] thrd_current
- [x] thrd_equal
- [x] thrd_sleep
- [x] mutexes mtx\_\*
- [x] condition variables cnd\_\*
- [x] thread-specific storage tss\_\*
- [x] once_flag
- [x] call_once
- [x] Unicode
- [x] char16_t
- [x] char32_t
- [x] mbrtoc16
- [x] mbrtoc32
- [x] c16rtomb
- [x] c32rtomb
- [ ] Runtime / allocation
- [ ] aligned_alloc
- [ ] quick_exit
- [x] at_quick_exit
- [x] timespec
- [x] timespec_get
- [ ] Exclusive "x" mode for fopen/freopen
- [x] Thread-local errno
- [x] Complex / floating support
- [ ] CMPLX
- [ ] CMPLXF
- [x] CMPLXL
- [ ] FLT_DECIMAL_DIG
- [ ] DBL_DECIMAL_DIG
- [ ] LDBL_DECIMAL_DIG
- [ ] \*\_TRUE_MIN
- [ ] \*\_HAS_SUBNORM
- [ ] Annex K Support
- [ ] `__STDC_LIB_EXT1__`, if implemented
- [ ] Bounds-checking interfaces
- [ ] \_s functions
- [ ] runtime-constraint handlers removed
- [x] gets() removed

## C17

- [ ] `__STDC_VERSION__` == 201710L
- [ ] All C11 features
- [ ] All applicable C11 defect-report corrections
- [ ] Updated atomic semantics
- [ ] Updated effective-type/object semantics where corrected
- [ ] Updated library wording from C11 defect reports
- [ ] Updated threading wording from C11 defect reports
- [ ] Updated complex/floating-point wording from C11 defect reports
- [ ] Updated generic-selection wording from C11 defect reports

## C23

### C23 new types

- [x] `_BitInt(N)`
- [x] signed `_BitInt(N)`
- [x] unsigned `_BitInt(N)`
- [x] Distinct type for each supported width
- [x] BITINT_MAXWIDTH
- [x] Bit-precise integer promotions/conversions
- [x] Bit-precise integer arithmetic
- [x] Bit-precise integer constant suffix wb
- [ ] Bit-precise unsigned constant suffix uwb
- [ ] Conditional decimal floating point

### Decimal types

only when the implementation advertises decimal IEC 60559 support through `__STDC_IEC_60559_DFP__`.

(not supported for clang)?

- [ ] `_Decimal32`
- [ ] `_Decimal64`
- [ ] `_Decimal128`
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
- [ ] char8_t is a typedef of unsigned char in <uchar.h>, and changing char[N] to char8_t[N].

### C23 keywords

- [x] bool is a keyword
- [x] true is a keyword
- [x] false is a keyword
- [ ] alignas is a keyword
- [ ] alignof is a keyword
- [ ] static_assert is a keyword
- [ ] thread_local is a keyword
- [ ] constexpr is a keyword
- [ ] typeof
- [ ] typeof_unqual
- [x] nullptr
- [ ] The underscore-prefixed historical forms remain relevant for compatibility, although \_Noreturn is deprecated.
- [x] C23 nullptr
- [x] nullptr null pointer constant
- [x] nullptr_t
- [x] Correct conversions from nullptr
- [x] Correct comparisons involving nullptr
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
- [x] C23 initialization
- [x] Empty initializer {}
- [x] T x = {};
- [x] Zero-initialization semantics for empty initializer
- [ ] Empty initialization of arrays
- [x] Empty initialization of structures
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
- [ ] `__STDC_NO_VLA__` now indicates absence of automatic VLA objects rather than all VM types
- [ ] WG14 explicitly made VM types mandatory while leaving automatic-storage VLAs conditional.

## C23 enumerations

- [ ] Fixed underlying enum type
- [ ] enum E : unsigned char
- [ ] Forward declaration where permitted with fixed type
- [ ] Enumerator values larger than int
- [ ] Improved selection of enumerator types
- [ ] Improved ordinary enumeration semantics

## C23 tagged types

- [ ] Revised struct compatibility rules
- [ ] Revised union compatibility rules
- [ ] Revised enum compatibility rules
- [ ] Compatible same-tag redeclarations in permitted cases

## C23 identifiers / Unicode

- [ ] Identifier rules based on Unicode Standard Annex #31
- [ ] Updated permitted Unicode identifier characters
- [ ] Correct normalization/identifier handling required by the standard

## C23 preprocessor

- [x] #elifdef
- [x] #elifndef
- [x] #warning
- [x] #embed
- [ ] `__has_include`
- [ ] `__has_c_attribute`
- [ ] `__VA_OPT__`- [ ] Empty variadic macro argument handling
- [ ] Updated variadic macro replacement rules
- [ ] #embed
- [ ] Basic #embed "file"
- [ ] Binary resource expansion
- [ ] limit(...)
- [ ] prefix(...)
- [ ] suffix(...)
- [ ] if_empty(...)
- [ ] implementation-defined/vendor embed parameters handled correctly
- [ ] `__has_embed` support where required by C23's preprocessing facilities
- [ ] C23 floating-point pragmas
- [x] #pragma STDC FENV_ROUND
- [x] #pragma STDC FENV_DEC_ROUND
- [ ] Updated IEC 60559 feature detection
- [ ] `__STDC_IEC_60559_BFP__`
- [ ] `__STDC_IEC_60559_DFP__`
- [ ] `__STDC_IEC_60559_COMPLEX__`
- [ ] C23 version
- [ ] `__STDC_VERSION__` == 202311L

## C23 library

## Headers

- [x] <stdbit.h>
- [x] <stdckdint.h>
- [x] Checked arithmetic
- [x] ckd_add
- [x] ckd_sub
- [x] ckd_mul
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

- [x] gmtime_r
- [x] localtime_r
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
- [ ] `__STDC_VERSION_FENV_H__`
- [ ] `__STDC_VERSION_MATH_H__`
- [ ] `__STDC_VERSION_STDINT_H__`
- [ ] `__STDC_VERSION_STDLIB_H__`
- [ ] `__STDC_VERSION_TGMATH_H__`
- [ ] `__STDC_VERSION_TIME_H__`
- [ ] `__STDC_VERSION_STDCKDINT_H__`
- [ ] `__STDC_VERSION_STDBIT_H__`

# GNU extensions

## GNU89

- [ ] Statement expressions
- [ ] Locally declared labels with `__label__`
- [x] Label addresses with `&&label`
- [x] Computed goto
- [ ] Nested functions
- [ ] Nested-function trampolines / static chains
- [ ] Omitted middle operand of `?:`
- [x] Case ranges
- [ ] `typeof`
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] `__alignof__`
- [x] `__int128`
- [x] `unsigned __int128`
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
- [x] `__real__`
- [x] `__imag__`
- [x] Variable-length arrays in GNU90
- [ ] VLA members in local structures
- [ ] VLA members in local unions
- [ ] Parameter forward declarations
- [ ] Zero-length arrays
- [ ] Flexible array members in unions
- [ ] Structures containing only a flexible array member
- [ ] Nested flexible-array extensions
- [ ] Static initialization of flexible arrays
- [x] Empty structures
- [ ] Union casts
- [x] Designated initializers in GNU90
- [ ] Range designators
- [ ] Historical `[index] value` initializer syntax
- [ ] Historical `field: value` initializer syntax
- [ ] Incomplete enum forward declarations
- [ ] Fixed enum underlying types
- [ ] Enum forward declarations with fixed underlying type
- [x] Basic `asm`
- [x] `__asm__`
- [x] Extended asm
- [x] Asm input operands
- [x] Asm output operands
- [x] Asm constraints
- [x] Matching asm constraints
- [x] Early-clobber asm operands
- [x] Read/write asm operands
- [x] Asm clobber lists
- [ ] `"memory"` asm clobber
- [x] `"cc"` asm clobber
- [x] Symbolic asm operand names
- [x] Asm operand modifiers
- [x] `asm volatile`
- [ ] `asm inline`
- [x] `asm goto`
- [x] Top-level asm
- [x] Target-specific asm constraints
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
- [x] `#pragma GCC poison`
- [ ] `#pragma GCC system_header`
- [ ] `#pragma GCC dependency`
- [x] GNU macro push/pop pragmas
- [ ] GNU visibility pragmas
- [x] GNU weak pragmas
- [x] GNU structure-layout pragmas
- [ ] GNU function-specific optimization pragmas
- [ ] GNU target-specific pragmas
- [x] `__GNUC__`
- [ ] `__GNUC_MINOR__`
- [ ] `__GNUC_PATCHLEVEL__`
- [ ] `__VERSION__`
- [ ] `__BASE_FILE__`
- [ ] `__INCLUDE_LEVEL__`
- [x] `__COUNTER__`
- [x] `__TIMESTAMP__`
- [x] Target-specific predefined macros
- [x] ABI-specific predefined macros
- [ ] `$` in identifiers where supported
- [ ] GNU extended escape handling
- [x] `__FUNCTION__`
- [ ] `__PRETTY_FUNCTION__`
- [ ] `__inline__`
- [ ] `__extension__`
- [ ] `__restrict__`
- [ ] Arithmetic on `void *`
- [ ] `sizeof(void)` GNU semantics
- [x] Arithmetic on function pointers
- [ ] `sizeof(function-type)` GNU semantics
- [ ] GNU prototype / old-style definition compatibility rules
- [ ] GNU array-pointer qualifier extensions
- [ ] GNU qualified function-type extensions

## GNU99

- [ ] All GNU90 extensions
- [ ] Statement expressions
- [ ] Local labels
- [x] Computed goto
- [ ] Nested functions
- [ ] Omitted-middle `?:`
- [x] Case ranges
- [ ] `typeof`
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] `__alignof__`
- [x] `__int128`
- [ ] Additional GNU floating types
- [ ] Decimal floating types where supported
- [ ] GNU fixed-point types
- [x] `__real__`
- [x] `__imag__`
- [ ] VLA structure / union members
- [ ] Parameter forward declarations
- [ ] Zero-length arrays
- [ ] Extended flexible-array semantics
- [x] Empty structures
- [ ] Union casts
- [ ] Range designators
- [ ] GNU enum extensions
- [x] GNU inline assembly
- [ ] `__thread`
- [ ] Named variadic macro arguments
- [ ] `, ##__VA_ARGS__`
- [ ] GNU preprocessor extensions
- [x] GNU pragmas
- [x] GNU predefined macros
- [ ] GNU alternate keywords
- [x] GNU pointer arithmetic extensions
- [x] GNU attributes
- [x] GCC builtins
- [x] GNU vector extensions

## GNU11

- [ ] All GNU99 extensions
- [ ] `__thread` in addition to standard `_Thread_local`
- [x] GNU atomic builtins in addition to C11 atomics
- [x] `__atomic_*` builtins
- [ ] Legacy `__sync_*` atomic builtins
- [x] GNU attributes not represented by C11 syntax
- [x] GNU vector extensions
- [x] GNU inline assembly
- [ ] GNU statement expressions
- [ ] GNU nested functions
- [x] GNU computed goto
- [x] GNU case ranges
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
- [x] Computed goto
- [ ] Nested functions
- [ ] Omitted-middle `?:`
- [x] Case ranges
- [ ] `typeof`
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] GNU numeric types
- [x] GNU VLA extensions
- [ ] Zero-length arrays
- [ ] Extended flexible-array semantics
- [x] Empty structures
- [ ] Union casts
- [ ] Range designators
- [ ] GNU enum extensions
- [x] GNU inline assembly
- [ ] GNU TLS
- [ ] GNU variadic macro extensions
- [ ] GNU preprocessor extensions
- [x] GNU attributes
- [x] GCC builtins
- [x] GNU vector extensions

## GNU23

- [ ] Statement expressions
- [ ] Locally declared labels
- [x] Label addresses
- [x] Computed goto
- [ ] Nested functions
- [ ] Nested-function trampolines / static chains
- [ ] Omitted middle operand of `?:`
- [x] Case ranges
- [ ] `__typeof__`
- [ ] `__typeof_unqual__`
- [ ] `__auto_type`
- [ ] `__alignof__`
- [x] `__int128`
- [ ] GNU additional floating types
- [ ] GNU fixed-point types
- [x] `__real__`
- [x] `__imag__`
- [ ] VLA members in local aggregates
- [ ] Parameter forward declarations
- [ ] Zero-length arrays
- [ ] Extended flexible-array semantics
- [x] Empty structures
- [ ] Union casts
- [ ] Range designators
- [ ] Incomplete enum forward declarations without fixed type
- [x] GNU inline assembly
- [ ] `__thread`
- [ ] Named variadic macro arguments
- [ ] `, ##__VA_ARGS__`
- [ ] GNU preprocessor extensions
- [x] GNU pragmas
- [x] GNU predefined macros
- [ ] GNU alternate keywords
- [x] GNU pointer arithmetic extensions
- [x] GNU attributes
- [x] GCC builtins
- [x] GNU vector extensions

# GNU Attributes

## General Syntax

- [x] `__attribute__((...))`
- [ ] `[[gnu::...]]`
- [x] Multiple attributes
- [x] Attributes on declarations
- [x] Attributes on types
- [x] Attributes on functions
- [x] Attributes on variables
- [ ] Attributes on labels
- [ ] Attributes on statements

## Type and Layout Attributes

- [x] `aligned`
- [x] `packed`
- [ ] `mode`
- [x] `vector_size`
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
- [x] `malloc`
- [x] `alloc_size`
- [x] `alloc_align`
- [x] `assume_aligned`
- [x] `returns_nonnull`
- [x] `nonnull`
- [x] `noreturn`
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

- [x] `alias`
- [ ] `weak`
- [x] `weakref`
- [ ] `section`
- [ ] `visibility`
- [ ] `externally_visible`
- [ ] `ifunc`
- [ ] `noplt`
- [ ] `common`
- [ ] `nocommon`
- [x] `constructor`
- [x] `destructor`
- [x] Constructor priority
- [x] Destructor priority
- [ ] `copy`

## Diagnostic and Contract Attributes

- [ ] `deprecated`
- [ ] `warning`
- [ ] `error`
- [ ] `format`
- [ ] `format_arg`
- [ ] `access`
- [x] `nonnull`
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

- [x] `__builtin_va_list`
- [x] `__builtin_va_start`
- [x] `__builtin_va_arg`
- [x] `__builtin_va_end`
- [x] `__builtin_va_copy`

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

- [x] GNU `qsort_r`

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
