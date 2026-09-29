
// Under Windows 64, int and long are 32-bits.  Make sure pointer math doesn't
// cause any sign extensions.


#define CR(Record, TYPE, Field) \
  ((TYPE *) ((unsigned char *) (Record) - (unsigned char *) &(((TYPE *) 0)->Field)))

typedef struct _LIST_ENTRY {
  struct _LIST_ENTRY  *ForwardLink;
  struct _LIST_ENTRY  *BackLink;
} LIST_ENTRY;

typedef struct {
  unsigned long long    Signature;
  LIST_ENTRY            Link;
} MEMORY_MAP;

int test(unsigned long long param)
{
  LIST_ENTRY      *Link;
  MEMORY_MAP      *Entry;

  Link = (LIST_ENTRY *) param;

  Entry = CR (Link, MEMORY_MAP, Link);
  return (int) Entry->Signature;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE__LIST_ENTRY:[0-9]+]] _LIST_ENTRY = struct {
// DEFAULT-NEXT:         field0 ForwardLink: ptr<@type[[TYPE__LIST_ENTRY]]>;
// DEFAULT-NEXT:         field1 BackLink: ptr<@type[[TYPE__LIST_ENTRY]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_LIST_ENTRY:[0-9]+]] LIST_ENTRY = @type[[TYPE__LIST_ENTRY]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 Signature: u64;
// DEFAULT-NEXT:         field1 Link: @type[[TYPE__LIST_ENTRY]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_MEMORY_MAP:[0-9]+]] MEMORY_MAP = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_param:[0-9]+]] param: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_Link:[0-9]+]] Link: ptr<@type[[TYPE__LIST_ENTRY]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_Entry:[0-9]+]] Entry: ptr<@type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE__LIST_ENTRY]]>>(%[[VALUE_Link]], int_to_ptr<ptr<@type[[TYPE__LIST_ENTRY]]>, reason=explicit>(read<u64>(%[[VALUE_param]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_Entry]], int_to_ptr<ptr<@type[[TYPE0]]>, reason=explicit>(ptr_diff<i64, element=u8, same_array=required, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE__LIST_ENTRY]]>>(%[[VALUE_Link]])), pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE__LIST_ENTRY]]>>(field1(deref(null<ptr<@type[[TYPE0]]>>)))))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_Entry]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
