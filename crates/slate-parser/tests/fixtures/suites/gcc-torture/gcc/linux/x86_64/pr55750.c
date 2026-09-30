/* PR middle-end/55750 */

extern void abort(void);

struct S {
  int m : 1;
  int n : 7;
} arr[2];

__attribute__((noinline, noclone)) void foo(unsigned i) { arr[i].n++; }

int main() {
  arr[0].m = -1;
  arr[0].n = (1 << 6) - 1;
  arr[1].m = 0;
  arr[1].n = -1;
  foo(0);
  foo(1);
  if (arr[0].m != -1 || arr[0].n != -(1 << 6) || arr[1].m != 0 || arr[1].n != 0)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 m: i32 : 1;
// DEFAULT-NEXT:         field1 n: i32 : 7;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(1)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_arr:[0-9]+]] arr: array<@type[[TYPE_S]], 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_i:[0-9]+]] i: u32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), read<u32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(bitfield1<unit=0, bytes=0..1, bits=1..8>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=1..8>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE0]]))), read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(0)))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=1..8>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(0)))), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(1)))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=1..8>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(0))))), neg<i32, overflow=ub>(const<i32>(1))), ne<i32>(read<i32>(bitfield1<unit=0, bytes=0..1, bits=1..8>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(0))))), neg<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6))))), ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(1))))), const<i32>(0))), ne<i32>(read<i32>(bitfield1<unit=0, bytes=0..1, bits=1..8>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(1))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
