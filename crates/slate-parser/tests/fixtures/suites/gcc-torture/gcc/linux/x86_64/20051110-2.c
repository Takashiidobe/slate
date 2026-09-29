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
// DEFAULT-NEXT:     global %[[VALUE_bytes:[0-9]+]] bytes: array<u8, 5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_unwind_adjustsp:[0-9]+]] @add_unwind_adjustsp(%[[VALUE_offset:[0-9]+]] offset: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_o]], reinterpret<u64, reason=assign, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(sub<i64, overflow=ub>(read<i64>(%[[VALUE_offset]]), widen<i64, reason=usual_arith>(const<i32>(516))), const<i32>(2))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n]], const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %[[VALUE_a:[0-9]+]] a:
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%[[VALUE_bytes]]), read<i32>(%[[VALUE_n]]))), truncate<u8, reason=assign, fits=unknown>(and<u64>(read<u64>(%[[VALUE_o]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(127))))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_o]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE1]]), const<i32>(7));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_o]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%[[VALUE_o]]), const<u64>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%[[VALUE_bytes]]), read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE3]])));
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE4]]))), const<i32>(128))));
// DEFAULT-NEXT:                         write<u8>(deref(read<ptr<u8>>(%[[VALUE3]])), read<u8>(%[[VALUE5]]));
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:                             goto %[[VALUE_a]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%[[VALUE_o]]), const<u64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_add_unwind_adjustsp]], widen<i64, reason=arg>(const<i32>(4132)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%[[VALUE_bytes]]), const<i32>(0)))))), const<i32>(136)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(%[[VALUE_bytes]]), const<i32>(1)))))), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
