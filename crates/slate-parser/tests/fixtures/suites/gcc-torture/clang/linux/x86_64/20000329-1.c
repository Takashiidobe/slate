// SLATE-FILECHECK-DEFINES DEFAULT

int giop_tx_big_endian;

inline
void
giop_encode_ulong (unsigned long i, char *buf)
{
  if (giop_tx_big_endian)
    {
      *(unsigned long *) buf = i;
    }
  else
    {
      *buf++ = i & 0xff;
      *buf++ = (i >> 8) & 0xff;
      *buf++ = (i >> 16) & 0xff;
      *buf = (i >> 24) & 0xff;
    }
}



static
double
time_giop_encode (unsigned long l)
{
  int c;
  char buf[4];

  for (c = 0; c < (512 * 1024 * 1024); ++c)
    {
      giop_encode_ulong (l, buf);
    }
}

int
main (int ac, char *av[])
{
  giop_tx_big_endian = 1;
  time_giop_encode (0);
}

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
// DEFAULT-NEXT:     global %[[VALUE_giop_tx_big_endian:[0-9]+]] giop_tx_big_endian: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_giop_encode_ulong:[0-9]+]] @giop_encode_ulong(%[[VALUE_i:[0-9]+]] i: u64, %[[VALUE_buf:[0-9]+]] buf: ptr<i8>) -> void [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_giop_tx_big_endian]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(deref(pointer_cast<ptr<u64>, reason=explicit>(read<ptr<i8>>(%[[VALUE_buf]]))), read<u64>(%[[VALUE_i]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_buf]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_buf]], read<ptr<i8>>(%[[VALUE1]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE0]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(and<u64>(read<u64>(%[[VALUE_i]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))))));
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_buf]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_buf]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE2]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_i]]), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))))));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_buf]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_buf]], read<ptr<i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE4]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_i]]), const<i32>(16)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))))));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE_buf]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_i]]), const<i32>(24)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_time_giop_encode:[0-9]+]] @time_giop_encode(%[[VALUE_l:[0-9]+]] l: u64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_buf_2:[0-9]+]] buf: array<i8, 4> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_c]]), mul<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(512), const<i32>(1024)), const<i32>(1024)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(u64, ptr<i8>) -> void>(%[[VALUE_giop_encode_ulong]], read<u64>(%[[VALUE_l]]), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_buf_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_ac:[0-9]+]] ac: i32, %[[VALUE_av:[0-9]+]] av: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_giop_tx_big_endian]], const<i32>(1));
// DEFAULT-NEXT:         call<f64, signature=fn(u64) -> f64>(%[[VALUE_time_giop_encode]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
