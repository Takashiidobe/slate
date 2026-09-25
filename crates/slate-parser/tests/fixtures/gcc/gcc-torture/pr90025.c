/* PR middle-end/90025 */

__attribute__((noipa)) void bar(char *p) {
  int i;
  for (i = 0; i < 6; i++)
    if (p[i] != "foobar"[i])
      __builtin_abort();
  for (; i < 32; i++)
    if (p[i] != '\0')
      __builtin_abort();
}

__attribute__((noipa)) void foo(__UINT32_TYPE__ x) {
  char s[32]                = {'f', 'o', 'o', 'b', 'a', 'r', 0};
  ((__UINT32_TYPE__ *)s)[2] = __builtin_bswap32(x);
  bar(s);
}

int main() {
  foo(0);
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
// DEFAULT-NEXT:     global %8 .str8: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([102, 111, 111, 98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @bar(%1 p: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%2), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%1), read<i32>(%2))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%8), read<i32>(%2))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%2), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%1), read<i32>(%2))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%4 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 s: array<i8, 32> [storage=automatic] = aggregate<array<i8, 32>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(102)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(111)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(111)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(98)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(114)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(pointer_cast<ptr<u32>, reason=explicit>(array_decay<ptr<i8>, length=Some(32)>(%5)), const<i32>(2))), call<u32, signature=fn(u32) -> u32>(__builtin_bswap32, read<u32>(%4)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(__builtin_bswap32, read<u32>(%4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%0, array_decay<ptr<i8>, length=Some(32)>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
