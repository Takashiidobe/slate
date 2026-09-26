/* PR tree-optimization/125291 */

char         buf[1111];
char        *archive_le16dec_filename = buf;
unsigned int archive_le16dec_end, archive_le16dec_fn_end,
    archive_le16dec_filename_size, archive_le16dec_offset;
char archive_le16dec_p[] = {21, 0x7f};

[[gnu::noipa]]
void archive_le16dec() {
  archive_le16dec_filename_size = (short)archive_le16dec_filename_size;
  unsigned char flagbits        = 0, flagbyte;
  archive_le16dec_end           = archive_le16dec_filename_size;
  archive_le16dec_fn_end        = archive_le16dec_filename_size * 2;
  archive_le16dec_filename_size = flagbits = 0;
  while (archive_le16dec_offset < archive_le16dec_end &&
         archive_le16dec_filename_size < archive_le16dec_fn_end) {
    if (!flagbits) {
      flagbyte = archive_le16dec_p[archive_le16dec_offset++];
      flagbits = 8;
    }
    flagbits -= 2;
    if (!(flagbyte >> flagbits & 3))
      archive_le16dec_filename_size++;
  }
}

int main() {
  archive_le16dec_filename_size = 2;
  archive_le16dec();
  if (archive_le16dec_filename_size != 1)
    __builtin_trap();
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
// DEFAULT-NEXT:     global %0 buf: array<i8, 1111> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %1 archive_le16dec_filename: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(1111)>(%0) [linkage=external];
// DEFAULT-NEXT:     global %2 archive_le16dec_end: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 archive_le16dec_fn_end: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 archive_le16dec_filename_size: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 archive_le16dec_offset: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 archive_le16dec_p: array<i8, 2> [storage=static] = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(21)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(127))) [linkage=external];
// DEFAULT-NEXT:     fn %7 @archive_le16dec() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(%4, reinterpret<u32, reason=assign, fits=unknown>(widen<i32, reason=assign>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(read<u32>(%4))))));
// DEFAULT-NEXT:         let %8 flagbits: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %9 flagbyte: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%2, read<u32>(%4));
// DEFAULT-NEXT:         write<u32>(%3, mul<u32, overflow=wrap>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         write<u8>(%8, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%4, widen<u32, reason=assign>(reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         while %11 logical_and<bool>(lt<u32>(read<u32>(%5), read<u32>(%2)), lt<u32>(read<u32>(%4), read<u32>(%3)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<u8>(read<u8>(%8), const<u8>(0)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %13: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                         let %14: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%13), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(%5, read<u32>(%14));
// DEFAULT-NEXT:                         write<u8>(%9, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), read<u32>(%13))))));
// DEFAULT-NEXT:                         write<u8>(%8, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %15: u8 [synthetic] = read<u8>(%8);
// DEFAULT-NEXT:                 let %16: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%15))), const<i32>(2))));
// DEFAULT-NEXT:                 write<u8>(%8, read<u8>(%16));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%9))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8)))), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:                     let %17: u32 [synthetic] = read<u32>(%4);
// DEFAULT-NEXT:                     let %18: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%17), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%4, read<u32>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_trap() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(%4, reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
