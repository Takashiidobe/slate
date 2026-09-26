/* Copyright (C) 2000 Free Software Foundation, Inc.
   Contributed by Nathan Sidwell 23 Feb 2000 <nathan@codesourcery.com> */

/* __alignof__ should never return a non-power of 2
   eg, sizeof(long double) might be 12, but that means it must be alignable
   on a 4 byte boundary. */

void abort(void);

void check(char const *type, int align) {
  if ((align & -align) != align) {
    abort();
  }
}

#define QUOTE_(s) #s
#define QUOTE(s)  QUOTE_(s)

#define check(t) check(QUOTE(t), __alignof__(t))

// This struct should have an alignment of the lcm of all the types. If one of
// the base alignments is not a power of two, then A cannot be power of two
// aligned.
struct A {
  char               c;
  signed short       ss;
  unsigned short     us;
  signed int         si;
  unsigned int       ui;
  signed long        sl;
  unsigned long      ul;
  signed long long   sll;
  unsigned long long ull;
  float              f;
  double             d;
  long double        ld;
  void              *dp;
  void               (*fp)();
};

int main() {
  check(void);
  check(char);
  check(signed short);
  check(unsigned short);
  check(signed int);
  check(unsigned int);
  check(signed long);
  check(unsigned long);
  check(signed long long);
  check(unsigned long long);
  check(float);
  check(double);
  check(long double);
  check(void *);
  check(void (*)());
  check(struct A);
  return 0;
}


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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 ss: i16;
// DEFAULT-NEXT:         field2 us: u16;
// DEFAULT-NEXT:         field3 si: i32;
// DEFAULT-NEXT:         field4 ui: u32;
// DEFAULT-NEXT:         field5 sl: i64;
// DEFAULT-NEXT:         field6 ul: u64;
// DEFAULT-NEXT:         field7 sll: i64;
// DEFAULT-NEXT:         field8 ull: u64;
// DEFAULT-NEXT:         field9 f: f32;
// DEFAULT-NEXT:         field10 d: f64;
// DEFAULT-NEXT:         field11 ld: f80;
// DEFAULT-NEXT:         field12 dp: ptr<void>;
// DEFAULT-NEXT:         field13 fp: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=96, align=16, offsets=[0, 2, 4, 8, 12, 16, 24, 32, 40, 48, 56, 64, 80, 88]];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([118, 111, 105, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 108, 111, 97, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([108, 111, 110, 103, 32, 100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([118, 111, 105, 100, 32, 42, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([118, 111, 105, 100, 32, 40, 42, 41, 40, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 116, 114, 117, 99, 116, 32, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @check(%2 type: ptr<const i8>, %3 align: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%3), neg<i32, overflow=ub>(read<i32>(%3))), read<i32>(%3))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%6)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%7)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%8)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%9)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%10)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%11)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%12)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%13)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%14)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%15)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%16)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%18)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%19)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%20)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%21)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(16))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
