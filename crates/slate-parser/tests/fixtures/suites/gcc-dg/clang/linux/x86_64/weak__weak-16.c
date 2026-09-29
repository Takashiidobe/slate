/* From PR37280.  */
/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-fno-common -Os" } */
/* { dg-final { scan-weak "kallsyms_token_index" } } */
/* { dg-final { scan-weak "kallsyms_token_table" } } */
/* { dg-skip-if "" { x86_64-*-mingw* } } */
/* NVPTX's weak is applied to the definition,  not declaration.  */
/* { dg-skip-if "" { nvptx-*-* } } */
/* { dg-skip-if PR119369 { amdgcn-*-* } } */

extern int kallsyms_token_index[] __attribute__((weak));
extern int kallsyms_token_table[] __attribute__((weak));
void kallsyms_expand_symbol(int *result)
{
  int len = *result;
  int *tptr;
  while(len) {
    tptr = &kallsyms_token_table[ kallsyms_token_index[*result] ];
    len--;
    while (*tptr) tptr++;
    *tptr = 1;
  }
 *result = 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fno-common
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
// DEFAULT-NEXT:     extern %[[VALUE_kallsyms_token_index:[0-9]+]] kallsyms_token_index: array<i32, incomplete> [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %[[VALUE_kallsyms_token_table:[0-9]+]] kallsyms_token_table: array<i32, incomplete> [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     fn %[[VALUE_kallsyms_expand_symbol:[0-9]+]] @kallsyms_expand_symbol(%[[VALUE_result:[0-9]+]] result: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE_result]])));
// DEFAULT-NEXT:         let %[[VALUE_tptr:[0-9]+]] tptr: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_len]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_tptr]], addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%[[VALUE_kallsyms_token_table]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%[[VALUE_kallsyms_token_index]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_result]]))))))))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 while %[[VALUE3:[0-9]+]] ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_tptr]]))), const<i32>(0))
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_tptr]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i32>>(%[[VALUE_tptr]], read<ptr<i32>>(%[[VALUE5]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_tptr]])), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_result]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
