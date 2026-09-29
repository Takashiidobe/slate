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
// DEFAULT-NEXT:     type @type[[TYPE_CORE_ADDR:[0-9]+]] CORE_ADDR = u64;
// DEFAULT-NEXT:     type @type[[TYPE_blockvector:[0-9]+]] blockvector = struct {
// DEFAULT-NEXT:         field0 nblocks: i32;
// DEFAULT-NEXT:         field1 block: array<ptr<@type[[TYPE_block:[0-9]+]]>, 2>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_symtab:[0-9]+]] symtab = struct {
// DEFAULT-NEXT:         field0 blockvector: ptr<@type[[TYPE_blockvector]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_sec:[0-9]+]] sec = struct {
// DEFAULT-NEXT:         field0 unused: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_symbol:[0-9]+]] symbol = struct {
// DEFAULT-NEXT:         field0 len: i32;
// DEFAULT-NEXT:         field1 name: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_block]] block = struct {
// DEFAULT-NEXT:         field0 startaddr: u64;
// DEFAULT-NEXT:         field1 endaddr: u64;
// DEFAULT-NEXT:         field2 function: ptr<@type[[TYPE_symbol]]>;
// DEFAULT-NEXT:         field3 superblock: ptr<@type[[TYPE_block]]>;
// DEFAULT-NEXT:         field4 gcc_compile_flag: u8;
// DEFAULT-NEXT:         field5 nsyms: i32;
// DEFAULT-NEXT:         field6 syms: array<@type[[TYPE_symbol]], 1>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8, 16, 24, 32, 36, 40]];
// DEFAULT-NEXT:     fn %[[VALUE_blockvector_for_pc_sect:[0-9]+]] @blockvector_for_pc_sect(%[[VALUE_pc:[0-9]+]] pc: u64, %[[VALUE_symtab:[0-9]+]] symtab: ptr<@type[[TYPE_symtab]]>) -> ptr<@type[[TYPE_blockvector]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_block]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bot:[0-9]+]] bot: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_top:[0-9]+]] top: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_half:[0-9]+]] half: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bl:[0-9]+]] bl: ptr<@type[[TYPE_blockvector]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_bl]], read<ptr<@type[[TYPE_blockvector]]>>(field0(deref(read<ptr<@type[[TYPE_symtab]]>>(%[[VALUE_symtab]])))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_block]]>>(%[[VALUE_b]], read<ptr<@type[[TYPE_block]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_block]]>>, subtract=false, element=ptr<@type[[TYPE_block]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_block]]>>, length=Some(2)>(field1(deref(read<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_bl]])))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_bot]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_top]], read<i32>(field0(deref(read<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_bl]])))));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] gt<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_top]]), read<i32>(%[[VALUE_bot]])), const<i32>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_half]], shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_top]]), read<i32>(%[[VALUE_bot]])), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_block]]>>(%[[VALUE_b]], read<ptr<@type[[TYPE_block]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_block]]>>, subtract=false, element=ptr<@type[[TYPE_block]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_block]]>>, length=Some(2)>(field1(deref(read<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_bl]])))), add<i32, overflow=ub>(read<i32>(%[[VALUE_bot]]), read<i32>(%[[VALUE_half]]))))));
// DEFAULT-NEXT:                 if le<u64>(read<u64>(field0(deref(read<ptr<@type[[TYPE_block]]>>(%[[VALUE_b]])))), read<u64>(%[[VALUE_pc]]))
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_bot]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), read<i32>(%[[VALUE_half]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_bot]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_top]], add<i32, overflow=ub>(read<i32>(%[[VALUE_bot]]), read<i32>(%[[VALUE_half]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] ge<i32>(read<i32>(%[[VALUE_bot]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_block]]>>(%[[VALUE_b]], read<ptr<@type[[TYPE_block]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_block]]>>, subtract=false, element=ptr<@type[[TYPE_block]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_block]]>>, length=Some(2)>(field1(deref(read<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_bl]])))), read<i32>(%[[VALUE_bot]])))));
// DEFAULT-NEXT:                 if gt<u64>(read<u64>(field1(deref(read<ptr<@type[[TYPE_block]]>>(%[[VALUE_b]])))), read<u64>(%[[VALUE_pc]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         return read<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_bl]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_bot]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_bot]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return null<ptr<@type[[TYPE_blockvector]]>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_block]] [storage=automatic] = aggregate<@type[[TYPE_block]], zero_fill=true>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(65536))), field2 = null<ptr<@type[[TYPE_symbol]]>>, field3 = null<ptr<@type[[TYPE_block]]>>, field4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field5 = const<i32>(20));
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_block]] [storage=automatic] = aggregate<@type[[TYPE_block]], zero_fill=true>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(65536))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(131072))), field2 = null<ptr<@type[[TYPE_symbol]]>>, field3 = null<ptr<@type[[TYPE_block]]>>, field4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field5 = const<i32>(20));
// DEFAULT-NEXT:         let %[[VALUE_bv:[0-9]+]] bv: @type[[TYPE_blockvector]] [storage=automatic] = aggregate<@type[[TYPE_blockvector]], zero_fill=false>(field0 = const<i32>(2), field1 = aggregate<array<ptr<@type[[TYPE_block]]>, 2>, zero_fill=false>(index0 = addr_of<ptr<@type[[TYPE_block]]>>(%[[VALUE_a]]), index1 = addr_of<ptr<@type[[TYPE_block]]>>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_symtab]] [storage=automatic] = aggregate<@type[[TYPE_symtab]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_bv]]));
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: ptr<@type[[TYPE_blockvector]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_ret]], call<ptr<@type[[TYPE_blockvector]]>, signature=fn(u64, ptr<@type[[TYPE_symtab]]>) -> ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_blockvector_for_pc_sect]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1280))), addr_of<ptr<@type[[TYPE_symtab]]>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_blockvector]]>, signature=fn(u64, ptr<@type[[TYPE_symtab]]>) -> ptr<@type[[TYPE_blockvector]]>>(%[[VALUE_blockvector_for_pc_sect]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1280))), addr_of<ptr<@type[[TYPE_symtab]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
