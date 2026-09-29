struct A {
  unsigned long p, q, r, s;
} x = {13, 14, 15, 16};

extern void abort(void);
extern void exit(int);

static inline struct A *bar(void) {
  struct A *r;

  switch (8) {
  case 2:
    abort();
    break;
  case 8:
    r = &x;
    break;
  default:
    abort();
    break;
  }
  return r;
}

void foo(unsigned long *x, int y) {
  if (y != 12)
    abort();
  if (x[0] != 1 || x[1] != 11)
    abort();
  if (x[2] != 2 || x[3] != 12)
    abort();
  if (x[4] != 3 || x[5] != 13)
    abort();
  if (x[6] != 4 || x[7] != 14)
    abort();
  if (x[8] != 5 || x[9] != 15)
    abort();
  if (x[10] != 6 || x[11] != 16)
    abort();
}

int main(void) {
  unsigned long a[40];
  int           b = 0;

  a[b++] = 1;
  a[b++] = 11;
  a[b++] = 2;
  a[b++] = 12;
  a[b++] = 3;
  a[b++] = bar()->p;
  a[b++] = 4;
  a[b++] = bar()->q;
  a[b++] = 5;
  a[b++] = bar()->r;
  a[b++] = 6;
  a[b++] = bar()->s;
  foo(a, b);
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 p: u64;
// DEFAULT-NEXT:         field1 q: u64;
// DEFAULT-NEXT:         field2 r: u64;
// DEFAULT-NEXT:         field3 s: u64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_A]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(13))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(14))), field2 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(15))), field3 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(16)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> ptr<@type[[TYPE_A]]> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<@type[[TYPE_A]]> [storage=automatic];
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] const<i32>(8)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(2):
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(8):
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_A]]>>(%[[VALUE_r]], addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_A]]>>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: ptr<u64>, %[[VALUE_y:[0-9]+]] y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(11)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(4)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(5)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(13)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(6)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(7)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(14)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(9)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(10)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_x_2]]), const<i32>(11)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<u64, 40> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE2]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE4]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(11))));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE6]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE8]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(12))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE10]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE12]]))), read<u64>(field0(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]])))));
// DEFAULT-NEXT:         read<u64>(field0(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]]))));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE14]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE16]]))), read<u64>(field1(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]])))));
// DEFAULT-NEXT:         read<u64>(field1(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]]))));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE18]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE20]]))), read<u64>(field2(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]])))));
// DEFAULT-NEXT:         read<u64>(field2(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]]))));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE22]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(6))));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE24]]))), read<u64>(field3(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]])))));
// DEFAULT-NEXT:         read<u64>(field3(deref(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>, i32) -> void>(%[[VALUE_foo]], array_decay<ptr<u64>, length=Some(40)>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
