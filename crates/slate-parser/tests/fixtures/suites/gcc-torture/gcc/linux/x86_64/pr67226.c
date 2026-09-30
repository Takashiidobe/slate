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
// DEFAULT-NEXT:     type @type[[TYPE_assembly_operand:[0-9]+]] assembly_operand = struct {
// DEFAULT-NEXT:         field0 type: i32;
// DEFAULT-NEXT:         field1 value: i32;
// DEFAULT-NEXT:         field2 symtype: i32;
// DEFAULT-NEXT:         field3 symflags: i32;
// DEFAULT-NEXT:         field4 marker: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     global %[[VALUE_to_input:[0-9]+]] to_input: @type[[TYPE_assembly_operand]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_from_input:[0-9]+]] from_input: @type[[TYPE_assembly_operand]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_assemblez_1:[0-9]+]] @assemblez_1(%[[VALUE_internal_number:[0-9]+]] internal_number: i32, %[[VALUE_o1:[0-9]+]] o1: @type[[TYPE_assembly_operand]]) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%[[VALUE_o1]])), read<i32>(field0(%[[VALUE_from_input]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t0:[0-9]+]] @t0(%[[VALUE_to:[0-9]+]] to: @type[[TYPE_assembly_operand]], %[[VALUE_from:[0-9]+]] from: @type[[TYPE_assembly_operand]]) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field1(%[[VALUE_to]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32, @type[[TYPE_assembly_operand]]) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_assemblez_1]], const<i32>(32), copy<@type[[TYPE_assembly_operand]], reason=arg>(read<@type[[TYPE_assembly_operand]]>(%[[VALUE_from]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_to_input]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_to_input]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_to_input]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_to_input]]), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field4(%[[VALUE_to_input]]), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_from_input]]), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_from_input]]), const<i32>(6));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_from_input]]), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_from_input]]), const<i32>(8));
// DEFAULT-NEXT:         write<i32>(field4(%[[VALUE_from_input]]), const<i32>(9));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_assembly_operand]], @type[[TYPE_assembly_operand]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_t0]], copy<@type[[TYPE_assembly_operand]], reason=arg>(read<@type[[TYPE_assembly_operand]]>(%[[VALUE_to_input]])), copy<@type[[TYPE_assembly_operand]], reason=arg>(read<@type[[TYPE_assembly_operand]]>(%[[VALUE_from_input]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
