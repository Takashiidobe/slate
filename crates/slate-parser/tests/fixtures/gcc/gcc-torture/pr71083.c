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
// DEFAULT-NEXT:     type @type0 uint32_t = u32;
// DEFAULT-NEXT:     type @type1 lock_chain = struct {
// DEFAULT-NEXT:         field0 irq_context: u32 : 2;
// DEFAULT-NEXT:         field1 depth: u32 : 6;
// DEFAULT-NEXT:         field2 base: u32 : 24;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(2), Some(8)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type2 lock_chain1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 base: u16;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %9 test: array<@type1, 101> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 test1: array<@type2, 101> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 chain: ptr<@type1>) -> ptr<@type1> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(bitfield2<unit=0, bytes=0..4, bits=8..32>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(read<ptr<@type1>>(%3), add<i32, overflow=ub>(read<i32>(%4), const<i32>(1))))), reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=8..32>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(read<ptr<@type1>>(%3), read<i32>(%4))))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type1>>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 chain: ptr<@type2>) -> ptr<@type2> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u16>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%7), add<i32, overflow=ub>(read<i32>(%8), const<i32>(1))))), read<u16>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%7), read<i32>(%8))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type2>>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<@type1>) -> ptr<@type1>>(%2, array_decay<ptr<@type1>, length=Some(101)>(%9));
// DEFAULT-NEXT:         call<ptr<@type2>, signature=fn(ptr<@type2>) -> ptr<@type2>>(%6, array_decay<ptr<@type2>, length=Some(101)>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
