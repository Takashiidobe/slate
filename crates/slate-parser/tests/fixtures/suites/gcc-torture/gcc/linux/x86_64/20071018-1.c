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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 rank: i32;
// DEFAULT-NEXT:         field1 name: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_mem:[0-9]+]] mem = struct {
// DEFAULT-NEXT:         field0 x: array<ptr<@type[[TYPE_foo]]>, 4>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_f:[0-9]+]] f: ptr<ptr<@type[[TYPE_foo]]>>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_foo]]>>(deref(read<ptr<ptr<@type[[TYPE_foo]]>>>(%[[VALUE_f]])), pointer_cast<ptr<@type[[TYPE_foo]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], const<u64>(16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_rank:[0-9]+]] rank: i32) -> ptr<@type[[TYPE_foo]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], const<u64>(32));
// DEFAULT-NEXT:         let %[[VALUE_as:[0-9]+]] as: ptr<@type[[TYPE_mem]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_mem]]>, reason=assign>(read<ptr<void>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_upper:[0-9]+]] upper: ptr<ptr<@type[[TYPE_foo]]>> [storage=automatic] = addr_of<ptr<ptr<@type[[TYPE_foo]]>>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_foo]]>>, subtract=false, element=ptr<@type[[TYPE_foo]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_foo]]>>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_mem]]>>(%[[VALUE_as]])))), sub<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_rank]]), const<i32>(8)), const<i32>(5)))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_foo]]>>(deref(read<ptr<ptr<@type[[TYPE_foo]]>>>(%[[VALUE_upper]])), null<ptr<@type[[TYPE_foo]]>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<@type[[TYPE_foo]]>>) -> void>(%[[VALUE_bar]], read<ptr<ptr<@type[[TYPE_foo]]>>>(%[[VALUE_upper]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_foo]]>>(deref(read<ptr<ptr<@type[[TYPE_foo]]>>>(%[[VALUE_upper]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_foo]]>>(call<ptr<@type[[TYPE_foo]]>, signature=fn(i32) -> ptr<@type[[TYPE_foo]]>>(%[[VALUE_foo]], const<i32>(1)), null<ptr<@type[[TYPE_foo]]>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
