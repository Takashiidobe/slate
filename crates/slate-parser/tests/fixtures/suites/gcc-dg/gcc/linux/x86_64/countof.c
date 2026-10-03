/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

#define assert(e)  ((e) ? (void) 0 : __builtin_abort ())

void
array (void)
{
  short a[7];

  static_assert (_Countof (a) == 7);
  static_assert (_Countof (unsigned [99]) == 99);
}

void
completed (void)
{
  int a[] = {1, 2, 3};

  static_assert (_Countof (a) == 3);
}

void
vla (void)
{
  unsigned n;

  n = 99;
  assert (_Countof (short [n - 10]) == 99 - 10);

  int v[n / 2];
  assert (_Countof (v) == 99 / 2);
}

void
member (void)
{
  struct {
    int a[8];
  } s;

  static_assert (_Countof (s.a) == 8);
}

void
vla_eval (void)
{
  int i;

  i = 7;
  assert (_Countof (struct {int x;}[i++]) == 7);
  assert (i == 7 + 1);

  int v[i];
  int (*p)[i];
  p = &v;
  assert (_Countof (*p++) == i);
  assert (p - 1 == &v);
}

void
array_noeval (void)
{
  long a[5];
  long (*p)[_Countof (a)];

  p = &a;
  static_assert (_Countof (*p++) == 5);
  assert (p == &a);
}

void
matrix_fixed (void)
{
  int i;

  static_assert (_Countof (int [7][4]) == 7);
  i = 3;
  static_assert (_Countof (int [7][i]) == 7);
}

void
matrix_vla (void)
{
  int i, j;

  i = 7;
  assert (_Countof (int [i++][4]) == 7);
  assert (i == 7 + 1);

  i = 9;
  j = 3;
  assert (_Countof (int [i++][j]) == 9);
  assert (i == 9 + 1);
}

void
no_parens(void)
{
  int n = 3;
  int a[7];
  int v[n];

  static_assert (_Countof a == 7); 
  assert (_Countof v == 3); 
}

int
main (void)
{
  array ();
  completed ();
  vla ();
  member ();
  vla_eval ();
  array_noeval ();
  matrix_fixed ();
  matrix_vla ();
  no_parens ();
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:         field0 a: array<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_array:[0-9]+]] @array() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i16, 7> [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_completed:[0-9]+]] @completed() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_vla:[0-9]+]] @vla() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_n]], reinterpret<u32, reason=assign, fits=always>(const<i32>(99)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_n]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE0]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(const<i32>(99), const<i32>(10)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_n]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: vla<i32, %[[VALUE1]]> [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(99), const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_member:[0-9]+]] @member() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vla_eval:[0-9]+]] @vla_eval() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], const<i32>(7));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE2]])));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i]]), add<i32, overflow=ub>(const<i32>(7), const<i32>(1)))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: vla<i32, %[[VALUE5]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<vla<i32, %[[VALUE6]]>> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_p]], pointer_cast<ptr<vla<i32, %[[VALUE6]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE5]]>>>(%[[VALUE_v_2]])));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<vla<i32, %[[VALUE6]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_p]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<vla<i32, %[[VALUE6]]>> [synthetic] = ptr_offset<ptr<vla<i32, %[[VALUE6]]>>, subtract=false, element=vla<i32, %[[VALUE6]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_p]], read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE8]]));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE6]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<ptr<vla<i32, %[[VALUE6]]>>>(ptr_offset<ptr<vla<i32, %[[VALUE6]]>>, subtract=true, element=vla<i32, %[[VALUE6]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_p]]), const<i32>(1)), pointer_cast<ptr<vla<i32, %[[VALUE6]]>>, reason=usual_arith>(addr_of<ptr<vla<i32, %[[VALUE5]]>>>(%[[VALUE_v_2]])))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_array_noeval:[0-9]+]] @array_noeval() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: array<i64, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<array<i64, 5>> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<array<i64, 5>>>(%[[VALUE_p_2]], addr_of<ptr<array<i64, 5>>>(%[[VALUE_a_3]]));
// DEFAULT-NEXT:         if eq<ptr<array<i64, 5>>>(read<ptr<array<i64, 5>>>(%[[VALUE_p_2]]), addr_of<ptr<array<i64, 5>>>(%[[VALUE_a_3]]))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_matrix_fixed:[0-9]+]] @matrix_fixed() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_2]], const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_matrix_vla:[0-9]+]] @matrix_vla() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], const<i32>(7));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE9]])));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE11]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i_3]]), add<i32, overflow=ub>(const<i32>(7), const<i32>(1)))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], const<i32>(9));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE12]])));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE14]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i_3]]), add<i32, overflow=ub>(const<i32>(9), const<i32>(1)))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_no_parens:[0-9]+]] @no_parens() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_a_4:[0-9]+]] a: array<i32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         let %[[VALUE_v_3:[0-9]+]] v: vla<i32, %[[VALUE16]]> [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE16]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_array]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_completed]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_vla]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_member]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_vla_eval]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_array_noeval]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_matrix_fixed]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_matrix_vla]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_no_parens]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
