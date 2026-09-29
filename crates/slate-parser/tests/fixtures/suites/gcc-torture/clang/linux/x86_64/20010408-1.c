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
// DEFAULT-NEXT:     type @type[[TYPE_win:[0-9]+]] win = struct {
// DEFAULT-NEXT:         field0 w_next: ptr<@type[[TYPE_win]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_auser:[0-9]+]] auser = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_comm:[0-9]+]] comm = struct {
// DEFAULT-NEXT:         field0 name: ptr<i8>;
// DEFAULT-NEXT:         field1 flags: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     extern %[[VALUE_windows:[0-9]+]] windows: ptr<@type[[TYPE_win]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_wtab:[0-9]+]] wtab: array<ptr<@type[[TYPE_win]]>, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_comms:[0-9]+]] comms: array<@type[[TYPE_comm]], incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([35, 63, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_WindowByNoN:[0-9]+]] @WindowByNoN(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_FindCommnr:[0-9]+]] @FindCommnr(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_AclSetPermCmd:[0-9]+]] @AclSetPermCmd(%[[VALUE2:[0-9]+]] <unnamed>: ptr<@type[[TYPE_auser]]>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<@type[[TYPE_comm]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_AclSetPermWin:[0-9]+]] @AclSetPermWin(%[[VALUE5:[0-9]+]] <unnamed>: ptr<@type[[TYPE_auser]]>, %[[VALUE6:[0-9]+]] <unnamed>: ptr<@type[[TYPE_auser]]>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<@type[[TYPE_win]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_AclSetPerm:[0-9]+]] @AclSetPerm(%[[VALUE_uu:[0-9]+]] uu: ptr<@type[[TYPE_auser]]>, %[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE_auser]]>, %[[VALUE_mode:[0-9]+]] mode: ptr<i8>, %[[VALUE_s:[0-9]+]] s: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: ptr<@type[[TYPE_win]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ch:[0-9]+]] ch: i8 [storage=automatic];
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         while %[[VALUE10:[0-9]+]] ne<i8>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_s]]))), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %[[VALUE11:[0-9]+]] widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_s]]))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE11]] const<i32>(42):
// DEFAULT-NEXT:                             return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_AclSetPerm]], read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_uu]]), read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_u]]), read<ptr<i8>>(%[[VALUE_mode]]), array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]]));
// DEFAULT-NEXT:                         case %[[VALUE11]] const<i32>(35):
// DEFAULT-NEXT:                             if ne<ptr<@type[[TYPE_auser]]>>(read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_uu]]), null<ptr<@type[[TYPE_auser]]>>)
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<@type[[TYPE_auser]]>, ptr<@type[[TYPE_auser]]>, ptr<i8>, ptr<@type[[TYPE_win]]>) -> i32>(%[[VALUE_AclSetPermWin]], read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_uu]]), read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_u]]), read<ptr<i8>>(%[[VALUE_mode]]), int_to_ptr<ptr<@type[[TYPE_win]]>, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<ptr<@type[[TYPE_win]]>>(%[[VALUE_w]], read<ptr<@type[[TYPE_win]]>>(%[[VALUE_windows]]));
// DEFAULT-NEXT:                                     condition: ne<ptr<@type[[TYPE_win]]>>(read<ptr<@type[[TYPE_win]]>>(%[[VALUE_w]]), null<ptr<@type[[TYPE_win]]>>)
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         write<ptr<@type[[TYPE_win]]>>(%[[VALUE_w]], read<ptr<@type[[TYPE_win]]>>(field0(deref(read<ptr<@type[[TYPE_win]]>>(%[[VALUE_w]])))));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         call<i32, signature=fn(ptr<@type[[TYPE_auser]]>, ptr<@type[[TYPE_auser]]>, ptr<i8>, ptr<@type[[TYPE_win]]>) -> i32>(%[[VALUE_AclSetPermWin]], null<ptr<@type[[TYPE_auser]]>>, read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_u]]), read<ptr<i8>>(%[[VALUE_mode]]), read<ptr<@type[[TYPE_win]]>>(%[[VALUE_w]]));
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_s]]);
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(%[[VALUE14]]));
// DEFAULT-NEXT:                         break %[[VALUE11]];
// DEFAULT-NEXT:                         case %[[VALUE11]] const<i32>(63):
// DEFAULT-NEXT:                             if ne<ptr<@type[[TYPE_auser]]>>(read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_uu]]), null<ptr<@type[[TYPE_auser]]>>)
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<@type[[TYPE_auser]]>, ptr<@type[[TYPE_auser]]>, ptr<i8>, ptr<@type[[TYPE_win]]>) -> i32>(%[[VALUE_AclSetPermWin]], read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_uu]]), read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_u]]), read<ptr<i8>>(%[[VALUE_mode]]), null<ptr<@type[[TYPE_win]]>>);
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                     condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(174))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         call<i32, signature=fn(ptr<@type[[TYPE_auser]]>, ptr<i8>, ptr<@type[[TYPE_comm]]>) -> i32>(%[[VALUE_AclSetPermCmd]], read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_u]]), read<ptr<i8>>(%[[VALUE_mode]]), addr_of<ptr<@type[[TYPE_comm]]>>(deref(ptr_offset<ptr<@type[[TYPE_comm]]>, subtract=false, element=@type[[TYPE_comm]], overflow=ub>(array_decay<ptr<@type[[TYPE_comm]]>, length=None>(%[[VALUE_comms]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_s]]);
// DEFAULT-NEXT:                         let %[[VALUE19:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(%[[VALUE19]]));
// DEFAULT-NEXT:                         break %[[VALUE11]];
// DEFAULT-NEXT:                         default %[[VALUE11]]:
// DEFAULT-NEXT:                             for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE_s]]));
// DEFAULT-NEXT:                                 condition: logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<i8>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_p]]))), const<i8>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])))), const<i32>(32))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])))), const<i32>(9))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])))), const<i32>(44)))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE21:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                                     let %[[VALUE22:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE22]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_ch]], read<i8>(deref(read<ptr<i8>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:                         if ne<i8>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_p]]))), const<i8>(0))
// DEFAULT-NEXT:                             let %[[VALUE23:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                             let %[[VALUE24:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE24]]));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%[[VALUE23]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_i]], call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_FindCommnr]], read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:                         if ne<i32>(call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_FindCommnr]], read<ptr<i8>>(%[[VALUE_s]])), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<@type[[TYPE_auser]]>, ptr<i8>, ptr<@type[[TYPE_comm]]>) -> i32>(%[[VALUE_AclSetPermCmd]], read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_u]]), read<ptr<i8>>(%[[VALUE_mode]]), addr_of<ptr<@type[[TYPE_comm]]>>(deref(ptr_offset<ptr<@type[[TYPE_comm]]>, subtract=false, element=@type[[TYPE_comm]], overflow=ub>(array_decay<ptr<@type[[TYPE_comm]]>, length=None>(%[[VALUE_comms]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_WindowByNoN]], read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:                             if logical_and<bool>(ge<i32>(call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_WindowByNoN]], read<ptr<i8>>(%[[VALUE_s]])), const<i32>(0)), ne<ptr<@type[[TYPE_win]]>>(read<ptr<@type[[TYPE_win]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_win]]>>, subtract=false, element=ptr<@type[[TYPE_win]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_win]]>>, length=None>(%[[VALUE_wtab]]), read<i32>(%[[VALUE_i]])))), null<ptr<@type[[TYPE_win]]>>))
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<@type[[TYPE_auser]]>, ptr<@type[[TYPE_auser]]>, ptr<i8>, ptr<@type[[TYPE_win]]>) -> i32>(%[[VALUE_AclSetPermWin]], null<ptr<@type[[TYPE_auser]]>>, read<ptr<@type[[TYPE_auser]]>>(%[[VALUE_u]]), read<ptr<i8>>(%[[VALUE_mode]]), read<ptr<@type[[TYPE_win]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_win]]>>, subtract=false, element=ptr<@type[[TYPE_win]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_win]]>>, length=None>(%[[VALUE_wtab]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                         if ne<i8>(read<i8>(%[[VALUE_ch]]), const<i8>(0))
// DEFAULT-NEXT:                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), neg<i32, overflow=ub>(const<i32>(1)))), read<i8>(%[[VALUE_ch]]));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
