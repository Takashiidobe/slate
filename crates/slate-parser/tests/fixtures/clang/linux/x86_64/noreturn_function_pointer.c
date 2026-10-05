// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef void (*handler_fn)(int);
typedef void __attribute__((noreturn)) noreturn_fn(int);

struct efi_boot_services {
  int __attribute__((noreturn)) (*exit)(void *, int, unsigned long, unsigned short *);
  void (*after)(int) __attribute__((noreturn));
  int __attribute__((noreturn)) not_callable;
};

void __attribute__((noreturn)) (*single)(int);
void __attribute__((noreturn)) (**double_pointer)(int);
void __attribute__((noreturn)) (*table[2])(int);
handler_fn __attribute__((noreturn)) typedef_pointer;
handler_fn __attribute__((noreturn)) typedef_table[2];
int __attribute__((noreturn)) plain;
void declared(void) __attribute__((noreturn));

void take(void __attribute__((noreturn)) (*callback)(int));

int run(struct efi_boot_services *services) {
  return services->exit(0, 1, 2, 0);
}

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN: ⚠ 'noreturn' attribute ignored; it applies only to function types
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/noreturn_function_pointer.c:8:22]
// WARN: 7 │   void (*after)(int) __attribute__((noreturn));
// WARN: 8 │   int __attribute__((noreturn)) not_callable;
// WARN: ·                      ────────
// WARN: 9 │ };
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'noreturn' attribute ignored; it applies only to function types
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/noreturn_function_pointer.c:16:20]
// WARN: 15 │ handler_fn __attribute__((noreturn)) typedef_table[2];
// WARN: 16 │ int __attribute__((noreturn)) plain;
// WARN: ·                    ────────
// WARN: 17 │ void declared(void) __attribute__((noreturn));
// WARN: ╰────
// SLATE-FILECHECK-END WARN
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
// DEFAULT-NEXT:     type @type[[TYPE_handler_fn:[0-9]+]] handler_fn = ptr<fn(i32) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_noreturn_fn:[0-9]+]] noreturn_fn = fn(i32) -> void;
// DEFAULT-NEXT:     type @type[[TYPE_efi_boot_services:[0-9]+]] efi_boot_services = struct {
// DEFAULT-NEXT:         field0 exit: ptr<fn(ptr<void>, i32, u64, ptr<u16>) -> i32>;
// DEFAULT-NEXT:         field1 after: ptr<fn(i32) -> void>;
// DEFAULT-NEXT:         field2 not_callable: i32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %[[VALUE_single:[0-9]+]] single: ptr<fn(i32) -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_double_pointer:[0-9]+]] double_pointer: ptr<ptr<fn(i32) -> void>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<ptr<fn(i32) -> void>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_typedef_pointer:[0-9]+]] typedef_pointer: ptr<fn(i32) -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_typedef_table:[0-9]+]] typedef_table: array<ptr<fn(i32) -> void>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_declared:[0-9]+]] @declared() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32) -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_run:[0-9]+]] @run(%[[VALUE_services:[0-9]+]] services: ptr<@type[[TYPE_efi_boot_services]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32>(read<ptr<fn(ptr<void>, i32, u64, ptr<u16>) -> i32>>(field0(deref(read<ptr<@type[[TYPE_efi_boot_services]]>>(%[[VALUE_services]])))), null<ptr<void>>, const<i32>(1), reinterpret<u64>(widen<i64>(const<i32>(2))), null<ptr<u16>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f80;
// IR-WARN-NEXT:         storage bool [size=1, align=1];
// IR-WARN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN-NEXT:         storage f16 [size=2, align=2];
// IR-WARN-NEXT:         storage f32 [size=4, align=4];
// IR-WARN-NEXT:         storage f64 [size=8, align=8];
// IR-WARN-NEXT:         storage f80 [size=16, align=16];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     type @type[[TYPE_handler_fn:[0-9]+]] handler_fn = ptr<fn(i32) -> void>;
// IR-WARN-NEXT:     type @type[[TYPE_noreturn_fn:[0-9]+]] noreturn_fn = fn(i32) -> void;
// IR-WARN-NEXT:     type @type[[TYPE_efi_boot_services:[0-9]+]] efi_boot_services = struct {
// IR-WARN-NEXT:         field0 exit: ptr<fn(ptr<void>, i32, u64, ptr<u16>) -> i32>;
// IR-WARN-NEXT:         field1 after: ptr<fn(i32) -> void>;
// IR-WARN-NEXT:         field2 not_callable: i32;
// IR-WARN-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-WARN-NEXT:     global %[[VALUE_single:[0-9]+]] single: ptr<fn(i32) -> void> [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_double_pointer:[0-9]+]] double_pointer: ptr<ptr<fn(i32) -> void>> [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<ptr<fn(i32) -> void>, 2> [storage=static] [align=16] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_typedef_pointer:[0-9]+]] typedef_pointer: ptr<fn(i32) -> void> [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_typedef_table:[0-9]+]] typedef_table: array<ptr<fn(i32) -> void>, 2> [storage=static] [align=16] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_declared:[0-9]+]] @declared() -> void [linkage=external] [noreturn];
// IR-WARN-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32) -> void>) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_run:[0-9]+]] @run(%[[VALUE_services:[0-9]+]] services: ptr<@type[[TYPE_efi_boot_services]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return call<i32>(read<ptr<fn(ptr<void>, i32, u64, ptr<u16>) -> i32>>(field0(deref(read<ptr<@type[[TYPE_efi_boot_services]]>>(%[[VALUE_services]])))), null<ptr<void>>, const<i32>(1), reinterpret<u64>(widen<i64>(const<i32>(2))), null<ptr<u16>>);
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
