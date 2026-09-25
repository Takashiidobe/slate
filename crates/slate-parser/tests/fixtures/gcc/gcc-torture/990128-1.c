extern void abort(void);
extern void exit(int);
extern int  printf(const char *, ...);

struct s {
  struct s *n;
}       *p;
struct s ss;
#define MAX 10
struct s sss[MAX];
int      count = 0;

void sub(struct s *p, struct s **pp);
int  look(struct s *p, struct s **pp);

int main(void) {
  struct s *pp;
  struct s *next;
  int       i;

  p    = &ss;
  next = p;
  for (i = 0; i < MAX; i++) {
    next->n = &sss[i];
    next    = next->n;
  }
  next->n = 0;

  sub(p, &pp);
  if (count != MAX + 2)
    abort();

  exit(0);
}

void sub(struct s *p, struct s **pp) {
  for (; look(p, pp);) {
    if (p)
      p = p->n;
    else
      break;
  }
}

int look(struct s *p, struct s **pp) {
  for (; p; p = p->n)
    ;
  *pp = p;
  count++;
  return (1);
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 n: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %4 p: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 ss: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 sss: array<@type0, 10> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%18 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @printf(%19 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @sub(%14 p: ptr<@type0>, %15 pp: ptr<ptr<@type0>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<ptr<@type0>>) -> i32>(%9, read<ptr<@type0>>(%14), read<ptr<ptr<@type0>>>(%15)), const<i32>(0))
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<ptr<@type0>>(read<ptr<@type0>>(%14), null<ptr<@type0>>)
// DEFAULT-NEXT:                         write<ptr<@type0>>(%14, read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%14)))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         break %25;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @look(%16 p: ptr<@type0>, %17 pp: ptr<ptr<@type0>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<ptr<@type0>>(read<ptr<@type0>>(%16), null<ptr<@type0>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<@type0>>(%16, read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%16)))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%17)), read<ptr<@type0>>(%16));
// DEFAULT-NEXT:         let %27: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%28));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 pp: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %12 next: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(%4, addr_of<ptr<@type0>>(%5));
// DEFAULT-NEXT:         write<ptr<@type0>>(%12, read<ptr<@type0>>(%4));
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%12))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%6), read<i32>(%13)))));
// DEFAULT-NEXT:                     write<ptr<@type0>>(%12, read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%12)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%12))), null<ptr<@type0>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<ptr<@type0>>) -> void>(%8, read<ptr<@type0>>(%4), addr_of<ptr<ptr<@type0>>>(%11));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), add<i32, overflow=ub>(const<i32>(10), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
