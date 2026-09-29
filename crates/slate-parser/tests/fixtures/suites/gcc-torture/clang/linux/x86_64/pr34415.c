const char *__attribute__((noinline)) foo(const char *p) {
  const char *end;
  int         len = 1;
  for (;;) {
    int c = *p;
    c     = (c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c);
    if (c == 'B')
      end = p;
    else if (c == 'A') {
      end = p;
      do
        p++;
      while (*p == '+');
    } else
      break;
    p++;
    len++;
  }
  if (len > 2 && *p == ':')
    p = end;
  return p;
}

int main(void) {
  const char *input = "Bbb:";
  return foo(input) != input + 2;
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([66, 98, 98, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<const i8>) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_end:[0-9]+]] end: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = widen<i32, reason=assign>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_c]], conditional<i32>(logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_c]]), const<i32>(97)), le<i32>(read<i32>(%[[VALUE_c]]), const<i32>(122))), add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_c]]), const<i32>(97)), const<i32>(65)), read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE_c]]), const<i32>(66))
// DEFAULT-NEXT:                         write<ptr<const i8>>(%[[VALUE_end]], read<ptr<const i8>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_c]]), const<i32>(65))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<ptr<const i8>>(%[[VALUE_end]], read<ptr<const i8>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                                 do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                                     let %[[VALUE2:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                                     let %[[VALUE3:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<const i8>>(%[[VALUE_p]], read<ptr<const i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:                                 while eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_p]])))), const<i32>(43));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             break %[[VALUE0]];
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_p]], read<ptr<const i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_len]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_len]]), const<i32>(2)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_p]])))), const<i32>(58)))
// DEFAULT-NEXT:             write<ptr<const i8>>(%[[VALUE_p]], read<ptr<const i8>>(%[[VALUE_end]]));
// DEFAULT-NEXT:         return read<ptr<const i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_input:[0-9]+]] input: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<const i8>>(call<ptr<const i8>, signature=fn(ptr<const i8>) -> ptr<const i8>>(%[[VALUE_foo]], read<ptr<const i8>>(%[[VALUE_input]])), ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_input]]), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
