__attribute__((dllimport)) extern int g_counter;

extern int renamed_target(int) asm("actual_symbol");

// @lowering-fn-begin
// @rewrite-fn-begin
int use_imported_data_and_link_name(void) {
  return g_counter + renamed_target(3);
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) { return use_imported_data_and_link_name(); }

// SLATE-FILECHECK-BEGIN lowering-msvc
// LOWERING-MSVC-DAG: fn use_imported_data_and_link_name() -> i32 {
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = unsafe { g_counter };
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = unsafe { actual_symbol({{__v[0-9]+}} as i32) };
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-MSVC-DAG:     return {{__v[0-9]+}};
// LOWERING-MSVC-DAG: }
// SLATE-FILECHECK-END lowering-msvc

// SLATE-FILECHECK-BEGIN rewrites-msvc
// REWRITES-MSVC-DAG: fn use_imported_data_and_link_name() -> i32 {
// REWRITES-MSVC-DAG:     let {{__v[0-9]+}}: i32 = unsafe { g_counter };
// REWRITES-MSVC-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-MSVC-DAG:     let {{__v[0-9]+}}: i32 = unsafe { actual_symbol({{__v[0-9]+}} as i32) };
// REWRITES-MSVC-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-MSVC-DAG:     {{__v[0-9]+}}
// REWRITES-MSVC-DAG: }
// SLATE-FILECHECK-END rewrites-msvc
