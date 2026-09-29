/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

struct GTeth_desc
{
  unsigned ed_cmdsts;
};
struct GTeth_softc
{
  struct GTeth_desc txq_desc[32];
};

void foo(struct GTeth_softc *sc)
{
  /* Verify that we retain the volatileness on the
     store until after optimization.  */
  volatile struct GTeth_desc *p = &sc->txq_desc[0];
  p->ed_cmdsts = 0;
}

/* { dg-final { scan-tree-dump "{v}" "optimized" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE_GTeth_desc:[0-9]+]] GTeth_desc = struct {
// DEFAULT-NEXT:         field0 ed_cmdsts: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_GTeth_softc:[0-9]+]] GTeth_softc = struct {
// DEFAULT-NEXT:         field0 txq_desc: array<@type[[TYPE_GTeth_desc]], 32>;
// DEFAULT-NEXT:     } [size=128, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_sc:[0-9]+]] sc: ptr<@type[[TYPE_GTeth_softc]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<volatile @type[[TYPE_GTeth_desc]]> [storage=automatic] = pointer_cast<ptr<volatile @type[[TYPE_GTeth_desc]]>, reason=assign>(addr_of<ptr<@type[[TYPE_GTeth_desc]]>>(deref(ptr_offset<ptr<@type[[TYPE_GTeth_desc]]>, subtract=false, element=@type[[TYPE_GTeth_desc]], overflow=ub>(array_decay<ptr<@type[[TYPE_GTeth_desc]]>, length=Some(32)>(field0(deref(read<ptr<@type[[TYPE_GTeth_softc]]>>(%[[VALUE_sc]])))), const<i32>(0)))));
// DEFAULT-NEXT:         write<u32, volatile>(field0(deref(read<ptr<volatile @type[[TYPE_GTeth_desc]]>>(%[[VALUE_p]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
