
typedef struct st {
  int a;
} ST;

int __attribute__((noinline, noclone)) foo(ST *s, int c) {
  int first = 1;
  int count = c;
  ST *item  = s;
  int a     = s->a;
  int x;

  while (count--) {
    x = item->a;
    if (first)
      first = 0;
    else if (x >= a)
      return 1;
    a = x;
    item++;
  }
  return 0;
}

extern void abort(void);

int main() {
  ST _1[2] = {{2}, {1}};
  if (foo(_1, 2) != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_st:[0-9]+]] st = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_ST:[0-9]+]] ST = @type[[TYPE_st]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_st]]>, %[[VALUE_c:[0-9]+]] c: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: i32 [storage=automatic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE_item:[0-9]+]] item: ptr<@type[[TYPE_st]]> [storage=automatic] = read<ptr<@type[[TYPE_st]]>>(%[[VALUE_s]]);
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_st]]>>(%[[VALUE_s]]))));
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(field0(deref(read<ptr<@type[[TYPE_st]]>>(%[[VALUE_item]])))));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_first]]), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_first]], const<i32>(0));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_a]]))
// DEFAULT-NEXT:                         return const<i32>(1);
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE_st]]> [synthetic] = read<ptr<@type[[TYPE_st]]>>(%[[VALUE_item]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<@type[[TYPE_st]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_st]]>, subtract=false, element=@type[[TYPE_st]], overflow=ub>(read<ptr<@type[[TYPE_st]]>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_st]]>>(%[[VALUE_item]], read<ptr<@type[[TYPE_st]]>>(%[[VALUE4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE__1:[0-9]+]] _1: array<@type[[TYPE_st]], 2> [storage=automatic] = aggregate<array<@type[[TYPE_st]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_st]], zero_fill=false>(field0 = const<i32>(2)), index1 = aggregate<@type[[TYPE_st]], zero_fill=false>(field0 = const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_st]]>, i32) -> i32>(%[[VALUE_foo]], array_decay<ptr<@type[[TYPE_st]]>, length=Some(2)>(%[[VALUE__1]]), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
