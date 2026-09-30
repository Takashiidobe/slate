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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 n: ptr<@type[[TYPE_s]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_s]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ss:[0-9]+]] ss: @type[[TYPE_s]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sss:[0-9]+]] sss: array<@type[[TYPE_s]], 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sub:[0-9]+]] @sub(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_s]]>, %[[VALUE_pp:[0-9]+]] pp: ptr<ptr<@type[[TYPE_s]]>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_s]]>, ptr<ptr<@type[[TYPE_s]]>>) -> i32>(%[[VALUE_look:[0-9]+]], read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_2]]), read<ptr<ptr<@type[[TYPE_s]]>>>(%[[VALUE_pp]])), const<i32>(0))
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<ptr<@type[[TYPE_s]]>>(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_2]]), null<ptr<@type[[TYPE_s]]>>)
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_2]], read<ptr<@type[[TYPE_s]]>>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_2]])))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         break %[[VALUE2]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_look]] @look(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_s]]>, %[[VALUE_pp_2:[0-9]+]] pp: ptr<ptr<@type[[TYPE_s]]>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<ptr<@type[[TYPE_s]]>>(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_3]]), null<ptr<@type[[TYPE_s]]>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_3]], read<ptr<@type[[TYPE_s]]>>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_3]])))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_s]]>>(deref(read<ptr<ptr<@type[[TYPE_s]]>>>(%[[VALUE_pp_2]])), read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_pp_3:[0-9]+]] pp: ptr<@type[[TYPE_s]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_next:[0-9]+]] next: ptr<@type[[TYPE_s]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_s]]>>(%[[VALUE_p]], addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_ss]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_s]]>>(%[[VALUE_next]], read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_s]]>>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_next]]))), addr_of<ptr<@type[[TYPE_s]]>>(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false, element=@type[[TYPE_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>, length=Some(10)>(%[[VALUE_sss]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_s]]>>(%[[VALUE_next]], read<ptr<@type[[TYPE_s]]>>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_next]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_s]]>>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_next]]))), null<ptr<@type[[TYPE_s]]>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_s]]>, ptr<ptr<@type[[TYPE_s]]>>) -> void>(%[[VALUE_sub]], read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p]]), addr_of<ptr<ptr<@type[[TYPE_s]]>>>(%[[VALUE_pp_3]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), add<i32, overflow=ub>(const<i32>(10), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
