/* { dg-do run } */
/* { dg-require-alias "" } */
/* { dg-options "-O2" } */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dw_cfi_struct
{
  struct dw_cfi_struct *dw_cfi_next;
  const char *dw_cfi_addr;
}
dw_cfi_node;

typedef struct dw_fde_struct
{
  const char *dw_fde_current_label;
  dw_cfi_node *dw_fde_cfi;
}
dw_fde_node;

dw_cfi_node *cie_cfi_head;
unsigned fde_table_in_use;
dw_fde_node *fde_table;

static __inline__ void
add_cfi (dw_cfi_node **list_head, dw_cfi_node *cfi)
{
  dw_cfi_node **p;

  for (p = list_head; (*p) != ((void *)0); p = &(*p)->dw_cfi_next)
    ;

  *p = cfi;
}

static __inline__ struct dw_cfi_struct *
new_cfi (void)
{
  dw_cfi_node *cfi = (dw_cfi_node *) malloc (sizeof (dw_cfi_node));

  memset (cfi, 0, sizeof (dw_cfi_node));
  return cfi;
}

char *
dwarf2out_cfi_label (void)
{
  static char label[20];
  static unsigned long label_num = 0;

  sprintf (label, "*.%s%u", "LCFI", (unsigned) (label_num++));
  return label;
}

void
add_fde_cfi (const char *label, dw_cfi_node *cfi)
{
  if (label)
    {
      dw_fde_node *fde = fde_table + fde_table_in_use - 1;

      if (*label == 0)
	label = dwarf2out_cfi_label ();

      if (fde->dw_fde_current_label == ((void *)0)
	  || strcmp (label, fde->dw_fde_current_label))
	{
	  dw_cfi_node *xcfi;

	  fde->dw_fde_current_label = label = strdup (label);

	  xcfi = new_cfi ();
	  xcfi->dw_cfi_addr = label;
	  add_cfi (&fde->dw_fde_cfi, xcfi);
	}

      add_cfi (&fde->dw_fde_cfi, cfi);
    }
  else
    add_cfi (&cie_cfi_head, cfi);
}

int
main ()
{
  dw_cfi_node *cfi;
  dw_fde_node *fde;

  fde_table_in_use = 1;
  fde_table = (dw_fde_node *) realloc (fde_table,
				       sizeof (dw_fde_node));
  memset (fde_table, 0, sizeof (dw_fde_node));
  cfi = new_cfi ();
  add_fde_cfi ("", cfi);

  fde = &fde_table[0];
  cfi = fde->dw_fde_cfi;

  if (cfi == NULL)
    abort ();

  if (cfi->dw_cfi_addr == NULL)
    abort ();

  if (strcmp ("*.LCFI0", cfi->dw_cfi_addr))
    abort ();

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 dw_cfi_struct = struct {
// DEFAULT-NEXT:         field0 dw_cfi_next: ptr<@type1>;
// DEFAULT-NEXT:         field1 dw_cfi_addr: ptr<const i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 dw_cfi_node = @type1;
// DEFAULT-NEXT:     type @type3 dw_fde_struct = struct {
// DEFAULT-NEXT:         field0 dw_fde_current_label: ptr<const i8>;
// DEFAULT-NEXT:         field1 dw_fde_cfi: ptr<@type1>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type4 dw_fde_node = @type3;
// DEFAULT-NEXT:     global %23 cie_cfi_head: ptr<@type1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %24 fde_table_in_use: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %25 fde_table: ptr<@type3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %33 label: array<i8, 20> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %34 label_num: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([42, 46, 37, 115, 37, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([76, 67, 70, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([42, 46, 76, 67, 70, 73, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @sprintf(%43 __s: ptr<i8> [restrict], %44 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @malloc(%45 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @realloc(%46 __ptr: ptr<void>, %47 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @memset(%48 __s: ptr<void>, %49 __c: i32, %50 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %16 @strcmp(%51 __s1: ptr<const i8>, %52 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %18 @strdup(%53 __s: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %26 @add_cfi(%27 list_head: ptr<ptr<@type1>>, %28 cfi: ptr<@type1>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %29 p: ptr<ptr<@type1>> [storage=automatic];
// DEFAULT-NEXT:         for %54
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<ptr<@type1>>>(%29, read<ptr<ptr<@type1>>>(%27));
// DEFAULT-NEXT:             condition: ne<ptr<@type1>>(read<ptr<@type1>>(deref(read<ptr<ptr<@type1>>>(%29))), null<ptr<@type1>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<ptr<@type1>>>(%29, addr_of<ptr<ptr<@type1>>>(field0(deref(read<ptr<@type1>>(deref(read<ptr<ptr<@type1>>>(%29)))))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         write<ptr<@type1>>(deref(read<ptr<ptr<@type1>>>(%29)), read<ptr<@type1>>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @new_cfi() -> ptr<@type1> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 cfi: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, const<u64>(16)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%31)), const<i32>(0), const<u64>(16));
// DEFAULT-NEXT:         return read<ptr<@type1>>(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @dwarf2out_cfi_label() -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %59: u64 [synthetic] = read<u64>(%34);
// DEFAULT-NEXT:         let %60: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%59), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%34, read<u64>(%60));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%3, array_decay<ptr<i8>, length=Some(20)>(%33), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%55)), array_decay<ptr<i8>, length=Some(5)>(%56), truncate<u32, reason=explicit, fits=unknown>(read<u64>(%59)));
// DEFAULT-NEXT:         return array_decay<ptr<i8>, length=Some(20)>(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @add_fde_cfi(%36 label: ptr<const i8>, %37 cfi: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<const i8>>(read<ptr<const i8>>(%36), null<ptr<const i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %38 fde: ptr<@type3> [storage=automatic] = ptr_offset<ptr<@type3>, subtract=true, element=@type3, overflow=ub>(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%25), read<u32>(%24)), const<i32>(1));
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%36)))), const<i32>(0))
// DEFAULT-NEXT:                     write<ptr<const i8>>(%36, pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn() -> ptr<i8>>(%32)));
// DEFAULT-NEXT:                     pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn() -> ptr<i8>>(%32));
// DEFAULT-NEXT:                 if logical_or<bool>(eq<ptr<const i8>>(read<ptr<const i8>>(field0(deref(read<ptr<@type3>>(%38)))), null<ptr<const i8>>), ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%16, read<ptr<const i8>>(%36), read<ptr<const i8>>(field0(deref(read<ptr<@type3>>(%38))))), const<i32>(0)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %39 xcfi: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:                         write<ptr<const i8>>(%36, pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%18, read<ptr<const i8>>(%36))));
// DEFAULT-NEXT:                         write<ptr<const i8>>(field0(deref(read<ptr<@type3>>(%38))), pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%18, read<ptr<const i8>>(%36))));
// DEFAULT-NEXT:                         pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%18, read<ptr<const i8>>(%36)));
// DEFAULT-NEXT:                         write<ptr<@type1>>(%39, call<ptr<@type1>, signature=fn() -> ptr<@type1>>(%30));
// DEFAULT-NEXT:                         call<ptr<@type1>, signature=fn() -> ptr<@type1>>(%30);
// DEFAULT-NEXT:                         write<ptr<const i8>>(field1(deref(read<ptr<@type1>>(%39))), read<ptr<const i8>>(%36));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<ptr<@type1>>, ptr<@type1>) -> void>(%26, addr_of<ptr<ptr<@type1>>>(field1(deref(read<ptr<@type3>>(%38)))), read<ptr<@type1>>(%39));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<ptr<@type1>>, ptr<@type1>) -> void>(%26, addr_of<ptr<ptr<@type1>>>(field1(deref(read<ptr<@type3>>(%38)))), read<ptr<@type1>>(%37));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<ptr<@type1>>, ptr<@type1>) -> void>(%26, addr_of<ptr<ptr<@type1>>>(%23), read<ptr<@type1>>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %41 cfi: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %42 fde: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%24, reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type3>>(%25, pointer_cast<ptr<@type3>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%8, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%25)), const<u64>(16))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type3>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%8, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%25)), const<u64>(16)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%25)), const<i32>(0), const<u64>(16));
// DEFAULT-NEXT:         write<ptr<@type1>>(%41, call<ptr<@type1>, signature=fn() -> ptr<@type1>>(%30));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn() -> ptr<@type1>>(%30);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<@type1>) -> void>(%35, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%57)), read<ptr<@type1>>(%41));
// DEFAULT-NEXT:         write<ptr<@type3>>(%42, addr_of<ptr<@type3>>(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%25), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<@type1>>(%41, read<ptr<@type1>>(field1(deref(read<ptr<@type3>>(%42)))));
// DEFAULT-NEXT:         if eq<ptr<@type1>>(read<ptr<@type1>>(%41), null<ptr<@type1>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(read<ptr<const i8>>(field1(deref(read<ptr<@type1>>(%41)))), null<ptr<const i8>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%58)), read<ptr<const i8>>(field1(deref(read<ptr<@type1>>(%41))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
