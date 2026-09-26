// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

extern struct win *windows, *wtab[];
struct win
{
  struct win *w_next;
};

struct auser;

struct comm
{
  char *name;
  int flags;
};

extern struct comm comms[];

extern int WindowByNoN (char *);
extern int FindCommnr (char *);
extern int AclSetPermCmd (struct auser *, char *, struct comm *);
extern int AclSetPermWin (struct auser *, struct auser *, char *, struct win *);


int
  AclSetPerm(uu, u, mode, s)
    struct auser *uu, *u;
char *mode, *s;
{
  struct win *w;
  int i;
  char *p, ch;

  do 
    {
    }
  while (0);

  while (*s)
    {
      switch (*s)
	{  
	case '*':
	  return AclSetPerm(uu, u, mode, "#?");
	case '#':
	  if (uu)
	    AclSetPermWin(uu, u, mode, (struct win *)1);
	  else
	    for (w = windows; w; w = w->w_next)
	      AclSetPermWin((struct auser *)0, u, mode, w);
	  s++;
	  break;
	case '?':
	  if (uu)
	    AclSetPermWin(uu, u, mode, (struct win *)0);
	  else
	    for (i = 0; i <= 174; i++)
	      AclSetPermCmd(u, mode, &comms[i]);
	  s++;
	  break;
	default:
	  for (p = s; *p && *p != ' ' && *p != '\t' && *p != ','; p++)
	    ;
	  if ((ch = *p))
	    *p++ = '\0';
	  if ((i = FindCommnr(s)) != -1)
	    AclSetPermCmd(u, mode, &comms[i]);
	  else if (((i = WindowByNoN(s)) >= 0) && wtab[i])
	    AclSetPermWin((struct auser *)0, u, mode, wtab[i]);
	  else
	    return -1;
	  if (ch)
	    p[-1] = ch;
	  s = p;
	}
    }

  return 0;
}

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
// DEFAULT-NEXT:     type @type0 win = struct incomplete;
// DEFAULT-NEXT:     type @type1 auser = struct incomplete;
// DEFAULT-NEXT:     type @type2 comm = struct {
// DEFAULT-NEXT:         field0 name: ptr<i8>;
// DEFAULT-NEXT:         field1 flags: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     extern %1 windows: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %2 wtab: array<ptr<@type0>, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %5 comms: array<@type2, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([35, 63, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @WindowByNoN(%19 <unnamed>: ptr<i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @FindCommnr(%20 <unnamed>: ptr<i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @AclSetPermCmd(%21 <unnamed>: ptr<@type1>, %22 <unnamed>: ptr<i8>, %23 <unnamed>: ptr<@type2>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @AclSetPermWin(%24 <unnamed>: ptr<@type1>, %25 <unnamed>: ptr<@type1>, %26 <unnamed>: ptr<i8>, %27 <unnamed>: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @AclSetPerm(%11 uu: ptr<@type1>, %12 u: ptr<@type1>, %13 mode: ptr<i8>, %14 s: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 w: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %16 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %18 ch: i8 [storage=automatic];
// DEFAULT-NEXT:         do %28
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         while %29 ne<i8>(read<i8>(deref(read<ptr<i8>>(%14))), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %30 widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%14))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %30 const<i32>(42):
// DEFAULT-NEXT:                             return call<i32, signature=fn(unprototyped) -> i32>(%10, read<ptr<@type1>>(%11), read<ptr<@type1>>(%12), read<ptr<i8>>(%13), array_decay<ptr<i8>, length=Some(3)>(%31));
// DEFAULT-NEXT:                         case %30 const<i32>(35):
// DEFAULT-NEXT:                             if ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>)
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<@type1>, ptr<@type1>, ptr<i8>, ptr<@type0>) -> i32>(%9, read<ptr<@type1>>(%11), read<ptr<@type1>>(%12), read<ptr<i8>>(%13), int_to_ptr<ptr<@type0>, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 for %32
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<ptr<@type0>>(%15, read<ptr<@type0>>(%1));
// DEFAULT-NEXT:                                     condition: ne<ptr<@type0>>(read<ptr<@type0>>(%15), null<ptr<@type0>>)
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         write<ptr<@type0>>(%15, read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%15)))));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         call<i32, signature=fn(ptr<@type1>, ptr<@type1>, ptr<i8>, ptr<@type0>) -> i32>(%9, null<ptr<@type1>>, read<ptr<@type1>>(%12), read<ptr<i8>>(%13), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                         let %35: ptr<i8> [synthetic] = read<ptr<i8>>(%14);
// DEFAULT-NEXT:                         let %36: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%35), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<i8>>(%14, read<ptr<i8>>(%36));
// DEFAULT-NEXT:                         break %30;
// DEFAULT-NEXT:                         case %30 const<i32>(63):
// DEFAULT-NEXT:                             if ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>)
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<@type1>, ptr<@type1>, ptr<i8>, ptr<@type0>) -> i32>(%9, read<ptr<@type1>>(%11), read<ptr<@type1>>(%12), read<ptr<i8>>(%13), null<ptr<@type0>>);
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 for %33
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:                                     condition: le<i32>(read<i32>(%16), const<i32>(174))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %37: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                                         let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%16, read<i32>(%38));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         call<i32, signature=fn(ptr<@type1>, ptr<i8>, ptr<@type2>) -> i32>(%8, read<ptr<@type1>>(%12), read<ptr<i8>>(%13), addr_of<ptr<@type2>>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=None>(%5), read<i32>(%16)))));
// DEFAULT-NEXT:                         let %39: ptr<i8> [synthetic] = read<ptr<i8>>(%14);
// DEFAULT-NEXT:                         let %40: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%39), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<i8>>(%14, read<ptr<i8>>(%40));
// DEFAULT-NEXT:                         break %30;
// DEFAULT-NEXT:                         default %30:
// DEFAULT-NEXT:                             for %34
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<ptr<i8>>(%17, read<ptr<i8>>(%14));
// DEFAULT-NEXT:                                 condition: logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<i8>(read<i8>(deref(read<ptr<i8>>(%17))), const<i8>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%17)))), const<i32>(32))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%17)))), const<i32>(9))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%17)))), const<i32>(44)))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %41: ptr<i8> [synthetic] = read<ptr<i8>>(%17);
// DEFAULT-NEXT:                                     let %42: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%41), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%17, read<ptr<i8>>(%42));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                         write<i8>(%18, read<i8>(deref(read<ptr<i8>>(%17))));
// DEFAULT-NEXT:                         if ne<i8>(read<i8>(deref(read<ptr<i8>>(%17))), const<i8>(0))
// DEFAULT-NEXT:                             let %43: ptr<i8> [synthetic] = read<ptr<i8>>(%17);
// DEFAULT-NEXT:                             let %44: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%43), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%17, read<ptr<i8>>(%44));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%43)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         write<i32>(%16, call<i32, signature=fn(ptr<i8>) -> i32>(%7, read<ptr<i8>>(%14)));
// DEFAULT-NEXT:                         if ne<i32>(call<i32, signature=fn(ptr<i8>) -> i32>(%7, read<ptr<i8>>(%14)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<@type1>, ptr<i8>, ptr<@type2>) -> i32>(%8, read<ptr<@type1>>(%12), read<ptr<i8>>(%13), addr_of<ptr<@type2>>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=None>(%5), read<i32>(%16)))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i32>(%16, call<i32, signature=fn(ptr<i8>) -> i32>(%6, read<ptr<i8>>(%14)));
// DEFAULT-NEXT:                             if logical_and<bool>(ge<i32>(call<i32, signature=fn(ptr<i8>) -> i32>(%6, read<ptr<i8>>(%14)), const<i32>(0)), ne<ptr<@type0>>(read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=None>(%2), read<i32>(%16)))), null<ptr<@type0>>))
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<@type1>, ptr<@type1>, ptr<i8>, ptr<@type0>) -> i32>(%9, null<ptr<@type1>>, read<ptr<@type1>>(%12), read<ptr<i8>>(%13), read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=None>(%2), read<i32>(%16)))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                         if ne<i8>(read<i8>(%18), const<i8>(0))
// DEFAULT-NEXT:                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), neg<i32, overflow=ub>(const<i32>(1)))), read<i8>(%18));
// DEFAULT-NEXT:                         write<ptr<i8>>(%14, read<ptr<i8>>(%17));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
