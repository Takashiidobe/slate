char          B[256 * sizeof(void *)];
typedef void *FILE;
typedef struct globals {
  int   c;
  FILE *l;
} __attribute__((may_alias)) T;
void        add_input_file(FILE *file) { (*(T *)&B).l[0] = file; }
extern void abort(void);
int         main() {
  FILE x;
  (*(T *)&B).l = &x;
  add_input_file((void *)-1);
  if ((*(T *)&B).l[0] != (void *)-1)
    abort();
  return 0;
}


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
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_globals:[0-9]+]] globals = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 l: ptr<ptr<void>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = @type[[TYPE_globals]];
// DEFAULT-NEXT:     global %[[VALUE_B:[0-9]+]] B: array<i8, 2048> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_input_file:[0-9]+]] @add_input_file(%[[VALUE_file:[0-9]+]] file: ptr<ptr<void>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(read<ptr<ptr<void>>>(field1(deref(pointer_cast<ptr<@type[[TYPE_globals]]>, reason=explicit>(addr_of<ptr<array<i8, 2048>>>(%[[VALUE_B]]))))), const<i32>(0))), pointer_cast<ptr<void>, reason=assign>(read<ptr<ptr<void>>>(%[[VALUE_file]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<ptr<void>>>(field1(deref(pointer_cast<ptr<@type[[TYPE_globals]]>, reason=explicit>(addr_of<ptr<array<i8, 2048>>>(%[[VALUE_B]])))), addr_of<ptr<ptr<void>>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%[[VALUE_add_input_file]], pointer_cast<ptr<ptr<void>>, reason=arg>(int_to_ptr<ptr<void>, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(read<ptr<ptr<void>>>(field1(deref(pointer_cast<ptr<@type[[TYPE_globals]]>, reason=explicit>(addr_of<ptr<array<i8, 2048>>>(%[[VALUE_B]]))))), const<i32>(0)))), int_to_ptr<ptr<void>, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
