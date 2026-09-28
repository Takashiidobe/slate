struct obstack {};
struct bitmap_head_def;
typedef struct bitmap_head_def       *bitmap;
typedef const struct bitmap_head_def *const_bitmap;
typedef unsigned long                 BITMAP_WORD;

typedef struct bitmap_obstack {
  struct bitmap_element_def *elements;
  struct bitmap_head_def    *heads;
  struct obstack             obstack;
} bitmap_obstack;
typedef struct bitmap_element_def {
  struct bitmap_element_def *next;
  struct bitmap_element_def *prev;
  unsigned int               indx;
  BITMAP_WORD                bits[(2)];
} bitmap_element;

struct bitmap_descriptor;

typedef struct bitmap_head_def {
  bitmap_element *first;
  bitmap_element *current;
  unsigned int    indx;
  bitmap_obstack *obstack;
} bitmap_head;

bitmap_element bitmap_zero_bits;

typedef struct {
  bitmap_element *elt1;
  bitmap_element *elt2;
  unsigned        word_no;
  BITMAP_WORD     bits;
} bitmap_iterator;

static __attribute__((noinline)) void bmp_iter_set_init(bitmap_iterator *bi,
                                                        const_bitmap     map,
                                                        unsigned  start_bit,
                                                        unsigned *bit_no) {
  bi->elt1 = map->first;
  bi->elt2 = ((void *)0);

  while (1) {
    if (!bi->elt1) {
      bi->elt1 = &bitmap_zero_bits;
      break;
    }

    if (bi->elt1->indx >= start_bit / (128u))
      break;
    bi->elt1 = bi->elt1->next;
  }

  if (bi->elt1->indx != start_bit / (128u))
    start_bit = bi->elt1->indx * (128u);

  bi->word_no   = start_bit / 64u % (2);
  bi->bits      = bi->elt1->bits[bi->word_no];
  bi->bits    >>= start_bit % 64u;

  start_bit += !bi->bits;

  *bit_no = start_bit;
}

static __inline__ __attribute__((always_inline)) void
bmp_iter_next(bitmap_iterator *bi, unsigned *bit_no) {
  bi->bits >>= 1;
  *bit_no   += 1;
}

static __inline__ __attribute__((always_inline)) unsigned char
bmp_iter_set(bitmap_iterator *bi, unsigned *bit_no) {
  if (bi->bits) {
    while (!(bi->bits & 1)) {
      bi->bits >>= 1;
      *bit_no   += 1;
    }
    return 1;
  }

  *bit_no = ((*bit_no + 64u - 1) / 64u * 64u);
  bi->word_no++;

  while (1) {
    while (bi->word_no != (2)) {
      bi->bits = bi->elt1->bits[bi->word_no];
      if (bi->bits) {
        while (!(bi->bits & 1)) {
          bi->bits >>= 1;
          *bit_no   += 1;
        }
        return 1;
      }
      *bit_no += 64u;
      bi->word_no++;
    }

    bi->elt1 = bi->elt1->next;
    if (!bi->elt1)
      return 0;
    *bit_no     = bi->elt1->indx * (128u);
    bi->word_no = 0;
  }
}

static void __attribute__((noinline)) foobar(bitmap_head *live_throughout) {
  bitmap_iterator rsi;
  unsigned int    regno;
  for (bmp_iter_set_init(&(rsi), (live_throughout), (0), &(regno));
       bmp_iter_set(&(rsi), &(regno)); bmp_iter_next(&(rsi), &(regno)))
    ;
}
int main() {
  bitmap_element elem            = {(void *)0, (void *)0, 0, {1, 1}};
  bitmap_head    live_throughout = {&elem, &elem, 0, (void *)0};
  foobar(&live_throughout);
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
// DEFAULT-NEXT:     type @type0 obstack = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type1 bitmap_head_def = struct {
// DEFAULT-NEXT:         field0 first: ptr<@type6>;
// DEFAULT-NEXT:         field1 current: ptr<@type6>;
// DEFAULT-NEXT:         field2 indx: u32;
// DEFAULT-NEXT:         field3 obstack: ptr<@type5>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type2 bitmap = ptr<@type1>;
// DEFAULT-NEXT:     type @type3 const_bitmap = ptr<const @type1>;
// DEFAULT-NEXT:     type @type4 BITMAP_WORD = u64;
// DEFAULT-NEXT:     type @type5 bitmap_obstack = struct {
// DEFAULT-NEXT:         field0 elements: ptr<@type6>;
// DEFAULT-NEXT:         field1 heads: ptr<@type1>;
// DEFAULT-NEXT:         field2 obstack: @type0;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type6 bitmap_element_def = struct incomplete;
// DEFAULT-NEXT:     type @type7 bitmap_obstack = @type5;
// DEFAULT-NEXT:     type @type8 bitmap_element = @type6;
// DEFAULT-NEXT:     type @type9 bitmap_descriptor = struct incomplete;
// DEFAULT-NEXT:     type @type10 bitmap_head = @type1;
// DEFAULT-NEXT:     type @type11 = struct {
// DEFAULT-NEXT:         field0 elt1: ptr<@type6>;
// DEFAULT-NEXT:         field1 elt2: ptr<@type6>;
// DEFAULT-NEXT:         field2 word_no: u32;
// DEFAULT-NEXT:         field3 bits: u64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type12 bitmap_iterator = @type11;
// DEFAULT-NEXT:     global %11 bitmap_zero_bits: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %14 @bmp_iter_set_init(%15 bi: ptr<@type11>, %16 map: ptr<const @type1>, %17 start_bit: u32, %18 bit_no: ptr<u32>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))), read<ptr<@type6>>(field0(deref(read<ptr<const @type1>>(%16)))));
// DEFAULT-NEXT:         write<ptr<@type6>>(field1(deref(read<ptr<@type11>>(%15))), null<ptr<@type6>>);
// DEFAULT-NEXT:         while %32 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<@type6>>(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15)))), null<ptr<@type6>>))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))), addr_of<ptr<@type6>>(%11));
// DEFAULT-NEXT:                         break %32;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ge<u32>(read<u32>(field2(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))))))), div<u32, by_zero=ub>(read<u32>(%17), const<u32>(128)))
// DEFAULT-NEXT:                     break %32;
// DEFAULT-NEXT:                 write<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))), read<ptr<@type6>>(field0(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field2(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))))))), div<u32, by_zero=ub>(read<u32>(%17), const<u32>(128)))
// DEFAULT-NEXT:             write<u32>(%17, mul<u32, overflow=wrap>(read<u32>(field2(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))))))), const<u32>(128)));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type11>>(%15))), rem<u32, by_zero=ub>(div<u32, by_zero=ub>(read<u32>(%17), const<u32>(64)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type11>>(%15))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(field3(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%15))))))), read<u32>(field2(deref(read<ptr<@type11>>(%15))))))));
// DEFAULT-NEXT:         let %38: ptr<@type11> [synthetic] = read<ptr<@type11>>(%15);
// DEFAULT-NEXT:         let %39: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type11>>(%38))));
// DEFAULT-NEXT:         let %40: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%39), rem<u32, by_zero=ub>(read<u32>(%17), const<u32>(64)));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type11>>(%38))), read<u64>(%40));
// DEFAULT-NEXT:         let %41: u32 [synthetic] = read<u32>(%17);
// DEFAULT-NEXT:         let %42: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%41), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(not<bool>(ne<u64>(read<u64>(field3(deref(read<ptr<@type11>>(%15)))), const<u64>(0))))));
// DEFAULT-NEXT:         write<u32>(%17, read<u32>(%42));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%18)), read<u32>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @bmp_iter_next(%20 bi: ptr<@type11>, %21 bit_no: ptr<u32>) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %43: ptr<@type11> [synthetic] = read<ptr<@type11>>(%20);
// DEFAULT-NEXT:         let %44: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type11>>(%43))));
// DEFAULT-NEXT:         let %45: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%44), const<i32>(1));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type11>>(%43))), read<u64>(%45));
// DEFAULT-NEXT:         let %46: ptr<u32> [synthetic] = read<ptr<u32>>(%21);
// DEFAULT-NEXT:         let %47: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%46)));
// DEFAULT-NEXT:         let %48: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%47), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%46)), read<u32>(%48));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @bmp_iter_set(%23 bi: ptr<@type11>, %24 bit_no: ptr<u32>) -> u8 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(read<u64>(field3(deref(read<ptr<@type11>>(%23)))), const<u64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %33 not<bool>(ne<u64>(and<u64>(read<u64>(field3(deref(read<ptr<@type11>>(%23)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(0)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %49: ptr<@type11> [synthetic] = read<ptr<@type11>>(%23);
// DEFAULT-NEXT:                         let %50: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type11>>(%49))));
// DEFAULT-NEXT:                         let %51: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%50), const<i32>(1));
// DEFAULT-NEXT:                         write<u64>(field3(deref(read<ptr<@type11>>(%49))), read<u64>(%51));
// DEFAULT-NEXT:                         let %52: ptr<u32> [synthetic] = read<ptr<u32>>(%24);
// DEFAULT-NEXT:                         let %53: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%52)));
// DEFAULT-NEXT:                         let %54: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%53), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(deref(read<ptr<u32>>(%52)), read<u32>(%54));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%24)), mul<u32, overflow=wrap>(div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(deref(read<ptr<u32>>(%24))), const<u32>(64)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), const<u32>(64)), const<u32>(64)));
// DEFAULT-NEXT:         let %55: ptr<@type11> [synthetic] = read<ptr<@type11>>(%23);
// DEFAULT-NEXT:         let %56: u32 [synthetic] = read<u32>(field2(deref(read<ptr<@type11>>(%55))));
// DEFAULT-NEXT:         let %57: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%56), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type11>>(%55))), read<u32>(%57));
// DEFAULT-NEXT:         while %34 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %35 ne<u32>(read<u32>(field2(deref(read<ptr<@type11>>(%23)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u64>(field3(deref(read<ptr<@type11>>(%23))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(field3(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%23))))))), read<u32>(field2(deref(read<ptr<@type11>>(%23))))))));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(field3(deref(read<ptr<@type11>>(%23)))), const<u64>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 while %36 not<bool>(ne<u64>(and<u64>(read<u64>(field3(deref(read<ptr<@type11>>(%23)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(0)))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %58: ptr<@type11> [synthetic] = read<ptr<@type11>>(%23);
// DEFAULT-NEXT:                                         let %59: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type11>>(%58))));
// DEFAULT-NEXT:                                         let %60: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%59), const<i32>(1));
// DEFAULT-NEXT:                                         write<u64>(field3(deref(read<ptr<@type11>>(%58))), read<u64>(%60));
// DEFAULT-NEXT:                                         let %61: ptr<u32> [synthetic] = read<ptr<u32>>(%24);
// DEFAULT-NEXT:                                         let %62: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%61)));
// DEFAULT-NEXT:                                         let %63: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%62), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                                         write<u32>(deref(read<ptr<u32>>(%61)), read<u32>(%63));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         let %64: ptr<u32> [synthetic] = read<ptr<u32>>(%24);
// DEFAULT-NEXT:                         let %65: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%64)));
// DEFAULT-NEXT:                         let %66: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%65), const<u32>(64));
// DEFAULT-NEXT:                         write<u32>(deref(read<ptr<u32>>(%64)), read<u32>(%66));
// DEFAULT-NEXT:                         let %67: ptr<@type11> [synthetic] = read<ptr<@type11>>(%23);
// DEFAULT-NEXT:                         let %68: u32 [synthetic] = read<u32>(field2(deref(read<ptr<@type11>>(%67))));
// DEFAULT-NEXT:                         let %69: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%68), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(field2(deref(read<ptr<@type11>>(%67))), read<u32>(%69));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%23))), read<ptr<@type6>>(field0(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%23))))))));
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<@type6>>(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%23)))), null<ptr<@type6>>))
// DEFAULT-NEXT:                     return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u32>(deref(read<ptr<u32>>(%24)), mul<u32, overflow=wrap>(read<u32>(field2(deref(read<ptr<@type6>>(field0(deref(read<ptr<@type11>>(%23))))))), const<u32>(128)));
// DEFAULT-NEXT:                 write<u32>(field2(deref(read<ptr<@type11>>(%23))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @foobar(%26 live_throughout: ptr<@type1>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 rsi: @type11 [storage=automatic];
// DEFAULT-NEXT:         let %28 regno: u32 [storage=automatic];
// DEFAULT-NEXT:         for %37
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type11>, ptr<const @type1>, u32, ptr<u32>) -> void>(%14, addr_of<ptr<@type11>>(%27), pointer_cast<ptr<const @type1>, reason=arg>(read<ptr<@type1>>(%26)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%28));
// DEFAULT-NEXT:             condition: ne<u8>(call<u8, signature=fn(ptr<@type11>, ptr<u32>) -> u8>(%22, addr_of<ptr<@type11>>(%27), addr_of<ptr<u32>>(%28)), const<u8>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type11>, ptr<u32>) -> void>(%19, addr_of<ptr<@type11>>(%27), addr_of<ptr<u32>>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %30 elem: @type6 [storage=automatic] = aggregate<@type6, zero_fill=false>(field0 = null<ptr<@type6>>, field1 = null<ptr<@type6>>, field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field3 = aggregate<array<u64, 2>, zero_fill=false>(index0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))), index1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)))));
// DEFAULT-NEXT:         let %31 live_throughout: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = addr_of<ptr<@type6>>(%30), field1 = addr_of<ptr<@type6>>(%30), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field3 = null<ptr<@type5>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%25, addr_of<ptr<@type1>>(%31));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
