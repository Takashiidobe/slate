/* PR rtl-optimization/21330 */

extern void abort(void);
extern int  strcmp(const char *, const char *);

int __attribute__((noinline)) bar(const char **x) { return *(*x)++; }

int __attribute__((noinline)) baz(int c) { return c != '@'; }

void __attribute__((noinline)) foo(const char **w, char *x, _Bool y, _Bool z) {
  char c = bar(w);
  int  i = 0;

  while (1) {
    x[i++] = c;
    c      = bar(w);
    if (y && c == '\'')
      break;
    if (z && c == '\"')
      break;
    if (!y && !z && !baz(c))
      break;
  }
  x[i] = 0;
}

int main(void) {
  char        buf[64];
  const char *p;
  p = "abcde'fgh";
  foo(&p, buf, 1, 0);
  if (strcmp(p, "fgh") != 0 || strcmp(buf, "abcde") != 0)
    abort();
  p = "ABCDEFG\"HI";
  foo(&p, buf, 0, 1);
  if (strcmp(p, "HI") != 0 || strcmp(buf, "ABCDEFG") != 0)
    abort();
  p = "abcd\"e'fgh";
  foo(&p, buf, 1, 1);
  if (strcmp(p, "e'fgh") != 0 || strcmp(buf, "abcd") != 0)
    abort();
  p = "ABCDEF'G\"HI";
  foo(&p, buf, 1, 1);
  if (strcmp(p, "G\"HI") != 0 || strcmp(buf, "ABCDEF") != 0)
    abort();
  p = "abcdef@gh";
  foo(&p, buf, 0, 0);
  if (strcmp(p, "gh") != 0 || strcmp(buf, "abcdef") != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 39, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 98, 99, 100, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([65, 66, 67, 68, 69, 70, 71, 34, 72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([65, 66, 67, 68, 69, 70, 71, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([97, 98, 99, 100, 34, 101, 39, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([101, 39, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 98, 99, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([65, 66, 67, 68, 69, 70, 39, 71, 34, 72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([71, 34, 72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([65, 66, 67, 68, 69, 70, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 64, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<ptr<const i8>>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<ptr<const i8>> [synthetic] = read<ptr<ptr<const i8>>>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(deref(read<ptr<ptr<const i8>>>(%[[VALUE2]])));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(read<ptr<ptr<const i8>>>(%[[VALUE2]])), read<ptr<const i8>>(%[[VALUE4]]));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_c:[0-9]+]] c: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(64)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_w:[0-9]+]] w: ptr<ptr<const i8>>, %[[VALUE_x_2:[0-9]+]] x: ptr<i8>, %[[VALUE_y:[0-9]+]] y: bool, %[[VALUE_z:[0-9]+]] z: bool) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(call<i32, signature=fn(ptr<ptr<const i8>>) -> i32>(%[[VALUE_bar]], read<ptr<ptr<const i8>>>(%[[VALUE_w]])));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %[[VALUE5:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_2]]), read<i32>(%[[VALUE6]]))), read<i8>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_c_2]], truncate<i8, reason=assign, fits=unknown>(call<i32, signature=fn(ptr<ptr<const i8>>) -> i32>(%[[VALUE_bar]], read<ptr<ptr<const i8>>>(%[[VALUE_w]]))));
// DEFAULT-NEXT:                 if logical_and<bool>(read<bool>(%[[VALUE_y]]), eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c_2]])), const<i32>(39)))
// DEFAULT-NEXT:                     break %[[VALUE5]];
// DEFAULT-NEXT:                 if logical_and<bool>(read<bool>(%[[VALUE_z]]), eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c_2]])), const<i32>(34)))
// DEFAULT-NEXT:                     break %[[VALUE5]];
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if logical_and<bool>(not<bool>(read<bool>(%[[VALUE_y]])), not<bool>(read<bool>(%[[VALUE_z]])))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE8]], not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_baz]], widen<i32, reason=arg>(read<i8>(%[[VALUE_c_2]]))), const<i32>(0))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE8]], const<bool>(false));
// DEFAULT-NEXT:                 if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:                     break %[[VALUE5]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_p]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%[[VALUE_foo]], addr_of<ptr<ptr<const i8>>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_p]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]]))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_3]]))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_p]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%[[VALUE_foo]], addr_of<ptr<ptr<const i8>>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_p]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_5]]))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_6]]))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_p]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_7]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%[[VALUE_foo]], addr_of<ptr<ptr<const i8>>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_p]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_8]]))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_9]]))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_p]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%[[VALUE_foo]], addr_of<ptr<ptr<const i8>>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_p]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_11]]))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_12]]))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_p]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_13]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%[[VALUE_foo]], addr_of<ptr<ptr<const i8>>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_p]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_14]]))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_15]]))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE13]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
