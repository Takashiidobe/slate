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
// DEFAULT-NEXT:     type @type0 inode = struct {
// DEFAULT-NEXT:         field0 i_size: i64;
// DEFAULT-NEXT:         field1 i_sb: ptr<@type1>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 super_block = struct {
// DEFAULT-NEXT:         field0 s_blocksize: i32;
// DEFAULT-NEXT:         field1 s_blocksize_bits: u8;
// DEFAULT-NEXT:         field2 s_hs: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type2 file = struct {
// DEFAULT-NEXT:         field0 f_pos: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %16 s: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 i: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 f: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%22 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @isofs_bread(%6 block: u32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%6), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @do_isofs_readdir(%8 inode: ptr<@type0>, %9 filp: ptr<@type2>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 bufsize: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type1>>(field1(deref(read<ptr<@type0>>(%8)))))));
// DEFAULT-NEXT:         let %11 bufbits: u8 [storage=automatic] = read<u8>(field1(deref(read<ptr<@type1>>(field1(deref(read<ptr<@type0>>(%8)))))));
// DEFAULT-NEXT:         let %12 block: u32 [storage=automatic];
// DEFAULT-NEXT:         let %13 offset: u32 [storage=automatic];
// DEFAULT-NEXT:         let %14 bh: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %15 hs: i32 [storage=automatic];
// DEFAULT-NEXT:         if ge<i64>(read<i64>(field0(deref(read<ptr<@type2>>(%9)))), read<i64>(field0(deref(read<ptr<@type0>>(%8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<u32>(%13, reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(and<i64>(read<i64>(field0(deref(read<ptr<@type2>>(%9)))), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(read<i32>(%10), const<i32>(1)))))));
// DEFAULT-NEXT:         write<u32>(%12, reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(field0(deref(read<ptr<@type2>>(%9)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11)))))));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(field2(deref(read<ptr<@type1>>(field1(deref(read<ptr<@type0>>(%8))))))));
// DEFAULT-NEXT:         while %23 lt<i64>(read<i64>(field0(deref(read<ptr<@type2>>(%9)))), read<i64>(field0(deref(read<ptr<@type0>>(%8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i8>>(read<ptr<i8>>(%14), null<ptr<i8>>))
// DEFAULT-NEXT:                     write<ptr<i8>>(%14, call<ptr<i8>, signature=fn(u32) -> ptr<i8>>(%5, read<u32>(%12)));
// DEFAULT-NEXT:                     call<ptr<i8>, signature=fn(u32) -> ptr<i8>>(%5, read<u32>(%12));
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%24)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%12), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))))));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%25));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%15), const<i32>(0))
// DEFAULT-NEXT:                     let %26: ptr<@type2> [synthetic] = read<ptr<@type2>>(%9);
// DEFAULT-NEXT:                     let %27: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type2>>(%26))));
// DEFAULT-NEXT:                     let %28: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%27), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(field0(deref(read<ptr<@type2>>(%26))), read<i64>(%28));
// DEFAULT-NEXT:                 if ge<u32>(read<u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%10)))
// DEFAULT-NEXT:                     let %29: u32 [synthetic] = read<u32>(%13);
// DEFAULT-NEXT:                     let %30: u32 [synthetic] = and<u32>(read<u32>(%29), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:                     write<u32>(%13, read<u32>(%30));
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(deref(read<ptr<i8>>(%14))), const<i8>(0))
// DEFAULT-NEXT:                     let %31: ptr<@type2> [synthetic] = read<ptr<@type2>>(%9);
// DEFAULT-NEXT:                     let %32: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type2>>(%31))));
// DEFAULT-NEXT:                     let %33: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%32), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(field0(deref(read<ptr<@type2>>(%31))), read<i64>(%33));
// DEFAULT-NEXT:                 let %34: ptr<@type2> [synthetic] = read<ptr<@type2>>(%9);
// DEFAULT-NEXT:                 let %35: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type2>>(%34))));
// DEFAULT-NEXT:                 let %36: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%35), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(deref(read<ptr<@type2>>(%34))), read<i64>(%36));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main(%20 argc: i32, %21 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field0(%16), const<i32>(512));
// DEFAULT-NEXT:         write<u8>(field1(%16), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))));
// DEFAULT-NEXT:         write<i64>(field0(%17), widen<i64, reason=assign>(const<i32>(2048)));
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(%17), addr_of<ptr<@type1>>(%16));
// DEFAULT-NEXT:         write<i64>(field0(%18), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type0>, ptr<@type2>) -> i32>(%7, addr_of<ptr<@type0>>(%17), addr_of<ptr<@type2>>(%18));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
