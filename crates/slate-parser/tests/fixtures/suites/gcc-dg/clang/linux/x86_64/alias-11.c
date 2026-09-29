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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_dw_cfi_struct:[0-9]+]] dw_cfi_struct = struct {
// DEFAULT-NEXT:         field0 dw_cfi_next: ptr<@type[[TYPE_dw_cfi_struct]]>;
// DEFAULT-NEXT:         field1 dw_cfi_addr: ptr<const i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_dw_cfi_node:[0-9]+]] dw_cfi_node = @type[[TYPE_dw_cfi_struct]];
// DEFAULT-NEXT:     type @type[[TYPE_dw_fde_struct:[0-9]+]] dw_fde_struct = struct {
// DEFAULT-NEXT:         field0 dw_fde_current_label: ptr<const i8>;
// DEFAULT-NEXT:         field1 dw_fde_cfi: ptr<@type[[TYPE_dw_cfi_struct]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_dw_fde_node:[0-9]+]] dw_fde_node = @type[[TYPE_dw_fde_struct]];
// DEFAULT-NEXT:     global %[[VALUE_cie_cfi_head:[0-9]+]] cie_cfi_head: ptr<@type[[TYPE_dw_cfi_struct]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fde_table_in_use:[0-9]+]] fde_table_in_use: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fde_table:[0-9]+]] fde_table: ptr<@type[[TYPE_dw_fde_struct]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_label:[0-9]+]] label: array<i8, 20> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_label_num:[0-9]+]] label_num: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([42, 46, 37, 115, 37, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([76, 67, 70, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([42, 46, 76, 67, 70, 73, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_realloc:[0-9]+]] @realloc(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>, %[[VALUE___size_2:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s_2:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strdup:[0-9]+]] @strdup(%[[VALUE___s_3:[0-9]+]] __s: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_cfi:[0-9]+]] @add_cfi(%[[VALUE_list_head:[0-9]+]] list_head: ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>, %[[VALUE_cfi:[0-9]+]] cfi: ptr<@type[[TYPE_dw_cfi_struct]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<ptr<@type[[TYPE_dw_cfi_struct]]>> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(%[[VALUE_p]], read<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(%[[VALUE_list_head]]));
// DEFAULT-NEXT:             condition: ne<ptr<@type[[TYPE_dw_cfi_struct]]>>(read<ptr<@type[[TYPE_dw_cfi_struct]]>>(deref(read<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(%[[VALUE_p]]))), null<ptr<@type[[TYPE_dw_cfi_struct]]>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(%[[VALUE_p]], addr_of<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(field0(deref(read<ptr<@type[[TYPE_dw_cfi_struct]]>>(deref(read<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(%[[VALUE_p]])))))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_dw_cfi_struct]]>>(deref(read<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(%[[VALUE_p]])), read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_new_cfi:[0-9]+]] @new_cfi() -> ptr<@type[[TYPE_dw_cfi_struct]]> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cfi_2:[0-9]+]] cfi: ptr<@type[[TYPE_dw_cfi_struct]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_dw_cfi_struct]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(16)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_2]])), const<i32>(0), const<u64>(16));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dwarf2out_cfi_label:[0-9]+]] @dwarf2out_cfi_label() -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_label_num]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_label_num]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_label]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]]), truncate<u32, reason=explicit, fits=unknown>(read<u64>(%[[VALUE1]])));
// DEFAULT-NEXT:         return array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_label]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_add_fde_cfi:[0-9]+]] @add_fde_cfi(%[[VALUE_label_2:[0-9]+]] label: ptr<const i8>, %[[VALUE_cfi_3:[0-9]+]] cfi: ptr<@type[[TYPE_dw_cfi_struct]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_label_2]]), null<ptr<const i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_fde:[0-9]+]] fde: ptr<@type[[TYPE_dw_fde_struct]]> [storage=automatic] = ptr_offset<ptr<@type[[TYPE_dw_fde_struct]]>, subtract=true, element=@type[[TYPE_dw_fde_struct]], overflow=ub>(ptr_offset<ptr<@type[[TYPE_dw_fde_struct]]>, subtract=false, element=@type[[TYPE_dw_fde_struct]], overflow=ub>(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_table]]), read<u32>(%[[VALUE_fde_table_in_use]])), const<i32>(1));
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_label_2]])))), const<i32>(0))
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_label_2]], pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn() -> ptr<i8>>(%[[VALUE_dwarf2out_cfi_label]])));
// DEFAULT-NEXT:                     pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn() -> ptr<i8>>(%[[VALUE_dwarf2out_cfi_label]]));
// DEFAULT-NEXT:                 if logical_or<bool>(eq<ptr<const i8>>(read<ptr<const i8>>(field0(deref(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde]])))), null<ptr<const i8>>), ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_label_2]]), read<ptr<const i8>>(field0(deref(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde]]))))), const<i32>(0)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xcfi:[0-9]+]] xcfi: ptr<@type[[TYPE_dw_cfi_struct]]> [storage=automatic];
// DEFAULT-NEXT:                         write<ptr<const i8>>(%[[VALUE_label_2]], pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_strdup]], read<ptr<const i8>>(%[[VALUE_label_2]]))));
// DEFAULT-NEXT:                         write<ptr<const i8>>(field0(deref(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde]]))), pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_strdup]], read<ptr<const i8>>(%[[VALUE_label_2]]))));
// DEFAULT-NEXT:                         pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_strdup]], read<ptr<const i8>>(%[[VALUE_label_2]])));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_xcfi]], call<ptr<@type[[TYPE_dw_cfi_struct]]>, signature=fn() -> ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_new_cfi]]));
// DEFAULT-NEXT:                         call<ptr<@type[[TYPE_dw_cfi_struct]]>, signature=fn() -> ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_new_cfi]]);
// DEFAULT-NEXT:                         write<ptr<const i8>>(field1(deref(read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_xcfi]]))), read<ptr<const i8>>(%[[VALUE_label_2]]));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>, ptr<@type[[TYPE_dw_cfi_struct]]>) -> void>(%[[VALUE_add_cfi]], addr_of<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(field1(deref(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde]])))), read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_xcfi]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>, ptr<@type[[TYPE_dw_cfi_struct]]>) -> void>(%[[VALUE_add_cfi]], addr_of<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(field1(deref(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde]])))), read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>, ptr<@type[[TYPE_dw_cfi_struct]]>) -> void>(%[[VALUE_add_cfi]], addr_of<ptr<ptr<@type[[TYPE_dw_cfi_struct]]>>>(%[[VALUE_cie_cfi_head]]), read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_cfi_4:[0-9]+]] cfi: ptr<@type[[TYPE_dw_cfi_struct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_fde_2:[0-9]+]] fde: ptr<@type[[TYPE_dw_fde_struct]]> [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_fde_table_in_use]], reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_table]], pointer_cast<ptr<@type[[TYPE_dw_fde_struct]]>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_table]])), const<u64>(16))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type[[TYPE_dw_fde_struct]]>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_table]])), const<u64>(16)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_table]])), const<i32>(0), const<u64>(16));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_4]], call<ptr<@type[[TYPE_dw_cfi_struct]]>, signature=fn() -> ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_new_cfi]]));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_dw_cfi_struct]]>, signature=fn() -> ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_new_cfi]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<@type[[TYPE_dw_cfi_struct]]>) -> void>(%[[VALUE_add_fde_cfi]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])), read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_4]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_2]], addr_of<ptr<@type[[TYPE_dw_fde_struct]]>>(deref(ptr_offset<ptr<@type[[TYPE_dw_fde_struct]]>, subtract=false, element=@type[[TYPE_dw_fde_struct]], overflow=ub>(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_table]]), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_4]], read<ptr<@type[[TYPE_dw_cfi_struct]]>>(field1(deref(read<ptr<@type[[TYPE_dw_fde_struct]]>>(%[[VALUE_fde_2]])))));
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_dw_cfi_struct]]>>(read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_4]]), null<ptr<@type[[TYPE_dw_cfi_struct]]>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(read<ptr<const i8>>(field1(deref(read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_4]])))), null<ptr<const i8>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_4]])), read<ptr<const i8>>(field1(deref(read<ptr<@type[[TYPE_dw_cfi_struct]]>>(%[[VALUE_cfi_4]]))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
