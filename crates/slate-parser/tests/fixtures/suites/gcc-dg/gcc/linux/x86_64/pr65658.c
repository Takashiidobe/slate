/* { dg-do compile } */
/* { dg-options "-Wuninitialized -O2 -Wno-implicit" } */
/* { dg-require-effective-target ptr32plus } */

extern int optind;
struct undefinfo
{
  unsigned long l1;
  unsigned long l2;
};
struct undeffoo
{
  char a[64];
  long b[4];
  int c[33];
};
struct problem
{
  __UINTPTR_TYPE__ l1;
  __UINTPTR_TYPE__ l2;
  __UINTPTR_TYPE__ l3;
  __UINTPTR_TYPE__ l4;
};
static unsigned int undef1, undef2, undef3, undef4, undef5, undef6;
static void *undefvp1;
extern struct undefinfo undefinfo;
static int
undefinit1 (void)
{
  struct undeffoo foo;
  int i;
  for (i = 0; i < 2000; i++)
    {
      undef6++;
      external_function5 (((void *) 0), 0, (void *) &foo);
    }
}

static int
undefinit2 (void *problemp, __UINTPTR_TYPE__ problem)
{
  int ret, u;
  if (undefinit1 ())
    return 1;
  if (fn10 ())
    return 1;
  for (u = 0; u < undef6; u++)
    {
      ret = external_function1 (3 + u * 10, 10);
      if (ret)
	return ret;
      external_function6 (0, 0, 0, problemp + problem);
      return 1;
    }
}

static int
fn6 (struct undefinfo *uip, struct problem *problem)
{
  unsigned long amt;
  if (external_function3 (((void *) 0), ((void *) 0), &amt, 0, 0))
    return 1;
  problem->l1 = (__UINTPTR_TYPE__) undefvp1;
  problem->l4 = uip->l1;
  problem->l3 = uip->l2;
  return 0;
}

static int
setup (void)
{
  struct problem problem;
  if (fn6 (&undefinfo, &problem))
    return 1;
  if (fn2 ())
    return 1;
  if (fn4 (101))
    return 1;
  if (undefinit2 ((void *) problem.l1, problem.l3 * 4))  /* { dg-bogus "problem.l3" "uninitialized variable warning" } */ 
    return 1;
}

int
main (int argc, char **argv)
{
  int optc;
  if (external_function (1))
    return 1;
  if (external_function (1))
    return 1;
  if (external_function (1))
    return 1;
  while ((optc =
	  getopt_long (argc, argv, ((void *) 0), ((void *) 0),
		       ((void *) 0))) != -1)
    {
      switch (optc)
	{
	case 0:
	  break;
	case 'F':
	  external_function (1);
	default:
	  return 1;
	}
    }
  if ((optind != 99))
    {
      return 1;
    }
  setup ();
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -Wno-implicit
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
// DEFAULT-NEXT:     type @type[[TYPE_undefinfo:[0-9]+]] undefinfo = struct {
// DEFAULT-NEXT:         field0 l1: u64;
// DEFAULT-NEXT:         field1 l2: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_undeffoo:[0-9]+]] undeffoo = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 64>;
// DEFAULT-NEXT:         field1 b: array<i64, 4>;
// DEFAULT-NEXT:         field2 c: array<i32, 33>;
// DEFAULT-NEXT:     } [size=232, align=8, offsets=[0, 64, 96]];
// DEFAULT-NEXT:     type @type[[TYPE_problem:[0-9]+]] problem = struct {
// DEFAULT-NEXT:         field0 l1: u64;
// DEFAULT-NEXT:         field1 l2: u64;
// DEFAULT-NEXT:         field2 l3: u64;
// DEFAULT-NEXT:         field3 l4: u64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     extern %[[VALUE_optind:[0-9]+]] optind: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_undef1:[0-9]+]] undef1: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_undef2:[0-9]+]] undef2: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_undef3:[0-9]+]] undef3: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_undef4:[0-9]+]] undef4: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_undef5:[0-9]+]] undef5: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_undef6:[0-9]+]] undef6: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_undefvp1:[0-9]+]] undefvp1: ptr<void> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_undefinfo:[0-9]+]] undefinfo: @type[[TYPE_undefinfo]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_external_function5:[0-9]+]] @external_function5(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_undefinit1:[0-9]+]] @undefinit1() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_foo:[0-9]+]] foo: @type[[TYPE_undeffoo]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2000))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_undef6]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_undef6]], read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function5]], null<ptr<void>>, const<i32>(0), pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type[[TYPE_undeffoo]]>>(%[[VALUE_foo]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn10:[0-9]+]] @fn10(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_external_function1:[0-9]+]] @external_function1(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_external_function6:[0-9]+]] @external_function6(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_undefinit2:[0-9]+]] @undefinit2(%[[VALUE_problemp:[0-9]+]] problemp: ptr<void>, %[[VALUE_problem:[0-9]+]] problem: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_undefinit1]]), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_fn10]]), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_u]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_u]])), read<u32>(%[[VALUE_undef6]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_u]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_u]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_ret]], call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function1]], add<i32, overflow=ub>(const<i32>(3), mul<i32, overflow=ub>(read<i32>(%[[VALUE_u]]), const<i32>(10))), const<i32>(10)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_ret]]), const<i32>(0))
// DEFAULT-NEXT:                         return read<i32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function6]], const<i32>(0), const<i32>(0), const<i32>(0), ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(read<ptr<void>>(%[[VALUE_problemp]]), read<u64>(%[[VALUE_problem]])));
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_external_function3:[0-9]+]] @external_function3(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn6:[0-9]+]] @fn6(%[[VALUE_uip:[0-9]+]] uip: ptr<@type[[TYPE_undefinfo]]>, %[[VALUE_problem_2:[0-9]+]] problem: ptr<@type[[TYPE_problem]]>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_amt:[0-9]+]] amt: u64 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function3]], null<ptr<void>>, null<ptr<void>>, addr_of<ptr<u64>>(%[[VALUE_amt]]), const<i32>(0), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         write<u64>(field0(deref(read<ptr<@type[[TYPE_problem]]>>(%[[VALUE_problem_2]]))), ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%[[VALUE_undefvp1]])));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type[[TYPE_problem]]>>(%[[VALUE_problem_2]]))), read<u64>(field0(deref(read<ptr<@type[[TYPE_undefinfo]]>>(%[[VALUE_uip]])))));
// DEFAULT-NEXT:         write<u64>(field2(deref(read<ptr<@type[[TYPE_problem]]>>(%[[VALUE_problem_2]]))), read<u64>(field1(deref(read<ptr<@type[[TYPE_undefinfo]]>>(%[[VALUE_uip]])))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_setup:[0-9]+]] @setup() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_problem_3:[0-9]+]] problem: @type[[TYPE_problem]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_undefinfo]]>, ptr<@type[[TYPE_problem]]>) -> i32>(%[[VALUE_fn6]], addr_of<ptr<@type[[TYPE_undefinfo]]>>(%[[VALUE_undefinfo]]), addr_of<ptr<@type[[TYPE_problem]]>>(%[[VALUE_problem_3]])), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_fn2]]), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_fn4]], const<i32>(101)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, u64) -> i32>(%[[VALUE_undefinit2]], int_to_ptr<ptr<void>, reason=explicit>(read<u64>(field0(%[[VALUE_problem_3]]))), mul<u64, overflow=wrap>(read<u64>(field2(%[[VALUE_problem_3]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_external_function:[0-9]+]] @external_function(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_getopt_long:[0-9]+]] @getopt_long(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_optc:[0-9]+]] optc: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function]], const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function]], const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function]], const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         while %[[VALUE8:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_getopt_long]], read<i32>(%[[VALUE_argc]]), read<ptr<ptr<i8>>>(%[[VALUE_argv]]), null<ptr<void>>, null<ptr<void>>, null<ptr<void>>);
// DEFAULT-NEXT:             write<i32>(%[[VALUE_optc]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE9]]), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %[[VALUE10:[0-9]+]] read<i32>(%[[VALUE_optc]])
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE10]] const<i32>(0):
// DEFAULT-NEXT:                             break %[[VALUE10]];
// DEFAULT-NEXT:                         case %[[VALUE10]] const<i32>(70):
// DEFAULT-NEXT:                             call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_external_function]], const<i32>(1));
// DEFAULT-NEXT:                         default %[[VALUE10]]:
// DEFAULT-NEXT:                             return const<i32>(1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_optind]]), const<i32>(99))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_setup]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
