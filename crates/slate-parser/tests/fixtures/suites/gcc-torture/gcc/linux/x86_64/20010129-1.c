/* { dg-options "-mtune=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } }
 */

extern void abort(void);
extern void exit(int);

long baz1(void *a) {
  static long l;
  return l++;
}

int baz2(const char *a) { return 0; }

int baz3(int i) {
  if (!i)
    abort();
  return 1;
}

void **bar;

int foo(void *a, long b, int c) {
  int    d = 0, e, f = 0, i;
  char   g[256];
  void **h;

  g[0] = '\n';
  g[1] = 0;

  while (baz1(a) < b) {
    if (g[0] != ' ' && g[0] != '\t') {
      f = 1;
      e = 0;
      if (!d && baz2(g) == 0) {
        if ((c & 0x10) == 0)
          continue;
        e = d = 1;
      }
      if (!((c & 0x10) && (c & 0x4000) && e) && (c & 2))
        continue;
      if ((c & 0x2000) && baz2(g) == 0)
        continue;
      if ((c & 0x1408) && baz2(g) == 0)
        continue;
      if ((c & 0x200) && baz2(g) == 0)
        continue;
      if (c & 0x80) {
        for (h = bar, i = 0; h; h = (void **)*h, i++)
          if (baz3(i))
            break;
      }
      f = 0;
    }
  }
  return 0;
}

int main() {
  void *n = 0;
  bar     = &n;
  foo(&n, 1, 0xc811);
  exit(0);
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
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_bar:[0-9]+]] bar: ptr<ptr<void>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_baz1:[0-9]+]] @baz1(%[[VALUE_a:[0-9]+]] a: ptr<void>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_l]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE1]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_l]], read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz2:[0-9]+]] @baz2(%[[VALUE_a_2:[0-9]+]] a: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz3:[0-9]+]] @baz3(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a_3:[0-9]+]] a: ptr<void>, %[[VALUE_b:[0-9]+]] b: i64, %[[VALUE_c:[0-9]+]] c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: ptr<ptr<void>> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] lt<i64>(call<i64, signature=fn(ptr<void>) -> i64>(%[[VALUE_baz1]], read<ptr<void>>(%[[VALUE_a_3]])), read<i64>(%[[VALUE_b]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]), const<i32>(0))))), const<i32>(32)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]), const<i32>(0))))), const<i32>(9)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_f]], const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_e]], const<i32>(0));
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                         if not<bool>(ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0)))
// DEFAULT-NEXT:                             write<bool>(%[[VALUE4]], eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_baz2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%[[VALUE4]], const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if eq<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(16)), const<i32>(0))
// DEFAULT-NEXT:                                     continue %[[VALUE3]];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_e]], const<i32>(1));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if logical_and<bool>(not<bool>(logical_and<bool>(logical_and<bool>(ne<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(16)), const<i32>(0)), ne<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(16384)), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0)))), ne<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(2)), const<i32>(0)))
// DEFAULT-NEXT:                             continue %[[VALUE3]];
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(8192)), const<i32>(0))
// DEFAULT-NEXT:                             write<bool>(%[[VALUE5]], eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_baz2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%[[VALUE5]], const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:                             continue %[[VALUE3]];
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(5128)), const<i32>(0))
// DEFAULT-NEXT:                             write<bool>(%[[VALUE6]], eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_baz2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%[[VALUE6]], const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:                             continue %[[VALUE3]];
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(512)), const<i32>(0))
// DEFAULT-NEXT:                             write<bool>(%[[VALUE7]], eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_baz2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_g]]))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%[[VALUE7]], const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:                             continue %[[VALUE3]];
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%[[VALUE_c]]), const<i32>(128)), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<ptr<ptr<void>>>(%[[VALUE_h]], read<ptr<ptr<void>>>(%[[VALUE_bar]]));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:                                     condition: ne<ptr<ptr<void>>>(read<ptr<ptr<void>>>(%[[VALUE_h]]), null<ptr<ptr<void>>>)
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         write<ptr<ptr<void>>>(%[[VALUE_h]], pointer_cast<ptr<ptr<void>>, reason=explicit>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_h]])))));
// DEFAULT-NEXT:                                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_baz3]], read<i32>(%[[VALUE_i_2]])), const<i32>(0))
// DEFAULT-NEXT:                                             break %[[VALUE8]];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_f]], const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         write<ptr<ptr<void>>>(%[[VALUE_bar]], addr_of<ptr<ptr<void>>>(%[[VALUE_n]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, i64, i32) -> i32>(%[[VALUE_foo]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<void>>>(%[[VALUE_n]])), widen<i64, reason=arg>(const<i32>(1)), const<i32>(51217));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
