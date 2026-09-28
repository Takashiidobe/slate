#include <stdio.h>

int main(void) {
  static const unsigned char data[] = {
#embed "c23_embed.bin"
  };
  static const unsigned char framed[] = {0xAA,
#embed "c23_embed.bin" prefix(0xBB, ) suffix(, 0xCC)
                                         , 0xDD};
  static const unsigned char empty[] = {
#embed "c23_embed_empty.bin" if_empty(0xEE)
  };
  for (size_t i = 0; i < sizeof(data); i++) {
    putchar(data[i]);
  }
  int framed_ok = sizeof(framed) == 8 && framed[0] == 0xAA &&
                  framed[1] == 0xBB && framed[2] == 'C' && framed[3] == '2' &&
                  framed[4] == '3' && framed[5] == '\n' && framed[6] == 0xCC &&
                  framed[7] == 0xDD;
  int empty_ok  = sizeof(empty) == 1 && empty[0] == 0xEE;
  return sizeof(data) == 4 && data[0] == 'C' && data[1] == '2' &&
                 data[2] == '3' && data[3] == '\n' && framed_ok && empty_ok
             ? 0
             : 1;
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %4 data: array<u8, 4> [storage=static] [const] = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(50))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(51))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(10)))) [linkage=internal];
// DEFAULT-NEXT:     global %5 framed: array<u8, 8> [storage=static] [const] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(170))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(187))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(50))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(51))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(10))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(204))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(221)))) [linkage=internal];
// DEFAULT-NEXT:     global %6 empty: array<u8, 1> [storage=static] [const] = aggregate<array<u8, 1>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(238)))) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @putchar(%10 __c: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %7 i: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%7), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: u64 [synthetic] = read<u64>(%7);
// DEFAULT-NEXT:                 let %13: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%7, read<u64>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%2, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(4)>(%4), read<u64>(%7)))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %8 framed_ok: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(0)))))), const<i32>(170))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(1)))))), const<i32>(187))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(2)))))), const<i32>(67))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(3)))))), const<i32>(50))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(4)))))), const<i32>(51))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(5)))))), const<i32>(10))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(6)))))), const<i32>(204))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(8)>(%5), const<i32>(7)))))), const<i32>(221))));
// DEFAULT-NEXT:         let %9 empty_ok: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(eq<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(1)>(%6), const<i32>(0)))))), const<i32>(238))));
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(4)>(%4), const<i32>(0)))))), const<i32>(67))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(4)>(%4), const<i32>(1)))))), const<i32>(50))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(4)>(%4), const<i32>(2)))))), const<i32>(51))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(4)>(%4), const<i32>(3)))))), const<i32>(10))), ne<i32>(read<i32>(%8), const<i32>(0))), ne<i32>(read<i32>(%9), const<i32>(0))), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
