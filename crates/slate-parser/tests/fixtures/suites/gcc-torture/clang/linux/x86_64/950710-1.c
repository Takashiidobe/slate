void abort(void);
void exit(int);

struct twelve {
  int a;
  int b;
  int c;
};

struct pair {
  int first;
  int second;
};

struct pair g() {
  struct pair p;
  return p;
}

static void f() {
  int i;
  for (i = 0; i < 1; i++) {
    int j;
    for (j = 0; j < 1; j++) {
      if (0) {
        int k;
        for (k = 0; k < 1; k++) {
          struct pair e = g();
        }
      } else {
        struct twelve a, b;
        if ((((char *)&b - (char *)&a) < 0
                 ? (-((char *)&b - (char *)&a))
                 : ((char *)&b - (char *)&a)) < sizeof(a))
          abort();
      }
    }
  }
}

int main(void) {
  f();
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
// DEFAULT-NEXT:     type @type[[TYPE_twelve:[0-9]+]] twelve = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> @type[[TYPE_pair]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: @type[[TYPE_pair]] [storage=automatic];
// DEFAULT-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:                     for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:                                         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                             init:
// DEFAULT-NEXT:                                                 write<i32>(%[[VALUE_k]], const<i32>(0));
// DEFAULT-NEXT:                                             condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(1))
// DEFAULT-NEXT:                                             increment: {
// DEFAULT-NEXT:                                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                                 yield void;
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                             body:
// DEFAULT-NEXT:                                                 {
// DEFAULT-NEXT:                                                     let %[[VALUE_e:[0-9]+]] e: @type[[TYPE_pair]] [storage=automatic] = copy<@type[[TYPE_pair]], reason=assign>(call<@type[[TYPE_pair]], signature=fn() -> @type[[TYPE_pair]], abi=sysv64() -> native_c>(%[[VALUE_g]]));
// DEFAULT-NEXT:                                                 }
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_twelve]] [storage=automatic];
// DEFAULT-NEXT:                                         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_twelve]] [storage=automatic];
// DEFAULT-NEXT:                                         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(conditional<i64>(lt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_twelve]]>>(%[[VALUE_b]])), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_twelve]]>>(%[[VALUE_a]]))), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_twelve]]>>(%[[VALUE_b]])), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_twelve]]>>(%[[VALUE_a]])))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_twelve]]>>(%[[VALUE_b]])), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_twelve]]>>(%[[VALUE_a]]))))), const<u64>(12))
// DEFAULT-NEXT:                                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
