# Features

Slate has full C23 support. If it doesn't support C23, it's a bug to be
fixed.

Checkbox markers:

- `[x]` — supported, verified by a fixture under `tests/`.
- `[ ]` — not yet audited; status unknown.
- `[~]` — audited and confirmed unsupported, reason in parens. Do not
  re-audit without new information; update the reason if the situation
  changes.

## C89

- [x] Trigraphs (via `slate translate -std=gnu11 -trigraphs file.c`; not on by default under gnu23)
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
- [x] #line
- [x] #error
- [x] #pragma
- [x] Predefined macros: `__LINE__`
- [x] `__FILE__`
- [x] `__DATE__`
- [x] `__TIME__`
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
- [x] incomplete types
- [x] compatible/composite types
- [x] Type qualifiers
- [x] const
- [x] volatile
- [x] qualified pointers
- [x] qualifier propagation rules
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
- [x] Prototype parameter type checking
- [x] Old-style/K&R function declarations (via `slate translate -std=gnu11 file.c`)
- [x] Old-style/K&R function definitions (via `slate translate -std=gnu11 file.c`)
- [x] Variadic functions via ...
- [x] Array declarators
- [x] Function-pointer declarators
- [x] Abstract declarators
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

- [x] <iso646.h>
- [x] Alternative operator macros such as and, or, not, xor
- [x] <wchar.h>
- [x] <wctype.h>
- [x] Expanded wchar_t support
- [x] Wide-character I/O
- [x] Wide-string functions
- [x] Multibyte <-> wide-character conversions
- [x] Wide-character classification
- [x] Wide-character case conversion

## C99

- [x] // line comments
- [x] Universal character names \uXXXX
- [x] Universal character names \UXXXXXXXX
- [x] Universal character names in identifiers
- [x] Increased minimum translation limits
- [x] Integer / arithmetic types
- [x] `_Bool`
- [x] long long
- [x] unsigned long long
- [~] Extended signed integer types (no compiler-provided extended-integer-type mechanism; `__int128` is a separate GNU builtin type)
- [~] Extended unsigned integer types (same as signed)
- [x] Revised integer conversion rules
- [x] Signed integer division truncates toward zero
- [x] Corresponding remainder semantics
- [x] Complex arithmetic
- [x] `_Complex`
- [x] float `_Complex`
- [x] double `_Complex`
- [x] long double `_Complex`
- [~] `_Imaginary` where supported by the standard's model (clang never implements `_Imaginary`, in any mode)
- [x] Complex literals through library macros/functions
- [x] Complex arithmetic semantics
- [x] Floating point
- [x] Hexadecimal floating constants, e.g. 0x1.8p+2
- [x] FLT_EVAL_METHOD
- [x] Floating-point environment semantics
- [x] Floating-point contraction rules
- [x] IEC 60559/IEEE-754 feature macros where applicable
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
- [x] VLA runtime sizeof
- [x] Function prototype [*] VLA notation
- [x] static inside array function parameters
- [x] const/volatile/restrict inside array parameter brackets
- [x] Flexible array members
- [x] Initialization
- [x] Nonconstant initializers for automatic aggregate objects
- [x] Array designated initializers [index] = value
- [x] Structure designated initializers .member = value
- [x] Nested designated initializers
- [x] Compound literals (T){...}
- [x] Enumerations
- [x] Trailing comma in enumerator list
- [x] Functions
- [x] `__func__`
- [x] Implicit return 0 from main
- [x] C99 inline linkage semantics
- [x] Removed C89 behavior
- [x] Reject implicit int
- [x] Reject implicit function declarations

### Preprocessor

- [x] Variadic macros ...
- [x] `__VA_ARGS__`
- [x] Empty arguments to function-like macros
- [x] `_Pragma`
- [x] #pragma STDC FENV_ACCESS
- [x] #pragma STDC FP_CONTRACT
- [x] #pragma STDC CX_LIMITED_RANGE

### C99 version / feature macros

- [x] `__STDC_VERSION__`== 199901L (via `slate translate -std=gnu99 file.c`; defaults to gnu23)
- [x] `__STDC_HOSTED__`
- [x] `__STDC_IEC_559__`
- [x] `__STDC_IEC_559_COMPLEX__`
- [x] `__STDC_ISO_10646__`

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

- [x] Memory model / concurrency
- [x] C11 memory model
- [x] Sequenced-before relation
- [x] Data-race rules
- [x] Happens-before rules
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
- [x] TLS with extern
- [x] Thread storage duration
- [x] Alignment
- [x] \_Alignof
- [x] \_Alignas
- [x] Extended/over-alignment
- [x] Fundamental alignment
- [x] Valid alignment constraints
- [x] Generic programming
- [x] \_Generic
- [x] Generic association lists
- [x] default generic association
- [x] Type-based selection semantics
- [x] Compile-time assertions
- [x] \_Static_assert(expr, "message")
- [x] Function properties
- [x] \_Noreturn
- [x] Structures / unions
- [x] Anonymous structure members
- [x] Anonymous union members
- [x] Unicode literals
- [x] u'…'
- [x] U'…'
- [x] u8"…"
- [x] u"…"
- [x] U"…"
- [x] Expression / object semantic changes
- [x] Finer-grained evaluation/sequencing rules
- [x] Temporary-object lifetime rules
- [x] Updated effective-type / memory-model interactions
- [~] Analyzability (Annex L; clang implements no analyzability guarantees)
- [~] Analyzability specification (same as above)
- [~] `__STDC_ANALYZABLE__` when supported (clang never defines this macro)
- [x] Conditional feature macros
- [x] `__STDC_NO_ATOMICS__`
- [x] `__STDC_NO_THREADS__`
- [x] `__STDC_NO_VLA__`
- [x] `__STDC_NO_COMPLEX__`
- [x] `__STDC_VERSION__` == 201112L (via `slate translate -std=gnu11 file.c`)

### C11 library

- [x] New headers
- [x] <stdalign.h>
- [x] <stdatomic.h>
- [x] <stdnoreturn.h>
- [x] <threads.h>
- [x] <uchar.h>
- [x] Atomics library
- [x] `atomic_*` typedefs
- [x] atomic_init
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
- [x] lock-free query facilities
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
- [x] Runtime / allocation
- [x] aligned_alloc
- [x] quick_exit
- [x] at_quick_exit
- [x] timespec
- [x] timespec_get
- [x] Exclusive "x" mode for fopen/freopen
- [x] Thread-local errno
- [x] Complex / floating support
- [x] CMPLX
- [x] CMPLXF
- [x] CMPLXL
- [x] FLT_DECIMAL_DIG
- [x] DBL_DECIMAL_DIG
- [x] LDBL_DECIMAL_DIG
- [x] \*\_TRUE_MIN
- [x] \*\_HAS_SUBNORM
- [~] Annex K Support (optional, not implemented by glibc/musl/bionic; POSIX `libc-shim` doesn't provide it — MSVC's own secure `_s` functions are separately supported, see below)
- [~] `__STDC_LIB_EXT1__`, if implemented (clang never defines this; no Annex K interfaces to gate behind it)
- [~] Bounds-checking interfaces (not provided on POSIX targets, see Annex K Support above)
- [x] \_s functions (MSVC target only, see `libc-shim/bits/msvc/secure/`)
- [~] runtime-constraint handlers removed (nothing to remove; Annex K constraint handlers were never modeled)
- [x] gets() removed

## C17

- [x] `__STDC_VERSION__` == 201710L (via `slate translate -std=gnu17 file.c`)
- [x] All C11 features
- [x] All applicable C11 defect-report corrections
- [x] Updated atomic semantics
- [x] Updated effective-type/object semantics where corrected
- [x] Updated library wording from C11 defect reports
- [x] Updated threading wording from C11 defect reports
- [x] Updated complex/floating-point wording from C11 defect reports
- [x] Updated generic-selection wording from C11 defect reports

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
- [x] Bit-precise unsigned constant suffix uwb
- [~] Conditional decimal floating point (gated on `_Decimal*` types, unimplemented by clang — see Decimal types below)

### Decimal types

only when the implementation advertises decimal IEC 60559 support through `__STDC_IEC_60559_DFP__`.

(not supported for clang)?

- [~] `_Decimal32` (clang has no DFP codegen support at all)
- [~] `_Decimal64` (clang has no DFP codegen support at all)
- [~] `_Decimal128` (clang has no DFP codegen support at all)
- [~] Decimal arithmetic (depends on `_Decimal*` types)
- [~] Decimal constants/suffixes (depends on `_Decimal*` types)
- [~] Decimal conversions (depends on `_Decimal*` types)
- [~] Decimal floating environment where advertised (depends on `_Decimal*` types)
- [x] C23 integer representation
- [x] Two's-complement signed integers are mandatory
- [x] Sign-and-magnitude representation no longer supported
- [x] Ones'-complement representation no longer supported
- [x] Corresponding signed integer minimum values updated

### C23 literals

- [x] Binary literals 0b...
- [x] Binary literals 0B...
- [x] Digit separators ', e.g. 1'000'000
- [x] Digit separators in integer constants
- [x] Digit separators in floating constants
- [x] u8'…' UTF-8 character constants
- [x] u8"…" has type char8_t[N]
- [x] UTF-16 semantics required for u"…"
- [x] UTF-32 semantics required for U"…"
- [x] char8_t is a typedef of unsigned char in <uchar.h>, and changing char[N] to char8_t[N].

### C23 keywords

- [x] bool is a keyword
- [x] true is a keyword
- [x] false is a keyword
- [x] alignas is a keyword
- [x] alignof is a keyword
- [x] static_assert is a keyword
- [x] thread_local is a keyword
- [x] constexpr is a keyword
- [x] typeof
- [x] typeof_unqual
- [x] nullptr
- [x] The underscore-prefixed historical forms remain relevant for compatibility, although \_Noreturn is deprecated.
- [x] C23 nullptr
- [x] nullptr null pointer constant
- [x] nullptr_t
- [x] Correct conversions from nullptr
- [x] Correct comparisons involving nullptr
- [x] \_Generic interaction with nullptr_t
- [x] C23 type inference
- [x] auto object type inference
- [x] Inference from initializer
- [x] Correct interaction between old storage-class use of auto and inferred-type use
- [x] Reject unsupported C++-style return-type inference
- [x] Reject unsupported parameter type inference
- [x] C23 typeof
- [x] typeof(expression)
- [x] typeof(type-name)
- [x] typeof_unqual(expression)
- [x] typeof_unqual(type-name)
- [x] Qualifier-preserving typeof
- [x] Qualifier-removing typeof_unqual
- [x] Unevaluated operand semantics where required
- [x] C23 constexpr
- [x] constexpr object declarations
- [x] Constant initialization requirements
- [x] Compile-time value semantics
- [x] Correct linkage/storage restrictions
- [x] Use in integer constant expressions where permitted
- [x] C23 initialization
- [x] Empty initializer {}
- [x] T x = {};
- [x] Zero-initialization semantics for empty initializer
- [x] Empty initialization of arrays
- [x] Empty initialization of structures
- [~] Empty initialization of unions (`U u = {};` only zeroes the first member's bytes, not the whole object; fails when a later member is larger, e.g. `union { int i; int j[4]; }` — tracked in `tests/fixtures.gcc-torture.unsupported/pr19687.c`)
- [~] C23 compound literals (plain compound literals work, see C99 above; the C23 storage-class-specifier extension does not, see below)
- [~] Storage-class specifier on compound literal (clang's parser rejects this syntax outright, verified through clang 22)
- [~] static compound literal where allowed (same clang parser limitation)
- [~] register compound literal where allowed (same clang parser limitation)
- [~] thread_local compound literal where allowed (same clang parser limitation)
- [~] constexpr compound literal where allowed by the grammar/rules (same clang parser limitation)
- [x] C23 attributes
- [x] General attribute system
- [x] [[...]] syntax
- [x] Attribute namespaces
- [x] Unknown attributes handled according to C23 rules
- [x] Duplicate attributes
- [x] Attribute placement rules
- [x] \_\_has_c_attribute
- [x] Standard attributes
- [x] [[deprecated]]
- [x] [[deprecated("reason")]]
- [x] [[fallthrough]]
- [x] [[maybe_unused]]
- [x] [[maybe_unused]] on labels
- [x] [[nodiscard]]
- [x] [[nodiscard("reason")]]
- [x] [[noreturn]]
- [x] [[reproducible]]
- [x] [[unsequenced]]
- [x] C23 static assertions
- [x] static_assert(expr)
- [x] static_assert(expr, "message")
- [x] \_Static_assert(expr)
- [x] static_assert no longer depends on <assert.h>
- [x] C23 function declarations
- [x] Old-style/K&R function definitions removed
- [x] Old-style function declarations without prototypes removed
- [x] f() means a function with no parameters
- [x] Unnamed parameters permitted in function definitions
- [x] Relaxed variadic parameter lists
- [x] void f(...); where permitted by C23
- [x] Updated compatibility rules
- [x] C23 labels / statements
- [x] Label immediately before a declaration
- [x] Label immediately before }
- [x] [[maybe_unused]] label
- [x] C23 arrays / qualifiers
- [x] Array type and element type have consistent const qualification
- [x] Array type and element type have consistent volatile qualification
- [x] Pointer-to-array qualifier compatibility changes
- [x] Updated \_Generic behavior resulting from array qualification rules
- [x] C23 VLA / variably-modified types
- [x] Variably-modified types are mandatory
- [x] Allocated-storage VLA types are supported
- [x] Automatic VLA objects may remain optional
- [x] `__STDC_NO_VLA__` now indicates absence of automatic VLA objects rather than all VM types
- [x] WG14 explicitly made VM types mandatory while leaving automatic-storage VLAs conditional.

## C23 enumerations

- [x] Fixed underlying enum type
- [x] enum E : unsigned char
- [x] Forward declaration where permitted with fixed type
- [x] Enumerator values larger than int
- [x] Improved selection of enumerator types
- [x] Improved ordinary enumeration semantics

## C23 tagged types

- [x] Revised struct compatibility rules
- [x] Revised union compatibility rules
- [x] Revised enum compatibility rules
- [x] Compatible same-tag redeclarations in permitted cases

## C23 identifiers / Unicode

- [x] Identifier rules based on Unicode Standard Annex #31
- [x] Updated permitted Unicode identifier characters
- [x] Correct normalization/identifier handling required by the standard

## C23 preprocessor

- [x] #elifdef
- [x] #elifndef
- [x] #warning
- [x] `__has_include`
- [x] `__has_c_attribute`
- [x] `__VA_OPT__`
- [x] Empty variadic macro argument handling
- [x] Updated variadic macro replacement rules
- [x] #embed
- [x] Basic #embed "file"
- [x] Binary resource expansion
- [x] limit(...)
- [x] prefix(...)
- [x] suffix(...)
- [x] if_empty(...)
- [~] implementation-defined/vendor embed parameters handled correctly (no vendor-specific embed parameters are modeled; only the standard ones)
- [x] `__has_embed` support where required by C23's preprocessing facilities
- [x] C23 floating-point pragmas
- [x] #pragma STDC FENV_ROUND
- [x] #pragma STDC FENV_DEC_ROUND
- [~] Updated IEC 60559 feature detection (clang defines none of the C23 `__STDC_IEC_60559_*` macros, verified `-std=gnu23 -dM -E`)
- [~] `__STDC_IEC_60559_BFP__` (clang never defines this)
- [~] `__STDC_IEC_60559_DFP__` (clang never defines this; also gated on unimplemented DFP)
- [~] `__STDC_IEC_60559_COMPLEX__` (clang never defines this)
- [x] C23 version
- [x] `__STDC_VERSION__` == 202311L

## C23 library

## Headers

- [x] <stdbit.h>
- [x] <stdckdint.h>
- [x] Checked arithmetic
- [x] ckd_add
- [x] ckd_sub
- [x] ckd_mul
- [x] Generic integer operand support
- [x] Overflow reporting semantics
- [x] Bit utilities
- [x] Implement all appropriate signed/unsigned variants/macros in <stdbit.h>:
- [x] leading-zero count
- [x] leading-one count
- [x] trailing-zero count
- [x] trailing-one count
- [x] first leading zero
- [x] first leading one
- [x] first trailing zero
- [x] first trailing one
- [x] population count / count ones
- [x] zero-bit count
- [x] single-bit test
- [x] bit width
- [x] bit floor
- [x] bit ceil
- [x] byte-order macros
- [x] UTF-8 library support
- [x] char8_t
- [x] mbrtoc8
- [x] c8rtomb
- [x] atomic_char8_t
- [x] ATOMIC_CHAR8_T_LOCK_FREE
- [x] Memory management
- [x] free_sized
- [x] free_aligned_sized
- [x] memalignment
- [x] Updated allocation/deallocation synchronization semantics
- [x] C23 realloc(ptr, 0) behavior
- [x] free_sized, free_aligned_sized, and memalignment are C23 additions.
- [x] Memory/string functions
- [x] memset_explicit
- [x] memccpy
- [x] strdup
- [x] strndup
- [~] qualifier-preserving memchr (libc-shim uses the classic `const void *` -> `void *` signature)
- [~] qualifier-preserving strchr (same as memchr)
- [~] qualifier-preserving strpbrk (same as memchr)
- [~] qualifier-preserving strrchr (same as memchr)
- [~] qualifier-preserving strstr (same as memchr)
- [~] corresponding qualifier-preserving wide-character operations (same as memchr)
- [x] Program support
- [x] unreachable()
- [x] C23 provides unreachable through <stddef.h>.

#### Time

- [x] gmtime_r
- [x] localtime_r
- [x] timespec_getres
- [x] C23 strftime extensions
- [x] C23 wcsftime extensions
- [~] updated time-related macros (no C23 time.h macro deltas beyond what's already implemented were identified)
- [x] asctime deprecated (gated to `__STDC_VERSION__ >= 202311L` so pre-C23 API probes aren't affected)
- [x] ctime deprecated (same as `asctime`)
- [x] Formatted I/O
- [x] Binary %b conversion
- [x] wN length modifiers
- [x] wfN length modifiers
- [~] H decimal-float length modifier (gated on unimplemented DFP)
- [~] D decimal-float length modifier (gated on unimplemented DFP)
- [~] DD decimal-float length modifier (gated on unimplemented DFP)
- [x] corresponding scanf-family support
- [x] corresponding printf-family support
- [~] Floating point (see gaps below)
- [~] New IEC 60559 binary functions where supported (`totalorder`, `getpayload`, `fromfp`/`ufromfp`, `setpayload`, `iseqsig`, etc. not declared in libc-shim math.h)
- [~] Decimal math functions where DFP is supported (gated on unimplemented DFP)
- [~] decimal dN function variants (gated on unimplemented DFP)
- [~] quantizedN (gated on unimplemented DFP)
- [~] samequantumdN (gated on unimplemented DFP)
- [~] quantumdN (gated on unimplemented DFP)
- [~] llquantexpdN (gated on unimplemented DFP)
- [~] decimal encode/decode functions (gated on unimplemented DFP)
- [x] floating-to-string formatting functions
- [x] additional <float.h> macros
- [~] revised IEC 60559 feature macros (see Updated IEC 60559 feature detection above — clang defines none of these)
- [x] Integer limits
- [x] Integer width macros
- [x] CHAR_WIDTH
- [x] SCHAR_WIDTH
- [x] UCHAR_WIDTH
- [x] SHRT_WIDTH
- [x] USHRT_WIDTH
- [x] INT_WIDTH
- [x] UINT_WIDTH
- [x] LONG_WIDTH
- [x] ULONG_WIDTH
- [x] LLONG_WIDTH
- [x] ULLONG_WIDTH
- [x] width macros for standard typedefs where specified
- [x] exact-width integer rules updated
- [x] [u]intN_t consistency with [u]int_leastN_t
- [x] Library version macros
- [x] `__STDC_VERSION_FENV_H__`
- [x] `__STDC_VERSION_MATH_H__`
- [x] `__STDC_VERSION_STDINT_H__`
- [x] `__STDC_VERSION_STDLIB_H__`
- [x] `__STDC_VERSION_TGMATH_H__`
- [x] `__STDC_VERSION_TIME_H__`
- [x] `__STDC_VERSION_STDCKDINT_H__`
- [x] `__STDC_VERSION_STDBIT_H__`

# GNU extensions

## Language extensions

- [x] Statement expressions
- [x] Locally declared labels with `__label__`
- [x] Label addresses with `&&label`
- [x] Computed goto
- [~] Nested functions (clang rejects nested function definitions entirely: "function definition is not allowed here")
- [~] Nested-function trampolines / static chains (gated on nested functions, which clang doesn't support)
- [x] Omitted middle operand of `?:`
- [x] Case ranges
- [x] `__typeof__`
- [x] `__typeof_unqual__`
- [x] `__auto_type`
- [x] `__alignof__`
- [x] `__int128`
- [x] `unsigned __int128`
- [x] `_Float16`
- [~] `_Float32` (unsupported by clang)
- [~] `_Float64` (unsupported by clang)
- [~] `_Float128` (unsupported by clang)
- [~] `_Float32x` (unsupported by clang)
- [~] `_Float64x` (unsupported by clang)
- [~] `_Float128x` (unsupported by clang)
- [~] GNU fixed-point types (clang doesn't implement `_Fract`/`_Accum` at all: "unknown type name")
- [~] GNU saturating fixed-point types (same — gated on unimplemented fixed-point types)
- [x] `__real__`
- [x] `__imag__`
- [~] VLA members in local structures (clang: "fields must have a constant size ... will never be supported")
- [~] VLA members in local unions (same clang limitation as structures)
- [~] Parameter forward declarations (clang's parser doesn't accept `;`-separated forward parameter declarations at all)
- [x] Zero-length arrays
- [x] Flexible array members in unions
- [x] Structures containing only a flexible array member
- [x] Nested flexible-array extensions
- [x] Static initialization of flexible arrays
- [x] Empty structures
- [~] Union casts (works with a runtime source value; fails as a compile-time constant initializer — CIR gap, tracked as slate-b4jj)
- [x] Range designators
- [x] Historical `[index] value` initializer syntax
- [x] Historical `field: value` initializer syntax
- [x] Incomplete enum forward declarations
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
- [x] `"memory"` asm clobber
- [x] `"cc"` asm clobber
- [x] Symbolic asm operand names
- [x] Asm operand modifiers
- [x] `asm volatile`
- [x] `asm inline`
- [x] `asm goto`
- [x] Top-level asm
- [x] Target-specific asm constraints
- [x] `__thread`
- [x] `extern __thread`
- [x] `static __thread`
- [x] GNU TLS models (`tls_model` attribute is recognized and ignored; no observable-behavior equivalent needed)
- [x] Named variadic macro arguments
- [x] Omitted variadic macro arguments
- [x] `, ##__VA_ARGS__` comma elision
- [x] `#include_next`
- [x] GNU line markers
- [x] `#pragma GCC diagnostic`
- [x] `#pragma GCC diagnostic push`
- [x] `#pragma GCC diagnostic pop`
- [x] `#pragma GCC diagnostic warning`
- [x] `#pragma GCC diagnostic error`
- [x] `#pragma GCC poison`
- [x] `#pragma GCC system_header`
- [x] `#pragma GCC dependency`
- [x] GNU macro push/pop pragmas
- [x] GNU visibility pragmas
- [x] GNU weak pragmas
- [x] GNU structure-layout pragmas
- [~] GNU function-specific optimization pragmas (`#pragma GCC optimize(...)` errors: "unsupported semantic directive")
- [~] GNU target-specific pragmas (`#pragma GCC target(...)` errors: "unsupported semantic directive")
- [x] `__GNUC__`
- [x] `__GNUC_MINOR__`
- [x] `__GNUC_PATCHLEVEL__`
- [x] `__VERSION__`
- [x] `__BASE_FILE__`
- [x] `__INCLUDE_LEVEL__`
- [x] `__COUNTER__`
- [x] `__TIMESTAMP__`
- [x] Target-specific predefined macros
- [x] ABI-specific predefined macros
- [x] `$` in identifiers where supported
- [x] GNU extended escape handling
- [x] `__FUNCTION__`
- [x] `__PRETTY_FUNCTION__`
- [x] `__inline__`
- [x] `__extension__`
- [x] `__restrict__`
- [x] Arithmetic on `void *`
- [x] `sizeof(void)` GNU semantics
- [x] Arithmetic on function pointers
- [x] `sizeof(function-type)` GNU semantics
- [x] GNU prototype / old-style definition compatibility rules (via `slate translate -std=gnu17 file.c`; clang accepts a non-prototype declaration followed by a K&R definition with only a deprecation warning)
- [~] GNU array-pointer qualifier extensions (pointer-to-array locals initialized from `&array` fail to compile: `*mut [T;N]` vs. decayed `*mut T` mismatch, tracked as slate-n4e4)
- [~] GNU qualified function-type extensions (clang itself doesn't parse cv-qualified function types in C mode: "expected ';' after top level declarator")

# GNU Attributes

## General Syntax

- [x] `__attribute__((...))`
- [x] `[[gnu::...]]`
- [x] Multiple attributes
- [x] Attributes on declarations
- [x] Attributes on types
- [x] Attributes on functions
- [x] Attributes on variables
- [x] Attributes on labels
- [x] Attributes on statements

## Type and Layout Attributes

- [x] `aligned`
- [x] `packed`
- [x] `mode`
- [x] `vector_size`
- [~] `transparent_union` (type attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `warn_if_not_aligned` (type attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `strict_flex_array` (field attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `counted_by` (field attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `hardbool` (type attribute; not yet covered by the function-attribute diagnostic pass)

## Function and Optimization Attributes

- [x] `always_inline`
- [x] `noinline`
- [~] `gnu_inline` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `flatten` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `hot` (no Rust equivalent; parsed and diagnosed, not lowered)
- [x] `cold`
- [~] `pure` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `const` (no Rust equivalent; parsed and diagnosed, not lowered)
- [x] `malloc`
- [x] `alloc_size`
- [x] `alloc_align`
- [x] `assume_aligned`
- [x] `returns_nonnull`
- [x] `nonnull`
- [x] `noreturn`
- [~] `nothrow` (no Rust equivalent; parsed and diagnosed, not lowered)
- [x] `warn_unused_result`
- [x] `used`
- [x] `unused`
- [x] `retain`
- [~] `leaf` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `noclone` (not recognized by our CIR-enabled Clang build; nothing to detect)
- [~] `no_icf` (not recognized by our CIR-enabled Clang build; nothing to detect)
- [~] `no_instrument_function` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `no_profile_instrument_function` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `no_sanitize` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `optimize` (not recognized by our CIR-enabled Clang build; nothing to detect. Rust's unstable `#[optimize(speed|size)]` is a plausible partial match if a future Clang parses the GCC `-O`-level string form)
- [x] `target`
- [~] `target_clones` (clang parses it, but CIR codegen errors: "Not Yet Implemented: getOrCreateCIRFunction: multi-version")
- [~] `target_version` (not recognized by our CIR-enabled clang build on this target; nothing to detect)

## Linking and Object Attributes

- [x] `alias`
- [x] `weak`
- [x] `weakref`
- [x] `section`
- [x] `visibility`
- [~] `externally_visible` (not recognized by our CIR-enabled Clang build; nothing to detect)
- [~] `ifunc` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `noplt` (not recognized by our CIR-enabled Clang build; nothing to detect)
- [~] `common` (variable attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `nocommon` (variable attribute; not yet covered by the function-attribute diagnostic pass)
- [x] `constructor`
- [x] `destructor`
- [x] Constructor priority
- [x] Destructor priority
- [~] `copy` (not recognized by our CIR-enabled clang build; nothing to detect)

## Diagnostic and Contract Attributes

- [x] `deprecated`
- [~] `warning` (no Rust equivalent; parsed and diagnosed, not lowered. Clang's AST does not distinguish `warning` from `error` in its dump, so both are diagnosed under one `warning_or_error` fact)
- [~] `error` (no Rust equivalent; parsed and diagnosed, not lowered; see `warning` above)
- [~] `format` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `format_arg` (no Rust equivalent; parsed and diagnosed, not lowered)
- [~] `access` (not recognized by our CIR-enabled Clang build; nothing to detect)
- [~] `null_terminated_string_arg` (not recognized by our CIR-enabled Clang build; nothing to detect)
- [~] `sentinel` (no Rust equivalent; parsed and diagnosed, not lowered)

## Variable and Statement Attributes

- [~] `cleanup` (variable attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `fallthrough` (statement attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `assume` (statement attribute; not yet covered by the function-attribute diagnostic pass)
- [~] `musttail` (statement attribute, no Rust equivalent; not yet covered by the function-attribute diagnostic pass)

# GCC Builtins

## Compile-Time Introspection

- [x] `__builtin_constant_p`
- [x] `__builtin_types_compatible_p`
- [x] `__builtin_choose_expr`
- [x] `__builtin_offsetof`
- [x] `__builtin_object_size`
- [x] `__builtin_dynamic_object_size`
- [x] `__builtin_has_attribute`

## Control Flow and Optimization

- [x] `__builtin_expect`
- [x] `__builtin_expect_with_probability`
- [x] `__builtin_unreachable`
- [x] `__builtin_trap`
- [x] `__builtin_assume_aligned`

## Bit and Arithmetic Builtins

- [x] `__builtin_clz`
- [x] `__builtin_clzl`
- [x] `__builtin_clzll`
- [x] `__builtin_ctz`
- [x] `__builtin_ctzl`
- [x] `__builtin_ctzll`
- [x] `__builtin_popcount`
- [x] `__builtin_popcountl`
- [x] `__builtin_popcountll`
- [x] `__builtin_parity`
- [x] `__builtin_parityl`
- [x] `__builtin_parityll`
- [x] `__builtin_bswap16`
- [x] `__builtin_bswap32`
- [x] `__builtin_bswap64`
- [~] `__builtin_bswap128` where supported (clang doesn't implement it: "use of undeclared identifier")
- [x] Signed overflow-checking builtins
- [x] Unsigned overflow-checking builtins
- [x] Generic overflow-checking builtins
- [x] Carry builtins
- [x] Borrow builtins

## Stack and Frame Builtins

- [x] `__builtin_alloca`
- [x] `__builtin_alloca_with_align`
- [x] `__builtin_frame_address` (lowers to the address of a real stack local in the current frame; see `tests/fixtures/gnu_frame_address.c` and `tests/fixtures.gcc-torture/frame-address.c`)
- [x] `__builtin_return_address`

## GNU Atomic Builtins

- [x] `__atomic_load`
- [x] `__atomic_load_n`
- [x] `__atomic_store`
- [x] `__atomic_store_n`
- [x] `__atomic_exchange`
- [x] `__atomic_exchange_n`
- [x] `__atomic_compare_exchange`
- [x] `__atomic_compare_exchange_n`
- [x] `__atomic_add_fetch`
- [x] `__atomic_sub_fetch`
- [x] `__atomic_and_fetch`
- [x] `__atomic_xor_fetch`
- [x] `__atomic_or_fetch`
- [x] `__atomic_nand_fetch`
- [x] `__atomic_fetch_add`
- [x] `__atomic_fetch_sub`
- [x] `__atomic_fetch_and`
- [x] `__atomic_fetch_xor`
- [x] `__atomic_fetch_or`
- [x] `__atomic_fetch_nand`
- [x] `__atomic_test_and_set`
- [x] `__atomic_clear`
- [x] `__atomic_thread_fence`
- [x] `__atomic_signal_fence`
- [x] `__atomic_always_lock_free`
- [x] `__atomic_is_lock_free`
- [x] `__ATOMIC_RELAXED`
- [x] `__ATOMIC_CONSUME`
- [x] `__ATOMIC_ACQUIRE`
- [x] `__ATOMIC_RELEASE`
- [x] `__ATOMIC_ACQ_REL`
- [x] `__ATOMIC_SEQ_CST`

## Legacy GNU Atomic Builtins

- [x] `__sync_fetch_and_add`
- [x] `__sync_fetch_and_sub`
- [x] `__sync_fetch_and_or`
- [x] `__sync_fetch_and_and`
- [x] `__sync_fetch_and_xor`
- [x] `__sync_fetch_and_nand`
- [x] `__sync_add_and_fetch`
- [x] `__sync_sub_and_fetch`
- [x] `__sync_or_and_fetch`
- [x] `__sync_and_and_fetch`
- [x] `__sync_xor_and_fetch`
- [x] `__sync_nand_and_fetch`
- [~] `__sync_bool_compare_and_swap` (CIR codegen: "unimplemented X86 builtin call: __sync_bool_compare_and_swap_4")
- [~] `__sync_val_compare_and_swap` (same CIR gap as `__sync_bool_compare_and_swap`)
- [~] `__sync_lock_test_and_set` (CIR codegen gap, same family as the compare-and-swap builtins)
- [~] `__sync_lock_release` (CIR codegen gap, same family as the compare-and-swap builtins)
- [x] `__sync_synchronize`

## Variadic ABI Builtins

- [x] `__builtin_va_list`
- [x] `__builtin_va_start`
- [x] `__builtin_va_arg`
- [x] `__builtin_va_end`
- [x] `__builtin_va_copy`

## Library Recognition Builtins

- [x] Memory-operation builtins
- [x] String-operation builtins
- [x] Formatted-I/O builtins
- [x] Math-function builtins
- [x] Floating-point classification builtins
- [x] Target-specific intrinsic builtins

# GNU Vector Extensions

- [x] `vector_size` attribute
- [x] GNU vector types
- [x] Vector initialization
- [x] Vector arithmetic
- [x] Vector bitwise operations
- [x] Vector comparisons
- [x] Vector conversions
- [x] Vector subscripting
- [x] Scalar-to-vector operations
- [x] Vector ABI passing
- [x] Vector ABI return values
- [x] Target-specific vector builtins

# GNU libc Feature-Test Modes

## ISO C90 Library

- [x] ISO C90 library interfaces

## ISO C95 Library

- [x] ISO C95 library interfaces

## `_ISOC99_SOURCE`

- [x] ISO C99 library interfaces

## `_ISOC11_SOURCE`

- [x] ISO C11 library interfaces

## C17 Library

- [x] ISO C17 library interfaces
- [ ] C11 defect-report corrections
- [x] No dedicated `_ISOC17_SOURCE` selector (matches real glibc, which has no such macro either — C17 shares C11's feature-test gate)

## `_ISOC23_SOURCE`

- [x] ISO C23 library interfaces

## `_ISOC2X_SOURCE`

- [~] Legacy selector for developing C23 library interfaces (`_ISOC2X_SOURCE` isn't in the `__GLIBC_USE_ISOC23` gate in `libc-shim/bits/glibc.h`, only `_ISOC23_SOURCE`/`_ISOC2Y_SOURCE`; moot in practice since slate always compiles as gnu23, which already implies the gate)

# `_GNU_SOURCE` Library Extensions

## Dynamic Memory and Allocation

- [x] GNU allocation extensions
- [x] `canonicalize_file_name`
- [x] GNU malloc inspection interfaces
- [~] GNU malloc debugging interfaces (`libc-shim/include/mcheck.h` is a stub: `#error "<mcheck.h> is not yet defined"`)

## String and Memory Functions

- [x] `mempcpy`
- [x] `memrchr`
- [x] `rawmemchr`
- [x] `strchrnul`
- [x] `strcasestr`
- [x] `strverscmp`
- [~] `strfry` (not declared anywhere in libc-shim)
- [x] `memfrob`
- [~] `strdupa` (declared in `libc-shim/include/string.h` as a plain function, but glibc only provides it as an alloca-based macro; linking fails with "undefined symbol: strdupa")
- [~] `strndupa` (same gap as `strdupa` — no macro form, not a real linkable symbol)

## Formatted Output and Streams

- [x] `asprintf`
- [x] `vasprintf`
- [x] `fopencookie`
- [x] `cookie_io_functions_t`
- [x] `open_memstream`

## Line Input

- [x] `getline`
- [x] `getdelim`

## Program and Process Interfaces

- [~] `on_exit` (not declared anywhere in libc-shim)
- [x] `secure_getenv`
- [x] `get_current_dir_name`
- [x] `execvpe`
- [x] `program_invocation_name`
- [x] `program_invocation_short_name`

## Argument Parsing

- [x] `getopt_long`
- [x] `getopt_long_only`
- [x] `argp_parse`
- [x] GNU `argp_*` interfaces

## Error Reporting

- [~] `error` (not declared anywhere in libc-shim)
- [~] `error_at_line` (not declared anywhere in libc-shim)

## Sorting

- [x] GNU `qsort_r`

## Auxiliary Vector

- [x] `getauxval`

## Backtrace Support

- [x] `backtrace`
- [x] `backtrace_symbols`
- [x] `backtrace_symbols_fd`

## Obstacks

- [x] `obstack_init`
- [x] `obstack_alloc`
- [x] `obstack_copy`
- [x] `obstack_copy0`
- [x] `obstack_blank`
- [x] `obstack_grow`
- [x] `obstack_grow0`
- [x] `obstack_finish`
- [x] `obstack_free`
- [x] GNU `obstack_*` interfaces

## Dynamic Linking

- [x] GNU dynamic-linker extensions
- [x] GNU `dl*` extensions
- [x] GNU symbol lookup extensions
- [~] GNU loader namespace extensions (`dlmopen`/`Lmid_t` not declared anywhere in libc-shim)

# `_DEFAULT_SOURCE`

- [x] Default glibc extension namespace
- [x] BSD-derived interfaces
- [x] SVID-derived interfaces (`bcopy`/`bzero`/`index`/`rindex`, `hcreate`/`hsearch` family, `drand48`, etc. all declared)
- [ ] Historical miscellaneous interfaces

# POSIX Feature-Test Levels

- [x] `_POSIX_SOURCE`
- [x] `_POSIX_C_SOURCE=1`
- [x] `_POSIX_C_SOURCE=2`
- [x] `_POSIX_C_SOURCE=199309L`
- [x] `_POSIX_C_SOURCE=199506L`
- [x] `_POSIX_C_SOURCE=200112L`
- [x] `_POSIX_C_SOURCE=200809L`
- [x] `_POSIX_C_SOURCE=202405L`

# X/Open Feature-Test Levels

- [x] `_XOPEN_SOURCE`
- [x] `_XOPEN_SOURCE=500`
- [x] `_XOPEN_SOURCE=600`
- [x] `_XOPEN_SOURCE=700`
- [x] `_XOPEN_SOURCE=800`

# Large-File Support

- [x] `_LARGEFILE_SOURCE`
- [x] `_LARGEFILE64_SOURCE`
- [x] `_FILE_OFFSET_BITS=64`
- [x] 64-bit `off_t` API mappings
- [x] Explicit `*64` large-file interfaces

# 64-Bit Time Support

- [x] `_TIME_BITS=64`
- [x] `_TIME_BITS=64` interaction with `_FILE_OFFSET_BITS=64`
- [x] 64-bit `time_t` ABI mappings (extensive `__REDIR` table in `libc-shim/include/time.h`, e.g. `time` -> `__time64`, `mktime` -> `__mktime64`, etc.)

# glibc Fortification

- [~] `_FORTIFY_SOURCE=1` (`libc-shim` never references `_FORTIFY_SOURCE`; defining it doesn't redirect e.g. `memcpy` to `__memcpy_chk` — verified with `slate translate`)
- [~] `_FORTIFY_SOURCE=2` (same — no dispatch logic at all)
- [~] `_FORTIFY_SOURCE=3` (same — no dispatch logic at all)
- [~] `__builtin_object_size` integration (the builtin itself works, see GCC Builtins above, but nothing wires it into fortified libc calls)
- [~] `__builtin_dynamic_object_size` integration (same as `__builtin_object_size`)
- [~] Fortified `__*_chk` interfaces (the `_chk` builtins translate on their own, see `tests/fixtures.unsupported/gnu_builtins_memory.c`, but plain calls are never redirected to them)
- [~] Compile-time bounds diagnostics (gated on the missing `_FORTIFY_SOURCE` dispatch above)
- [~] Runtime bounds checking (gated on the missing `_FORTIFY_SOURCE` dispatch above)
