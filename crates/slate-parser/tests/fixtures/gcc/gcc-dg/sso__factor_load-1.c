/* PR tree-optimization/126729 */
/* { dg-do run } */

struct S0 { int a; int v; } __attribute__((scalar_storage_order("little-endian")));
union U0 { struct S0 s; int i[2]; } __attribute__((scalar_storage_order("little-endian")));
struct S1 { int a; int v; } __attribute__((scalar_storage_order("big-endian")));
union U1 { struct S1 s; int i[2]; } __attribute__((scalar_storage_order("big-endian")));


__attribute__((noinline))
int f(void *a, bool b, bool bb)
{
  if (b)
  {
    union U0 t = *((union U0*)a);
    if (bb)
      return t.i[0];
    return t.s.a;
  }
  {
    union U1 t = *((union U1*)a);
    if (bb)
      return t.i[0];
    return t.s.a;
  }
}

__attribute__((noinline))
int f2(void *a, bool b, bool bb)
{
  if (b)
  {
    union U1 t = *((union U1*)a);
    if (bb)
      return t.i[0];
    return t.s.a;
  }
  {
    union U1 t = *((union U1*)a);
    if (bb)
      return t.i[0];
    return t.s.a;
  }
}

int main()
{
  union U1 a;
  union U0 b;
  int t = 0xabcd;
  a.s.a = t;
  b.s.a = t;
  if (f((void*)&a, 0, 0) != t)
    __builtin_abort();
  if (f((void*)&b, 1, 0) != t)
    __builtin_abort();
  if (f2((void*)&a, 1, 0) != t)
    __builtin_abort();
  if (f2((void*)&a, 0, 0) != t)
    __builtin_abort();
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 S0 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 v: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 U0 = union {
// DEFAULT-NEXT:         field0 s: @type0;
// DEFAULT-NEXT:         field1 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 S1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 v: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type3 U1 = union {
// DEFAULT-NEXT:         field0 s: @type2;
// DEFAULT-NEXT:         field1 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %4 @f(%5 a: ptr<void>, %6 b: bool, %7 bb: bool) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if read<bool>(%6)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %8 t: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1>(deref(pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<void>>(%5)))));
// DEFAULT-NEXT:                 if read<bool>(%7)
// DEFAULT-NEXT:                     return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%8)), const<i32>(0))));
// DEFAULT-NEXT:                 return read<i32>(field0(field0(%8)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %9 t: @type3 [storage=automatic] = copy<@type3, reason=assign>(read<@type3>(deref(pointer_cast<ptr<@type3>, reason=explicit>(read<ptr<void>>(%5)))));
// DEFAULT-NEXT:             if read<bool>(%7)
// DEFAULT-NEXT:                 return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%9)), const<i32>(0))));
// DEFAULT-NEXT:             return read<i32>(field0(field0(%9)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f2(%11 a: ptr<void>, %12 b: bool, %13 bb: bool) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %14 t: @type3 [storage=automatic] = copy<@type3, reason=assign>(read<@type3>(deref(pointer_cast<ptr<@type3>, reason=explicit>(read<ptr<void>>(%11)))));
// DEFAULT-NEXT:                 if read<bool>(%13)
// DEFAULT-NEXT:                     return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%14)), const<i32>(0))));
// DEFAULT-NEXT:                 return read<i32>(field0(field0(%14)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %15 t: @type3 [storage=automatic] = copy<@type3, reason=assign>(read<@type3>(deref(pointer_cast<ptr<@type3>, reason=explicit>(read<ptr<void>>(%11)))));
// DEFAULT-NEXT:             if read<bool>(%13)
// DEFAULT-NEXT:                 return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%15)), const<i32>(0))));
// DEFAULT-NEXT:             return read<i32>(field0(field0(%15)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 a: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %18 b: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %19 t: i32 [storage=automatic] = const<i32>(43981);
// DEFAULT-NEXT:         write<i32>(field0(field0(%17)), read<i32>(%19));
// DEFAULT-NEXT:         write<i32>(field0(field0(%18)), read<i32>(%19));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, bool, bool) -> i32>(%4, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type3>>(%17)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0))), read<i32>(%19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, bool, bool) -> i32>(%4, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type1>>(%18)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0))), read<i32>(%19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, bool, bool) -> i32>(%10, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type3>>(%17)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0))), read<i32>(%19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, bool, bool) -> i32>(%10, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type3>>(%17)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0))), read<i32>(%19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
