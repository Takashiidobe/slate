/* { dg-options "/Zc:preprocessor" } */
#if defined(_WIN64)
typedef unsigned long long size_t;
#elif defined(_WIN32)
typedef unsigned int size_t;
#else
typedef unsigned long size_t;
#endif
#define NULL ((void *)0)
void abort(void);
size_t strlen(const char *);
int memcmp(const void *, const void *, size_t);
int strcmp(const char *, const char *);

#define TYPECHECK(to_check, to_return) (sizeof(to_check) ? (to_return) : (to_return))
#define PTRLEN_DECL_LITERAL(s) {TYPECHECK("" s "", s), sizeof(s) - 1}

typedef struct ptrlen {
    const void *ptr;
    size_t      len;
} ptrlen;

typedef enum { TOK_LPAR, TOK_RPAR, TOK_AND, TOK_OR, TOK_NOT } Token;

static const struct op {
    ptrlen text;
    Token  type;
} operators[] = {
    {PTRLEN_DECL_LITERAL("("), TOK_LPAR},
    {PTRLEN_DECL_LITERAL(")"), TOK_RPAR},
    {PTRLEN_DECL_LITERAL("&&"), TOK_AND},
    {PTRLEN_DECL_LITERAL("||"), TOK_OR},
    {PTRLEN_DECL_LITERAL("!"), TOK_NOT},
};

#define NAMES(X) X("ping") X("query") X("session-bind@openssh.com")
#define DEF_NAME(name) PTRLEN_DECL_LITERAL(name),
static const ptrlen names[] = {NAMES(DEF_NAME)};

static int hash_a(int x) { return x * 3; }
static int hash_b(int x) { return x + 7; }

struct hash_vt {
    int (*fn)(int);
    const char *desc;
};

static const struct hash_vt vt_a = {hash_a, "a"};
static const struct hash_vt vt_b = {hash_b, "b"};

static const struct sign_extra {
    const struct hash_vt *hash;
    int (*fallback)(int);
    const char *alg;
    ptrlen      prefix;
} extra = {sizeof(long) == 4 ? &vt_a : &vt_b, 1 ? hash_b : hash_a, "Ed25519", PTRLEN_DECL_LITERAL("")};

static char buffer_a[8] = "first", buffer_b[8] = "second";
static char *const pick = sizeof(void *) == 8 ? buffer_a : buffer_b + 1;
static const Token *const picks[] = {0 ? NULL : &operators[3].type, sizeof(int) == 4 ? &operators[1].type : NULL};

#define BIT(y, value) | (value >= y && value < y + 32 ? 1UL << (value >= y && value < y + 32 ? (value - y) : 0) : 0)
#define TYPES(X, y) X(y, 1) X(y, 2) X(y, 20) X(y, 31) X(y, 32) X(y, 50) X(y, 94) X(y, 100) X(y, 255)
#define WORD(y) (0 TYPES(BIT, (32 * y)))

static const unsigned valid_bitmap[] = {WORD(0), WORD(1), WORD(2), WORD(3), WORD(4), WORD(5), WORD(6), WORD(7)};

#define CHECK(c) \
    do {           \
        if (!(c))  \
            abort(); \
    } while (0)

static int same(ptrlen p, const char *s) { return p.len == strlen(s) && memcmp(p.ptr, s, p.len) == 0; }

int main(void) {
    static const char *const texts[] = {"(", ")", "&&", "||", "!"};
    for (size_t i = 0; i < sizeof operators / sizeof operators[0]; i++) {
        CHECK(operators[i].type == (Token)i);
        CHECK(same(operators[i].text, texts[i]));
    }
    CHECK(sizeof names / sizeof names[0] == 3);
    CHECK(same(names[0], "ping") && same(names[1], "query") && same(names[2], "session-bind@openssh.com"));
    CHECK(extra.hash == &vt_a && extra.hash->fn(5) == 15 && extra.fallback(5) == 12);
    CHECK(strcmp(extra.alg, "Ed25519") == 0 && same(extra.prefix, ""));
    CHECK(pick == (sizeof(void *) == 8 ? buffer_a : buffer_b + 1));
    pick[0] = 'F';
    CHECK(strcmp(buffer_a, sizeof(void *) == 8 ? "First" : "first") == 0);
    CHECK(strcmp(buffer_b, sizeof(void *) == 8 ? "second" : "sFcond") == 0);
    CHECK(*picks[0] == TOK_OR && *picks[1] == TOK_RPAR);
    static const unsigned expected[] = {0x80100006u, 0x00040001u, 0x40000000u, 0x00000010u, 0, 0, 0, 0x80000000u};
    for (size_t i = 0; i < sizeof valid_bitmap / sizeof valid_bitmap[0]; i++)
        CHECK(valid_bitmap[i] == expected[i]);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CHECK__2 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if ($c) == 0 {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CHECK {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if !$c {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn strlen([[ARG_0:arg[0-9]+]]: *const i8) -> u64;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn memcmp([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u64) -> i32;
// I686-PC-WINDOWS-MSVC-NEXT:     fn strlen([[ARG_0:arg[0-9]+]]: *const i8) -> u32;
// I686-PC-WINDOWS-MSVC-NEXT:     fn memcmp([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u32) -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn strlen([[ARG_0:arg[0-9]+]]: *const i8) -> u64;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn memcmp([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn strcmp([[ARG_0]]: *const i8, [[ARG_1]]: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ptrlen {
// CHECK-NEXT:     ptr: *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:     len: u64,
// I686-PC-WINDOWS-MSVC-NEXT:     len: u32,
// AARCH64-PC-WINDOWS-MSVC-NEXT:     len: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct op {
// CHECK-NEXT:     text: ptrlen,
// CHECK-NEXT:     r#type: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut operators: __SlateAlign16<[op; 5]> = __SlateAlign16([
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_0:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_0]].text = ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT: static mut operators: [op; 5] = [
// I686-PC-WINDOWS-MSVC-NEXT:     op {
// I686-PC-WINDOWS-MSVC-NEXT:         text: ptrlen {
// I686-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT: static mut operators: [op; 5] = [
// AARCH64-PC-WINDOWS-MSVC-NEXT:     {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_0:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_0]].text = ptrlen {
// AARCH64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u64) != 0u64 {
// CHECK-NEXT:                 b"(\0".as_ptr() as *mut i8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 b"(\0".as_ptr() as *mut i8
// CHECK-NEXT:             }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u64).wrapping_sub(1u64),
// X86_64-PC-WINDOWS-MSVC-NEXT:         };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_0]].r#type = 0u32;
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_0]]
// I686-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u32).wrapping_sub(1u32),
// I686-PC-WINDOWS-MSVC-NEXT:         },
// I686-PC-WINDOWS-MSVC-NEXT:         r#type: 0u32,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u64).wrapping_sub(1u64),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_0]].r#type = 0u32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_1:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_1]].text = ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:     op {
// I686-PC-WINDOWS-MSVC-NEXT:         text: ptrlen {
// I686-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_1:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_1]].text = ptrlen {
// AARCH64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u64) != 0u64 {
// CHECK-NEXT:                 b")\0".as_ptr() as *mut i8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 b")\0".as_ptr() as *mut i8
// CHECK-NEXT:             }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u64).wrapping_sub(1u64),
// X86_64-PC-WINDOWS-MSVC-NEXT:         };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_1]].r#type = 1u32;
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_1]]
// I686-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u32).wrapping_sub(1u32),
// I686-PC-WINDOWS-MSVC-NEXT:         },
// I686-PC-WINDOWS-MSVC-NEXT:         r#type: 1u32,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u64).wrapping_sub(1u64),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_1]].r#type = 1u32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_1]]
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_2:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_2]].text = ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 3]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:     op {
// I686-PC-WINDOWS-MSVC-NEXT:         text: ptrlen {
// I686-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 3]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_2:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_2]].text = ptrlen {
// AARCH64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 3]>() as u64) != 0u64 {
// CHECK-NEXT:                 b"&&\0".as_ptr() as *mut i8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 b"&&\0".as_ptr() as *mut i8
// CHECK-NEXT:             }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 3]>() as u64).wrapping_sub(1u64),
// X86_64-PC-WINDOWS-MSVC-NEXT:         };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_2]].r#type = 2u32;
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_2]]
// I686-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 3]>() as u32).wrapping_sub(1u32),
// I686-PC-WINDOWS-MSVC-NEXT:         },
// I686-PC-WINDOWS-MSVC-NEXT:         r#type: 2u32,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 3]>() as u64).wrapping_sub(1u64),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_2]].r#type = 2u32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_2]]
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_3:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_3]].text = ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 3]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:     op {
// I686-PC-WINDOWS-MSVC-NEXT:         text: ptrlen {
// I686-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 3]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_3:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_3]].text = ptrlen {
// AARCH64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 3]>() as u64) != 0u64 {
// CHECK-NEXT:                 b"||\0".as_ptr() as *mut i8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 b"||\0".as_ptr() as *mut i8
// CHECK-NEXT:             }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 3]>() as u64).wrapping_sub(1u64),
// X86_64-PC-WINDOWS-MSVC-NEXT:         };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_3]].r#type = 3u32;
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_3]]
// I686-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 3]>() as u32).wrapping_sub(1u32),
// I686-PC-WINDOWS-MSVC-NEXT:         },
// I686-PC-WINDOWS-MSVC-NEXT:         r#type: 3u32,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 3]>() as u64).wrapping_sub(1u64),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_3]].r#type = 3u32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_3]]
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_4:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_4]].text = ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:     op {
// I686-PC-WINDOWS-MSVC-NEXT:         text: ptrlen {
// I686-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let mut [[T_4:__t[0-9]+]]: op = unsafe { std::mem::zeroed() };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_4]].text = ptrlen {
// AARCH64-PC-WINDOWS-MSVC-NEXT:             ptr: (if (std::mem::size_of::<[i8; 2]>() as u64) != 0u64 {
// CHECK-NEXT:                 b"!\0".as_ptr() as *mut i8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 b"!\0".as_ptr() as *mut i8
// CHECK-NEXT:             }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u64).wrapping_sub(1u64),
// X86_64-PC-WINDOWS-MSVC-NEXT:         };
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_4]].r#type = 4u32;
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_4]]
// I686-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u32).wrapping_sub(1u32),
// I686-PC-WINDOWS-MSVC-NEXT:         },
// I686-PC-WINDOWS-MSVC-NEXT:         r#type: 4u32,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             len: (std::mem::size_of::<[i8; 2]>() as u64).wrapping_sub(1u64),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         };
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_4]].r#type = 4u32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_4]]
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT: ]);
// I686-PC-WINDOWS-MSVC-NEXT: ];
// AARCH64-PC-WINDOWS-MSVC-NEXT: ];
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut names: __SlateAlign16<[ptrlen; 3]> = __SlateAlign16([
// I686-PC-WINDOWS-MSVC-NEXT: static mut names: [ptrlen; 3] = [
// AARCH64-PC-WINDOWS-MSVC-NEXT: static mut names: [ptrlen; 3] = [
// CHECK-NEXT:     ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 5]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 5]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 5]>() as u64) != 0u64 {
// CHECK-NEXT:             b"ping\0".as_ptr() as *mut i8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             b"ping\0".as_ptr() as *mut i8
// CHECK-NEXT:         }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 5]>() as u64).wrapping_sub(1u64),
// I686-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 5]>() as u32).wrapping_sub(1u32),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 5]>() as u64).wrapping_sub(1u64),
// CHECK-NEXT:     },
// CHECK-NEXT:     ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 6]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 6]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 6]>() as u64) != 0u64 {
// CHECK-NEXT:             b"query\0".as_ptr() as *mut i8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             b"query\0".as_ptr() as *mut i8
// CHECK-NEXT:         }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 6]>() as u64).wrapping_sub(1u64),
// I686-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 6]>() as u32).wrapping_sub(1u32),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 6]>() as u64).wrapping_sub(1u64),
// CHECK-NEXT:     },
// CHECK-NEXT:     ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 25]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 25]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 25]>() as u64) != 0u64 {
// CHECK-NEXT:             b"session-bind@openssh.com\0".as_ptr() as *mut i8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             b"session-bind@openssh.com\0".as_ptr() as *mut i8
// CHECK-NEXT:         }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 25]>() as u64).wrapping_sub(1u64),
// I686-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 25]>() as u32).wrapping_sub(1u32),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 25]>() as u64).wrapping_sub(1u64),
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT: ]);
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// X86_64-PC-WINDOWS-MSVC-NEXT: struct __SlateAlign16<T>(T);
// I686-PC-WINDOWS-MSVC-NEXT: ];
// AARCH64-PC-WINDOWS-MSVC-NEXT: ];
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn hash_a(x: i32) -> i32 {
// CHECK-NEXT:     x * 3
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn hash_b(x: i32) -> i32 {
// CHECK-NEXT:     x + 7
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct hash_vt {
// CHECK-NEXT:     r#fn: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     desc: *const i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut vt_a: hash_vt = hash_vt {
// CHECK-NEXT:     r#fn: Some(hash_a),
// CHECK-NEXT:     desc: (b"a\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut vt_b: hash_vt = hash_vt {
// CHECK-NEXT:     r#fn: Some(hash_b),
// CHECK-NEXT:     desc: (b"b\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct sign_extra {
// CHECK-NEXT:     hash: *const hash_vt,
// CHECK-NEXT:     fallback: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     alg: *const i8,
// CHECK-NEXT:     prefix: ptrlen,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut extra: sign_extra = sign_extra {
// X86_64-PC-WINDOWS-MSVC-NEXT:     hash: if (std::mem::size_of::<i32>() as u64) == 4u64 {
// I686-PC-WINDOWS-MSVC-NEXT:     hash: if (std::mem::size_of::<i32>() as u32) == 4u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     hash: if (std::mem::size_of::<i32>() as u64) == 4u64 {
// CHECK-NEXT:         unsafe { std::ptr::addr_of!(vt_a) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { std::ptr::addr_of!(vt_b) }
// CHECK-NEXT:     },
// CHECK-NEXT:     fallback: {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_0]]: Option<unsafe extern "C-unwind" fn(i32) -> i32> = if 1i32 != 0i32 {
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_0:__t[0-9]+]]: Option<unsafe extern "C-unwind" fn(i32) -> i32> = if 1i32 != 0i32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_0]]: Option<unsafe extern "C-unwind" fn(i32) -> i32> = if 1i32 != 0i32 {
// CHECK-NEXT:             Some(hash_b)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             Some(hash_a)
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     alg: (b"Ed25519\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     prefix: ptrlen {
// X86_64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 1]>() as u64) != 0u64 {
// I686-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 1]>() as u32) != 0u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         ptr: (if (std::mem::size_of::<[i8; 1]>() as u64) != 0u64 {
// CHECK-NEXT:             b"\0".as_ptr() as *mut i8
// CHECK-NEXT:         } else {
// CHECK-NEXT:             b"\0".as_ptr() as *mut i8
// CHECK-NEXT:         }) as *const (),
// X86_64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 1]>() as u64).wrapping_sub(1u64),
// I686-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 1]>() as u32).wrapping_sub(1u32),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         len: (std::mem::size_of::<[i8; 1]>() as u64).wrapping_sub(1u64),
// CHECK-NEXT:     },
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut buffer_a: [i8; 8] = [102, 105, 114, 115, 116, 0, 0, 0];
// CHECK-EMPTY:
// CHECK-NEXT: static mut buffer_b: [i8; 8] = [115, 101, 99, 111, 110, 100, 0, 0];
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut pick: *mut i8 = if (std::mem::size_of::<*mut ()>() as u64) == 8u64 {
// I686-PC-WINDOWS-MSVC-NEXT: static mut pick: *mut i8 = if (std::mem::size_of::<*mut ()>() as u32) == 8u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT: static mut pick: *mut i8 = if (std::mem::size_of::<*mut ()>() as u64) == 8u64 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(buffer_a) as *mut i8 }
// CHECK-NEXT: } else {
// CHECK-NEXT:     unsafe { unsafe { std::ptr::addr_of_mut!(buffer_b) as *mut i8 }.offset(1i32 as isize) }
// CHECK-NEXT: };
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut picks: __SlateAlign16<[*const u32; 2]> = __SlateAlign16([
// I686-PC-WINDOWS-MSVC-NEXT: static mut picks: [*const u32; 2] = [
// AARCH64-PC-WINDOWS-MSVC-NEXT: static mut picks: [*const u32; 2] = [
// CHECK-NEXT:     if 0i32 != 0i32 {
// CHECK-NEXT:         std::ptr::null::<u32>()
// CHECK-NEXT:     } else {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::ptr::addr_of!(operators.0[3i32 as usize].r#type) }
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::ptr::addr_of!(operators[3i32 as usize].r#type) }
// AARCH64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::ptr::addr_of!(operators[3i32 as usize].r#type) }
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (std::mem::size_of::<i32>() as u64) == 4u64 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::ptr::addr_of!(operators.0[1i32 as usize].r#type) }
// I686-PC-WINDOWS-MSVC-NEXT:     if (std::mem::size_of::<i32>() as u32) == 4u32 {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::ptr::addr_of!(operators[1i32 as usize].r#type) }
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if (std::mem::size_of::<i32>() as u64) == 4u64 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::ptr::addr_of!(operators[1i32 as usize].r#type) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null::<u32>()
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT: ]);
// I686-PC-WINDOWS-MSVC-NEXT: ];
// AARCH64-PC-WINDOWS-MSVC-NEXT: ];
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut valid_bitmap: __SlateAlign16<[u32; 8]> = __SlateAlign16([
// I686-PC-WINDOWS-MSVC-NEXT: static mut valid_bitmap: [u32; 8] = [
// AARCH64-PC-WINDOWS-MSVC-NEXT: static mut valid_bitmap: [u32; 8] = [
// CHECK-NEXT:     0u32 | if 1i32 >= 0i32 && 1i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 0i32 && 1i32 < 32i32 {
// CHECK-NEXT:             1i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 0i32 && 2i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 0i32 && 2i32 < 32i32 {
// CHECK-NEXT:             2i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 0i32 && 20i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 0i32 && 20i32 < 32i32 {
// CHECK-NEXT:             20i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 0i32 && 31i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 0i32 && 31i32 < 32i32 {
// CHECK-NEXT:             31i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 0i32 && 32i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 0i32 && 32i32 < 32i32 {
// CHECK-NEXT:             32i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 0i32 && 50i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 0i32 && 50i32 < 32i32 {
// CHECK-NEXT:             50i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 0i32 && 94i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 0i32 && 94i32 < 32i32 {
// CHECK-NEXT:             94i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 0i32 && 100i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 0i32 && 100i32 < 32i32 {
// CHECK-NEXT:             100i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 0i32 && 255i32 < 32i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 0i32 && 255i32 < 32i32 {
// CHECK-NEXT:             255i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// CHECK-NEXT:     0u32 | if 1i32 >= 32i32 && 1i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 32i32 && 1i32 < 64i32 {
// CHECK-NEXT:             -31i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 32i32 && 2i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 32i32 && 2i32 < 64i32 {
// CHECK-NEXT:             -30i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 32i32 && 20i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 32i32 && 20i32 < 64i32 {
// CHECK-NEXT:             -12i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 32i32 && 31i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 32i32 && 31i32 < 64i32 {
// CHECK-NEXT:             -1i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 32i32 && 32i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 32i32 && 32i32 < 64i32 {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 32i32 && 50i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 32i32 && 50i32 < 64i32 {
// CHECK-NEXT:             18i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 32i32 && 94i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 32i32 && 94i32 < 64i32 {
// CHECK-NEXT:             62i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 32i32 && 100i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 32i32 && 100i32 < 64i32 {
// CHECK-NEXT:             68i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 32i32 && 255i32 < 64i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 32i32 && 255i32 < 64i32 {
// CHECK-NEXT:             223i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// CHECK-NEXT:     0u32 | if 1i32 >= 64i32 && 1i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 64i32 && 1i32 < 96i32 {
// CHECK-NEXT:             -63i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 64i32 && 2i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 64i32 && 2i32 < 96i32 {
// CHECK-NEXT:             -62i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 64i32 && 20i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 64i32 && 20i32 < 96i32 {
// CHECK-NEXT:             -44i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 64i32 && 31i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 64i32 && 31i32 < 96i32 {
// CHECK-NEXT:             -33i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 64i32 && 32i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 64i32 && 32i32 < 96i32 {
// CHECK-NEXT:             -32i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 64i32 && 50i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 64i32 && 50i32 < 96i32 {
// CHECK-NEXT:             -14i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 64i32 && 94i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 64i32 && 94i32 < 96i32 {
// CHECK-NEXT:             30i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 64i32 && 100i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 64i32 && 100i32 < 96i32 {
// CHECK-NEXT:             36i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 64i32 && 255i32 < 96i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 64i32 && 255i32 < 96i32 {
// CHECK-NEXT:             191i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// CHECK-NEXT:     0u32 | if 1i32 >= 96i32 && 1i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 96i32 && 1i32 < 128i32 {
// CHECK-NEXT:             -95i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 96i32 && 2i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 96i32 && 2i32 < 128i32 {
// CHECK-NEXT:             -94i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 96i32 && 20i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 96i32 && 20i32 < 128i32 {
// CHECK-NEXT:             -76i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 96i32 && 31i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 96i32 && 31i32 < 128i32 {
// CHECK-NEXT:             -65i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 96i32 && 32i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 96i32 && 32i32 < 128i32 {
// CHECK-NEXT:             -64i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 96i32 && 50i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 96i32 && 50i32 < 128i32 {
// CHECK-NEXT:             -46i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 96i32 && 94i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 96i32 && 94i32 < 128i32 {
// CHECK-NEXT:             -2i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 96i32 && 100i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 96i32 && 100i32 < 128i32 {
// CHECK-NEXT:             4i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 96i32 && 255i32 < 128i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 96i32 && 255i32 < 128i32 {
// CHECK-NEXT:             159i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// CHECK-NEXT:     0u32 | if 1i32 >= 128i32 && 1i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 128i32 && 1i32 < 160i32 {
// CHECK-NEXT:             -127i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 128i32 && 2i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 128i32 && 2i32 < 160i32 {
// CHECK-NEXT:             -126i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 128i32 && 20i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 128i32 && 20i32 < 160i32 {
// CHECK-NEXT:             -108i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 128i32 && 31i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 128i32 && 31i32 < 160i32 {
// CHECK-NEXT:             -97i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 128i32 && 32i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 128i32 && 32i32 < 160i32 {
// CHECK-NEXT:             -96i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 128i32 && 50i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 128i32 && 50i32 < 160i32 {
// CHECK-NEXT:             -78i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 128i32 && 94i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 128i32 && 94i32 < 160i32 {
// CHECK-NEXT:             -34i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 128i32 && 100i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 128i32 && 100i32 < 160i32 {
// CHECK-NEXT:             -28i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 128i32 && 255i32 < 160i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 128i32 && 255i32 < 160i32 {
// CHECK-NEXT:             127i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// CHECK-NEXT:     0u32 | if 1i32 >= 160i32 && 1i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 160i32 && 1i32 < 192i32 {
// CHECK-NEXT:             -159i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 160i32 && 2i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 160i32 && 2i32 < 192i32 {
// CHECK-NEXT:             -158i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 160i32 && 20i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 160i32 && 20i32 < 192i32 {
// CHECK-NEXT:             -140i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 160i32 && 31i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 160i32 && 31i32 < 192i32 {
// CHECK-NEXT:             -129i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 160i32 && 32i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 160i32 && 32i32 < 192i32 {
// CHECK-NEXT:             -128i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 160i32 && 50i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 160i32 && 50i32 < 192i32 {
// CHECK-NEXT:             -110i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 160i32 && 94i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 160i32 && 94i32 < 192i32 {
// CHECK-NEXT:             -66i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 160i32 && 100i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 160i32 && 100i32 < 192i32 {
// CHECK-NEXT:             -60i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 160i32 && 255i32 < 192i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 160i32 && 255i32 < 192i32 {
// CHECK-NEXT:             95i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// CHECK-NEXT:     0u32 | if 1i32 >= 192i32 && 1i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 192i32 && 1i32 < 224i32 {
// CHECK-NEXT:             -191i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 192i32 && 2i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 192i32 && 2i32 < 224i32 {
// CHECK-NEXT:             -190i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 192i32 && 20i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 192i32 && 20i32 < 224i32 {
// CHECK-NEXT:             -172i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 192i32 && 31i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 192i32 && 31i32 < 224i32 {
// CHECK-NEXT:             -161i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 192i32 && 32i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 192i32 && 32i32 < 224i32 {
// CHECK-NEXT:             -160i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 192i32 && 50i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 192i32 && 50i32 < 224i32 {
// CHECK-NEXT:             -142i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 192i32 && 94i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 192i32 && 94i32 < 224i32 {
// CHECK-NEXT:             -98i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 192i32 && 100i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 192i32 && 100i32 < 224i32 {
// CHECK-NEXT:             -92i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 192i32 && 255i32 < 224i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 192i32 && 255i32 < 224i32 {
// CHECK-NEXT:             63i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// CHECK-NEXT:     0u32 | if 1i32 >= 224i32 && 1i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 1i32 >= 224i32 && 1i32 < 256i32 {
// CHECK-NEXT:             -223i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 2i32 >= 224i32 && 2i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 2i32 >= 224i32 && 2i32 < 256i32 {
// CHECK-NEXT:             -222i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 20i32 >= 224i32 && 20i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 20i32 >= 224i32 && 20i32 < 256i32 {
// CHECK-NEXT:             -204i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 31i32 >= 224i32 && 31i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 31i32 >= 224i32 && 31i32 < 256i32 {
// CHECK-NEXT:             -193i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 32i32 >= 224i32 && 32i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 32i32 >= 224i32 && 32i32 < 256i32 {
// CHECK-NEXT:             -192i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 50i32 >= 224i32 && 50i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 50i32 >= 224i32 && 50i32 < 256i32 {
// CHECK-NEXT:             -174i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 94i32 >= 224i32 && 94i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 94i32 >= 224i32 && 94i32 < 256i32 {
// CHECK-NEXT:             -130i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 100i32 >= 224i32 && 100i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 100i32 >= 224i32 && 100i32 < 256i32 {
// CHECK-NEXT:             -124i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     } | if 255i32 >= 224i32 && 255i32 < 256i32 {
// CHECK-NEXT:         1u32 << if 255i32 >= 224i32 && 255i32 < 256i32 {
// CHECK-NEXT:             31i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0u32
// CHECK-NEXT:     },
// X86_64-PC-WINDOWS-MSVC-NEXT: ]);
// I686-PC-WINDOWS-MSVC-NEXT: ];
// AARCH64-PC-WINDOWS-MSVC-NEXT: ];
// CHECK-EMPTY:
// CHECK-NEXT: fn same(p: ptrlen, s: *const i8) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool =
// CHECK-NEXT:         p.len == unsafe { strlen(s) } && (unsafe { memcmp(p.ptr, s as *const (), p.len) }) == 0;
// CHECK-NEXT:     [[V_0]] as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0..((std::mem::size_of::<[op; 5]>() as u64) / (std::mem::size_of::<op>() as u64)) {
// X86_64-PC-WINDOWS-MSVC-NEXT:         CHECK!(((unsafe { operators.0[i as usize].r#type }) as u32) == (i as u32));
// X86_64-PC-WINDOWS-MSVC-NEXT:         CHECK__2!(same(unsafe { operators.0[i as usize].text }, unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             texts.0[i as usize]
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0..((std::mem::size_of::<[op; 5]>() as u32) / (std::mem::size_of::<op>() as u32)) {
// I686-PC-WINDOWS-MSVC-NEXT:         CHECK!(((unsafe { operators[i as usize].r#type }) as u32) == (i as u32));
// I686-PC-WINDOWS-MSVC-NEXT:         CHECK__2!(same(unsafe { operators[i as usize].text }, unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:             texts[i as usize]
// AARCH64-PC-WINDOWS-MSVC-NEXT:     for i in 0..((std::mem::size_of::<[op; 5]>() as u64) / (std::mem::size_of::<op>() as u64)) {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         CHECK!(((unsafe { operators[i as usize].r#type }) as u32) == (i as u32));
// AARCH64-PC-WINDOWS-MSVC-NEXT:         CHECK__2!(same(unsafe { operators[i as usize].text }, unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:             texts[i as usize]
// CHECK-NEXT:         }));
// CHECK-NEXT:     }
// CHECK-NEXT:     CHECK!(
// X86_64-PC-WINDOWS-MSVC-NEXT:         (std::mem::size_of::<[ptrlen; 3]>() as u64) / (std::mem::size_of::<ptrlen>() as u64) == 3
// I686-PC-WINDOWS-MSVC-NEXT:         (std::mem::size_of::<[ptrlen; 3]>() as u32) / (std::mem::size_of::<ptrlen>() as u32) == 3
// AARCH64-PC-WINDOWS-MSVC-NEXT:         (std::mem::size_of::<[ptrlen; 3]>() as u64) / (std::mem::size_of::<ptrlen>() as u64) == 3
// CHECK-NEXT:     );
// CHECK-NEXT:     loop {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[V_1:__v[0-9]+]]: bool = same(unsafe { names.0[0] }, c"ping".as_ptr()) != 0
// X86_64-PC-WINDOWS-MSVC-NEXT:             && same(unsafe { names.0[1] }, c"query".as_ptr()) != 0;
// I686-PC-WINDOWS-MSVC-NEXT:         let [[V_1:__v[0-9]+]]: bool = same(unsafe { names[0] }, c"ping".as_ptr()) != 0
// I686-PC-WINDOWS-MSVC-NEXT:             && same(unsafe { names[1] }, c"query".as_ptr()) != 0;
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[V_1:__v[0-9]+]]: bool = same(unsafe { names[0] }, c"ping".as_ptr()) != 0
// AARCH64-PC-WINDOWS-MSVC-NEXT:             && same(unsafe { names[1] }, c"query".as_ptr()) != 0;
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: bool =
// X86_64-PC-WINDOWS-MSVC-NEXT:             [[V_1]] && same(unsafe { names.0[2] }, c"session-bind@openssh.com".as_ptr()) != 0;
// I686-PC-WINDOWS-MSVC-NEXT:             [[V_1]] && same(unsafe { names[2] }, c"session-bind@openssh.com".as_ptr()) != 0;
// AARCH64-PC-WINDOWS-MSVC-NEXT:             [[V_1]] && same(unsafe { names[2] }, c"session-bind@openssh.com".as_ptr()) != 0;
// CHECK-NEXT:         if ![[V_2]] {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: bool = (unsafe { extra.hash }) == unsafe { std::ptr::addr_of!(vt_a) }
// CHECK-NEXT:             && (unsafe { unsafe { (*extra.hash).r#fn }.unwrap()(5i32) }) == 15;
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: bool = [[V_3]] && (unsafe { unsafe { extra.fallback }.unwrap()(5i32) }) == 12;
// CHECK-NEXT:         if ![[V_4]] {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: bool = (unsafe { strcmp(extra.alg, c"Ed25519".as_ptr()) }) == 0
// CHECK-NEXT:             && same(unsafe { extra.prefix }, c"".as_ptr()) != 0;
// CHECK-NEXT:         if ![[V_5]] {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         (unsafe { pick })
// X86_64-PC-WINDOWS-MSVC-NEXT:             == if (std::mem::size_of::<*mut ()>() as u64) == 8 {
// I686-PC-WINDOWS-MSVC-NEXT:             == if (std::mem::size_of::<*mut ()>() as u32) == 8 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:             == if (std::mem::size_of::<*mut ()>() as u64) == 8 {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(buffer_a) as *mut i8 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe { (std::ptr::addr_of_mut!(buffer_b) as *mut i8).add(1) }
// CHECK-NEXT:             }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { pick }.add(0) = 70;
// CHECK-NEXT:     }
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(buffer_a) as *mut i8) as *const i8,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 (if (std::mem::size_of::<*mut ()>() as u64) == 8 {
// I686-PC-WINDOWS-MSVC-NEXT:                 (if (std::mem::size_of::<*mut ()>() as u32) == 8 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 (if (std::mem::size_of::<*mut ()>() as u64) == 8 {
// CHECK-NEXT:                     c"First".as_ptr() as *mut i8
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     c"first".as_ptr() as *mut i8
// CHECK-NEXT:                 }) as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(buffer_b) as *mut i8) as *const i8,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 (if (std::mem::size_of::<*mut ()>() as u64) == 8 {
// I686-PC-WINDOWS-MSVC-NEXT:                 (if (std::mem::size_of::<*mut ()>() as u32) == 8 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 (if (std::mem::size_of::<*mut ()>() as u64) == 8 {
// CHECK-NEXT:                     c"second".as_ptr() as *mut i8
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     c"sFcond".as_ptr() as *mut i8
// CHECK-NEXT:                 }) as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0
// CHECK-NEXT:     );
// X86_64-PC-WINDOWS-MSVC-NEXT:     CHECK!(((unsafe { *picks.0[0] }) as u32) == 3 && ((unsafe { *picks.0[1] }) as u32) == 1);
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0..((std::mem::size_of::<[u32; 8]>() as u64) / (std::mem::size_of::<u32>() as u64)) {
// X86_64-PC-WINDOWS-MSVC-NEXT:         CHECK!((unsafe { valid_bitmap.0[i as usize] }) == unsafe { expected.0[i as usize] });
// I686-PC-WINDOWS-MSVC-NEXT:     CHECK!(((unsafe { *picks[0] }) as u32) == 3 && ((unsafe { *picks[1] }) as u32) == 1);
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0..((std::mem::size_of::<[u32; 8]>() as u32) / (std::mem::size_of::<u32>() as u32)) {
// I686-PC-WINDOWS-MSVC-NEXT:         CHECK!((unsafe { valid_bitmap[i as usize] }) == unsafe { expected[i as usize] });
// AARCH64-PC-WINDOWS-MSVC-NEXT:     CHECK!(((unsafe { *picks[0] }) as u32) == 3 && ((unsafe { *picks[1] }) as u32) == 1);
// AARCH64-PC-WINDOWS-MSVC-NEXT:     for i in 0..((std::mem::size_of::<[u32; 8]>() as u64) / (std::mem::size_of::<u32>() as u64)) {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         CHECK!((unsafe { valid_bitmap[i as usize] }) == unsafe { expected[i as usize] });
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut texts: __SlateAlign16<[*const i8; 5]> = __SlateAlign16([
// I686-PC-WINDOWS-MSVC-NEXT: static mut texts: [*const i8; 5] = [
// AARCH64-PC-WINDOWS-MSVC-NEXT: static mut texts: [*const i8; 5] = [
// CHECK-NEXT:     (b"(\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b")\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"&&\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"||\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"!\0".as_ptr() as *mut i8) as *const i8,
// X86_64-PC-WINDOWS-MSVC-NEXT: ]);
// I686-PC-WINDOWS-MSVC-NEXT: ];
// AARCH64-PC-WINDOWS-MSVC-NEXT: ];
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut expected: __SlateAlign16<[u32; 8]> = __SlateAlign16([
// X86_64-PC-WINDOWS-MSVC-NEXT:     2148532230u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     262145u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     1073741824u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     16u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     0u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     0u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     0u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     2147483648u32,
// X86_64-PC-WINDOWS-MSVC-NEXT: ]);
// I686-PC-WINDOWS-MSVC-NEXT: static mut expected: [u32; 8] = [2148532230, 262145, 1073741824, 16, 0, 0, 0, 2147483648];
// AARCH64-PC-WINDOWS-MSVC-NEXT: static mut expected: [u32; 8] = [2148532230, 262145, 1073741824, 16, 0, 0, 0, 2147483648];
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
