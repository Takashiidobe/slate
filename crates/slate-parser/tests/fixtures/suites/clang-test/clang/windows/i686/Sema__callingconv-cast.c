
// expected-note@+1 {{consider defining 'mismatched_before_winapi' with the 'stdcall' calling convention}}
void mismatched_before_winapi(int x) {}

#ifdef MSVC
#define WINAPI __stdcall
#else
#define WINAPI __attribute__((stdcall))
#endif

// expected-note@+1 3 {{consider defining 'mismatched' with the 'stdcall' calling convention}}
void mismatched(int x) {}

// expected-note@+1 {{consider defining 'mismatched_declaration' with the 'stdcall' calling convention}}
void mismatched_declaration(int x);

// expected-note@+1 {{consider defining 'suggest_fix_first_redecl' with the 'stdcall' calling convention}}
void suggest_fix_first_redecl(int x);
void suggest_fix_first_redecl(int x);

typedef void (WINAPI *callback_t)(int);
void take_callback(callback_t callback);

void WINAPI mismatched_stdcall(int x) {}

void take_opaque_fn(void (*callback)(int));

int main(void) {
  // expected-warning@+1 {{cast between incompatible calling conventions 'cdecl' and 'stdcall'}}
  take_callback((callback_t)mismatched);

  // expected-warning@+1 {{cast between incompatible calling conventions 'cdecl' and 'stdcall'}}
  callback_t callback = (callback_t)mismatched; // warns
  (void)callback;

  // expected-warning@+1 {{cast between incompatible calling conventions 'cdecl' and 'stdcall'}}
  callback = (callback_t)&mismatched; // warns

  // No warning, just to show we don't drill through other kinds of unary operators.
  callback = (callback_t)!mismatched;

  // expected-warning@+1 {{cast between incompatible calling conventions 'cdecl' and 'stdcall'}}
  callback = (callback_t)&mismatched_before_winapi; // warns

  // Probably a bug, but we don't warn.
  void (*callback2)(int) = mismatched;
  take_callback((callback_t)callback2);

  // Another way to suppress the warning.
  take_callback((callback_t)(void*)mismatched);

  // Warn on declarations as well as definitions.
  // expected-warning@+1 {{cast between incompatible calling conventions 'cdecl' and 'stdcall'}}
  take_callback((callback_t)mismatched_declaration);
  // expected-warning@+1 {{cast between incompatible calling conventions 'cdecl' and 'stdcall'}}
  take_callback((callback_t)suggest_fix_first_redecl);

  // Don't warn, because we're casting from stdcall to cdecl. Usually that means
  // the programmer is rinsing the function pointer through some kind of opaque
  // API.
  take_opaque_fn((void (*)(int))mismatched_stdcall);
}

// SLATE-FILECHECK-DEFINES CFG0 -DMSVC
// SLATE-FILECHECK-STD CFG0 gnu17
// SLATE-FILECHECK-PREFIX-ARGS CFG0 -Wcast-calling-convention -Wno-pointer-bool-conversion
// SLATE-FILECHECK-DEFINES CFG1 -DMSVC
// SLATE-FILECHECK-STD CFG1 gnu17
// SLATE-FILECHECK-PREFIX-ARGS CFG1 -Wcast-calling-convention

// SLATE-FILECHECK-BEGIN CFG0
// CFG0: module {
// CFG0-NEXT:     target "i686-pc-windows-msvc" {
// CFG0-NEXT:         endian = little;
// CFG0-NEXT:         pointer [size=4, align=4];
// CFG0-NEXT:         stack_alignment = 4;
// CFG0-NEXT:         long_double = f64;
// CFG0-NEXT:         storage bool [size=1, align=1];
// CFG0-NEXT:         storage i8, u8 [size=1, align=1];
// CFG0-NEXT:         storage i16, u16 [size=2, align=2];
// CFG0-NEXT:         storage i32, u32 [size=4, align=4];
// CFG0-NEXT:         storage i64, u64 [size=8, align=8];
// CFG0-NEXT:         storage i128, u128 [size=16, align=16];
// CFG0-NEXT:         storage bf16 [size=2, align=2];
// CFG0-NEXT:         storage f16 [size=2, align=2];
// CFG0-NEXT:         storage f32 [size=4, align=4];
// CFG0-NEXT:         storage f64 [size=8, align=8];
// CFG0-NEXT:         storage f128 [size=16, align=16];
// CFG0-NEXT:         storage d32 [size=4, align=4];
// CFG0-NEXT:         storage d64 [size=8, align=8];
// CFG0-NEXT:         storage d128 [size=16, align=16];
// CFG0-NEXT:     }
// CFG0-NEXT:     type @type[[TYPE_callback_t:[0-9]+]] callback_t = ptr<fn stdcall(i32) -> void>;
// CFG0-NEXT:     fn %[[VALUE_mismatched_before_winapi:[0-9]+]] @mismatched_before_winapi(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// CFG0-NEXT:     }
// CFG0-NEXT:     fn %[[VALUE_mismatched:[0-9]+]] @mismatched(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// CFG0-NEXT:     }
// CFG0-NEXT:     fn %[[VALUE_mismatched_declaration:[0-9]+]] @mismatched_declaration(%[[VALUE_x_3:[0-9]+]] x: i32) -> void [linkage=external];
// CFG0-NEXT:     fn %[[VALUE_suggest_fix_first_redecl:[0-9]+]] @suggest_fix_first_redecl(%[[VALUE_x_4:[0-9]+]] x: i32) -> void [linkage=external];
// CFG0-NEXT:     fn %[[VALUE_take_callback:[0-9]+]] @take_callback(%[[VALUE_callback:[0-9]+]] callback: ptr<fn stdcall(i32) -> void>) -> void [linkage=external];
// CFG0-NEXT:     fn %[[VALUE_mismatched_stdcall:[0-9]+]] @mismatched_stdcall(%[[VALUE_x_5:[0-9]+]] x: i32) -> void [linkage=external] [abi=x86_win32 stdcall(scalar) -> void] [fallthrough=ret_void] {
// CFG0-NEXT:     }
// CFG0-NEXT:     fn %[[VALUE_take_opaque_fn:[0-9]+]] @take_opaque_fn(%[[VALUE_callback_2:[0-9]+]] callback: ptr<fn(i32) -> void>) -> void [linkage=external];
// CFG0-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CFG0-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]])));
// CFG0-NEXT:         let %[[VALUE_callback_3:[0-9]+]] callback: ptr<fn stdcall(i32) -> void> [storage=automatic] = pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]));
// CFG0-NEXT:         read<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]]);
// CFG0-NEXT:         write<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(addr_of<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]])));
// CFG0-NEXT:         write<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]], int_to_ptr<ptr<fn stdcall(i32) -> void>, reason=explicit>(not<bool>(ne<ptr<fn(i32) -> void>>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]), null<ptr<fn(i32) -> void>>))));
// CFG0-NEXT:         write<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(addr_of<ptr<fn(i32) -> void>>(%[[VALUE_mismatched_before_winapi]])));
// CFG0-NEXT:         let %[[VALUE_callback2:[0-9]+]] callback2: ptr<fn(i32) -> void> [storage=automatic] = function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]);
// CFG0-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(read<ptr<fn(i32) -> void>>(%[[VALUE_callback2]])));
// CFG0-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]))));
// CFG0-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched_declaration]])));
// CFG0-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_suggest_fix_first_redecl]])));
// CFG0-NEXT:         call<void, signature=fn(ptr<fn(i32) -> void>) -> void>(%[[VALUE_take_opaque_fn]], pointer_cast<ptr<fn(i32) -> void>, reason=explicit>(function_decay<ptr<fn stdcall(i32) -> void>>(%[[VALUE_mismatched_stdcall]])));
// CFG0-NEXT:     }
// CFG0-NEXT: }
// SLATE-FILECHECK-END CFG0
// SLATE-FILECHECK-BEGIN CFG1
// CFG1: module {
// CFG1-NEXT:     target "i686-pc-windows-msvc" {
// CFG1-NEXT:         endian = little;
// CFG1-NEXT:         pointer [size=4, align=4];
// CFG1-NEXT:         stack_alignment = 4;
// CFG1-NEXT:         long_double = f64;
// CFG1-NEXT:         storage bool [size=1, align=1];
// CFG1-NEXT:         storage i8, u8 [size=1, align=1];
// CFG1-NEXT:         storage i16, u16 [size=2, align=2];
// CFG1-NEXT:         storage i32, u32 [size=4, align=4];
// CFG1-NEXT:         storage i64, u64 [size=8, align=8];
// CFG1-NEXT:         storage i128, u128 [size=16, align=16];
// CFG1-NEXT:         storage bf16 [size=2, align=2];
// CFG1-NEXT:         storage f16 [size=2, align=2];
// CFG1-NEXT:         storage f32 [size=4, align=4];
// CFG1-NEXT:         storage f64 [size=8, align=8];
// CFG1-NEXT:         storage f128 [size=16, align=16];
// CFG1-NEXT:         storage d32 [size=4, align=4];
// CFG1-NEXT:         storage d64 [size=8, align=8];
// CFG1-NEXT:         storage d128 [size=16, align=16];
// CFG1-NEXT:     }
// CFG1-NEXT:     type @type[[TYPE_callback_t:[0-9]+]] callback_t = ptr<fn stdcall(i32) -> void>;
// CFG1-NEXT:     fn %[[VALUE_mismatched_before_winapi:[0-9]+]] @mismatched_before_winapi(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// CFG1-NEXT:     }
// CFG1-NEXT:     fn %[[VALUE_mismatched:[0-9]+]] @mismatched(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// CFG1-NEXT:     }
// CFG1-NEXT:     fn %[[VALUE_mismatched_declaration:[0-9]+]] @mismatched_declaration(%[[VALUE_x_3:[0-9]+]] x: i32) -> void [linkage=external];
// CFG1-NEXT:     fn %[[VALUE_suggest_fix_first_redecl:[0-9]+]] @suggest_fix_first_redecl(%[[VALUE_x_4:[0-9]+]] x: i32) -> void [linkage=external];
// CFG1-NEXT:     fn %[[VALUE_take_callback:[0-9]+]] @take_callback(%[[VALUE_callback:[0-9]+]] callback: ptr<fn stdcall(i32) -> void>) -> void [linkage=external];
// CFG1-NEXT:     fn %[[VALUE_mismatched_stdcall:[0-9]+]] @mismatched_stdcall(%[[VALUE_x_5:[0-9]+]] x: i32) -> void [linkage=external] [abi=x86_win32 stdcall(scalar) -> void] [fallthrough=ret_void] {
// CFG1-NEXT:     }
// CFG1-NEXT:     fn %[[VALUE_take_opaque_fn:[0-9]+]] @take_opaque_fn(%[[VALUE_callback_2:[0-9]+]] callback: ptr<fn(i32) -> void>) -> void [linkage=external];
// CFG1-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CFG1-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]])));
// CFG1-NEXT:         let %[[VALUE_callback_3:[0-9]+]] callback: ptr<fn stdcall(i32) -> void> [storage=automatic] = pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]));
// CFG1-NEXT:         read<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]]);
// CFG1-NEXT:         write<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(addr_of<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]])));
// CFG1-NEXT:         write<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]], int_to_ptr<ptr<fn stdcall(i32) -> void>, reason=explicit>(not<bool>(ne<ptr<fn(i32) -> void>>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]), null<ptr<fn(i32) -> void>>))));
// CFG1-NEXT:         write<ptr<fn stdcall(i32) -> void>>(%[[VALUE_callback_3]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(addr_of<ptr<fn(i32) -> void>>(%[[VALUE_mismatched_before_winapi]])));
// CFG1-NEXT:         let %[[VALUE_callback2:[0-9]+]] callback2: ptr<fn(i32) -> void> [storage=automatic] = function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]);
// CFG1-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(read<ptr<fn(i32) -> void>>(%[[VALUE_callback2]])));
// CFG1-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched]]))));
// CFG1-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_mismatched_declaration]])));
// CFG1-NEXT:         call<void, signature=fn(ptr<fn stdcall(i32) -> void>) -> void>(%[[VALUE_take_callback]], pointer_cast<ptr<fn stdcall(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> void>>(%[[VALUE_suggest_fix_first_redecl]])));
// CFG1-NEXT:         call<void, signature=fn(ptr<fn(i32) -> void>) -> void>(%[[VALUE_take_opaque_fn]], pointer_cast<ptr<fn(i32) -> void>, reason=explicit>(function_decay<ptr<fn stdcall(i32) -> void>>(%[[VALUE_mismatched_stdcall]])));
// CFG1-NEXT:     }
// CFG1-NEXT: }
// SLATE-FILECHECK-END CFG1
