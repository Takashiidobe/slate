void abort(void);
void exit(int);

struct inode {
  long long           i_size;
  struct super_block *i_sb;
};

struct file {
  long long f_pos;
};

struct super_block {
  int           s_blocksize;
  unsigned char s_blocksize_bits;
  int           s_hs;
};

static char *isofs_bread(unsigned int block) {
  if (block)
    abort();
  exit(0);
}

static int do_isofs_readdir(struct inode *inode, struct file *filp) {
  int           bufsize = inode->i_sb->s_blocksize;
  unsigned char bufbits = inode->i_sb->s_blocksize_bits;
  unsigned int  block, offset;
  char         *bh = 0;
  int           hs;

  if (filp->f_pos >= inode->i_size)
    return 0;

  offset = filp->f_pos & (bufsize - 1);
  block  = filp->f_pos >> bufbits;
  hs     = inode->i_sb->s_hs;

  while (filp->f_pos < inode->i_size) {
    if (!bh)
      bh = isofs_bread(block);

    hs += block << bufbits;

    if (hs == 0)
      filp->f_pos++;

    if (offset >= bufsize)
      offset &= bufsize - 1;

    if (*bh)
      filp->f_pos++;

    filp->f_pos++;
  }
  return 0;
}

struct super_block s;
struct inode       i;
struct file        f;

int main(int argc, char **argv) {
  s.s_blocksize      = 512;
  s.s_blocksize_bits = 9;
  i.i_size           = 2048;
  i.i_sb             = &s;
  f.f_pos            = 0;

  do_isofs_readdir(&i, &f);
  abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_inode:[0-9]+]] inode = struct {
// DEFAULT-NEXT:         field0 i_size: i64;
// DEFAULT-NEXT:         field1 i_sb: ptr<@type[[TYPE_super_block:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_super_block]] super_block = struct {
// DEFAULT-NEXT:         field0 s_blocksize: i32;
// DEFAULT-NEXT:         field1 s_blocksize_bits: u8;
// DEFAULT-NEXT:         field2 s_hs: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_file:[0-9]+]] file = struct {
// DEFAULT-NEXT:         field0 f_pos: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_super_block]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: @type[[TYPE_inode]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: @type[[TYPE_file]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_isofs_bread:[0-9]+]] @isofs_bread(%[[VALUE_block:[0-9]+]] block: u32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_block]]), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_do_isofs_readdir:[0-9]+]] @do_isofs_readdir(%[[VALUE_inode:[0-9]+]] inode: ptr<@type[[TYPE_inode]]>, %[[VALUE_filp:[0-9]+]] filp: ptr<@type[[TYPE_file]]>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_bufsize:[0-9]+]] bufsize: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_super_block]]>>(field1(deref(read<ptr<@type[[TYPE_inode]]>>(%[[VALUE_inode]])))))));
// DEFAULT-NEXT:         let %[[VALUE_bufbits:[0-9]+]] bufbits: u8 [storage=automatic] = read<u8>(field1(deref(read<ptr<@type[[TYPE_super_block]]>>(field1(deref(read<ptr<@type[[TYPE_inode]]>>(%[[VALUE_inode]])))))));
// DEFAULT-NEXT:         let %[[VALUE_block_2:[0-9]+]] block: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_offset:[0-9]+]] offset: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bh:[0-9]+]] bh: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %[[VALUE_hs:[0-9]+]] hs: i32 [storage=automatic];
// DEFAULT-NEXT:         if ge<i64>(read<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE_filp]])))), read<i64>(field0(deref(read<ptr<@type[[TYPE_inode]]>>(%[[VALUE_inode]])))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_offset]], reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(and<i64>(read<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE_filp]])))), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_bufsize]]), const<i32>(1)))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_block_2]], reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE_filp]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_bufbits]])))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_hs]], read<i32>(field2(deref(read<ptr<@type[[TYPE_super_block]]>>(field1(deref(read<ptr<@type[[TYPE_inode]]>>(%[[VALUE_inode]]))))))));
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] lt<i64>(read<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE_filp]])))), read<i64>(field0(deref(read<ptr<@type[[TYPE_inode]]>>(%[[VALUE_inode]])))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_bh]]), null<ptr<i8>>))
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_bh]], call<ptr<i8>, signature=fn(u32) -> ptr<i8>>(%[[VALUE_isofs_bread]], read<u32>(%[[VALUE_block_2]])));
// DEFAULT-NEXT:                     call<ptr<i8>, signature=fn(u32) -> ptr<i8>>(%[[VALUE_isofs_bread]], read<u32>(%[[VALUE_block_2]]));
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_hs]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE2]])), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_block_2]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_bufbits]]))))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_hs]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_hs]]), const<i32>(0))
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<@type[[TYPE_file]]> [synthetic] = read<ptr<@type[[TYPE_file]]>>(%[[VALUE_filp]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE4]]))));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE5]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE4]]))), read<i64>(%[[VALUE6]]));
// DEFAULT-NEXT:                 if ge<u32>(read<u32>(%[[VALUE_offset]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_bufsize]])))
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_offset]]);
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: u32 [synthetic] = and<u32>(read<u32>(%[[VALUE7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_bufsize]]), const<i32>(1))));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_offset]], read<u32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_bh]]))), const<i8>(0))
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: ptr<@type[[TYPE_file]]> [synthetic] = read<ptr<@type[[TYPE_file]]>>(%[[VALUE_filp]]);
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE9]]))));
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE10]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE9]]))), read<i64>(%[[VALUE11]]));
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: ptr<@type[[TYPE_file]]> [synthetic] = read<ptr<@type[[TYPE_file]]>>(%[[VALUE_filp]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE12]]))));
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE13]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(deref(read<ptr<@type[[TYPE_file]]>>(%[[VALUE12]]))), read<i64>(%[[VALUE14]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_s]]), const<i32>(512));
// DEFAULT-NEXT:         write<u8>(field1(%[[VALUE_s]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))));
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_i]]), widen<i64, reason=assign>(const<i32>(2048)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_super_block]]>>(field1(%[[VALUE_i]]), addr_of<ptr<@type[[TYPE_super_block]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_f]]), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE_inode]]>, ptr<@type[[TYPE_file]]>) -> i32>(%[[VALUE_do_isofs_readdir]], addr_of<ptr<@type[[TYPE_inode]]>>(%[[VALUE_i]]), addr_of<ptr<@type[[TYPE_file]]>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
