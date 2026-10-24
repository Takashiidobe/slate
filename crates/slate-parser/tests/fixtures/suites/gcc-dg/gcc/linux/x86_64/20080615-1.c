/* { dg-do compile }  */
/* { dg-options "-w -fpermissive -O2" } */

static int *see_bb_splay_ar = ((void *) 0);
static void
see_merge_and_eliminate_extensions (void)
{
  int i = 0;
  printf ("* Phase 2: Merge and eliminate locally redundant extensions.  *\n");
  splay_tree_foreach (see_bb_splay_ar[i], ((void *) 0), ((void *) 0));
}
static void
see_main (void)
{
  int i = 0;
  see_merge_and_eliminate_extensions ();
  printf ("Searching register properties in bb %d\n", i);
}
gate_handle_see (void)
{
}
rest_of_handle_see (void)
{
  see_main ();
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_see_bb_splay_ar:[0-9]+]] see_bb_splay_ar: ptr<i32> [storage=static] = null<ptr<i32>> [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 65> [storage=static] = code_units<array<i8, 65>>([42, 32, 80, 104, 97, 115, 101, 32, 50, 58, 32, 77, 101, 114, 103, 101, 32, 97, 110, 100, 32, 101, 108, 105, 109, 105, 110, 97, 116, 101, 32, 108, 111, 99, 97, 108, 108, 121, 32, 114, 101, 100, 117, 110, 100, 97, 110, 116, 32, 101, 120, 116, 101, 110, 115, 105, 111, 110, 115, 46, 32, 32, 42, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([83, 101, 97, 114, 99, 104, 105, 110, 103, 32, 114, 101, 103, 105, 115, 116, 101, 114, 32, 112, 114, 111, 112, 101, 114, 116, 105, 101, 115, 32, 105, 110, 32, 98, 98, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_splay_tree_foreach:[0-9]+]] @splay_tree_foreach(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_see_merge_and_eliminate_extensions:[0-9]+]] @see_merge_and_eliminate_extensions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(65)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_splay_tree_foreach]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_see_bb_splay_ar]]), read<i32>(%[[VALUE_i]])))), null<ptr<void>>, null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_see_main:[0-9]+]] @see_main() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_see_merge_and_eliminate_extensions]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gate_handle_see:[0-9]+]] @gate_handle_see() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_rest_of_handle_see:[0-9]+]] @rest_of_handle_see() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_see_main]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
