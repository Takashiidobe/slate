/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-options "-O2 -mabi=64" { target { mips*-*-linux* && mips64 } } } */

struct offset_v1 {
    int k_uniqueness;
};

struct offset_v2 {
 long v;
} __attribute__ ((__packed__));

struct reiserfs_key {
    int k_objectid;
    union {
 struct offset_v1 k_offset_v1;
 struct offset_v2 k_offset_v2;
    } u;
};

struct item_head
{
 struct reiserfs_key ih_key;
 int ih_version;
};

static void set_offset_v2_k_type(struct offset_v2 *v2)
{
    v2->v &= 1;
}

static void set_le_key_k_type (int version, struct reiserfs_key * key)
{
    version ? (key->u.k_offset_v1.k_uniqueness = 1)
	    : set_offset_v2_k_type(&(key->u.k_offset_v2));
}

static void set_le_ih_k_type (struct item_head * ih)
{
    set_le_key_k_type((__builtin_constant_p((ih)->ih_version) ? (ih)->ih_version : (ih)->ih_version), &(ih->ih_key));
}

void boo(struct item_head *ih, const char *body);

void direct2indirect(void)
{
    struct item_head *p_le_ih;
    struct item_head ind_ih;
    unsigned int unfm_ptr;

    if (__builtin_expect(32, 0)) __asm__ ("break");

    set_le_ih_k_type (&ind_ih);

    if (__builtin_constant_p(p_le_ih) ? 1 : 2) {
        (__builtin_constant_p(__builtin_constant_p(1) == 1));
      boo(&ind_ih, (char *)&unfm_ptr);
    }
}

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
// DEFAULT-NEXT:     type @type[[TYPE_offset_v1:[0-9]+]] offset_v1 = struct {
// DEFAULT-NEXT:         field0 k_uniqueness: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_offset_v2:[0-9]+]] offset_v2 = struct {
// DEFAULT-NEXT:         field0 v: i64;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_reiserfs_key:[0-9]+]] reiserfs_key = struct {
// DEFAULT-NEXT:         field0 k_objectid: i32;
// DEFAULT-NEXT:         field1 u: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = union {
// DEFAULT-NEXT:         field0 k_offset_v1: @type[[TYPE_offset_v1]];
// DEFAULT-NEXT:         field1 k_offset_v2: @type[[TYPE_offset_v2]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_item_head:[0-9]+]] item_head = struct {
// DEFAULT-NEXT:         field0 ih_key: @type[[TYPE_reiserfs_key]];
// DEFAULT-NEXT:         field1 ih_version: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 12]];
// DEFAULT-NEXT:     fn %[[VALUE_set_offset_v2_k_type:[0-9]+]] @set_offset_v2_k_type(%[[VALUE_v2:[0-9]+]] v2: ptr<@type[[TYPE_offset_v2]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_offset_v2]]> [synthetic] = read<ptr<@type[[TYPE_offset_v2]]>>(%[[VALUE_v2]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type[[TYPE_offset_v2]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = and<i64>(read<i64>(%[[VALUE1]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(field0(deref(read<ptr<@type[[TYPE_offset_v2]]>>(%[[VALUE0]]))), read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_set_le_key_k_type:[0-9]+]] @set_le_key_k_type(%[[VALUE_version:[0-9]+]] version: i32, %[[VALUE_key:[0-9]+]] key: ptr<@type[[TYPE_reiserfs_key]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_version]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(field0(field0(field1(deref(read<ptr<@type[[TYPE_reiserfs_key]]>>(%[[VALUE_key]]))))), const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_offset_v2]]>) -> void>(%[[VALUE_set_offset_v2_k_type]], addr_of<ptr<@type[[TYPE_offset_v2]]>>(field1(field1(deref(read<ptr<@type[[TYPE_reiserfs_key]]>>(%[[VALUE_key]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_set_le_ih_k_type:[0-9]+]] @set_le_ih_k_type(%[[VALUE_ih:[0-9]+]] ih: ptr<@type[[TYPE_item_head]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<@type[[TYPE_reiserfs_key]]>) -> void>(%[[VALUE_set_le_key_k_type]], conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), read<i32>(field1(deref(read<ptr<@type[[TYPE_item_head]]>>(%[[VALUE_ih]])))), read<i32>(field1(deref(read<ptr<@type[[TYPE_item_head]]>>(%[[VALUE_ih]]))))), addr_of<ptr<@type[[TYPE_reiserfs_key]]>>(field0(deref(read<ptr<@type[[TYPE_item_head]]>>(%[[VALUE_ih]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_boo:[0-9]+]] @boo(%[[VALUE_ih_2:[0-9]+]] ih: ptr<@type[[TYPE_item_head]]>, %[[VALUE_body:[0-9]+]] body: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE3:[0-9]+]] <unnamed>: i64, %[[VALUE4:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_direct2indirect:[0-9]+]] @direct2indirect() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p_le_ih:[0-9]+]] p_le_ih: ptr<@type[[TYPE_item_head]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ind_ih:[0-9]+]] ind_ih: @type[[TYPE_item_head]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_unfm_ptr:[0-9]+]] unfm_ptr: u32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], widen<i64, reason=arg>(const<i32>(32)), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0))
// DEFAULT-NEXT:             asm "break" [dialect=att] [options=nostack];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_item_head]]>) -> void>(%[[VALUE_set_le_ih_k_type]], addr_of<ptr<@type[[TYPE_item_head]]>>(%[[VALUE_ind_ih]]));
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), const<i32>(1), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 const<i32>(1);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_item_head]]>, ptr<const i8>) -> void>(%[[VALUE_boo]], addr_of<ptr<@type[[TYPE_item_head]]>>(%[[VALUE_ind_ih]]), pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<u32>>(%[[VALUE_unfm_ptr]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
