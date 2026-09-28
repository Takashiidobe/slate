struct assembly_operand {
  int type, value, symtype, symflags, marker;
};

struct assembly_operand to_input, from_input;

void __attribute__((__noinline__, __noclone__))
assemblez_1(int internal_number, struct assembly_operand o1) {
  if (o1.type != from_input.type)
    __builtin_abort();
}

void __attribute__((__noinline__, __noclone__))
t0(struct assembly_operand to, struct assembly_operand from) {
  if (to.value == 0)
    assemblez_1(32, from);
  else
    __builtin_abort();
}

int main(void) {
  to_input.value    = 0;
  to_input.type     = 1;
  to_input.symtype  = 2;
  to_input.symflags = 3;
  to_input.marker   = 4;

  from_input.value    = 5;
  from_input.type     = 6;
  from_input.symtype  = 7;
  from_input.symflags = 8;
  from_input.marker   = 9;

  t0(to_input, from_input);

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
// DEFAULT-NEXT:     type @type0 assembly_operand = struct {
// DEFAULT-NEXT:         field0 type: i32;
// DEFAULT-NEXT:         field1 value: i32;
// DEFAULT-NEXT:         field2 symtype: i32;
// DEFAULT-NEXT:         field3 symflags: i32;
// DEFAULT-NEXT:         field4 marker: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     global %1 to_input: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 from_input: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @assemblez_1(%4 internal_number: i32, %5 o1: @type0) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%5)), read<i32>(field0(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @t0(%7 to: @type0, %8 from: @type0) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field1(%7)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32, @type0) -> void, abi=sysv64(scalar, native_c) -> void>(%3, const<i32>(32), copy<@type0, reason=arg>(read<@type0>(%8)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field1(%1), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(%1), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field2(%1), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field3(%1), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field4(%1), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(field1(%2), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(field0(%2), const<i32>(6));
// DEFAULT-NEXT:         write<i32>(field2(%2), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(field3(%2), const<i32>(8));
// DEFAULT-NEXT:         write<i32>(field4(%2), const<i32>(9));
// DEFAULT-NEXT:         call<void, signature=fn(@type0, @type0) -> void, abi=sysv64(native_c, native_c) -> void>(%6, copy<@type0, reason=arg>(read<@type0>(%1)), copy<@type0, reason=arg>(read<@type0>(%2)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
