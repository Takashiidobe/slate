/* Distilled from optimization/863.  */

extern void abort(void);
extern void exit(int);
extern void ok(int);

typedef struct {
  int x, y, z;
} Data;

void find(Data *first, Data *last) {
  int i;
  for (i = (last - first) >> 2; i > 0; --i)
    ok(i);
  abort();
}

void ok(int i) {
  if (i != 1)
    abort();
  exit(0);
}

int main() {
  Data DataList[4];
  find(DataList + 0, DataList + 4);
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:         field2 z: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 Data = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @ok(%9 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @find(%6 first: ptr<@type0>, %7 last: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, truncate<i32, reason=assign, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(ptr_diff<i64, element=@type0, same_array=required, overflow=ub>(read<ptr<@type0>>(%7), read<ptr<@type0>>(%6)), const<i32>(2))));
// DEFAULT-NEXT:             condition: gt<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%2, read<i32>(%8));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 DataList: array<@type0, 4> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<@type0>) -> void>(%5, ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(%11), const<i32>(0)), ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(%11), const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
