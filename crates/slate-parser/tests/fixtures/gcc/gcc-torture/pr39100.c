/* Bad PTA results (incorrect store handling) was causing us to delete
 *na = 0 store.  */

typedef struct E {
  int       p;
  struct E *n;
} *EP;

typedef struct C {
  EP    x;
  short cn, cp;
} *CP;

__attribute__((noinline)) CP foo(CP h, EP x) {
  EP pl = 0, *pa = &pl;
  EP nl = 0, *na = &nl;
  EP n;

  while (x) {
    n = x->n;
    if ((x->p & 1) == 1) {
      h->cp++;
      *pa = x;
      pa  = &((*pa)->n);
    } else {
      h->cn++;
      *na = x;
      na  = &((*na)->n);
    }
    x = n;
  }
  *pa  = nl;
  *na  = 0;
  h->x = pl;
  return h;
}

int main(void) {
  struct C c    = {0, 0, 0};
  struct E e[2] = {{0, &e[1]}, {1, 0}};
  EP       p;

  foo(&c, &e[0]);
  if (c.cn != 1 || c.cp != 1)
    __builtin_abort();
  if (c.x != &e[1])
    __builtin_abort();
  if (e[1].n != &e[0])
    __builtin_abort();
  if (e[0].n)
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
// DEFAULT-NEXT:     type @type0 E = struct {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:         field1 n: ptr<@type0>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 EP = ptr<@type0>;
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type0>;
// DEFAULT-NEXT:         field1 cn: i16;
// DEFAULT-NEXT:         field2 cp: i16;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 10]];
// DEFAULT-NEXT:     type @type3 CP = ptr<@type2>;
// DEFAULT-NEXT:     fn %4 @foo(%5 h: ptr<@type2>, %6 x: ptr<@type0>) -> ptr<@type2> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 pl: ptr<@type0> [storage=automatic] = null<ptr<@type0>>;
// DEFAULT-NEXT:         let %8 pa: ptr<ptr<@type0>> [storage=automatic] = addr_of<ptr<ptr<@type0>>>(%7);
// DEFAULT-NEXT:         let %9 nl: ptr<@type0> [storage=automatic] = null<ptr<@type0>>;
// DEFAULT-NEXT:         let %10 na: ptr<ptr<@type0>> [storage=automatic] = addr_of<ptr<ptr<@type0>>>(%9);
// DEFAULT-NEXT:         let %11 n: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         while %16 ne<ptr<@type0>>(read<ptr<@type0>>(%6), null<ptr<@type0>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type0>>(%11, read<ptr<@type0>>(field1(deref(read<ptr<@type0>>(%6)))));
// DEFAULT-NEXT:                 if eq<i32>(and<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%6)))), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %17: ptr<@type2> [synthetic] = read<ptr<@type2>>(%5);
// DEFAULT-NEXT:                         let %18: i16 [synthetic] = read<i16>(field2(deref(read<ptr<@type2>>(%17))));
// DEFAULT-NEXT:                         let %19: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%18)), const<i32>(1)));
// DEFAULT-NEXT:                         write<i16>(field2(deref(read<ptr<@type2>>(%17))), read<i16>(%19));
// DEFAULT-NEXT:                         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%8)), read<ptr<@type0>>(%6));
// DEFAULT-NEXT:                         write<ptr<ptr<@type0>>>(%8, addr_of<ptr<ptr<@type0>>>(field1(deref(read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%8)))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %20: ptr<@type2> [synthetic] = read<ptr<@type2>>(%5);
// DEFAULT-NEXT:                         let %21: i16 [synthetic] = read<i16>(field1(deref(read<ptr<@type2>>(%20))));
// DEFAULT-NEXT:                         let %22: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%21)), const<i32>(1)));
// DEFAULT-NEXT:                         write<i16>(field1(deref(read<ptr<@type2>>(%20))), read<i16>(%22));
// DEFAULT-NEXT:                         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%10)), read<ptr<@type0>>(%6));
// DEFAULT-NEXT:                         write<ptr<ptr<@type0>>>(%10, addr_of<ptr<ptr<@type0>>>(field1(deref(read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%10)))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<ptr<@type0>>(%6, read<ptr<@type0>>(%11));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%8)), read<ptr<@type0>>(%9));
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%10)), null<ptr<@type0>>);
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(deref(read<ptr<@type2>>(%5))), read<ptr<@type0>>(%7));
// DEFAULT-NEXT:         return read<ptr<@type2>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 c: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = null<ptr<@type0>>, field1 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %14 e: array<@type0, 2> [storage=automatic] [align=16] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%14), const<i32>(1))))), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type0>>));
// DEFAULT-NEXT:         let %15 p: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<@type2>, signature=fn(ptr<@type2>, ptr<@type0>) -> ptr<@type2>>(%4, addr_of<ptr<@type2>>(%13), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%14), const<i32>(0)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(%13))), const<i32>(1)), ne<i32>(widen<i32, reason=promotion>(read<i16>(field2(%13))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(field0(%13)), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%14), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%14), const<i32>(1))))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%14), const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%14), const<i32>(0))))), null<ptr<@type0>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
