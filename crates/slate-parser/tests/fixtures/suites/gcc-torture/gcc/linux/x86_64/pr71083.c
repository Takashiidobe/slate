__extension__ typedef __UINT32_TYPE__ uint32_t;

struct lock_chain {
  uint32_t irq_context : 2, depth : 6, base : 24;
};

__attribute__((noinline, noclone)) struct lock_chain *
foo(struct lock_chain *chain) {
  int i;
  for (i = 0; i < 100; i++) {
    chain[i + 1].base = chain[i].base;
  }
  return chain;
}

struct lock_chain1 {
  char           x;
  unsigned short base;
} __attribute__((packed));

__attribute__((noinline, noclone)) struct lock_chain1 *
bar(struct lock_chain1 *chain) {
  int i;
  for (i = 0; i < 100; i++) {
    chain[i + 1].base = chain[i].base;
  }
  return chain;
}

struct lock_chain  test[101];
struct lock_chain1 test1[101];

int main() {
  foo(test);
  bar(test1);
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
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_lock_chain:[0-9]+]] lock_chain = struct {
// DEFAULT-NEXT:         field0 irq_context: u32 : 2;
// DEFAULT-NEXT:         field1 depth: u32 : 6;
// DEFAULT-NEXT:         field2 base: u32 : 24;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(2), Some(8)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_lock_chain1:[0-9]+]] lock_chain1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 base: u16;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %[[VALUE_test:[0-9]+]] test: array<@type[[TYPE_lock_chain]], 101> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_test1:[0-9]+]] test1: array<@type[[TYPE_lock_chain1]], 101> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_chain:[0-9]+]] chain: ptr<@type[[TYPE_lock_chain]]>) -> ptr<@type[[TYPE_lock_chain]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(bitfield2<unit=0, bytes=0..4, bits=8..32>(deref(ptr_offset<ptr<@type[[TYPE_lock_chain]]>, subtract=false, element=@type[[TYPE_lock_chain]], overflow=ub>(read<ptr<@type[[TYPE_lock_chain]]>>(%[[VALUE_chain]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1))))), reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=8..32>(deref(ptr_offset<ptr<@type[[TYPE_lock_chain]]>, subtract=false, element=@type[[TYPE_lock_chain]], overflow=ub>(read<ptr<@type[[TYPE_lock_chain]]>>(%[[VALUE_chain]]), read<i32>(%[[VALUE_i]]))))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_lock_chain]]>>(%[[VALUE_chain]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_chain_2:[0-9]+]] chain: ptr<@type[[TYPE_lock_chain1]]>) -> ptr<@type[[TYPE_lock_chain1]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u16>(field1(deref(ptr_offset<ptr<@type[[TYPE_lock_chain1]]>, subtract=false, element=@type[[TYPE_lock_chain1]], overflow=ub>(read<ptr<@type[[TYPE_lock_chain1]]>>(%[[VALUE_chain_2]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(1))))), read<u16>(field1(deref(ptr_offset<ptr<@type[[TYPE_lock_chain1]]>, subtract=false, element=@type[[TYPE_lock_chain1]], overflow=ub>(read<ptr<@type[[TYPE_lock_chain1]]>>(%[[VALUE_chain_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_lock_chain1]]>>(%[[VALUE_chain_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_lock_chain]]>, signature=fn(ptr<@type[[TYPE_lock_chain]]>) -> ptr<@type[[TYPE_lock_chain]]>>(%[[VALUE_foo]], array_decay<ptr<@type[[TYPE_lock_chain]]>, length=Some(101)>(%[[VALUE_test]]));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_lock_chain1]]>, signature=fn(ptr<@type[[TYPE_lock_chain1]]>) -> ptr<@type[[TYPE_lock_chain1]]>>(%[[VALUE_bar]], array_decay<ptr<@type[[TYPE_lock_chain1]]>, length=Some(101)>(%[[VALUE_test1]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
