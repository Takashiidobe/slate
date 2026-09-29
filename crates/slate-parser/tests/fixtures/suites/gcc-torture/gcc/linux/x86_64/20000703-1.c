void abort(void);
void exit(int);
struct baz {
  char         a[17];
  char         b[3];
  unsigned int c;
  unsigned int d;
};

void foo(struct baz *p, unsigned int c, unsigned int d) {
  __builtin_memcpy(p->b, "abc", 3);
  p->c = c;
  p->d = d;
}

void bar(struct baz *p, unsigned int c, unsigned int d) {
  ({
    void *s = (p);
    __builtin_memset(s, '\0', sizeof(struct baz));
    s;
  });
  __builtin_memcpy(p->a, "01234567890123456", 17);
  __builtin_memcpy(p->b, "abc", 3);
  p->c = c;
  p->d = d;
}

int main() {
  struct baz p;
  foo(&p, 71, 18);
  if (p.c != 71 || p.d != 18)
    abort();
  bar(&p, 59, 26);
  if (p.c != 59 || p.d != 26)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_baz:[0-9]+]] baz = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 17>;
// DEFAULT-NEXT:         field1 b: array<i8, 3>;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:     } [size=28, align=4, offsets=[0, 17, 20, 24]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_baz]]>, %[[VALUE_c:[0-9]+]] c: u32, %[[VALUE_d:[0-9]+]] d: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(field1(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p]]))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p]]))), read<u32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<u32>(field3(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p]]))), read<u32>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE5:[0-9]+]] <unnamed>: i32, %[[VALUE6:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_baz]]>, %[[VALUE_c_2:[0-9]+]] c: u32, %[[VALUE_d_2:[0-9]+]] d: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_s:[0-9]+]] s: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_s]]), const<i32>(0), const<u64>(28));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE7]], read<ptr<void>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(field0(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p_2]]))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(17))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(field1(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p_2]]))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p_2]]))), read<u32>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         write<u32>(field3(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p_2]]))), read<u32>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: @type[[TYPE_baz]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_baz]]>, u32, u32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p_3]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(71)), reinterpret<u32, reason=arg, fits=always>(const<i32>(18)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(read<u32>(field2(%[[VALUE_p_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(71))), ne<u32>(read<u32>(field3(%[[VALUE_p_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(18))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_baz]]>, u32, u32) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_baz]]>>(%[[VALUE_p_3]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(59)), reinterpret<u32, reason=arg, fits=always>(const<i32>(26)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(read<u32>(field2(%[[VALUE_p_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(59))), ne<u32>(read<u32>(field3(%[[VALUE_p_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(26))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
