struct foo {
  int i;
  int j;
};

int bar(struct foo *k, int k2, int f, int f2) {
  int *p, *q;
  int  res;
  if (f)
    p = &k->i;
  else
    p = &k->j;
  res  = *p;
  k->i = 1;
  if (f2)
    q = p;
  else
    q = &k2;
  return res + *q;
}

extern void abort(void);

int main() {
  struct foo k;
  k.i = 0;
  k.j = 1;
  if (bar(&k, 1, 1, 1) != 1)
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_k:[0-9]+]] k: ptr<@type[[TYPE_foo]]>, %[[VALUE_k2:[0-9]+]] k2: i32, %[[VALUE_f:[0-9]+]] f: i32, %[[VALUE_f2:[0-9]+]] f2: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%[[VALUE_p]], addr_of<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_k]])))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%[[VALUE_p]], addr_of<ptr<i32>>(field1(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_k]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_res]], read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_k]]))), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_f2]]), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%[[VALUE_q]], read<ptr<i32>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%[[VALUE_q]], addr_of<ptr<i32>>(%[[VALUE_k2]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_res]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_q]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_k_2:[0-9]+]] k: @type[[TYPE_foo]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_k_2]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_k_2]]), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_foo]]>, i32, i32, i32) -> i32>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_k_2]]), const<i32>(1), const<i32>(1), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
