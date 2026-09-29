void abort(void);
void exit(int);

typedef unsigned short Uint16;
typedef unsigned int   Uint;

Uint f() {
  Uint16        token;
  Uint          count;
  static Uint16 values[1] = {0x9300};

  token = values[0];
  count = token >> 8;

  return count;
}

int main() {
  if (f() != 0x93)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_Uint16:[0-9]+]] Uint16 = u16;
// DEFAULT-NEXT:     type @type[[TYPE_Uint:[0-9]+]] Uint = u32;
// DEFAULT-NEXT:     global %[[VALUE_values:[0-9]+]] values: array<u16, 1> [storage=static] = aggregate<array<u16, 1>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(37632)))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_token:[0-9]+]] token: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(%[[VALUE_token]], read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1)>(%[[VALUE_values]]), const<i32>(0)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_count]], reinterpret<u32, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_token]]))), const<i32>(8))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_count]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_f]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(147)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
