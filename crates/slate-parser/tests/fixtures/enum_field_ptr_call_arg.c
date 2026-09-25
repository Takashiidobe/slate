typedef enum { REPEAT_NONE, REPEAT_CHECK, REPEAT_VALID } FSE_repeat;

typedef struct {
  FSE_repeat mode;
} Entropy;

static unsigned select_type(FSE_repeat *repeatMode, unsigned count) {
  if (count == 0) {
    *repeatMode = REPEAT_NONE;
    return 0;
  }
  return (unsigned)*repeatMode + count;
}

int run(Entropy *e, unsigned count) {
  return (int)select_type(&e->mode, count);
}

int main(void) {
  Entropy e;
  e.mode = REPEAT_VALID;
  return run(&e, 0) + run(&e, 5);
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 REPEAT_NONE = const<i32>(0);
// DEFAULT-NEXT:         %1 REPEAT_CHECK = const<i32>(1);
// DEFAULT-NEXT:         %2 REPEAT_VALID = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 FSE_repeat = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 mode: @type0;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 Entropy = @type2;
// DEFAULT-NEXT:     fn %7 @select_type(%8 repeatMode: ptr<@type0>, %9 count: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%9), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<@type0>(deref(read<ptr<@type0>>(%8)), int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                 return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(enum_to_int<u32, reason=promotion>(read<@type0>(deref(read<ptr<@type0>>(%8)))), read<u32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @run(%11 e: ptr<@type2>, %12 count: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(ptr<@type0>, u32) -> u32>(%7, addr_of<ptr<@type0>>(field0(deref(read<ptr<@type2>>(%11)))), read<u32>(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 e: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(field0(%14), int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(ptr<@type2>, u32) -> i32>(%10, addr_of<ptr<@type2>>(%14), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), call<i32, signature=fn(ptr<@type2>, u32) -> i32>(%10, addr_of<ptr<@type2>>(%14), reinterpret<u32, reason=arg, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
