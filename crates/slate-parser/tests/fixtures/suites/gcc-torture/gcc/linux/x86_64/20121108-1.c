char     temp[] = "192.168.190.160";
unsigned result = (((((192u << 8) | 168u) << 8) | 190u) << 8) | 160u;

int strtoul1(const char *a, char **b, int c) __attribute__((noinline, noclone));
int strtoul1(const char *a, char **b, int c) {
  *b = a + 3;
  if (a == temp)
    return 192;
  else if (a == temp + 4)
    return 168;
  else if (a == temp + 8)
    return 190;
  else if (a == temp + 12)
    return 160;
  __builtin_abort();
}

int string_to_ip(const char *s) __attribute__((noinline, noclone));
int string_to_ip(const char *s) {
  int   addr;
  char *e;
  int   i;

  if (s == 0)
    return (0);

  for (addr = 0, i = 0; i < 4; ++i) {
    int val   = s ? strtoul1(s, &e, 10) : 0;
    addr    <<= 8;
    addr     |= (val & 0xFF);
    if (s) {
      s = (*e) ? e + 1 : e;
    }
  }

  return addr;
}

int main(void) {
  int t = string_to_ip(temp);
  __builtin_printf("%x\n", t);
  __builtin_printf("%x\n", result);
  if (t != result)
    __builtin_abort();
  __builtin_printf("WORKS.\n");
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
// DEFAULT-NEXT:     global %[[VALUE_temp:[0-9]+]] temp: array<i8, 16> [storage=static] [align=16] = code_units<array<i8, 16>>([49, 57, 50, 46, 49, 54, 56, 46, 49, 57, 48, 46, 49, 54, 48, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_result:[0-9]+]] result: u32 [storage=static] = or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(192), const<i32>(8)), const<u32>(168)), const<i32>(8)), const<u32>(190)), const<i32>(8)), const<u32>(160)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([87, 79, 82, 75, 83, 46, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strtoul1:[0-9]+]] @strtoul1(%[[VALUE_a:[0-9]+]] a: ptr<const i8>, %[[VALUE_b:[0-9]+]] b: ptr<ptr<i8>>, %[[VALUE_c:[0-9]+]] c: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%[[VALUE_b]])), pointer_cast<ptr<i8>, reason=assign>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_a]]), const<i32>(3))));
// DEFAULT-NEXT:         if eq<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_temp]])))
// DEFAULT-NEXT:             return const<i32>(192);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_temp]]), const<i32>(4))))
// DEFAULT-NEXT:                 return const<i32>(168);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 if eq<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_temp]]), const<i32>(8))))
// DEFAULT-NEXT:                     return const<i32>(190);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if eq<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_temp]]), const<i32>(12))))
// DEFAULT-NEXT:                         return const<i32>(160);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort:[0-9]+]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_string_to_ip:[0-9]+]] @string_to_ip(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_addr:[0-9]+]] addr: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s]]), null<ptr<const i8>>)
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_addr]], const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_val:[0-9]+]] val: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:                     if ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s]]), null<ptr<const i8>>)
// DEFAULT-NEXT:                         write<i32>(%[[VALUE3]], call<i32, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) -> i32>(%[[VALUE_strtoul1]], read<ptr<const i8>>(%[[VALUE_s]]), addr_of<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(10)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%[[VALUE3]], const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_val]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_addr]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE4]]), const<i32>(8));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_addr]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_addr]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE6]]), and<i32>(read<i32>(%[[VALUE_val]]), const<i32>(255)));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_addr]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                     if ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s]]), null<ptr<const i8>>)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<const i8>>(%[[VALUE_s]], pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i8>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_e]]))), const<i8>(0)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_e]]), const<i32>(1)), read<ptr<i8>>(%[[VALUE_e]]))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_addr]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE8:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_string_to_ip]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_temp]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), read<i32>(%[[VALUE_t]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), read<u32>(%[[VALUE_result]]));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_t]])), read<u32>(%[[VALUE_result]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
