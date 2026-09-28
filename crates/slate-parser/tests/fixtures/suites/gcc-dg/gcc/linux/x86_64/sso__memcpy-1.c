/* { dg-do run } */

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

#define __big_endian_attr__ scalar_storage_order("big-endian")
#define __little_endian_attr__ scalar_storage_order("little-endian")

typedef union
{
  uint32_t val;
  uint8_t v[4];
} __attribute__((__big_endian_attr__)) upal_u32be_t;

typedef union
{
  uint32_t val;
  uint8_t v[4];
} __attribute__((__little_endian_attr__)) upal_u32le_t;

static inline uint32_t native_to_big_endian(uint32_t t)
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  return t;
#else
  return __builtin_bswap32(t);
#endif
}
static inline uint32_t native_to_little_endian(uint32_t t)
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  return __builtin_bswap32(t);
#else
  return t;
#endif
}
#define test(p, p1, i) do { if (p[i] != p1[i]) __builtin_abort (); } while (0)

#define tests(p, p1) do { test(p, p1, 0); test(p, p1, 1); \
                          test(p, p1, 2); test(p, p1, 3); } while (0)

int main(void)
{
  const uint32_t u = 0x12345678;

  upal_u32be_t tempb;
  __builtin_memcpy (&tempb, &u, sizeof(uint32_t));
  uint32_t bu = tempb.val;
  uint32_t b1u = native_to_big_endian(u);
  tests (((uint8_t*)&bu), ((uint8_t*)&b1u));

  upal_u32le_t templ;
  __builtin_memcpy (&templ, &u, sizeof(uint32_t));
  uint32_t lu = templ.val;
  uint32_t l1u = native_to_little_endian(u);
  tests (((uint8_t*)&lu), ((uint8_t*)&l1u));

  return 0;
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
// DEFAULT-NEXT:     type @type0 uint8_t = u8;
// DEFAULT-NEXT:     type @type1 uint32_t = u32;
// DEFAULT-NEXT:     type @type2 = union {
// DEFAULT-NEXT:         field0 val: u32;
// DEFAULT-NEXT:         field1 v: array<u8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 upal_u32be_t = @type2;
// DEFAULT-NEXT:     type @type4 = union {
// DEFAULT-NEXT:         field0 val: u32;
// DEFAULT-NEXT:         field1 v: array<u8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type5 upal_u32le_t = @type4;
// DEFAULT-NEXT:     fn %19 @__builtin_bswap32(%18 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @native_to_big_endian(%7 t: u32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%19, read<u32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @native_to_little_endian(%9 t: u32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @__builtin_memcpy(%20 <unnamed>: ptr<void>, %21 <unnamed>: ptr<const void>, %22 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %26 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 u: u32 [storage=automatic] [const] = reinterpret<u32, reason=assign, fits=always>(const<i32>(305419896));
// DEFAULT-NEXT:         let %12 tempb: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%12)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const u32>>(%11)), const<u64>(4));
// DEFAULT-NEXT:         let %13 bu: u32 [storage=automatic] = read<u32>(field0(%12));
// DEFAULT-NEXT:         let %14 b1u: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%6, read<u32>(%11));
// DEFAULT-NEXT:         do %24
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %25
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%13)), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%14)), const<i32>(0)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %27
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%13)), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%14)), const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %28
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%13)), const<i32>(2)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%14)), const<i32>(2)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %29
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%13)), const<i32>(3)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%14)), const<i32>(3)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         let %15 templ: @type4 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type4>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const u32>>(%11)), const<u64>(4));
// DEFAULT-NEXT:         let %16 lu: u32 [storage=automatic] = read<u32>(field0(%15));
// DEFAULT-NEXT:         let %17 l1u: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%8, read<u32>(%11));
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %31
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%16)), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%17)), const<i32>(0)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %32
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%16)), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%17)), const<i32>(1)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %33
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%16)), const<i32>(2)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%17)), const<i32>(2)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %34
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%16)), const<i32>(3)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%17)), const<i32>(3)))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
