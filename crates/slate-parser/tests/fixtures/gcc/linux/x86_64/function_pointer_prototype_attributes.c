// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct list_head;
typedef int __attribute__((nonnull(2, 3))) (*list_cmp_func_t)(void *, const struct list_head *, const struct list_head *);
typedef int (*fprintf_ftype)(void *, const char *, ...) __attribute__((format(printf, 2, 3)));
typedef int printf_like(const char *, ...) __attribute__((format(printf, 1, 2)));
typedef void *(*allocator_fn)(unsigned long) __attribute__((alloc_size(1)));

struct disassemble_info {
  fprintf_ftype fprintf_func;
  int (*fprintf_styled_func)(void *, int, const char *, ...) __attribute__((format(printf, 3, 4)));
  void *(*aligned_alloc)(unsigned long, unsigned long) __attribute__((alloc_align(1)));
  int __attribute__((nonnull)) not_a_function;
};

int __attribute__((nonnull(1))) (**double_pointer)(void *);
int (*table[2])(const char *, ...) __attribute__((format(printf, 1, 2)));
int __attribute__((format(printf, 1, 2))) plain;
struct __attribute__((nonnull)) not_a_function_record { int value; };
struct __attribute__((noreturn)) not_noreturn_record { int value; };

void list_sort(void *priv, struct list_head *head, list_cmp_func_t cmp);
void callback(int __attribute__((nonnull(1))) (*visit)(void *), int *pointer __attribute__((nonnull)), int count __attribute__((nonnull)));

int print(struct disassemble_info *info) {
  return info->fprintf_func(0, "%d", 1);
}

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN: ⚠ 'nonnull' attribute ignored; it applies only to functions, methods, and
// WARN: │ parameters
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:12:22]
// WARN: 11 │   void *(*aligned_alloc)(unsigned long, unsigned long) __attribute__((alloc_align(1)));
// WARN: 12 │   int __attribute__((nonnull)) not_a_function;
// WARN: ·                      ───────
// WARN: 13 │ };
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'nonnull' attribute ignored; it applies only to functions, methods, and
// WARN: │ parameters
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:15:20]
// WARN: 14 │
// WARN: 15 │ int __attribute__((nonnull(1))) (**double_pointer)(void *);
// WARN: ·                    ───────
// WARN: 16 │ int (*table[2])(const char *, ...) __attribute__((format(printf, 1, 2)));
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'format' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:16:51]
// WARN: 15 │ int __attribute__((nonnull(1))) (**double_pointer)(void *);
// WARN: 16 │ int (*table[2])(const char *, ...) __attribute__((format(printf, 1, 2)));
// WARN: ·                                                   ──────
// WARN: 17 │ int __attribute__((format(printf, 1, 2))) plain;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'format' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:17:20]
// WARN: 16 │ int (*table[2])(const char *, ...) __attribute__((format(printf, 1, 2)));
// WARN: 17 │ int __attribute__((format(printf, 1, 2))) plain;
// WARN: ·                    ──────
// WARN: 18 │ struct __attribute__((nonnull)) not_a_function_record { int value; };
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'nonnull' attribute ignored; it applies only to functions, methods, and
// WARN: │ parameters
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:18:23]
// WARN: 17 │ int __attribute__((format(printf, 1, 2))) plain;
// WARN: 18 │ struct __attribute__((nonnull)) not_a_function_record { int value; };
// WARN: ·                       ───────
// WARN: 19 │ struct __attribute__((noreturn)) not_noreturn_record { int value; };
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'noreturn' attribute ignored; it applies only to function types
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:19:23]
// WARN: 18 │ struct __attribute__((nonnull)) not_a_function_record { int value; };
// WARN: 19 │ struct __attribute__((noreturn)) not_noreturn_record { int value; };
// WARN: ·                       ────────
// WARN: 20 │
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'nonnull' attribute ignored; it applies only to functions, methods, and
// WARN: │ parameters
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:22:93]
// WARN: 21 │ void list_sort(void *priv, struct list_head *head, list_cmp_func_t cmp);
// WARN: 22 │ void callback(int __attribute__((nonnull(1))) (*visit)(void *), int *pointer __attribute__((nonnull)), int count __attribute__((nonnull)));
// WARN: ·                                                                                             ───────
// WARN: 23 │
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'nonnull' attribute ignored; it applies only to functions, methods, and
// WARN: │ parameters
// WARN: ╭─[tests/fixtures/gcc/linux/x86_64/function_pointer_prototype_attributes.c:22:129]
// WARN: 21 │ void list_sort(void *priv, struct list_head *head, list_cmp_func_t cmp);
// WARN: 22 │ void callback(int __attribute__((nonnull(1))) (*visit)(void *), int *pointer __attribute__((nonnull)), int count __attribute__((nonnull)));
// WARN: ·                                                                                                                                 ───────
// WARN: 23 │
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
// DEFAULT-NEXT:     type @type[[TYPE_list_head:[0-9]+]] list_head = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_list_cmp_func_t:[0-9]+]] list_cmp_func_t = ptr<fn(ptr<void>, ptr<const @type[[TYPE_list_head]]>, ptr<const @type[[TYPE_list_head]]>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_fprintf_ftype:[0-9]+]] fprintf_ftype = ptr<fn(ptr<void>, ptr<const i8>, ...) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_printf_like:[0-9]+]] printf_like = fn(ptr<const i8>, ...) -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_allocator_fn:[0-9]+]] allocator_fn = ptr<fn(u64) -> ptr<void>>;
// DEFAULT-NEXT:     type @type[[TYPE_disassemble_info:[0-9]+]] disassemble_info = struct {
// DEFAULT-NEXT:         field0 fprintf_func: ptr<fn(ptr<void>, ptr<const i8>, ...) -> i32>;
// DEFAULT-NEXT:         field1 fprintf_styled_func: ptr<fn(ptr<void>, i32, ptr<const i8>, ...) -> i32>;
// DEFAULT-NEXT:         field2 aligned_alloc: ptr<fn(u64, u64) -> ptr<void>>;
// DEFAULT-NEXT:         field3 not_a_function: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_not_a_function_record:[0-9]+]] not_a_function_record = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_not_noreturn_record:[0-9]+]] not_noreturn_record = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_double_pointer:[0-9]+]] double_pointer: ptr<ptr<fn(ptr<void>) -> i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<ptr<fn(ptr<const i8>, ...) -> i32>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_list_sort:[0-9]+]] @list_sort(%[[VALUE_priv:[0-9]+]] priv: ptr<void>, %[[VALUE_head:[0-9]+]] head: ptr<@type[[TYPE_list_head]]>, %[[VALUE_cmp:[0-9]+]] cmp: ptr<fn(ptr<void>, ptr<const @type[[TYPE_list_head]]>, ptr<const @type[[TYPE_list_head]]>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_callback:[0-9]+]] @callback(%[[VALUE_visit:[0-9]+]] visit: ptr<fn(ptr<void>) -> i32>, %[[VALUE_pointer:[0-9]+]] pointer: ptr<i32>, %[[VALUE_count:[0-9]+]] count: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_print:[0-9]+]] @print(%[[VALUE_info:[0-9]+]] info: ptr<@type[[TYPE_disassemble_info]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32>(read<ptr<fn(ptr<void>, ptr<const i8>, ...) -> i32>>(field0(deref(read<ptr<@type[[TYPE_disassemble_info]]>>(%[[VALUE_info]])))), null<ptr<void>>, pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), const<i32>(1));
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
// IR-WARN-NEXT:     type @type[[TYPE_list_head:[0-9]+]] list_head = struct incomplete;
// IR-WARN-NEXT:     type @type[[TYPE_list_cmp_func_t:[0-9]+]] list_cmp_func_t = ptr<fn(ptr<void>, ptr<const @type[[TYPE_list_head]]>, ptr<const @type[[TYPE_list_head]]>) -> i32>;
// IR-WARN-NEXT:     type @type[[TYPE_fprintf_ftype:[0-9]+]] fprintf_ftype = ptr<fn(ptr<void>, ptr<const i8>, ...) -> i32>;
// IR-WARN-NEXT:     type @type[[TYPE_printf_like:[0-9]+]] printf_like = fn(ptr<const i8>, ...) -> i32;
// IR-WARN-NEXT:     type @type[[TYPE_allocator_fn:[0-9]+]] allocator_fn = ptr<fn(u64) -> ptr<void>>;
// IR-WARN-NEXT:     type @type[[TYPE_disassemble_info:[0-9]+]] disassemble_info = struct {
// IR-WARN-NEXT:         field0 fprintf_func: ptr<fn(ptr<void>, ptr<const i8>, ...) -> i32>;
// IR-WARN-NEXT:         field1 fprintf_styled_func: ptr<fn(ptr<void>, i32, ptr<const i8>, ...) -> i32>;
// IR-WARN-NEXT:         field2 aligned_alloc: ptr<fn(u64, u64) -> ptr<void>>;
// IR-WARN-NEXT:         field3 not_a_function: i32;
// IR-WARN-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// IR-WARN-NEXT:     type @type[[TYPE_not_a_function_record:[0-9]+]] not_a_function_record = struct {
// IR-WARN-NEXT:         field0 value: i32;
// IR-WARN-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-WARN-NEXT:     type @type[[TYPE_not_noreturn_record:[0-9]+]] not_noreturn_record = struct {
// IR-WARN-NEXT:         field0 value: i32;
// IR-WARN-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-WARN-NEXT:     global %[[VALUE_double_pointer:[0-9]+]] double_pointer: ptr<ptr<fn(ptr<void>) -> i32>> [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<ptr<fn(ptr<const i8>, ...) -> i32>, 2> [storage=static] [align=16] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// IR-WARN-NEXT:     fn %[[VALUE_list_sort:[0-9]+]] @list_sort(%[[VALUE_priv:[0-9]+]] priv: ptr<void>, %[[VALUE_head:[0-9]+]] head: ptr<@type[[TYPE_list_head]]>, %[[VALUE_cmp:[0-9]+]] cmp: ptr<fn(ptr<void>, ptr<const @type[[TYPE_list_head]]>, ptr<const @type[[TYPE_list_head]]>) -> i32>) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_callback:[0-9]+]] @callback(%[[VALUE_visit:[0-9]+]] visit: ptr<fn(ptr<void>) -> i32>, %[[VALUE_pointer:[0-9]+]] pointer: ptr<i32>, %[[VALUE_count:[0-9]+]] count: i32) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_print:[0-9]+]] @print(%[[VALUE_info:[0-9]+]] info: ptr<@type[[TYPE_disassemble_info]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return call<i32>(read<ptr<fn(ptr<void>, ptr<const i8>, ...) -> i32>>(field0(deref(read<ptr<@type[[TYPE_disassemble_info]]>>(%[[VALUE_info]])))), null<ptr<void>>, pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), const<i32>(1));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
