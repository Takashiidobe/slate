/* PR optimization/11320 */
/* Origin: Andreas Schwab <schwab@suse.de> */

/* Verify that the scheduler correctly computes the dependencies
   in the presence of conditional instructions.  */

int strcmp(const char *, const char *);
int ap_standalone;

const char *ap_check_cmd_context(void *a, int b) { return 0; }

const char *server_type(void *a, void *b, char *arg) {
  const char *err = ap_check_cmd_context(a, 0x01 | 0x02 | 0x04 | 0x08 | 0x10);
  if (err)
    return err;

  if (!strcmp(arg, "inetd"))
    ap_standalone = 0;
  else if (!strcmp(arg, "standalone"))
    ap_standalone = 1;
  else
    return "ServerType must be either 'inetd' or 'standalone'";

  return 0;
}

int main() {
  server_type(0, 0, "standalone");
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
// DEFAULT-NEXT:     global %[[VALUE_ap_standalone:[0-9]+]] ap_standalone: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([105, 110, 101, 116, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 116, 97, 110, 100, 97, 108, 111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 50> [storage=static] = code_units<array<i8, 50>>([83, 101, 114, 118, 101, 114, 84, 121, 112, 101, 32, 109, 117, 115, 116, 32, 98, 101, 32, 101, 105, 116, 104, 101, 114, 32, 39, 105, 110, 101, 116, 100, 39, 32, 111, 114, 32, 39, 115, 116, 97, 110, 100, 97, 108, 111, 110, 101, 39, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 116, 97, 110, 100, 97, 108, 111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ap_check_cmd_context:[0-9]+]] @ap_check_cmd_context(%[[VALUE_a:[0-9]+]] a: ptr<void>, %[[VALUE_b:[0-9]+]] b: i32) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<const i8>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_server_type:[0-9]+]] @server_type(%[[VALUE_a_2:[0-9]+]] a: ptr<void>, %[[VALUE_b_2:[0-9]+]] b: ptr<void>, %[[VALUE_arg:[0-9]+]] arg: ptr<i8>) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_err:[0-9]+]] err: ptr<const i8> [storage=automatic] = call<ptr<const i8>, signature=fn(ptr<void>, i32) -> ptr<const i8>>(%[[VALUE_ap_check_cmd_context]], read<ptr<void>>(%[[VALUE_a_2]]), or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(1), const<i32>(2)), const<i32>(4)), const<i32>(8)), const<i32>(16)));
// DEFAULT-NEXT:         if ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_err]]), null<ptr<const i8>>)
// DEFAULT-NEXT:             return read<ptr<const i8>>(%[[VALUE_err]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_arg]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str]]))), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_ap_standalone]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_arg]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_2]]))), const<i32>(0)))
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_ap_standalone]], const<i32>(1));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(50)>(%[[VALUE_str_3]]));
// DEFAULT-NEXT:         return null<ptr<const i8>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<const i8>, signature=fn(ptr<void>, ptr<void>, ptr<i8>) -> ptr<const i8>>(%[[VALUE_server_type]], null<ptr<void>>, null<ptr<void>>, array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
