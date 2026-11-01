#include <stdio.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define TWICE(x) ((x) + (x))
#define BIT(n) (1 << (n))
#define NEXT() (*cursor++)
#define NEXT_FROM(p) (*(p)++)
#define BUMP_AND(x, y) ((x)++, (y))
#define BOTH(a, b) ((a) && (b))
#define EITHER(a, b) ((a) || (b))
#define PICK(c, a, b) ((c) ? (a) : (b))
#define CALL_WITH(f, x) ((*(f))(x))
#define FIELD_OF(s) ((s)->count)
#define AT(t, i) ((t)[i].count)
#define SMALLER(t, n, m, d) (AT(t, n) < AT(t, m) || (AT(t, n) == AT(t, m) && (d)[n] <= (d)[m]))
#define ASSIGN_TO(p, v) (*(p) = (v))
#define HOLDER(v) ((struct holder){(v), (v) + 1})
#define ADDRESS_OF_LITERAL(v) (&(struct holder){(v), 0})
#define STMT_EXPR(x) ({ int doubled = (x) * 2; doubled + 1; })
#define NOTHING(x) ((void)(x))
#define LOCAL_READ(x) ((x) + hidden)
#define COUNT_CALL() (counted_call())
#define TEXT_LENGTH(s) strlen(s)
#define WIDE_MIN(a, b) ((a) > (unsigned long)(b) ? (unsigned long)(b) : (a))

struct holder {
    int count;
    int other;
};

static int table[] = {5, 7, 11, 13, 17};
static int *cursor = table;
static int calls;
static int trace[16];
static int trace_length;
static int global_size = BIT(3);

static int counted_call(void) {
    calls++;
    return calls * 10;
}

static int note(int value) {
    trace[trace_length++] = value;
    return value;
}

static int double_it(int value) {
    return value * 2;
}

static void show_trace(const char *label) {
    printf("%s:", label);
    for (int i = 0; i < trace_length; i++) {
        printf(" %d", trace[i]);
    }
    printf("\n");
    trace_length = 0;
}

static void plain_values(void) {
    int a = 3, b = 9;
    int smallest = MIN(a, b);
    int largest = MAX(a, b);
    printf("%d %d %d\n", smallest, largest, MIN(a + 1, b - 1) * 2);
    printf("%d %d\n", MAX(MAX(a, b), 4), TWICE(MIN(a, b)));
    long wide = MIN(5000000000L, 7000000000L);
    unsigned char small = 200;
    printf("%ld %d %d\n", wide, MAX(small, 3), TWICE(small));
    double half = MIN(0.5, 1.5);
    printf("%.2f %d %d\n", half, BIT(a), (int)TEXT_LENGTH("four"));
}

static void argument_effects(void) {
    int i = 1;
    int twice = TWICE(note(i));
    printf("%d %d\n", twice, i);
    int j = 2;
    int picked = MAX(j++, 1);
    printf("%d %d\n", picked, j);
    int calls_before = calls;
    int low = MIN(counted_call(), 1000);
    printf("%d %d\n", low, calls - calls_before);
    int k = 0;
    int nested = MIN(MAX(k++, 1), 5);
    printf("%d %d %d\n", nested, k, TWICE(k));
    int total = note(1) + TWICE(note(2)) + note(3);
    printf("%d\n", total);
    show_trace("order");
}

static void hoisted_bodies(void) {
    int r = 100;
    r += NEXT();
    printf("%d %d\n", r, (int)(cursor - table));
    int *local = table;
    int sum = NEXT_FROM(local);
    sum = sum * 2 + NEXT_FROM(local);
    printf("%d %d\n", sum, (int)(local - table));
    int count = 0;
    int value = BUMP_AND(count, count + 10);
    printf("%d %d\n", value, count);
    int target = 0;
    int stored = ASSIGN_TO(&target, 42) + 1;
    printf("%d %d\n", stored, target);
    printf("%d\n", STMT_EXPR(r));
    printf("%d %d\n", COUNT_CALL(), COUNT_CALL());
}

static void short_circuit(void) {
    int hit = 0;
    int zero = 0, one = 1;
    printf("%d\n", BOTH(zero, hit++));
    printf("%d\n", hit);
    printf("%d\n", EITHER(one, hit++));
    printf("%d\n", hit);
    printf("%d\n", zero && MIN(hit++, 5));
    printf("%d\n", hit);
    printf("%d\n", one || TWICE(note(4)));
    printf("%d\n", one && TWICE(note(5)));
    printf("%d\n", hit);
    printf("%d\n", BOTH(note(1), note(0)) + EITHER(note(0), note(2)));
    show_trace("logic");
}

static void conditionals(void) {
    int hit = 0;
    int taken = PICK(hit, hit++, hit + 5);
    printf("%d %d\n", taken, hit);
    int chosen = hit ? MIN(hit++, 9) : MAX(hit, 9);
    printf("%d %d\n", chosen, hit);
    int inner = PICK(TWICE(hit), note(7), note(8));
    printf("%d\n", inner);
    show_trace("pick");
    one_more:
    hit ? NOTHING(hit) : NOTHING(hit + 1);
    if (hit < 2) {
        hit++;
        goto one_more;
    }
    printf("%d %d\n", MIN(hit, 2), MAX(hit, 2));
}

static void structures(void) {
    struct holder pair = {3, 4};
    struct holder *pointer = &pair;
    struct holder many[3] = {{1, 0}, {2, 0}, {2, 0}};
    int depth[3] = {4, 5, 3};
    int (*function)(int) = double_it;
    printf("%d %d\n", FIELD_OF(pointer), AT(many, 1));
    printf("%d %d\n", SMALLER(many, 0, 1, depth), SMALLER(many, 1, 2, depth));
    printf("%d\n", CALL_WITH(function, 21));
    struct holder made = HOLDER(8);
    printf("%d %d\n", made.count, made.other);
    struct holder *kept = ADDRESS_OF_LITERAL(6);
    printf("%d\n", kept->count);
    FIELD_OF(pointer) = 30;
    AT(many, 2) += 1;
    printf("%d %d\n", pair.count, many[2].count);
}

static void positions(int selector) {
    int hidden = 2;
    int size = 0;
    while (MIN(size, 3) < 3) {
        size++;
    }
    for (int i = MAX(0, -1); i < MIN(2, size); i = i + BIT(0)) {
        printf("loop %d\n", i);
    }
    do {
        size--;
    } while (MAX(size, 0) > 1);
    static int kept_static = BIT(2);
    int sized[BIT(2)] = {MIN(1, 2), 2, 3, MAX(3, 4)};
    printf("%d %d %d %d\n", size, kept_static, sized[3], (int)sizeof(MIN(1L, 2L)));
    printf("%d\n", LOCAL_READ(1));
    switch (selector) {
    case BIT(1):
        printf("two\n");
        break;
    case MAX(BIT(1), 5):
        printf("five\n");
        break;
    default:
        printf("%d\n", MIN(selector, 100));
        break;
    }
    if (MAX(selector, 0)) {
        printf("positive\n");
    }
    int shifted = table[MIN(selector, 4)];
    printf("%d %d %d\n", shifted, global_size, (int)WIDE_MIN(100UL, global_size));
}

static int with_jumps(int limit) {
    int total = 0;
    int i = 0;
again:
    if (i >= MIN(limit, 4)) {
        goto done;
    }
    total += TWICE(i);
    total += NEXT_FROM(cursor) > MAX(i, 100) ? 1 : 2;
    i++;
    cursor = table;
    goto again;
done:
    return total;
}

int main(void) {
    plain_values();
    argument_effects();
    hoisted_bodies();
    short_circuit();
    conditionals();
    structures();
    positions(2);
    positions(5);
    positions(9);
    printf("%d\n", with_jumps(3));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_values.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_values.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_values: 51 items, 0 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:double_it: 2 variants
// CHECK-NEXT:   spliced fn:show_trace: 2 variants
// CHECK-NEXT:   spliced fn:plain_values: 2 variants
// CHECK-NEXT:   spliced fn:argument_effects: 2 variants
// CHECK-NEXT:   spliced fn:hoisted_bodies: 2 variants
// CHECK-NEXT:   spliced fn:short_circuit: 2 variants
// CHECK-NEXT:   spliced fn:conditionals: 2 variants
// CHECK-NEXT:   spliced fn:structures: 2 variants
// CHECK-NEXT:   spliced fn:positions: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 23 function-like, 19 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline no_statement_region: 16
// CHECK-NEXT:   inline argument_reads_macro_local: 14
// CHECK-NEXT:   inline inside_argument: 8
// CHECK-NEXT:   inline compound_literal_or_temporary: 4
// CHECK-NEXT:   inline local_in_dispatch_function: 4
// CHECK-NEXT:   inline void_value: 4
// CHECK-NEXT:   macro ADDRESS_OF_LITERAL: 0 lifted, inline: compound_literal_or_temporary 2
// CHECK-NEXT:   macro ASSIGN_TO: 2 lifted, inline:
// CHECK-NEXT:   macro AT: 18 lifted, inline: no_statement_region 2
// CHECK-NEXT:   macro BIT: 4 lifted, inline: inside_argument 2, no_statement_region 8
// CHECK-NEXT:   macro BOTH: 2 lifted, inline: argument_reads_macro_local 2
// CHECK-NEXT:   macro BUMP_AND: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_WITH: 2 lifted, inline:
// CHECK-NEXT:   macro COUNT_CALL: 4 lifted, inline:
// CHECK-NEXT:   macro EITHER: 2 lifted, inline: argument_reads_macro_local 2
// CHECK-NEXT:   macro FIELD_OF: 2 lifted, inline: no_statement_region 2
// CHECK-NEXT:   macro HOLDER: 0 lifted, inline: compound_literal_or_temporary 2
// CHECK-NEXT:   macro LOCAL_READ: 2 lifted, inline:
// CHECK-NEXT:   macro MAX: 20 lifted, inline: argument_reads_macro_local 2, inside_argument 4, no_statement_region 2
// CHECK-NEXT:   macro MIN: 24 lifted, inline: argument_reads_macro_local 6, inside_argument 2, no_statement_region 2
// CHECK-NEXT:   macro NEXT: 2 lifted, inline:
// CHECK-NEXT:   macro NEXT_FROM: 4 lifted, inline: local_in_dispatch_function 2
// CHECK-NEXT:   macro NOTHING: 0 lifted, inline: void_value 4
// CHECK-NEXT:   macro PICK: 0 lifted, inline: argument_reads_macro_local 2, local_in_dispatch_function 2
// CHECK-NEXT:   macro SMALLER: 4 lifted, inline:
// CHECK-NEXT:   macro STMT_EXPR: 2 lifted, inline:
// CHECK-NEXT:   macro TEXT_LENGTH: 2 lifted, inline:
// CHECK-NEXT:   macro TWICE: 18 lifted, inline:
// CHECK-NEXT:   macro WIDE_MIN: 2 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ASSIGN_TO {
// CHECK-NEXT:     ($p:expr, $v:expr) => {{[{][{]}}
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *$p = $v;
// CHECK-NEXT:         }
// CHECK-NEXT:         $v
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! AT {
// CHECK-NEXT:     ($t:expr, $i:expr) => {
// CHECK-NEXT:         $t[($i) as usize].count
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BIT {
// CHECK-NEXT:     ($n:expr) => {
// CHECK-NEXT:         1i32 << $n
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BOTH {
// CHECK-NEXT:     ($a:expr, $b:expr) => {{[{][{]}}
// CHECK-NEXT:         let __v: bool = ($a) != 0 && ($b) != 0;
// CHECK-NEXT:         __v
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BUMP_AND {
// CHECK-NEXT:     ($x:expr, $y:expr) => {{[{][{]}}
// CHECK-NEXT:         let __v: i32 = $x;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = __v + 1;
// CHECK-NEXT:         $x = [[V_0]];
// CHECK-NEXT:         $y
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CALL_WITH {
// CHECK-NEXT:     ($f:expr, $x:expr) => {
// CHECK-NEXT:         unsafe { $f.unwrap()($x) }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! COUNT_CALL {
// CHECK-NEXT:     () => {
// CHECK-NEXT:         counted_call()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! EITHER {
// CHECK-NEXT:     ($a:expr, $b:expr) => {{[{][{]}}
// CHECK-NEXT:         let __v: bool = ($a) != 0 || ($b) != 0;
// CHECK-NEXT:         __v
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FIELD_OF {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         unsafe { (*$s).count }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! LOCAL_READ {
// CHECK-NEXT:     ($x:expr, $hidden:expr) => {
// CHECK-NEXT:         ($x) + $hidden
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MAX {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         if ($a) > $b { $a } else { $b }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MAX__2 {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         if ((($a) as u32) as i32) > $b {
// CHECK-NEXT:             (($a) as u32) as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             $b
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MIN__2 {
// CHECK-NEXT:     ($a:expr, $b:expr) => {{[{][{]}}
// CHECK-NEXT:         let __v: i32 = if ($a) < $b { $a } else { $b };
// CHECK-NEXT:         __v
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MIN {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         if ($a) < $b { $a } else { $b }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NEXT_FROM {
// CHECK-NEXT:     ($p:expr) => {{[{][{]}}
// CHECK-NEXT:         let __v: *mut i32 = $p;
// CHECK-NEXT:         let [[V_0]]: *mut i32 = unsafe { __v.add(1) };
// CHECK-NEXT:         $p = [[V_0]];
// CHECK-NEXT:         unsafe { *__v }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NEXT {
// CHECK-NEXT:     () => {{[{][{]}}
// CHECK-NEXT:         let __v: *mut i32 = unsafe { cursor };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cursor = __v.add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { *__v }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SMALLER {
// CHECK-NEXT:     ($t:expr, $n:expr, $m:expr, $d:expr) => {
// CHECK-NEXT:         AT!($t, $n) < AT!($t, $m)
// CHECK-NEXT:             || AT!($t, $n) == AT!($t, $m) && $d[($n) as usize] <= $d[($m) as usize]
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! STMT_EXPR {
// CHECK-NEXT:     ($x:expr) => {{[{][{]}}
// CHECK-NEXT:         let mut __v: i32;
// CHECK-NEXT:         let [[V_0]]: i32 = ($x) * 2;
// CHECK-NEXT:         __v = [[V_0]] + 1;
// CHECK-NEXT:         __v
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TEXT_LENGTH {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         unsafe { strlen(($s) as *const c_char) }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TWICE {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         ($x) + $x
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TWICE__2 {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         ((($x) as u32) as i32) + ((($x) as u32) as i32)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WIDE_MIN {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         if ($a) > (($b) as c_ulong) {
// CHECK-NEXT:             ($b) as c_ulong
// CHECK-NEXT:         } else {
// CHECK-NEXT:             $a
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strlen([[ARG_0:arg[0-9]+]]: *const c_char) -> size_t;
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holder {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     other: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut table: __SlateAlignLargeArray<[i32; 5]> =
// CHECK-NEXT:     __SlateAlignLargeArray([5i32, 7i32, 11i32, 13i32, 17i32]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut cursor: *mut i32 = unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut trace: __SlateAlignLargeArray<[i32; 16]> =
// CHECK-NEXT:     __SlateAlignLargeArray(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut trace_length: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut global_size: i32 = 8;
// CHECK-EMPTY:
// CHECK-NEXT: fn counted_call() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { calls }) * 10
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn note(value: i32) -> i32 {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = unsafe { trace_length };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         trace_length = [[V_1]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         trace.0[[[V_1]] as usize] = value;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg_attr(
// CHECK-NEXT:     unix,
// CHECK-NEXT:     unsafe(link_section = ".text.slate_distinct.macro_values.double_it")
// CHECK-NEXT: )]
// CHECK-NEXT: extern "C-unwind" fn double_it(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn show_trace(label: *const c_char) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%s:".as_ptr(), label) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%s:".as_ptr(), label) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < unsafe { trace_length } {
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             print!(" {}", unsafe { trace.0[i as usize] });
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { slate_support::printf(c" %d".as_ptr(), trace.0[i as usize]) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"\n".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("");
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         trace_length = 0;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn plain_values() {
// CHECK-NEXT:     let a: i32 = 3;
// CHECK-NEXT:     let b: i32 = 9;
// CHECK-NEXT:     let smallest: i32 = MIN!(a, b);
// CHECK-NEXT:     let largest: i32 = MAX!(a, b);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 smallest,
// CHECK-NEXT:                 largest,
// CHECK-NEXT:                 MIN!(a + 1, b - 1) * 2,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 MAX!(if a > b { a } else { b }, 4i32),
// CHECK-NEXT:                 TWICE!(if a < b { a } else { b }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let wide: c_long = MIN!(5000000000i64, 7000000000i64) as c_long;
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {} {}", smallest, largest, MIN!(a + 1, b - 1) * 2);
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             MAX!(if a > b { a } else { b }, 4i32),
// CHECK-NEXT:             TWICE!(if a < b { a } else { b })
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     let wide: c_long = MIN!((5000000000 as c_long), (7000000000 as c_long));
// CHECK-NEXT:     let small: u8 = 200u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%ld %d %d\n".as_ptr(),
// CHECK-NEXT:                 wide,
// CHECK-NEXT:                 MAX__2!(small, 3i32),
// CHECK-NEXT:                 TWICE__2!(small),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%ld %d %d\n".as_ptr(),
// CHECK-NEXT:                 wide,
// CHECK-NEXT:                 MAX__2!(small, 3i32),
// CHECK-NEXT:                 TWICE__2!(small),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let half: f64 = MIN!(0.5f64, 1.5f64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%.2f %d %d\n".as_ptr(),
// CHECK-NEXT:                 half,
// CHECK-NEXT:                 BIT!(a),
// CHECK-NEXT:                 (TEXT_LENGTH!(c"four".as_ptr() as *mut c_char) as u32) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%.2f %d %d\n".as_ptr(),
// CHECK-NEXT:                 half,
// CHECK-NEXT:                 BIT!(a),
// CHECK-NEXT:                 (TEXT_LENGTH!(c"four".as_ptr() as *mut c_char) as u32) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn argument_effects() {
// CHECK-NEXT:     let i: i32 = 1;
// CHECK-NEXT:     let twice: i32 = TWICE!(note(i));
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), twice, i) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", twice, i);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut j: i32 = 2;
// CHECK-NEXT:     let mut picked: i32 = 0;
// CHECK-NEXT:     let [[V_1]]: i32 = j;
// CHECK-NEXT:     j = [[V_1]] + 1;
// CHECK-NEXT:     let [[V_0]]: i32 = if [[V_1]] > 1 {
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         j = [[V_2]] + 1;
// CHECK-NEXT:         [[V_2]]
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1
// CHECK-NEXT:     };
// CHECK-NEXT:     picked = [[V_0]];
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), picked, j) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", picked, j);
// CHECK-NEXT:     }
// CHECK-NEXT:     let calls_before: i32 = unsafe { calls };
// CHECK-NEXT:     let low: i32 = MIN__2!(counted_call(), 1000i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), low, calls - calls_before) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", low, (unsafe { calls }) - calls_before);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut k: i32 = 0;
// CHECK-NEXT:     let mut nested: i32 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:     k = [[V_3]] + 1;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = if [[V_3]] > 1 {
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         k = [[V_5]] + 1;
// CHECK-NEXT:         [[V_5]]
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = if [[V_4]] < 5 {
// CHECK-NEXT:         let [[V_7:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         k = [[V_7]] + 1;
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: i32 = if [[V_7]] > 1 {
// CHECK-NEXT:             let [[V_9:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:             k = [[V_9]] + 1;
// CHECK-NEXT:             [[V_9]]
// CHECK-NEXT:         } else {
// CHECK-NEXT:             1
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_8]]
// CHECK-NEXT:     } else {
// CHECK-NEXT:         5
// CHECK-NEXT:     };
// CHECK-NEXT:     nested = [[V_6]];
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d %d\n".as_ptr(), nested, k, TWICE!(k)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {} {}", nested, k, TWICE!(k));
// CHECK-NEXT:     }
// CHECK-NEXT:     let total: i32 = note(1i32) + TWICE!(note(2i32)) + note(3i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), total) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", total);
// CHECK-NEXT:     }
// CHECK-NEXT:     show_trace(c"order".as_ptr());
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn hoisted_bodies() {
// CHECK-NEXT:     let mut r: i32 = 100;
// CHECK-NEXT:     let [[V_1]]: i32 = NEXT!();
// CHECK-NEXT:     r += [[V_1]];
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 r,
// CHECK-NEXT:                 (unsafe { cursor }
// CHECK-NEXT:                     .offset_from((std::ptr::addr_of_mut!(table.0) as *mut i32) as *mut i32)
// CHECK-NEXT:                     as i64) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             r,
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 unsafe { cursor }
// CHECK-NEXT:                     .offset_from((std::ptr::addr_of_mut!(table.0) as *mut i32) as *mut i32)
// CHECK-NEXT:             }) as c_long) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut local: *mut i32 = unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 };
// CHECK-NEXT:     let mut sum: i32 = NEXT_FROM!(local);
// CHECK-NEXT:     sum = sum * 2 + NEXT_FROM!(local);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 sum,
// CHECK-NEXT:                 (local.offset_from((std::ptr::addr_of_mut!(table.0) as *mut i32) as *mut i32)
// CHECK-NEXT:                     as i64) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             sum,
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 local.offset_from((std::ptr::addr_of_mut!(table.0) as *mut i32) as *mut i32)
// CHECK-NEXT:             }) as c_long) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     let value: i32 = BUMP_AND!(count, count + 10);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), value, count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", value, count);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut target: i32 = 0;
// CHECK-NEXT:     let stored: i32 = ASSIGN_TO!(std::ptr::addr_of_mut!(target), 42i32) + 1;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), stored, target) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), STMT_EXPR!(r)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), COUNT_CALL!(), COUNT_CALL!()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", stored, target);
// CHECK-NEXT:         println!("{}", STMT_EXPR!(r));
// CHECK-NEXT:         println!("{} {}", COUNT_CALL!(), COUNT_CALL!());
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn short_circuit() {
// CHECK-NEXT:     let mut hit: i32 = 0;
// CHECK-NEXT:     let zero: i32 = 0;
// CHECK-NEXT:     let one: i32 = 1;
// CHECK-NEXT:     let [[V_1]]: bool = if zero != 0 {
// CHECK-NEXT:         let [[V_10:__v[0-9]+]]: i32 = hit;
// CHECK-NEXT:         hit = [[V_10]] + 1;
// CHECK-NEXT:         [[V_10]] != 0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         false
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), [[V_1]] as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), hit) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", [[V_1]] as i32);
// CHECK-NEXT:         println!("{}", hit);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_2]]: bool = if one != 0 {
// CHECK-NEXT:         true
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_11:__v[0-9]+]]: i32 = hit;
// CHECK-NEXT:         hit = [[V_11]] + 1;
// CHECK-NEXT:         [[V_11]] != 0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), [[V_2]] as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), hit) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", [[V_2]] as i32);
// CHECK-NEXT:         println!("{}", hit);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_3]]: bool = if zero != 0 {
// CHECK-NEXT:         let [[V_12:__v[0-9]+]]: i32 = hit;
// CHECK-NEXT:         hit = [[V_12]] + 1;
// CHECK-NEXT:         let [[V_5]]: i32 = if [[V_12]] < 5 {
// CHECK-NEXT:             let [[V_13:__v[0-9]+]]: i32 = hit;
// CHECK-NEXT:             hit = [[V_13]] + 1;
// CHECK-NEXT:             [[V_13]]
// CHECK-NEXT:         } else {
// CHECK-NEXT:             5
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_5]] != 0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         false
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), [[V_3]] as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), hit) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 (one != 0 || TWICE!(note(4i32)) != 0) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 (one != 0 && TWICE!(note(5i32)) != 0) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), hit) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 (BOTH!(note(1i32), note(0i32)) as i32) + (EITHER!(note(0i32), note(2i32)) as i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", [[V_3]] as i32);
// CHECK-NEXT:         println!("{}", hit);
// CHECK-NEXT:         println!("{}", (one != 0 || TWICE!(note(4i32)) != 0) as i32);
// CHECK-NEXT:         println!("{}", (one != 0 && TWICE!(note(5i32)) != 0) as i32);
// CHECK-NEXT:         println!("{}", hit);
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{}",
// CHECK-NEXT:             (BOTH!(note(1i32), note(0i32)) as i32) + (EITHER!(note(0i32), note(2i32)) as i32)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     show_trace(c"logic".as_ptr());
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn conditionals() {
// CHECK-NEXT:     let mut [[V_0]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_3]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_5]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut hit: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hit = 0;
// CHECK-NEXT:         if hit != 0 {
// CHECK-NEXT:             let [[V_7]]: i32 = hit;
// CHECK-NEXT:             hit = [[V_7]] + 1;
// CHECK-NEXT:             [[V_14]] = [[V_7]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_14]] = hit + 5;
// CHECK-NEXT:         }
// CHECK-NEXT:         let taken: i32 = [[V_14]];
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             slate_support::printf(c"%d %d\n".as_ptr(), taken, hit);
// CHECK-NEXT:             fflush(std::ptr::null_mut());
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("{} {}", taken, hit);
// CHECK-NEXT:         }
// CHECK-NEXT:         if hit != 0 {
// CHECK-NEXT:             let [[V_4]]: i32 = hit;
// CHECK-NEXT:             hit = [[V_4]] + 1;
// CHECK-NEXT:             if [[V_4]] < 9 {
// CHECK-NEXT:                 let [[V_15:__v[0-9]+]]: i32 = hit;
// CHECK-NEXT:                 hit = [[V_15]] + 1;
// CHECK-NEXT:                 [[V_3]] = [[V_15]];
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 [[V_3]] = 9;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[V_5]] = [[V_3]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_5]] = MAX!(hit, 9i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         let chosen: i32 = [[V_5]];
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             slate_support::printf(c"%d %d\n".as_ptr(), chosen, hit);
// CHECK-NEXT:             fflush(std::ptr::null_mut());
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("{} {}", chosen, hit);
// CHECK-NEXT:         }
// CHECK-NEXT:         if TWICE!(hit) != 0 {
// CHECK-NEXT:             [[V_0]] = note(7i32);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_0]] = note(8i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         let inner: i32 = [[V_0]];
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             slate_support::printf(c"%d\n".as_ptr(), inner);
// CHECK-NEXT:             fflush(std::ptr::null_mut());
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("{}", inner);
// CHECK-NEXT:         }
// CHECK-NEXT:         show_trace(c"pick".as_ptr());
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if hit == 0 {
// CHECK-NEXT:                 hit + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             if hit < 2 {
// CHECK-NEXT:                 hit += 1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             slate_support::printf(c"%d %d\n".as_ptr(), MIN!(hit, 2i32), MAX!(hit, 2i32));
// CHECK-NEXT:             fflush(std::ptr::null_mut());
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("{} {}", MIN!(hit, 2i32), MAX!(hit, 2i32));
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn structures() {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: holder = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: holder = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pair: holder = holder { count: 3, other: 4 };
// CHECK-NEXT:     let pointer: *mut holder = std::ptr::addr_of_mut!(pair);
// CHECK-NEXT:     let mut many: __SlateAlignLargeArray<[holder; 3]> = __SlateAlignLargeArray([
// CHECK-NEXT:         holder { count: 1, other: 0 },
// CHECK-NEXT:         holder { count: 2, other: 0 },
// CHECK-NEXT:         holder { count: 2, other: 0 },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let depth: [i32; 3] = [4, 5, 3];
// CHECK-NEXT:     let function: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(double_it);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"%d %d\n".as_ptr(), FIELD_OF!(pointer), AT!(many.0, 1i32))
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 SMALLER!(many.0, 0i32, 1i32, depth) as i32,
// CHECK-NEXT:                 SMALLER!(many.0, 1i32, 2i32, depth) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), CALL_WITH!(function, 21i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", FIELD_OF!(pointer), AT!(many.0, 1i32));
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             SMALLER!(many.0, 0i32, 1i32, depth) as i32,
// CHECK-NEXT:             SMALLER!(many.0, 1i32, 2i32, depth) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("{}", CALL_WITH!(function, 21i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     let made: holder = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = holder {
// CHECK-NEXT:                 count: 8,
// CHECK-NEXT:                 other: 8 + 1,
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), made.count, made.other) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", made.count, made.other);
// CHECK-NEXT:     }
// CHECK-NEXT:     let kept: *mut holder = unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(*{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = holder { count: 6, other: 0 };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), (*kept).count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", unsafe { (*kept).count });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*pointer).count = 30;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0]]: *mut holder = unsafe { (many.0.as_mut_ptr() as *mut holder).add(2) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_0]]).count = (*[[V_0]]).count + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), pair.count, many.0[2].count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", pair.count, many.0[2].count);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn positions(selector: i32) {
// CHECK-NEXT:     let hidden: i32 = 2;
// CHECK-NEXT:     let mut size: i32 = 0;
// CHECK-NEXT:     while MIN!(size, 3i32) < 3 {
// CHECK-NEXT:         size += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = MAX!(0i32, -1i32);
// CHECK-NEXT:     while i < MIN!(2i32, size) {
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { slate_support::printf(c"loop %d\n".as_ptr(), i) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("loop {}", i);
// CHECK-NEXT:         }
// CHECK-NEXT:         i += BIT!(0i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         size -= 1;
// CHECK-NEXT:         if MAX!(size, 0i32) <= 1 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let sized: __SlateAlignLargeArray<[i32; 4]> =
// CHECK-NEXT:         __SlateAlignLargeArray([MIN!(1i32, 2i32), 2i32, 3i32, MAX!(3i32, 4i32)]);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 size,
// CHECK-NEXT:                 kept_static,
// CHECK-NEXT:                 sized.0[3],
// CHECK-NEXT:                 4i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), LOCAL_READ!(1i32, hidden)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {}",
// CHECK-NEXT:             size,
// CHECK-NEXT:             unsafe { kept_static },
// CHECK-NEXT:             sized.0[3],
// CHECK-NEXT:             8i32
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("{}", LOCAL_READ!(1i32, hidden));
// CHECK-NEXT:     }
// CHECK-NEXT:     match selector {
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { slate_support::printf(c"two\n".as_ptr()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 println!("two");
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         5 => {
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { slate_support::printf(c"five\n".as_ptr()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 println!("five");
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { slate_support::printf(c"%d\n".as_ptr(), MIN!(selector, 100i32)) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 println!("{}", MIN!(selector, 100i32));
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if MAX!(selector, 0i32) != 0 {
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { slate_support::printf(c"positive\n".as_ptr()) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("positive");
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let shifted: i32 = unsafe { table.0[MIN!(selector, 4i32) as usize] };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 shifted,
// CHECK-NEXT:                 global_size,
// CHECK-NEXT:                 WIDE_MIN!((100 as c_ulong), unsafe { global_size }) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             shifted,
// CHECK-NEXT:             unsafe { global_size },
// CHECK-NEXT:             (WIDE_MIN!((100 as c_ulong), unsafe { global_size }) as u32) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut kept_static: i32 = 4;
// CHECK-EMPTY:
// CHECK-NEXT: fn with_jumps(limit: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if i >= MIN!(limit, 4i32) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let [[V_4]]: i32 = total;
// CHECK-NEXT:                 total = [[V_4]] + TWICE!(i);
// CHECK-NEXT:                 let [[V_3]]: *mut i32 = cursor;
// CHECK-NEXT:                 cursor = [[V_3]].add(1);
// CHECK-NEXT:                 total += if *[[V_3]] > MAX!(i, 100i32) { 1 } else { 2 };
// CHECK-NEXT:                 i += 1;
// CHECK-NEXT:                 cursor = std::ptr::addr_of_mut!(table.0) as *mut i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return total;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     plain_values();
// CHECK-NEXT:     argument_effects();
// CHECK-NEXT:     hoisted_bodies();
// CHECK-NEXT:     short_circuit();
// CHECK-NEXT:     conditionals();
// CHECK-NEXT:     structures();
// CHECK-NEXT:     positions(2i32);
// CHECK-NEXT:     positions(5i32);
// CHECK-NEXT:     positions(9i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), with_jumps(3i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", with_jumps(3i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
