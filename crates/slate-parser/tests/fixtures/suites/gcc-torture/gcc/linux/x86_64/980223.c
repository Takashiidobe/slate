void abort(void);

typedef struct {
  char *addr;
  long  type;
} object;

object bar(object blah) { abort(); }

object foo(object x, object y) {
  object z = *(object *)(x.addr);
  if (z.type & 64) {
    y = *(object *)(z.addr + sizeof(object));
    z = *(object *)(z.addr);
    if (z.type & 64)
      y = bar(y);
  }
  return y;
}

int    nil;
object cons1[2] = {{(char *)&nil, 0}, {(char *)&nil, 0}};
object cons2[2] = {{(char *)&cons1, 64}, {(char *)&nil, 0}};

int main(void) {
  object x     = {(char *)&cons2, 64};
  object y     = {(char *)&nil, 0};
  object three = foo(x, y);
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 addr: ptr<i8>;
// DEFAULT-NEXT:         field1 type: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 object = @type0;
// DEFAULT-NEXT:     global %9 nil: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 cons1: array<@type0, 2> [storage=static] [align=16] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%9)), field1 = widen<i64, reason=assign>(const<i32>(0))), index1 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%9)), field1 = widen<i64, reason=assign>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %11 cons2: array<@type0, 2> [storage=static] [align=16] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<array<@type0, 2>>>(%10)), field1 = widen<i64, reason=assign>(const<i32>(64))), index1 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%9)), field1 = widen<i64, reason=assign>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @bar(%4 blah: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo(%6 x: @type0, %7 y: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 z: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(deref(pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i8>>(field0(%6))))));
// DEFAULT-NEXT:         if ne<i64>(and<i64>(read<i64>(field1(%8)), widen<i64, reason=usual_arith>(const<i32>(64))), const<i64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<@type0>(%7, copy<@type0, reason=assign>(read<@type0>(deref(pointer_cast<ptr<@type0>, reason=explicit>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field0(%8)), const<u64>(16)))))));
// DEFAULT-NEXT:                 write<@type0>(%8, copy<@type0, reason=assign>(read<@type0>(deref(pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i8>>(field0(%8)))))));
// DEFAULT-NEXT:                 if ne<i64>(and<i64>(read<i64>(field1(%8)), widen<i64, reason=usual_arith>(const<i32>(64))), const<i64>(0))
// DEFAULT-NEXT:                     write<@type0>(%7, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%3, copy<@type0, reason=arg>(read<@type0>(%7)))));
// DEFAULT-NEXT:                     copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%3, copy<@type0, reason=arg>(read<@type0>(%7))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 x: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<array<@type0, 2>>>(%11)), field1 = widen<i64, reason=assign>(const<i32>(64)));
// DEFAULT-NEXT:         let %14 y: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%9)), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %15 three: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn(@type0, @type0) -> @type0, abi=sysv64(native_c, native_c) -> native_c>(%5, copy<@type0, reason=arg>(read<@type0>(%13)), copy<@type0, reason=arg>(read<@type0>(%14))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
