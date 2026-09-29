void abort(void);
void exit(int);

unsigned bug(unsigned short value, unsigned short *buffer,
             unsigned short *bufend);

unsigned short buf[] = {1, 4, 16, 64, 256};
int            main() {
  if (bug(512, buf, buf + 3) != 491)
    abort();

  exit(0);
}

unsigned bug(unsigned short value, unsigned short *buffer,
             unsigned short *bufend) {
  unsigned short *tmp;

  for (tmp = buffer; tmp < bufend; tmp++)
    value -= *tmp;

  return value;
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
// DEFAULT-NEXT:     global %[[VALUE_buf:[0-9]+]] buf: array<u16, 5> [storage=static] = aggregate<array<u16, 5>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(4))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(16))), index3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(64))), index4 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(256)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bug:[0-9]+]] @bug(%[[VALUE_value:[0-9]+]] value: u16, %[[VALUE_buffer:[0-9]+]] buffer: ptr<u16>, %[[VALUE_bufend:[0-9]+]] bufend: ptr<u16>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: ptr<u16> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<u16>>(%[[VALUE_tmp]], read<ptr<u16>>(%[[VALUE_buffer]]));
// DEFAULT-NEXT:             condition: lt<ptr<u16>>(read<ptr<u16>>(%[[VALUE_tmp]]), read<ptr<u16>>(%[[VALUE_bufend]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<u16> [synthetic] = read<ptr<u16>>(%[[VALUE_tmp]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u16>>(%[[VALUE_tmp]], read<ptr<u16>>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_value]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE4]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(read<ptr<u16>>(%[[VALUE_tmp]]))))))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_value]], read<u16>(%[[VALUE5]]));
// DEFAULT-NEXT:         return widen<u32, reason=return>(read<u16>(%[[VALUE_value]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u16, ptr<u16>, ptr<u16>) -> u32>(%[[VALUE_bug]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(512))), array_decay<ptr<u16>, length=Some(5)>(%[[VALUE_buf]]), ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(5)>(%[[VALUE_buf]]), const<i32>(3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(491)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
