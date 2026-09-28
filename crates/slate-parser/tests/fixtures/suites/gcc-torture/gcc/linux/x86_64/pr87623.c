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
// DEFAULT-NEXT:     type @type0 be = struct {
// DEFAULT-NEXT:         field0 pad: array<u16, 1>;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u8;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2, 3]];
// DEFAULT-NEXT:     type @type1 t_be = @type0;
// DEFAULT-NEXT:     type @type2 le = struct {
// DEFAULT-NEXT:         field0 pad: array<u16, 3>;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u8;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 6, 7]];
// DEFAULT-NEXT:     type @type3 t_le = @type2;
// DEFAULT-NEXT:     fn %4 @a_or_b_different(%5 x: ptr<@type0>, %6 y: ptr<@type2>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(deref(read<ptr<@type0>>(%5)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(deref(read<ptr<@type2>>(%6))))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field2(deref(read<ptr<@type0>>(%5)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field2(deref(read<ptr<@type2>>(%6)))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 x: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>(field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %9 y: @type2 [storage=automatic] = aggregate<@type2, zero_fill=true>(field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type2>) -> i32>(%4, addr_of<ptr<@type0>>(%8), addr_of<ptr<@type2>>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
