extern void abort(void);

struct foo {
  int   rank;
  char *name;
};

struct mem {
  struct foo *x[4];
};

void __attribute__((noinline)) bar(struct foo **f) {
  *f = __builtin_malloc(sizeof(struct foo));
}
struct foo *__attribute__((noinline, noclone)) foo(int rank) {
  void        *x     = __builtin_malloc(sizeof(struct mem));
  struct mem  *as    = x;
  struct foo **upper = &as->x[rank * 8 - 5];
  *upper             = 0;
  bar(upper);
  return *upper;
}

int main() {
  if (foo(1) == 0)
    abort();
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 rank: i32;
// DEFAULT-NEXT:         field1 name: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 mem = struct {
// DEFAULT-NEXT:         field0 x: array<ptr<@type0>, 4>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @__builtin_malloc(%11 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @bar(%4 f: ptr<ptr<@type0>>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%4)), pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%12, const<u64>(16))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%12, const<u64>(16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo(%6 rank: i32) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 x: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%12, const<u64>(32));
// DEFAULT-NEXT:         let %8 as: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%7));
// DEFAULT-NEXT:         let %9 upper: ptr<ptr<@type0>> [storage=automatic] = addr_of<ptr<ptr<@type0>>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(4)>(field0(deref(read<ptr<@type1>>(%8)))), sub<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%6), const<i32>(8)), const<i32>(5)))));
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%9)), null<ptr<@type0>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<@type0>>) -> void>(%3, read<ptr<ptr<@type0>>>(%9));
// DEFAULT-NEXT:         return read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<ptr<@type0>>(call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%5, const<i32>(1)), null<ptr<@type0>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
