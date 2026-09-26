/* { dg-do run } */
/* { dg-options "-w -Wno-psabi" } */
/* { dg-require-effective-target int32plus } */

typedef int v4si __attribute__((vector_size(16)));
struct T {
  v4si i[2];
  int  j;
} __attribute__((packed));

static v4si __attribute__((noinline)) foo(struct T t) { return t.i[0]; }

static struct T *__attribute__((noinline)) init() {
  char *p = __builtin_malloc(sizeof(struct T) + 1);
  p++;
  __builtin_memset(p, 1, sizeof(struct T));
  return (struct T *)p;
}

int main() {
  struct T *p;
  p = init();
  if (foo(*p)[0] != 0x01010101)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 v4si = vector<i32, 4>;
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 i: array<vector<i32, 4>, 2>;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=36, align=1, offsets=[0, 32]];
// DEFAULT-NEXT:     fn %2 @foo(%3 t: @type1) -> vector<i32, 4> [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<vector<i32, 4>>(deref(ptr_offset<ptr<vector<i32, 4>>, subtract=false, element=vector<i32, 4>, overflow=ub>(array_decay<ptr<vector<i32, 4>>, length=Some(2)>(field0(%3)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_malloc(%8 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @__builtin_memset(%10 <unnamed>: ptr<void>, %11 <unnamed>: i32, %12 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @init() -> ptr<@type1> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%9, add<u64, overflow=wrap>(const<u64>(36), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         let %15: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:         let %16: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, read<ptr<i8>>(%16));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%5)), const<i32>(1), const<u64>(36));
// DEFAULT-NEXT:         return pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<i8>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 p: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type1>>(%7, call<ptr<@type1>, signature=fn() -> ptr<@type1>>(%4));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn() -> ptr<@type1>>(%4);
// DEFAULT-NEXT:         if ne<i32>(lane<i32>(call<vector<i32, 4>, signature=fn(@type1) -> vector<i32, 4>, abi=sysv64(native_c) -> direct>(%2, copy<@type1, reason=arg>(read<@type1>(deref(read<ptr<@type1>>(%7))))), const<i32>(0)), const<i32>(16843009))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
