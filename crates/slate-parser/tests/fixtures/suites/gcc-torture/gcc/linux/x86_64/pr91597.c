/* PR tree-optimization/91597 */

enum E { A, B, C };
struct __attribute__((aligned(4))) S {
  enum E e;
};

enum E foo(struct S *o) {
  if (((__UINTPTR_TYPE__)(o) & 1) == 0)
    return o->e;
  else
    return A;
}

int bar(struct S *o) { return foo(o) == B || foo(o) == C; }

static inline void baz(struct S *o, int d) {
  if (__builtin_expect(!bar(o), 0))
    __builtin_abort();
  if (d > 2)
    return;
  baz(o, d + 1);
}

void qux(struct S *o) {
  switch (o->e) {
  case A:
    return;
  case B:
    baz(o, 0);
    break;
  case C:
    baz(o, 0);
    break;
  }
}

struct S s = {C};

int main() {
  qux(&s);
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
// DEFAULT-NEXT:     type @type0 E = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(0);
// DEFAULT-NEXT:         %1 B = const<i32>(1);
// DEFAULT-NEXT:         %2 C = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 e: @type0;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %14 s: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 o: ptr<@type1>) -> @type0 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u64>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<@type1>>(%6)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return read<@type0>(field0(deref(read<ptr<@type1>>(%6))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return int_to_enum<@type0, reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar(%8 o: ptr<@type1>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(call<@type0, signature=fn(ptr<@type1>) -> @type0>(%5, read<ptr<@type1>>(%8))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, eq<u32>(enum_to_int<u32, reason=promotion>(call<@type0, signature=fn(ptr<@type1>) -> @type0>(%5, read<ptr<@type1>>(%8))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(read<bool>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @__builtin_expect(%16 <unnamed>: i64, %17 <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %19 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @baz(%10 o: ptr<@type1>, %11 d: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%18, from_bool<i64, reason=arg>(not<bool>(ne<i32>(call<i32, signature=fn(ptr<@type1>) -> i32>(%7, read<ptr<@type1>>(%10)), const<i32>(0)))), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%11), const<i32>(2))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%9, read<ptr<@type1>>(%10), add<i32, overflow=ub>(read<i32>(%11), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @qux(%13 o: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %20 enum_to_int<u32, reason=promotion>(read<@type0>(field0(deref(read<ptr<@type1>>(%13)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %20 const<u32>(0):
// DEFAULT-NEXT:                     return;
// DEFAULT-NEXT:                 case %20 const<u32>(1):
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type1>, i32) -> void>(%9, read<ptr<@type1>>(%13), const<i32>(0));
// DEFAULT-NEXT:                 break %20;
// DEFAULT-NEXT:                 case %20 const<u32>(2):
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type1>, i32) -> void>(%9, read<ptr<@type1>>(%13), const<i32>(0));
// DEFAULT-NEXT:                 break %20;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%12, addr_of<ptr<@type1>>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
