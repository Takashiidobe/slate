/* extracted from gdb sources */

typedef unsigned long long CORE_ADDR;

struct blockvector;

struct symtab {
  struct blockvector *blockvector;
};

struct sec {
  void *unused;
};

struct symbol {
  int   len;
  char *name;
};

struct block {
  CORE_ADDR      startaddr, endaddr;
  struct symbol *function;
  struct block  *superblock;
  unsigned char  gcc_compile_flag;
  int            nsyms;
  struct symbol  syms[1];
};

struct blockvector {
  int           nblocks;
  struct block *block[2];
};

struct blockvector *blockvector_for_pc_sect(register CORE_ADDR pc,
                                            struct symtab     *symtab) {
  register struct block *b;
  register int           bot, top, half;
  struct blockvector    *bl;

  bl = symtab->blockvector;
  b  = bl->block[0];

  bot = 0;
  top = bl->nblocks;

  while (top - bot > 1) {
    half = (top - bot + 1) >> 1;
    b    = bl->block[bot + half];
    if (b->startaddr <= pc)
      bot += half;
    else
      top = bot + half;
  }

  while (bot >= 0) {
    b = bl->block[bot];
    if (b->endaddr > pc) {
      return bl;
    }
    bot--;
  }
  return 0;
}

int main(void) {
  struct block       a  = {0, 0x10000, 0, 0, 1, 20};
  struct block       b  = {0x10000, 0x20000, 0, 0, 1, 20};
  struct blockvector bv = {2, {&a, &b}};
  struct symtab      s  = {&bv};

  struct blockvector *ret;

  ret = blockvector_for_pc_sect(0x500, &s);

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
// DEFAULT-NEXT:     type @type0 CORE_ADDR = u64;
// DEFAULT-NEXT:     type @type1 blockvector = struct {
// DEFAULT-NEXT:         field0 nblocks: i32;
// DEFAULT-NEXT:         field1 block: array<ptr<@type5>, 2>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 symtab = struct {
// DEFAULT-NEXT:         field0 blockvector: ptr<@type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 sec = struct {
// DEFAULT-NEXT:         field0 unused: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type4 symbol = struct {
// DEFAULT-NEXT:         field0 len: i32;
// DEFAULT-NEXT:         field1 name: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type5 block = struct {
// DEFAULT-NEXT:         field0 startaddr: u64;
// DEFAULT-NEXT:         field1 endaddr: u64;
// DEFAULT-NEXT:         field2 function: ptr<@type4>;
// DEFAULT-NEXT:         field3 superblock: ptr<@type5>;
// DEFAULT-NEXT:         field4 gcc_compile_flag: u8;
// DEFAULT-NEXT:         field5 nsyms: i32;
// DEFAULT-NEXT:         field6 syms: array<@type4, 1>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8, 16, 24, 32, 36, 40]];
// DEFAULT-NEXT:     fn %6 @blockvector_for_pc_sect(%7 pc: u64, %8 symtab: ptr<@type2>) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 b: ptr<@type5> [storage=automatic];
// DEFAULT-NEXT:         let %10 bot: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 top: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 half: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 bl: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type1>>(%13, read<ptr<@type1>>(field0(deref(read<ptr<@type2>>(%8)))));
// DEFAULT-NEXT:         write<ptr<@type5>>(%9, read<ptr<@type5>>(deref(ptr_offset<ptr<ptr<@type5>>, subtract=false, element=ptr<@type5>, overflow=ub>(array_decay<ptr<ptr<@type5>>, length=Some(2)>(field1(deref(read<ptr<@type1>>(%13)))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(field0(deref(read<ptr<@type1>>(%13)))));
// DEFAULT-NEXT:         while %20 gt<i32>(sub<i32, overflow=ub>(read<i32>(%11), read<i32>(%10)), const<i32>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%12, shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%11), read<i32>(%10)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<@type5>>(%9, read<ptr<@type5>>(deref(ptr_offset<ptr<ptr<@type5>>, subtract=false, element=ptr<@type5>, overflow=ub>(array_decay<ptr<ptr<@type5>>, length=Some(2)>(field1(deref(read<ptr<@type1>>(%13)))), add<i32, overflow=ub>(read<i32>(%10), read<i32>(%12))))));
// DEFAULT-NEXT:                 if le<u64>(read<u64>(field0(deref(read<ptr<@type5>>(%9)))), read<u64>(%7))
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                     let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), read<i32>(%12));
// DEFAULT-NEXT:                     write<i32>(%10, read<i32>(%23));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<i32>(%11, add<i32, overflow=ub>(read<i32>(%10), read<i32>(%12)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while %21 ge<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type5>>(%9, read<ptr<@type5>>(deref(ptr_offset<ptr<ptr<@type5>>, subtract=false, element=ptr<@type5>, overflow=ub>(array_decay<ptr<ptr<@type5>>, length=Some(2)>(field1(deref(read<ptr<@type1>>(%13)))), read<i32>(%10)))));
// DEFAULT-NEXT:                 if gt<u64>(read<u64>(field1(deref(read<ptr<@type5>>(%9)))), read<u64>(%7))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         return read<ptr<@type1>>(%13);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%25));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return null<ptr<@type1>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 a: @type5 [storage=automatic] = aggregate<@type5, zero_fill=true>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(65536))), field2 = null<ptr<@type4>>, field3 = null<ptr<@type5>>, field4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field5 = const<i32>(20));
// DEFAULT-NEXT:         let %16 b: @type5 [storage=automatic] = aggregate<@type5, zero_fill=true>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(65536))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(131072))), field2 = null<ptr<@type4>>, field3 = null<ptr<@type5>>, field4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field5 = const<i32>(20));
// DEFAULT-NEXT:         let %17 bv: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(2), field1 = aggregate<array<ptr<@type5>, 2>, zero_fill=false>(index0 = addr_of<ptr<@type5>>(%15), index1 = addr_of<ptr<@type5>>(%16)));
// DEFAULT-NEXT:         let %18 s: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = addr_of<ptr<@type1>>(%17));
// DEFAULT-NEXT:         let %19 ret: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type1>>(%19, call<ptr<@type1>, signature=fn(u64, ptr<@type2>) -> ptr<@type1>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1280))), addr_of<ptr<@type2>>(%18)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(u64, ptr<@type2>) -> ptr<@type1>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1280))), addr_of<ptr<@type2>>(%18));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
