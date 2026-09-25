/* This used to generate unaligned accesses at -O2 because of IVOPTS.  */

struct packed_struct {
  struct packed_struct1 {
    unsigned char cc11;
    unsigned char cc12;
  } __attribute__((packed)) pst1;
  struct packed_struct2 {
    unsigned char  cc21;
    unsigned char  cc22;
    unsigned short ss[104];
    unsigned char  cc23[13];
  } __attribute__((packed)) pst2[4];
} __attribute__((packed));

typedef struct {
  int                  ii;
  struct packed_struct buf;
} info_t;

static unsigned short g;

static void __attribute__((noinline)) dummy(unsigned short s) { g = s; }

static int foo(info_t *info) {
  int i, j;

  for (i = 0; i < info->buf.pst1.cc11; i++)
    for (j = 0; j < info->buf.pst2[i].cc22; j++)
      dummy(info->buf.pst2[i].ss[j]);

  return 0;
}

int main(void) {
  info_t info;
  info.buf.pst1.cc11    = 2;
  info.buf.pst2[0].cc22 = info.buf.pst2[1].cc22 = 8;
  return foo(&info);
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
// DEFAULT-NEXT:     type @type0 packed_struct = struct {
// DEFAULT-NEXT:         field0 pst1: @type1;
// DEFAULT-NEXT:         field1 pst2: array<@type2, 4>;
// DEFAULT-NEXT:     } [size=894, align=1, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type1 packed_struct1 = struct {
// DEFAULT-NEXT:         field0 cc11: u8;
// DEFAULT-NEXT:         field1 cc12: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type2 packed_struct2 = struct {
// DEFAULT-NEXT:         field0 cc21: u8;
// DEFAULT-NEXT:         field1 cc22: u8;
// DEFAULT-NEXT:         field2 ss: array<u16, 104>;
// DEFAULT-NEXT:         field3 cc23: array<u8, 13>;
// DEFAULT-NEXT:     } [size=223, align=1, offsets=[0, 1, 2, 210]];
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 ii: i32;
// DEFAULT-NEXT:         field1 buf: @type0;
// DEFAULT-NEXT:     } [size=900, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type4 info_t = @type3;
// DEFAULT-NEXT:     global %5 g: u16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %6 @dummy(%7 s: u16) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u16>(%5, read<u16>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @foo(%9 info: ptr<@type3>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(field1(deref(read<ptr<@type3>>(%9)))))))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %15
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%11), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(field1(deref(read<ptr<@type3>>(%9))))), read<i32>(%10))))))))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %18: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%11, read<i32>(%19));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         call<void, signature=fn(u16) -> void>(%6, read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(104)>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(field1(deref(read<ptr<@type3>>(%9))))), read<i32>(%10))))), read<i32>(%11)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 info: @type3 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(field0(field0(field1(%13))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         write<u8>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(field1(%13))), const<i32>(1)))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         write<u8>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(field1(%13))), const<i32>(0)))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<@type3>) -> i32>(%8, addr_of<ptr<@type3>>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
