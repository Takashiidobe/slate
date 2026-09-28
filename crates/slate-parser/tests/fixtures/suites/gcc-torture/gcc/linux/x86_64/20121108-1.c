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
// DEFAULT-NEXT:     global %0 temp: array<i8, 16> [storage=static] [align=16] = code_units<array<i8, 16>>([49, 57, 50, 46, 49, 54, 56, 46, 49, 57, 48, 46, 49, 54, 48, 0]) [linkage=external];
// DEFAULT-NEXT:     global %1 result: u32 [storage=static] = or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(192), const<i32>(8)), const<u32>(168)), const<i32>(8)), const<u32>(190)), const<i32>(8)), const<u32>(160)) [linkage=external];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([87, 79, 82, 75, 83, 46, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %5 @strtoul1(%6 a: ptr<const i8>, %7 b: ptr<ptr<i8>>, %8 c: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%7)), pointer_cast<ptr<i8>, reason=assign>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%6), const<i32>(3))));
// DEFAULT-NEXT:         if eq<ptr<const i8>>(read<ptr<const i8>>(%6), pointer_cast<ptr<const i8>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(16)>(%0)))
// DEFAULT-NEXT:             return const<i32>(192);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<ptr<const i8>>(read<ptr<const i8>>(%6), pointer_cast<ptr<const i8>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%0), const<i32>(4))))
// DEFAULT-NEXT:                 return const<i32>(168);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 if eq<ptr<const i8>>(read<ptr<const i8>>(%6), pointer_cast<ptr<const i8>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%0), const<i32>(8))))
// DEFAULT-NEXT:                     return const<i32>(190);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if eq<ptr<const i8>>(read<ptr<const i8>>(%6), pointer_cast<ptr<const i8>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%0), const<i32>(12))))
// DEFAULT-NEXT:                         return const<i32>(160);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%21);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @string_to_ip(%11 s: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 addr: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 e: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %14 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<ptr<const i8>>(read<ptr<const i8>>(%11), null<ptr<const i8>>)
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %15 val: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %31: i32 [synthetic];
// DEFAULT-NEXT:                     if ne<ptr<const i8>>(read<ptr<const i8>>(%11), null<ptr<const i8>>)
// DEFAULT-NEXT:                         write<i32>(%31, call<i32, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) -> i32>(%5, read<ptr<const i8>>(%11), addr_of<ptr<ptr<i8>>>(%13), const<i32>(10)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%31, const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(%15, read<i32>(%31));
// DEFAULT-NEXT:                     let %32: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                     let %33: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%32), const<i32>(8));
// DEFAULT-NEXT:                     write<i32>(%12, read<i32>(%33));
// DEFAULT-NEXT:                     let %34: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                     let %35: i32 [synthetic] = or<i32>(read<i32>(%34), and<i32>(read<i32>(%15), const<i32>(255)));
// DEFAULT-NEXT:                     write<i32>(%12, read<i32>(%35));
// DEFAULT-NEXT:                     if ne<ptr<const i8>>(read<ptr<const i8>>(%11), null<ptr<const i8>>)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<const i8>>(%11, pointer_cast<ptr<const i8>, reason=assign>(conditional<ptr<i8>>(ne<i8>(read<i8>(deref(read<ptr<i8>>(%13))), const<i8>(0)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%13), const<i32>(1)), read<ptr<i8>>(%13))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @__builtin_printf(%24 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 t: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>) -> i32>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%26)), read<i32>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%27)), read<u32>(%1));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%17)), read<u32>(%1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%21);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%28)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
