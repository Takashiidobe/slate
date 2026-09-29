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
// DEFAULT-NEXT:     type @type0 offset_v1 = struct {
// DEFAULT-NEXT:         field0 k_uniqueness: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 offset_v2 = struct {
// DEFAULT-NEXT:         field0 v: i64;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type2 reiserfs_key = struct {
// DEFAULT-NEXT:         field0 k_objectid: i32;
// DEFAULT-NEXT:         field1 u: @type3;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type3 = union {
// DEFAULT-NEXT:         field0 k_offset_v1: @type0;
// DEFAULT-NEXT:         field1 k_offset_v2: @type1;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 item_head = struct {
// DEFAULT-NEXT:         field0 ih_key: @type2;
// DEFAULT-NEXT:         field1 ih_version: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 12]];
// DEFAULT-NEXT:     fn %5 @set_offset_v2_k_type(%6 v2: ptr<@type1>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %24: ptr<@type1> [synthetic] = read<ptr<@type1>>(%6);
// DEFAULT-NEXT:         let %25: i64 [synthetic] = read<i64>(field0(deref(read<ptr<@type1>>(%24))));
// DEFAULT-NEXT:         let %26: i64 [synthetic] = and<i64>(read<i64>(%25), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(field0(deref(read<ptr<@type1>>(%24))), read<i64>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @set_le_key_k_type(%8 version: i32, %9 key: ptr<@type2>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(field0(field0(field1(deref(read<ptr<@type2>>(%9))))), const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type1>) -> void>(%5, addr_of<ptr<@type1>>(field1(field1(deref(read<ptr<@type2>>(%9))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @set_le_ih_k_type(%11 ih: ptr<@type4>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<@type2>) -> void>(%7, conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), read<i32>(field1(deref(read<ptr<@type4>>(%11)))), read<i32>(field1(deref(read<ptr<@type4>>(%11))))), addr_of<ptr<@type2>>(field0(deref(read<ptr<@type4>>(%11)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @boo(%19 ih: ptr<@type4>, %20 body: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %23 @__builtin_expect(%21 <unnamed>: i64, %22 <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %15 @direct2indirect() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 p_le_ih: ptr<@type4> [storage=automatic];
// DEFAULT-NEXT:         let %17 ind_ih: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %18 unfm_ptr: u32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%23, widen<i64, reason=arg>(const<i32>(32)), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0))
// DEFAULT-NEXT:             asm "break" [dialect=att] [options=nostack];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>) -> void>(%10, addr_of<ptr<@type4>>(%17));
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), const<i32>(1), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 const<i32>(1);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type4>, ptr<const i8>) -> void>(%14, addr_of<ptr<@type4>>(%17), pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<u32>>(%18))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
