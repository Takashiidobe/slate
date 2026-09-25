extern int renamed asm("real_name");
extern int concatenated __asm("con" "cat");
int function_label(void) __asm__("function_symbol");
extern int first asm("first_symbol"), second, third __asm__("third_symbol");
extern int with_attribute asm("attributed") __attribute__((weak));
static int initialized asm("initialized_symbol") = 3;
typedef int labeled_type asm("typedef_symbol");
int (*function_pointer)(void) asm("pointer_symbol");
register long stack_pointer asm("rsp");

void locals(void) {
  register int eax_register asm("eax");
  register long percent_register asm("%r9"), hash_register asm("#r10b") = 0;
  register int high_byte asm("ah");
  register int numbered asm("0x7");
  register int xmm_register asm("xmm16");
  static int static_local asm("static_local_symbol");
  extern int extern_local asm("extern_local_symbol");
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
// DEFAULT-NEXT:     type @type0 labeled_type = i32;
// DEFAULT-NEXT:     extern %0 renamed: i32 [storage=static] [linkage=external] [asm_name="real_name"];
// DEFAULT-NEXT:     extern %1 concatenated: i32 [storage=static] [linkage=external] [asm_name="concat"];
// DEFAULT-NEXT:     extern %3 first: i32 [storage=static] [linkage=external] [asm_name="first_symbol"];
// DEFAULT-NEXT:     extern %4 second: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %5 third: i32 [storage=static] [linkage=external] [asm_name="third_symbol"];
// DEFAULT-NEXT:     extern %6 with_attribute: i32 [storage=static] [linkage=external] [asm_name="attributed"] [weak];
// DEFAULT-NEXT:     global %7 initialized: i32 [storage=static] = const<i32>(3) [linkage=internal] [asm_name="initialized_symbol"];
// DEFAULT-NEXT:     global %9 function_pointer: ptr<fn() -> i32> [storage=static] [linkage=external] [asm_name="pointer_symbol"];
// DEFAULT-NEXT:     global %10 stack_pointer: i64 [storage=static] [register="rsp"] [linkage=external];
// DEFAULT-NEXT:     global %18 static_local: i32 [storage=static] [linkage=internal] [asm_name="static_local_symbol"];
// DEFAULT-NEXT:     extern %19 extern_local: i32 [storage=static] [linkage=external] [asm_name="extern_local_symbol"];
// DEFAULT-NEXT:     fn %2 @function_label() -> i32 [linkage=external] [asm_name="function_symbol"];
// DEFAULT-NEXT:     fn %11 @locals() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 eax_register: i32 [storage=automatic] [register="eax"];
// DEFAULT-NEXT:         let %13 percent_register: i64 [storage=automatic] [register="%r9"];
// DEFAULT-NEXT:         let %14 hash_register: i64 [storage=automatic] [register="#r10b"] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %15 high_byte: i32 [storage=automatic] [register="ah"];
// DEFAULT-NEXT:         let %16 numbered: i32 [storage=automatic] [register="0x7"];
// DEFAULT-NEXT:         let %17 xmm_register: i32 [storage=automatic] [register="xmm16"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
