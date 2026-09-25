/* PR target/70566.  */

#define NULL 0

struct mystruct {
  unsigned int f1 : 1;
  unsigned int f2 : 1;
  unsigned int f3 : 1;
};

__attribute__((noinline)) void myfunc(int a, void *b) {}
__attribute__((noinline)) int  myfunc2(void *a) { return 0; }

static void set_f2(struct mystruct *user, int f2) {
  if (user->f2 != f2)
    myfunc(myfunc2(NULL), NULL);
  else
    __builtin_abort();
}

__attribute__((noinline)) void foo(void *data) {
  struct mystruct *user = data;
  if (!user->f2)
    set_f2(user, 1);
}

int main(void) {
  struct mystruct a;
  a.f1 = 1;
  a.f2 = 0;
  foo(&a);
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
// DEFAULT-NEXT:     type @type0 mystruct = struct {
// DEFAULT-NEXT:         field0 f1: u32 : 1;
// DEFAULT-NEXT:         field1 f2: u32 : 1;
// DEFAULT-NEXT:         field2 f3: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(1), Some(2)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %1 @myfunc(%2 a: i32, %3 b: ptr<void>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @myfunc2(%5 a: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @set_f2(%7 user: ptr<@type0>, %8 f2: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=1..2>(deref(read<ptr<@type0>>(%7))))), read<i32>(%8))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<void>) -> void>(%1, call<i32, signature=fn(ptr<void>) -> i32>(%4, null<ptr<void>>), null<ptr<void>>);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @foo(%10 data: ptr<void>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 user: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(read<ptr<void>>(%10));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=1..2>(deref(read<ptr<@type0>>(%11))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type0>, i32) -> void>(%6, read<ptr<@type0>>(%11), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%13), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%13), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%9, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%13)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
