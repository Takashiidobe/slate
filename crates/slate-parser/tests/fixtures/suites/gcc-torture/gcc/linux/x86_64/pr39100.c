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
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = struct {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:         field1 n: ptr<@type[[TYPE_E]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_EP:[0-9]+]] EP = ptr<@type[[TYPE_E]]>;
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type[[TYPE_E]]>;
// DEFAULT-NEXT:         field1 cn: i16;
// DEFAULT-NEXT:         field2 cp: i16;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 10]];
// DEFAULT-NEXT:     type @type[[TYPE_CP:[0-9]+]] CP = ptr<@type[[TYPE_C]]>;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_h:[0-9]+]] h: ptr<@type[[TYPE_C]]>, %[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_E]]>) -> ptr<@type[[TYPE_C]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pl:[0-9]+]] pl: ptr<@type[[TYPE_E]]> [storage=automatic] = null<ptr<@type[[TYPE_E]]>>;
// DEFAULT-NEXT:         let %[[VALUE_pa:[0-9]+]] pa: ptr<ptr<@type[[TYPE_E]]>> [storage=automatic] = addr_of<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_pl]]);
// DEFAULT-NEXT:         let %[[VALUE_nl:[0-9]+]] nl: ptr<@type[[TYPE_E]]> [storage=automatic] = null<ptr<@type[[TYPE_E]]>>;
// DEFAULT-NEXT:         let %[[VALUE_na:[0-9]+]] na: ptr<ptr<@type[[TYPE_E]]>> [storage=automatic] = addr_of<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_nl]]);
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: ptr<@type[[TYPE_E]]> [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<ptr<@type[[TYPE_E]]>>(read<ptr<@type[[TYPE_E]]>>(%[[VALUE_x]]), null<ptr<@type[[TYPE_E]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_E]]>>(%[[VALUE_n]], read<ptr<@type[[TYPE_E]]>>(field1(deref(read<ptr<@type[[TYPE_E]]>>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 if eq<i32>(and<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_E]]>>(%[[VALUE_x]])))), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE_C]]> [synthetic] = read<ptr<@type[[TYPE_C]]>>(%[[VALUE_h]]);
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i16 [synthetic] = read<i16>(field2(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE1]]))));
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE2]])), const<i32>(1)));
// DEFAULT-NEXT:                         write<i16>(field2(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE1]]))), read<i16>(%[[VALUE3]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_E]]>>(deref(read<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_pa]])), read<ptr<@type[[TYPE_E]]>>(%[[VALUE_x]]));
// DEFAULT-NEXT:                         write<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_pa]], addr_of<ptr<ptr<@type[[TYPE_E]]>>>(field1(deref(read<ptr<@type[[TYPE_E]]>>(deref(read<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_pa]])))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: ptr<@type[[TYPE_C]]> [synthetic] = read<ptr<@type[[TYPE_C]]>>(%[[VALUE_h]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i16 [synthetic] = read<i16>(field1(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE4]]))));
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE5]])), const<i32>(1)));
// DEFAULT-NEXT:                         write<i16>(field1(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE4]]))), read<i16>(%[[VALUE6]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_E]]>>(deref(read<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_na]])), read<ptr<@type[[TYPE_E]]>>(%[[VALUE_x]]));
// DEFAULT-NEXT:                         write<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_na]], addr_of<ptr<ptr<@type[[TYPE_E]]>>>(field1(deref(read<ptr<@type[[TYPE_E]]>>(deref(read<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_na]])))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_E]]>>(%[[VALUE_x]], read<ptr<@type[[TYPE_E]]>>(%[[VALUE_n]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_E]]>>(deref(read<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_pa]])), read<ptr<@type[[TYPE_E]]>>(%[[VALUE_nl]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_E]]>>(deref(read<ptr<ptr<@type[[TYPE_E]]>>>(%[[VALUE_na]])), null<ptr<@type[[TYPE_E]]>>);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_E]]>>(field0(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_h]]))), read<ptr<@type[[TYPE_E]]>>(%[[VALUE_pl]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_C]]>>(%[[VALUE_h]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_C]] [storage=automatic] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = null<ptr<@type[[TYPE_E]]>>, field1 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: array<@type[[TYPE_E]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_E]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_E]], zero_fill=false>(field0 = const<i32>(0), field1 = addr_of<ptr<@type[[TYPE_E]]>>(deref(ptr_offset<ptr<@type[[TYPE_E]]>, subtract=false, element=@type[[TYPE_E]], overflow=ub>(array_decay<ptr<@type[[TYPE_E]]>, length=Some(2)>(%[[VALUE_e]]), const<i32>(1))))), index1 = aggregate<@type[[TYPE_E]], zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type[[TYPE_E]]>>));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_E]]> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_C]]>, signature=fn(ptr<@type[[TYPE_C]]>, ptr<@type[[TYPE_E]]>) -> ptr<@type[[TYPE_C]]>>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_C]]>>(%[[VALUE_c]]), addr_of<ptr<@type[[TYPE_E]]>>(deref(ptr_offset<ptr<@type[[TYPE_E]]>, subtract=false, element=@type[[TYPE_E]], overflow=ub>(array_decay<ptr<@type[[TYPE_E]]>, length=Some(2)>(%[[VALUE_e]]), const<i32>(0)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_c]]))), const<i32>(1)), ne<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_c]]))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_E]]>>(read<ptr<@type[[TYPE_E]]>>(field0(%[[VALUE_c]])), addr_of<ptr<@type[[TYPE_E]]>>(deref(ptr_offset<ptr<@type[[TYPE_E]]>, subtract=false, element=@type[[TYPE_E]], overflow=ub>(array_decay<ptr<@type[[TYPE_E]]>, length=Some(2)>(%[[VALUE_e]]), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_E]]>>(read<ptr<@type[[TYPE_E]]>>(field1(deref(ptr_offset<ptr<@type[[TYPE_E]]>, subtract=false, element=@type[[TYPE_E]], overflow=ub>(array_decay<ptr<@type[[TYPE_E]]>, length=Some(2)>(%[[VALUE_e]]), const<i32>(1))))), addr_of<ptr<@type[[TYPE_E]]>>(deref(ptr_offset<ptr<@type[[TYPE_E]]>, subtract=false, element=@type[[TYPE_E]], overflow=ub>(array_decay<ptr<@type[[TYPE_E]]>, length=Some(2)>(%[[VALUE_e]]), const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_E]]>>(read<ptr<@type[[TYPE_E]]>>(field1(deref(ptr_offset<ptr<@type[[TYPE_E]]>, subtract=false, element=@type[[TYPE_E]], overflow=ub>(array_decay<ptr<@type[[TYPE_E]]>, length=Some(2)>(%[[VALUE_e]]), const<i32>(0))))), null<ptr<@type[[TYPE_E]]>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
