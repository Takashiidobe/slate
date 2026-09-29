/* PR middle-end/87623 */
/* Testcase by George Thopas <george.thopas@gmail.com> */

struct be {
  unsigned short pad[1];
  unsigned char  a;
  unsigned char  b;
} __attribute__((scalar_storage_order("big-endian")));

typedef struct be t_be;

struct le {
  unsigned short pad[3];
  unsigned char  a;
  unsigned char  b;
};

typedef struct le t_le;

int a_or_b_different(t_be *x, t_le *y) {
  return (x->a != y->a) || (x->b != y->b);
}

int main(void) {
  t_be x = {.a = 1, .b = 2};
  t_le y = {.a = 1, .b = 2};

  if (a_or_b_different(&x, &y))
    __builtin_abort();

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
// DEFAULT-NEXT:     type @type[[TYPE_be:[0-9]+]] be = struct {
// DEFAULT-NEXT:         field0 pad: array<u16, 1>;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u8;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_t_be:[0-9]+]] t_be = @type[[TYPE_be]];
// DEFAULT-NEXT:     type @type[[TYPE_le:[0-9]+]] le = struct {
// DEFAULT-NEXT:         field0 pad: array<u16, 3>;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u8;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 6, 7]];
// DEFAULT-NEXT:     type @type[[TYPE_t_le:[0-9]+]] t_le = @type[[TYPE_le]];
// DEFAULT-NEXT:     fn %[[VALUE_a_or_b_different:[0-9]+]] @a_or_b_different(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_be]]>, %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_le]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(deref(read<ptr<@type[[TYPE_be]]>>(%[[VALUE_x]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(deref(read<ptr<@type[[TYPE_le]]>>(%[[VALUE_y]]))))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field2(deref(read<ptr<@type[[TYPE_be]]>>(%[[VALUE_x]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field2(deref(read<ptr<@type[[TYPE_le]]>>(%[[VALUE_y]])))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_be]] [storage=automatic] = aggregate<@type[[TYPE_be]], zero_fill=true>(field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_le]] [storage=automatic] = aggregate<@type[[TYPE_le]], zero_fill=true>(field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_be]]>, ptr<@type[[TYPE_le]]>) -> i32>(%[[VALUE_a_or_b_different]], addr_of<ptr<@type[[TYPE_be]]>>(%[[VALUE_x_2]]), addr_of<ptr<@type[[TYPE_le]]>>(%[[VALUE_y_2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
