/* Test whether the -Wcast-qual handles cv-qualified functions correctly.  */
/* { dg-do compile } */
/* { dg-options "-Wcast-qual" } */

typedef int (intfn_t) (int);
typedef void (voidfn_t) (void);

typedef const intfn_t *constfn_t;
typedef volatile voidfn_t *noreturnfn_t;

intfn_t intfn;
const intfn_t constfn;
voidfn_t voidfn;
volatile voidfn_t noreturnfn;

intfn_t *i1 = intfn;
intfn_t *i2 = (intfn_t *) intfn;
intfn_t *i3 = constfn;
intfn_t *i4 = (intfn_t *) constfn; /* { dg-bogus "discards qualifier" } */

constfn_t p1 = intfn; /* { dg-warning "makes '__attribute__..const..' qualified function" } */
constfn_t p2 = (constfn_t) intfn; /* { dg-warning "adds '__attribute__..const..' qualifier" } */
constfn_t p3 = constfn;
constfn_t p4 = (constfn_t) constfn;

voidfn_t *v1 = voidfn;
voidfn_t *v2 = (voidfn_t *) voidfn;
voidfn_t *v3 = noreturnfn;
voidfn_t *v4 = (voidfn_t *) noreturnfn; /* { dg-bogus "discards qualifier" } */

noreturnfn_t n1 = voidfn; /* { dg-warning "makes '__attribute__..noreturn..' qualified function" } */
noreturnfn_t n2 = (noreturnfn_t) voidfn; /* { dg-warning "adds '__attribute__..noreturn..' qualifier" } */
noreturnfn_t n3 = noreturnfn;
noreturnfn_t n4 = (noreturnfn_t) noreturnfn;

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_intfn_t:[0-9]+]] intfn_t = fn(i32) -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_voidfn_t:[0-9]+]] voidfn_t = fn() -> void;
// DEFAULT-NEXT:     type @type[[TYPE_constfn_t:[0-9]+]] constfn_t = ptr<const fn(i32) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_noreturnfn_t:[0-9]+]] noreturnfn_t = ptr<volatile fn() -> void>;
// DEFAULT-NEXT:     global %[[VALUE_i1:[0-9]+]] i1: ptr<fn(i32) -> i32> [storage=static] = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_intfn:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i2:[0-9]+]] i2: ptr<fn(i32) -> i32> [storage=static] = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_intfn]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i3:[0-9]+]] i3: ptr<fn(i32) -> i32> [storage=static] = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_constfn:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i4:[0-9]+]] i4: ptr<fn(i32) -> i32> [storage=static] = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_constfn]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p1:[0-9]+]] p1: ptr<const fn(i32) -> i32> [storage=static] = pointer_cast<ptr<const fn(i32) -> i32>, reason=assign>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_intfn]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p2:[0-9]+]] p2: ptr<const fn(i32) -> i32> [storage=static] = pointer_cast<ptr<const fn(i32) -> i32>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_intfn]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p3:[0-9]+]] p3: ptr<const fn(i32) -> i32> [storage=static] = pointer_cast<ptr<const fn(i32) -> i32>, reason=assign>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_constfn]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p4:[0-9]+]] p4: ptr<const fn(i32) -> i32> [storage=static] = pointer_cast<ptr<const fn(i32) -> i32>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_constfn]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_voidfn:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_voidfn]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v3:[0-9]+]] v3: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_noreturnfn:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v4:[0-9]+]] v4: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_noreturnfn]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n1:[0-9]+]] n1: ptr<volatile fn() -> void> [storage=static] = pointer_cast<ptr<volatile fn() -> void>, reason=assign>(function_decay<ptr<fn() -> void>>(%[[VALUE_voidfn]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n2:[0-9]+]] n2: ptr<volatile fn() -> void> [storage=static] = pointer_cast<ptr<volatile fn() -> void>, reason=explicit>(function_decay<ptr<fn() -> void>>(%[[VALUE_voidfn]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n3:[0-9]+]] n3: ptr<volatile fn() -> void> [storage=static] = pointer_cast<ptr<volatile fn() -> void>, reason=assign>(function_decay<ptr<fn() -> void>>(%[[VALUE_noreturnfn]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n4:[0-9]+]] n4: ptr<volatile fn() -> void> [storage=static] = pointer_cast<ptr<volatile fn() -> void>, reason=explicit>(function_decay<ptr<fn() -> void>>(%[[VALUE_noreturnfn]])) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_intfn]] @intfn(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_constfn]] @constfn(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_voidfn]] @voidfn() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_noreturnfn]] @noreturnfn() -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
