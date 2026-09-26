void add_unwind_adjustsp(long);
void abort(void);

unsigned char bytes[5];

int flag;

void add_unwind_adjustsp(long offset) {
  int           n;
  unsigned long o;

  o = (long)((offset - 0x204) >> 2);

  n = 0;
  do {
  a:
    bytes[n]   = o & 0x7f;
    o        >>= 7;
    if (o) {
      bytes[n] |= 0x80;
      if (flag)
        goto a;
    }
    n++;
  } while (o);
}

int main(void) {
  add_unwind_adjustsp(4132);
  if (bytes[0] != 0x88 || bytes[1] != 0x07)
    abort();
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
// DEFAULT-NEXT:     global %2 bytes: array<u8, 5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 flag: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @add_unwind_adjustsp(%5 offset: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 n: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 o: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%7, reinterpret<u64, reason=assign, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(sub<i64, overflow=ub>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(516))), const<i32>(2))));
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:         do %10
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %4 a:
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%2), read<i32>(%6))), truncate<u8, reason=assign, fits=unknown>(and<u64>(read<u64>(%7), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(127))))));
// DEFAULT-NEXT:                 let %11: u64 [synthetic] = read<u64>(%7);
// DEFAULT-NEXT:                 let %12: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%11), const<i32>(7));
// DEFAULT-NEXT:                 write<u64>(%7, read<u64>(%12));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%7), const<u64>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %13: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%2), read<i32>(%6));
// DEFAULT-NEXT:                         let %14: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%13)));
// DEFAULT-NEXT:                         let %15: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(128))));
// DEFAULT-NEXT:                         write<u8>(deref(read<ptr<u8>>(%13)), read<u8>(%15));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                             goto %4;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%17));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%7), const<u64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%0, widen<i64, reason=arg>(const<i32>(4132)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%2), const<i32>(0)))))), const<i32>(136)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%2), const<i32>(1)))))), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
