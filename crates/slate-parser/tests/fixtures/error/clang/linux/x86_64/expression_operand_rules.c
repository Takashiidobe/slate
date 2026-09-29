// SLATE-FILECHECK-DEFINES NOT_STRUCT NOT_STRUCT
// SLATE-FILECHECK-ERROR NOT_STRUCT
// SLATE-FILECHECK-DEFINES ADDR_BITFIELD ADDR_BITFIELD
// SLATE-FILECHECK-ERROR ADDR_BITFIELD
// SLATE-FILECHECK-DEFINES ADDR_REGISTER ADDR_REGISTER
// SLATE-FILECHECK-ERROR ADDR_REGISTER
// SLATE-FILECHECK-DEFINES ADDR_LANE ADDR_LANE
// SLATE-FILECHECK-ERROR ADDR_LANE
// SLATE-FILECHECK-DEFINES ADDR_RVALUE ADDR_RVALUE
// SLATE-FILECHECK-ERROR ADDR_RVALUE
// SLATE-FILECHECK-DEFINES UNKNOWN_MEMBER UNKNOWN_MEMBER
// SLATE-FILECHECK-ERROR UNKNOWN_MEMBER
// SLATE-FILECHECK-DEFINES MEMBER_NONRECORD MEMBER_NONRECORD
// SLATE-FILECHECK-ERROR MEMBER_NONRECORD
// SLATE-FILECHECK-DEFINES ARROW_NONPOINTER ARROW_NONPOINTER
// SLATE-FILECHECK-ERROR ARROW_NONPOINTER
// SLATE-FILECHECK-DEFINES NONCALLABLE NONCALLABLE
// SLATE-FILECHECK-ERROR NONCALLABLE
// SLATE-FILECHECK-DEFINES ARGUMENT_COUNT ARGUMENT_COUNT
// SLATE-FILECHECK-ERROR ARGUMENT_COUNT
// SLATE-FILECHECK-DEFINES VECTOR_INDEX VECTOR_INDEX
// SLATE-FILECHECK-ERROR VECTOR_INDEX
// SLATE-FILECHECK-DEFINES POINTER_OFFSET POINTER_OFFSET
// SLATE-FILECHECK-ERROR POINTER_OFFSET
// SLATE-FILECHECK-DEFINES POINTER_SUBTRACT POINTER_SUBTRACT
// SLATE-FILECHECK-ERROR POINTER_SUBTRACT
// SLATE-FILECHECK-DEFINES UNARY_PLUS UNARY_PLUS
// SLATE-FILECHECK-ERROR UNARY_PLUS
// SLATE-FILECHECK-DEFINES ASSIGN_RVALUE ASSIGN_RVALUE
// SLATE-FILECHECK-ERROR ASSIGN_RVALUE
// SLATE-FILECHECK-DEFINES INCREMENT_STRUCT INCREMENT_STRUCT
// SLATE-FILECHECK-ERROR INCREMENT_STRUCT
// SLATE-FILECHECK-DEFINES SIZEOF_INCOMPLETE SIZEOF_INCOMPLETE
// SLATE-FILECHECK-ERROR SIZEOF_INCOMPLETE
// SLATE-FILECHECK-DEFINES TYPEOF_BITFIELD TYPEOF_BITFIELD
// SLATE-FILECHECK-ERROR TYPEOF_BITFIELD
// SLATE-FILECHECK-DEFINES BITCAST_SIZE BITCAST_SIZE
// SLATE-FILECHECK-ERROR BITCAST_SIZE
// SLATE-FILECHECK-DEFINES VA_ARG VA_ARG
// SLATE-FILECHECK-ERROR VA_ARG
// SLATE-FILECHECK-DEFINES OVERFLOW_TYPE OVERFLOW_TYPE
// SLATE-FILECHECK-ERROR OVERFLOW_TYPE
// SLATE-FILECHECK-DEFINES FLOAT_CLASS FLOAT_CLASS
// SLATE-FILECHECK-ERROR FLOAT_CLASS
// SLATE-FILECHECK-DEFINES SHUFFLE_ARITY SHUFFLE_ARITY
// SLATE-FILECHECK-ERROR SHUFFLE_ARITY
// SLATE-FILECHECK-DEFINES CONVERTVECTOR_LANES CONVERTVECTOR_LANES
// SLATE-FILECHECK-ERROR CONVERTVECTOR_LANES
// SLATE-FILECHECK-DEFINES COMPLEX_REMAINDER COMPLEX_REMAINDER
// SLATE-FILECHECK-ERROR COMPLEX_REMAINDER
// SLATE-FILECHECK-DEFINES FLOAT_SHIFT FLOAT_SHIFT
// SLATE-FILECHECK-ERROR FLOAT_SHIFT
// SLATE-FILECHECK-DEFINES DEREF_INT DEREF_INT
// SLATE-FILECHECK-ERROR DEREF_INT
// SLATE-FILECHECK-DEFINES SUBSCRIPT_INT SUBSCRIPT_INT
// SLATE-FILECHECK-ERROR SUBSCRIPT_INT
// SLATE-FILECHECK-DEFINES REAL_POINTER REAL_POINTER
// SLATE-FILECHECK-ERROR REAL_POINTER
// SLATE-FILECHECK-DEFINES CONDITIONAL_MISMATCH CONDITIONAL_MISMATCH
// SLATE-FILECHECK-ERROR CONDITIONAL_MISMATCH
// SLATE-FILECHECK-DEFINES ATOMIC_COUNT ATOMIC_COUNT
// SLATE-FILECHECK-ERROR ATOMIC_COUNT
// SLATE-FILECHECK-DEFINES ATOMIC_NONPOINTER ATOMIC_NONPOINTER
// SLATE-FILECHECK-ERROR ATOMIC_NONPOINTER
// SLATE-FILECHECK-DEFINES ASM_BITFIELD ASM_BITFIELD
// SLATE-FILECHECK-ERROR ASM_BITFIELD
struct S { int a; } s;
struct B { int x : 3; } b;
struct I;
typedef int v4 __attribute__((vector_size(16)));
v4 v;
struct S g(void);
#if defined(NOT_STRUCT)
int f(void) { return !s; }
#elif defined(ADDR_BITFIELD)
void *f(void) { return &b.x; }
#elif defined(ADDR_REGISTER)
void *f(void) { register int r = 0; return &r; }
#elif defined(ADDR_LANE)
void *f(void) { return &v[0]; }
#elif defined(ADDR_RVALUE)
void *f(void) { return &(g().a); }
#elif defined(UNKNOWN_MEMBER)
int f(void) { return s.nope; }
#elif defined(MEMBER_NONRECORD)
int f(int i) { return i.x; }
#elif defined(ARROW_NONPOINTER)
int f(void) { return s->a; }
#elif defined(NONCALLABLE)
int f(int i) { return i(); }
#elif defined(ARGUMENT_COUNT)
int h(int, int);
int f(void) { return h(1); }
#elif defined(VECTOR_INDEX)
int f(void) { return v[1.0]; }
#elif defined(POINTER_OFFSET)
int *f(int *p) { return p + 1.0; }
#elif defined(POINTER_SUBTRACT)
long f(int *p, long *q) { return p - q; }
#elif defined(UNARY_PLUS)
int *f(int *p) { return +p; }
#elif defined(ASSIGN_RVALUE)
void f(void) { 1 = 2; }
#elif defined(INCREMENT_STRUCT)
void f(void) { s++; }
#elif defined(SIZEOF_INCOMPLETE)
unsigned long f(struct I *p) { return sizeof(*p); }
#elif defined(TYPEOF_BITFIELD)
void f(void) { __typeof__(b.x) y; }
#elif defined(BITCAST_SIZE)
long f(int i) { return __builtin_bit_cast(long, i); }
#elif defined(VA_ARG)
int f(int i) { return __builtin_va_arg(i, int); }
#elif defined(OVERFLOW_TYPE)
int f(void) { float r; return __builtin_add_overflow(1, 2, &r); }
#elif defined(FLOAT_CLASS)
int f(void) { return __builtin_isnan(1); }
#elif defined(SHUFFLE_ARITY)
v4 f(void) { return __builtin_shufflevector(v); }
#elif defined(CONVERTVECTOR_LANES)
typedef int v2 __attribute__((vector_size(8)));
v2 f(void) { return __builtin_convertvector(v, v2); }
#elif defined(COMPLEX_REMAINDER)
_Complex double f(_Complex double z) { return z % z; }
#elif defined(FLOAT_SHIFT)
double f(double d) { return d << 1; }
#elif defined(DEREF_INT)
int f(int i) { return *i; }
#elif defined(SUBSCRIPT_INT)
int f(int i) { return i[1]; }
#elif defined(REAL_POINTER)
int f(int *p) { return __real__ p; }
#elif defined(CONDITIONAL_MISMATCH)
int f(int c) { return (c ? s : 1).a; }
#elif defined(ATOMIC_COUNT)
int f(int *p) { return __atomic_load_n(p); }
#elif defined(ATOMIC_NONPOINTER)
int f(int i) { return __atomic_load_n(i, 0); }
#elif defined(ASM_BITFIELD)
void f(void) { __asm__("" : : "m"(b.x)); }
#endif

// SLATE-FILECHECK-BEGIN NOT_STRUCT
// NOT_STRUCT: Error:   × semantic analysis failed
// NOT_STRUCT: Error:
// NOT_STRUCT: × non-scalar condition
// NOT_STRUCT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:8:22]
// NOT_STRUCT: 7 │ #if defined(NOT_STRUCT)
// NOT_STRUCT: 8 │ int f(void) { return !s; }
// NOT_STRUCT: ·                      ──
// NOT_STRUCT: 9 │ #elif defined(ADDR_BITFIELD)
// NOT_STRUCT: ╰────
// SLATE-FILECHECK-END NOT_STRUCT
// SLATE-FILECHECK-BEGIN ADDR_BITFIELD
// ADDR_BITFIELD: Error:   × semantic analysis failed
// ADDR_BITFIELD: Error:
// ADDR_BITFIELD: × address of a bit-field
// ADDR_BITFIELD: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:10:24]
// ADDR_BITFIELD: 9 │ #elif defined(ADDR_BITFIELD)
// ADDR_BITFIELD: 10 │ void *f(void) { return &b.x; }
// ADDR_BITFIELD: ·                        ────
// ADDR_BITFIELD: 11 │ #elif defined(ADDR_REGISTER)
// ADDR_BITFIELD: ╰────
// SLATE-FILECHECK-END ADDR_BITFIELD
// SLATE-FILECHECK-BEGIN ADDR_REGISTER
// ADDR_REGISTER: Error:   × semantic analysis failed
// ADDR_REGISTER: Error:
// ADDR_REGISTER: × address of register variable requested
// ADDR_REGISTER: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:12:44]
// ADDR_REGISTER: 11 │ #elif defined(ADDR_REGISTER)
// ADDR_REGISTER: 12 │ void *f(void) { register int r = 0; return &r; }
// ADDR_REGISTER: ·                                            ──
// ADDR_REGISTER: 13 │ #elif defined(ADDR_LANE)
// ADDR_REGISTER: ╰────
// SLATE-FILECHECK-END ADDR_REGISTER
// SLATE-FILECHECK-BEGIN ADDR_LANE
// ADDR_LANE: Error:   × semantic analysis failed
// ADDR_LANE: Error:
// ADDR_LANE: × address of a vector element
// ADDR_LANE: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:14:24]
// ADDR_LANE: 13 │ #elif defined(ADDR_LANE)
// ADDR_LANE: 14 │ void *f(void) { return &v[0]; }
// ADDR_LANE: ·                        ─────
// ADDR_LANE: 15 │ #elif defined(ADDR_RVALUE)
// ADDR_LANE: ╰────
// SLATE-FILECHECK-END ADDR_LANE
// SLATE-FILECHECK-BEGIN ADDR_RVALUE
// ADDR_RVALUE: Error:   × semantic analysis failed
// ADDR_RVALUE: Error:
// ADDR_RVALUE: × address of an rvalue
// ADDR_RVALUE: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:16:24]
// ADDR_RVALUE: 15 │ #elif defined(ADDR_RVALUE)
// ADDR_RVALUE: 16 │ void *f(void) { return &(g().a); }
// ADDR_RVALUE: ·                        ────────
// ADDR_RVALUE: 17 │ #elif defined(UNKNOWN_MEMBER)
// ADDR_RVALUE: ╰────
// SLATE-FILECHECK-END ADDR_RVALUE
// SLATE-FILECHECK-BEGIN UNKNOWN_MEMBER
// UNKNOWN_MEMBER: Error:   × semantic analysis failed
// UNKNOWN_MEMBER: Error:
// UNKNOWN_MEMBER: × unknown member
// UNKNOWN_MEMBER: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:18:22]
// UNKNOWN_MEMBER: 17 │ #elif defined(UNKNOWN_MEMBER)
// UNKNOWN_MEMBER: 18 │ int f(void) { return s.nope; }
// UNKNOWN_MEMBER: ·                      ──────
// UNKNOWN_MEMBER: 19 │ #elif defined(MEMBER_NONRECORD)
// UNKNOWN_MEMBER: ╰────
// SLATE-FILECHECK-END UNKNOWN_MEMBER
// SLATE-FILECHECK-BEGIN MEMBER_NONRECORD
// MEMBER_NONRECORD: Error:   × semantic analysis failed
// MEMBER_NONRECORD: Error:
// MEMBER_NONRECORD: × member of incomplete or non-record
// MEMBER_NONRECORD: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:20:23]
// MEMBER_NONRECORD: 19 │ #elif defined(MEMBER_NONRECORD)
// MEMBER_NONRECORD: 20 │ int f(int i) { return i.x; }
// MEMBER_NONRECORD: ·                       ───
// MEMBER_NONRECORD: 21 │ #elif defined(ARROW_NONPOINTER)
// MEMBER_NONRECORD: ╰────
// SLATE-FILECHECK-END MEMBER_NONRECORD
// SLATE-FILECHECK-BEGIN ARROW_NONPOINTER
// ARROW_NONPOINTER: Error:   × semantic analysis failed
// ARROW_NONPOINTER: Error:
// ARROW_NONPOINTER: × member reference base is not a pointer
// ARROW_NONPOINTER: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:22:22]
// ARROW_NONPOINTER: 21 │ #elif defined(ARROW_NONPOINTER)
// ARROW_NONPOINTER: 22 │ int f(void) { return s->a; }
// ARROW_NONPOINTER: ·                      ────
// ARROW_NONPOINTER: 23 │ #elif defined(NONCALLABLE)
// ARROW_NONPOINTER: ╰────
// SLATE-FILECHECK-END ARROW_NONPOINTER
// SLATE-FILECHECK-BEGIN NONCALLABLE
// NONCALLABLE: Error:   × semantic analysis failed
// NONCALLABLE: Error:
// NONCALLABLE: × non-function callee
// NONCALLABLE: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:24:23]
// NONCALLABLE: 23 │ #elif defined(NONCALLABLE)
// NONCALLABLE: 24 │ int f(int i) { return i(); }
// NONCALLABLE: ·                       ───
// NONCALLABLE: 25 │ #elif defined(ARGUMENT_COUNT)
// NONCALLABLE: ╰────
// SLATE-FILECHECK-END NONCALLABLE
// SLATE-FILECHECK-BEGIN ARGUMENT_COUNT
// ARGUMENT_COUNT: Error:   × semantic analysis failed
// ARGUMENT_COUNT: Error:
// ARGUMENT_COUNT: × call argument count
// ARGUMENT_COUNT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:27:22]
// ARGUMENT_COUNT: 26 │ int h(int, int);
// ARGUMENT_COUNT: 27 │ int f(void) { return h(1); }
// ARGUMENT_COUNT: ·                      ────
// ARGUMENT_COUNT: 28 │ #elif defined(VECTOR_INDEX)
// ARGUMENT_COUNT: ╰────
// SLATE-FILECHECK-END ARGUMENT_COUNT
// SLATE-FILECHECK-BEGIN VECTOR_INDEX
// VECTOR_INDEX: Error:   × semantic analysis failed
// VECTOR_INDEX: Error:
// VECTOR_INDEX: × vector index is not an integer
// VECTOR_INDEX: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:29:22]
// VECTOR_INDEX: 28 │ #elif defined(VECTOR_INDEX)
// VECTOR_INDEX: 29 │ int f(void) { return v[1.0]; }
// VECTOR_INDEX: ·                      ──────
// VECTOR_INDEX: 30 │ #elif defined(POINTER_OFFSET)
// VECTOR_INDEX: ╰────
// SLATE-FILECHECK-END VECTOR_INDEX
// SLATE-FILECHECK-BEGIN POINTER_OFFSET
// POINTER_OFFSET: Error:   × semantic analysis failed
// POINTER_OFFSET: Error:
// POINTER_OFFSET: × noninteger pointer offset
// POINTER_OFFSET: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:31:25]
// POINTER_OFFSET: 30 │ #elif defined(POINTER_OFFSET)
// POINTER_OFFSET: 31 │ int *f(int *p) { return p + 1.0; }
// POINTER_OFFSET: ·                         ───────
// POINTER_OFFSET: 32 │ #elif defined(POINTER_SUBTRACT)
// POINTER_OFFSET: ╰────
// SLATE-FILECHECK-END POINTER_OFFSET
// SLATE-FILECHECK-BEGIN POINTER_SUBTRACT
// POINTER_SUBTRACT: Error:   × semantic analysis failed
// POINTER_SUBTRACT: Error:
// POINTER_SUBTRACT: × incompatible pointer subtraction
// POINTER_SUBTRACT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:33:34]
// POINTER_SUBTRACT: 32 │ #elif defined(POINTER_SUBTRACT)
// POINTER_SUBTRACT: 33 │ long f(int *p, long *q) { return p - q; }
// POINTER_SUBTRACT: ·                                  ─────
// POINTER_SUBTRACT: 34 │ #elif defined(UNARY_PLUS)
// POINTER_SUBTRACT: ╰────
// SLATE-FILECHECK-END POINTER_SUBTRACT
// SLATE-FILECHECK-BEGIN UNARY_PLUS
// UNARY_PLUS: Error:   × semantic analysis failed
// UNARY_PLUS: Error:
// UNARY_PLUS: × non-numeric unary plus
// UNARY_PLUS: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:35:25]
// UNARY_PLUS: 34 │ #elif defined(UNARY_PLUS)
// UNARY_PLUS: 35 │ int *f(int *p) { return +p; }
// UNARY_PLUS: ·                         ──
// UNARY_PLUS: 36 │ #elif defined(ASSIGN_RVALUE)
// UNARY_PLUS: ╰────
// SLATE-FILECHECK-END UNARY_PLUS
// SLATE-FILECHECK-BEGIN ASSIGN_RVALUE
// ASSIGN_RVALUE: Error:   × semantic analysis failed
// ASSIGN_RVALUE: Error:
// ASSIGN_RVALUE: × expression is not assignable
// ASSIGN_RVALUE: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:37:16]
// ASSIGN_RVALUE: 36 │ #elif defined(ASSIGN_RVALUE)
// ASSIGN_RVALUE: 37 │ void f(void) { 1 = 2; }
// ASSIGN_RVALUE: ·                ─────
// ASSIGN_RVALUE: 38 │ #elif defined(INCREMENT_STRUCT)
// ASSIGN_RVALUE: ╰────
// SLATE-FILECHECK-END ASSIGN_RVALUE
// SLATE-FILECHECK-BEGIN INCREMENT_STRUCT
// INCREMENT_STRUCT: Error:   × semantic analysis failed
// INCREMENT_STRUCT: Error:
// INCREMENT_STRUCT: × non-arithmetic operand
// INCREMENT_STRUCT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:39:16]
// INCREMENT_STRUCT: 38 │ #elif defined(INCREMENT_STRUCT)
// INCREMENT_STRUCT: 39 │ void f(void) { s++; }
// INCREMENT_STRUCT: ·                ───
// INCREMENT_STRUCT: 40 │ #elif defined(SIZEOF_INCOMPLETE)
// INCREMENT_STRUCT: ╰────
// SLATE-FILECHECK-END INCREMENT_STRUCT
// SLATE-FILECHECK-BEGIN SIZEOF_INCOMPLETE
// SIZEOF_INCOMPLETE: Error:   × semantic analysis failed
// SIZEOF_INCOMPLETE: Error:
// SIZEOF_INCOMPLETE: × sizeof of incomplete type
// SIZEOF_INCOMPLETE: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:41:39]
// SIZEOF_INCOMPLETE: 40 │ #elif defined(SIZEOF_INCOMPLETE)
// SIZEOF_INCOMPLETE: 41 │ unsigned long f(struct I *p) { return sizeof(*p); }
// SIZEOF_INCOMPLETE: ·                                       ──────────
// SIZEOF_INCOMPLETE: 42 │ #elif defined(TYPEOF_BITFIELD)
// SIZEOF_INCOMPLETE: ╰────
// SLATE-FILECHECK-END SIZEOF_INCOMPLETE
// SLATE-FILECHECK-BEGIN TYPEOF_BITFIELD
// TYPEOF_BITFIELD: Error:   × semantic analysis failed
// TYPEOF_BITFIELD: Error:
// TYPEOF_BITFIELD: × typeof applied to a bit-field
// TYPEOF_BITFIELD: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:43:27]
// TYPEOF_BITFIELD: 42 │ #elif defined(TYPEOF_BITFIELD)
// TYPEOF_BITFIELD: 43 │ void f(void) { __typeof__(b.x) y; }
// TYPEOF_BITFIELD: ·                           ───
// TYPEOF_BITFIELD: 44 │ #elif defined(BITCAST_SIZE)
// TYPEOF_BITFIELD: ╰────
// SLATE-FILECHECK-END TYPEOF_BITFIELD
// SLATE-FILECHECK-BEGIN BITCAST_SIZE
// BITCAST_SIZE: Error:   × semantic analysis failed
// BITCAST_SIZE: Error:
// BITCAST_SIZE: × bit cast between types of different sizes
// BITCAST_SIZE: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:45:24]
// BITCAST_SIZE: 44 │ #elif defined(BITCAST_SIZE)
// BITCAST_SIZE: 45 │ long f(int i) { return __builtin_bit_cast(long, i); }
// BITCAST_SIZE: ·                        ───────────────────────────
// BITCAST_SIZE: 46 │ #elif defined(VA_ARG)
// BITCAST_SIZE: ╰────
// SLATE-FILECHECK-END BITCAST_SIZE
// SLATE-FILECHECK-BEGIN VA_ARG
// VA_ARG: Error:   × semantic analysis failed
// VA_ARG: Error:
// VA_ARG: × va_arg of non-va_list
// VA_ARG: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:47:23]
// VA_ARG: 46 │ #elif defined(VA_ARG)
// VA_ARG: 47 │ int f(int i) { return __builtin_va_arg(i, int); }
// VA_ARG: ·                       ────────────────────────
// VA_ARG: 48 │ #elif defined(OVERFLOW_TYPE)
// VA_ARG: ╰────
// SLATE-FILECHECK-END VA_ARG
// SLATE-FILECHECK-BEGIN OVERFLOW_TYPE
// OVERFLOW_TYPE: Error:   × semantic analysis failed
// OVERFLOW_TYPE: Error:
// OVERFLOW_TYPE: × overflow builtin operand type
// OVERFLOW_TYPE: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:49:31]
// OVERFLOW_TYPE: 48 │ #elif defined(OVERFLOW_TYPE)
// OVERFLOW_TYPE: 49 │ int f(void) { float r; return __builtin_add_overflow(1, 2, &r); }
// OVERFLOW_TYPE: ·                               ────────────────────────────────
// OVERFLOW_TYPE: 50 │ #elif defined(FLOAT_CLASS)
// OVERFLOW_TYPE: ╰────
// SLATE-FILECHECK-END OVERFLOW_TYPE
// SLATE-FILECHECK-BEGIN FLOAT_CLASS
// FLOAT_CLASS: Error:   × semantic analysis failed
// FLOAT_CLASS: Error:
// FLOAT_CLASS: × floating classification builtin operand
// FLOAT_CLASS: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:51:22]
// FLOAT_CLASS: 50 │ #elif defined(FLOAT_CLASS)
// FLOAT_CLASS: 51 │ int f(void) { return __builtin_isnan(1); }
// FLOAT_CLASS: ·                      ──────────────────
// FLOAT_CLASS: 52 │ #elif defined(SHUFFLE_ARITY)
// FLOAT_CLASS: ╰────
// SLATE-FILECHECK-END FLOAT_CLASS
// SLATE-FILECHECK-BEGIN SHUFFLE_ARITY
// SHUFFLE_ARITY: Error:   × semantic analysis failed
// SHUFFLE_ARITY: Error:
// SHUFFLE_ARITY: × shuffle builtin arity
// SHUFFLE_ARITY: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:53:21]
// SHUFFLE_ARITY: 52 │ #elif defined(SHUFFLE_ARITY)
// SHUFFLE_ARITY: 53 │ v4 f(void) { return __builtin_shufflevector(v); }
// SHUFFLE_ARITY: ·                     ──────────────────────────
// SHUFFLE_ARITY: 54 │ #elif defined(CONVERTVECTOR_LANES)
// SHUFFLE_ARITY: ╰────
// SLATE-FILECHECK-END SHUFFLE_ARITY
// SLATE-FILECHECK-BEGIN CONVERTVECTOR_LANES
// CONVERTVECTOR_LANES: Error:   × semantic analysis failed
// CONVERTVECTOR_LANES: Error:
// CONVERTVECTOR_LANES: × convertvector operands differ in lane count
// CONVERTVECTOR_LANES: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:56:21]
// CONVERTVECTOR_LANES: 55 │ typedef int v2 __attribute__((vector_size(8)));
// CONVERTVECTOR_LANES: 56 │ v2 f(void) { return __builtin_convertvector(v, v2); }
// CONVERTVECTOR_LANES: ·                     ──────────────────────────────
// CONVERTVECTOR_LANES: 57 │ #elif defined(COMPLEX_REMAINDER)
// CONVERTVECTOR_LANES: ╰────
// SLATE-FILECHECK-END CONVERTVECTOR_LANES
// SLATE-FILECHECK-BEGIN COMPLEX_REMAINDER
// COMPLEX_REMAINDER: Error:   × semantic analysis failed
// COMPLEX_REMAINDER: Error:
// COMPLEX_REMAINDER: × complex operator
// COMPLEX_REMAINDER: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:58:47]
// COMPLEX_REMAINDER: 57 │ #elif defined(COMPLEX_REMAINDER)
// COMPLEX_REMAINDER: 58 │ _Complex double f(_Complex double z) { return z % z; }
// COMPLEX_REMAINDER: ·                                               ─────
// COMPLEX_REMAINDER: 59 │ #elif defined(FLOAT_SHIFT)
// COMPLEX_REMAINDER: ╰────
// SLATE-FILECHECK-END COMPLEX_REMAINDER
// SLATE-FILECHECK-BEGIN FLOAT_SHIFT
// FLOAT_SHIFT: Error:   × semantic analysis failed
// FLOAT_SHIFT: Error:
// FLOAT_SHIFT: × invalid operands to binary expression: f64 << i32
// FLOAT_SHIFT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:60:29]
// FLOAT_SHIFT: 59 │ #elif defined(FLOAT_SHIFT)
// FLOAT_SHIFT: 60 │ double f(double d) { return d << 1; }
// FLOAT_SHIFT: ·                             ──────
// FLOAT_SHIFT: 61 │ #elif defined(DEREF_INT)
// FLOAT_SHIFT: ╰────
// SLATE-FILECHECK-END FLOAT_SHIFT
// SLATE-FILECHECK-BEGIN DEREF_INT
// DEREF_INT: Error:   × semantic analysis failed
// DEREF_INT: Error:
// DEREF_INT: × indirection of a non-pointer
// DEREF_INT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:62:23]
// DEREF_INT: 61 │ #elif defined(DEREF_INT)
// DEREF_INT: 62 │ int f(int i) { return *i; }
// DEREF_INT: ·                       ──
// DEREF_INT: 63 │ #elif defined(SUBSCRIPT_INT)
// DEREF_INT: ╰────
// SLATE-FILECHECK-END DEREF_INT
// SLATE-FILECHECK-BEGIN SUBSCRIPT_INT
// SUBSCRIPT_INT: Error:   × semantic analysis failed
// SUBSCRIPT_INT: Error:
// SUBSCRIPT_INT: × subscripted value is not an array, pointer, or vector
// SUBSCRIPT_INT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:64:23]
// SUBSCRIPT_INT: 63 │ #elif defined(SUBSCRIPT_INT)
// SUBSCRIPT_INT: 64 │ int f(int i) { return i[1]; }
// SUBSCRIPT_INT: ·                       ────
// SUBSCRIPT_INT: 65 │ #elif defined(REAL_POINTER)
// SUBSCRIPT_INT: ╰────
// SLATE-FILECHECK-END SUBSCRIPT_INT
// SLATE-FILECHECK-BEGIN REAL_POINTER
// REAL_POINTER: Error:   × semantic analysis failed
// REAL_POINTER: Error:
// REAL_POINTER: × non-numeric operand
// REAL_POINTER: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:66:24]
// REAL_POINTER: 65 │ #elif defined(REAL_POINTER)
// REAL_POINTER: 66 │ int f(int *p) { return __real__ p; }
// REAL_POINTER: ·                        ──────────
// REAL_POINTER: 67 │ #elif defined(CONDITIONAL_MISMATCH)
// REAL_POINTER: ╰────
// SLATE-FILECHECK-END REAL_POINTER
// SLATE-FILECHECK-BEGIN CONDITIONAL_MISMATCH
// CONDITIONAL_MISMATCH: Error:   × semantic analysis failed
// CONDITIONAL_MISMATCH: Error:
// CONDITIONAL_MISMATCH: × conditional operands have incompatible types
// CONDITIONAL_MISMATCH: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:68:24]
// CONDITIONAL_MISMATCH: 67 │ #elif defined(CONDITIONAL_MISMATCH)
// CONDITIONAL_MISMATCH: 68 │ int f(int c) { return (c ? s : 1).a; }
// CONDITIONAL_MISMATCH: ·                        ─────────
// CONDITIONAL_MISMATCH: 69 │ #elif defined(ATOMIC_COUNT)
// CONDITIONAL_MISMATCH: ╰────
// SLATE-FILECHECK-END CONDITIONAL_MISMATCH
// SLATE-FILECHECK-BEGIN ATOMIC_COUNT
// ATOMIC_COUNT: Error:   × semantic analysis failed
// ATOMIC_COUNT: Error:
// ATOMIC_COUNT: × atomic builtin argument count
// ATOMIC_COUNT: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:70:24]
// ATOMIC_COUNT: 69 │ #elif defined(ATOMIC_COUNT)
// ATOMIC_COUNT: 70 │ int f(int *p) { return __atomic_load_n(p); }
// ATOMIC_COUNT: ·                        ──────────────────
// ATOMIC_COUNT: 71 │ #elif defined(ATOMIC_NONPOINTER)
// ATOMIC_COUNT: ╰────
// SLATE-FILECHECK-END ATOMIC_COUNT
// SLATE-FILECHECK-BEGIN ATOMIC_NONPOINTER
// ATOMIC_NONPOINTER: Error:   × semantic analysis failed
// ATOMIC_NONPOINTER: Error:
// ATOMIC_NONPOINTER: × address argument to atomic builtin must be a pointer
// ATOMIC_NONPOINTER: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:72:23]
// ATOMIC_NONPOINTER: 71 │ #elif defined(ATOMIC_NONPOINTER)
// ATOMIC_NONPOINTER: 72 │ int f(int i) { return __atomic_load_n(i, 0); }
// ATOMIC_NONPOINTER: ·                       ─────────────────────
// ATOMIC_NONPOINTER: 73 │ #elif defined(ASM_BITFIELD)
// ATOMIC_NONPOINTER: ╰────
// SLATE-FILECHECK-END ATOMIC_NONPOINTER
// SLATE-FILECHECK-BEGIN ASM_BITFIELD
// ASM_BITFIELD: Error:   × semantic analysis failed
// ASM_BITFIELD: Error:
// ASM_BITFIELD: × address of a bit-field
// ASM_BITFIELD: ╭─[tests/fixtures/error/clang/linux/x86_64/expression_operand_rules.c:74:35]
// ASM_BITFIELD: 73 │ #elif defined(ASM_BITFIELD)
// ASM_BITFIELD: 74 │ void f(void) { __asm__("" : : "m"(b.x)); }
// ASM_BITFIELD: ·                                   ───
// ASM_BITFIELD: 75 │ #endif
// ASM_BITFIELD: ╰────
// SLATE-FILECHECK-END ASM_BITFIELD
