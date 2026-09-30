/* The bit-field below would have a problem if __INT_MAX__ is too
   small.  */
void abort(void);
void exit(int);

#if __INT_MAX__ < 2147483647
int main(void) { exit(0); }
#else
typedef struct {
  unsigned b0 : 1, f1 : 17, b18 : 1, b19 : 1, b20 : 1, f2 : 11;
} bf;

typedef union {
  bf       b;
  unsigned w;
} bu;

bu f(bu i) {
  bu o = i;

  if (o.b.b0)
    o.b.b18 = 1, o.b.b20 = 1;
  else
    o.b.b18 = 0, o.b.b20 = 0;

  return o;
}

int main(void) {
  bu a;
  bu r;

  a.w    = 0x4000000;
  a.b.b0 = 0;
  r      = f(a);
  if (a.w != r.w)
    abort();
  exit(0);
}
#endif



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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 b0: u32 : 1;
// DEFAULT-NEXT:         field1 f1: u32 : 17;
// DEFAULT-NEXT:         field2 b18: u32 : 1;
// DEFAULT-NEXT:         field3 b19: u32 : 1;
// DEFAULT-NEXT:         field4 b20: u32 : 1;
// DEFAULT-NEXT:         field5 f2: u32 : 11;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 2, 2, 2, 2], bit_offsets=[Some(0), Some(1), Some(18), Some(19), Some(20), Some(21)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_bf:[0-9]+]] bf = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 b: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 w: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_bu:[0-9]+]] bu = @type[[TYPE1]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_i:[0-9]+]] i: @type[[TYPE1]]) -> @type[[TYPE1]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: @type[[TYPE1]] [storage=automatic] = copy<@type[[TYPE1]], reason=assign>(read<@type[[TYPE1]]>(%[[VALUE_i]]));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..1>(field0(%[[VALUE_o]])))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(bitfield2<unit=0, bytes=0..4, bits=18..19>(field0(%[[VALUE_o]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u32>(bitfield4<unit=0, bytes=0..4, bits=20..21>(field0(%[[VALUE_o]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(bitfield2<unit=0, bytes=0..4, bits=18..19>(field0(%[[VALUE_o]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<u32>(bitfield4<unit=0, bytes=0..4, bits=20..21>(field0(%[[VALUE_o]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return copy<@type[[TYPE1]], reason=return>(read<@type[[TYPE1]]>(%[[VALUE_o]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field1(%[[VALUE_a]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(67108864)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..1>(field0(%[[VALUE_a]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(%[[VALUE_r]], copy<@type[[TYPE1]], reason=assign>(call<@type[[TYPE1]], signature=fn(@type[[TYPE1]]) -> @type[[TYPE1]], abi=sysv64(native_c) -> native_c>(%[[VALUE_f]], copy<@type[[TYPE1]], reason=arg>(read<@type[[TYPE1]]>(%[[VALUE_a]])))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field1(%[[VALUE_a]])), read<u32>(field1(%[[VALUE_r]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
