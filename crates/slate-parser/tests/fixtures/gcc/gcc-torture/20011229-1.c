// SLATE-FILECHECK-DEFINES DEFAULT

/* ICE: call insn does not satisfy its constraints, MMIX port.
   Origin: ghostscript-6.52, reduction from hp@bitrange.com.  */

/* { dg-require-effective-target indirect_calls } */

struct s0
{
  void (*init_color)(void *, void *);
};
struct s1
{
  void (*map_cmyk)(short, void *, void **, void *);
  void (*map_rgb_alpha)(short, void *, void **, void *);
};
struct s5
{
  long fill1; int fill2;
  long fill3; unsigned int fill4, fill5;
};
struct s2
{
  struct s5 x, y;
};
struct s3
{
  long dev_color;
  unsigned int key;
};
struct s4
{
  unsigned char spp;
  int alpha;
  struct mc_
  {
    unsigned int values[14];
    unsigned int mask, test;
    int exact;
  } mask_color;
  void **pis;
  struct s0 *pcs;
  struct dd_
  {
    struct s2 row[2];
    struct s2 pixel0;
  } dda;
  struct s3 clues[256];
};
extern struct s1 *get_cmap_procs (void **, void *);
int image_render_color (struct s4 *, unsigned char *, int, void *);
int
image_render_color (struct s4 *penum, unsigned char *buffer,
		    int data_x, void *dev) 
{
  struct s3 *clues = penum->clues;
  void **pis = penum->pis;
  struct s2 pnext;
  struct s0 *pcs = penum->pcs;
  struct s1 *cmap_procs = get_cmap_procs(pis, dev);
  void (*map_4)(short, void *, void **, void *) =
    (penum->alpha ? cmap_procs->map_rgb_alpha : cmap_procs->map_cmyk);
  unsigned int mask = penum->mask_color.mask;
  unsigned int test = penum->mask_color.test;
  struct s3 *pic_next = &clues[1];
  int spp = penum->spp;
  unsigned char *psrc = buffer + data_x * spp;
  unsigned char v[6];

  pnext = penum->dda.pixel0;
  __builtin_memset (&v, 0, sizeof(v));
  (*(pcs)->init_color) (0, 0);

  if (spp == 4)
    {
      v[0] = psrc[0];
      v[1] = psrc[1];
      if ((buffer[0] & mask) == test && penum->mask_color.exact)
	pic_next->dev_color = 0;
      (*map_4)(v[0], &pic_next->dev_color, pis, dev);
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
// DEFAULT-NEXT:     type @type0 s0 = struct {
// DEFAULT-NEXT:         field0 init_color: ptr<fn(ptr<void>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 s1 = struct {
// DEFAULT-NEXT:         field0 map_cmyk: ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>;
// DEFAULT-NEXT:         field1 map_rgb_alpha: ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 s5 = struct {
// DEFAULT-NEXT:         field0 fill1: i64;
// DEFAULT-NEXT:         field1 fill2: i32;
// DEFAULT-NEXT:         field2 fill3: i64;
// DEFAULT-NEXT:         field3 fill4: u32;
// DEFAULT-NEXT:         field4 fill5: u32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24, 28]];
// DEFAULT-NEXT:     type @type3 s2 = struct {
// DEFAULT-NEXT:         field0 x: @type2;
// DEFAULT-NEXT:         field1 y: @type2;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 32]];
// DEFAULT-NEXT:     type @type4 s3 = struct {
// DEFAULT-NEXT:         field0 dev_color: i64;
// DEFAULT-NEXT:         field1 key: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type5 s4 = struct {
// DEFAULT-NEXT:         field0 spp: u8;
// DEFAULT-NEXT:         field1 alpha: i32;
// DEFAULT-NEXT:         field2 mask_color: @type6;
// DEFAULT-NEXT:         field3 pis: ptr<ptr<void>>;
// DEFAULT-NEXT:         field4 pcs: ptr<@type0>;
// DEFAULT-NEXT:         field5 dda: @type7;
// DEFAULT-NEXT:         field6 clues: array<@type4, 256>;
// DEFAULT-NEXT:     } [size=4384, align=8, offsets=[0, 4, 8, 80, 88, 96, 288]];
// DEFAULT-NEXT:     type @type6 mc_ = struct {
// DEFAULT-NEXT:         field0 values: array<u32, 14>;
// DEFAULT-NEXT:         field1 mask: u32;
// DEFAULT-NEXT:         field2 test: u32;
// DEFAULT-NEXT:         field3 exact: i32;
// DEFAULT-NEXT:     } [size=68, align=4, offsets=[0, 56, 60, 64]];
// DEFAULT-NEXT:     type @type7 dd_ = struct {
// DEFAULT-NEXT:         field0 row: array<@type3, 2>;
// DEFAULT-NEXT:         field1 pixel0: @type3;
// DEFAULT-NEXT:     } [size=192, align=8, offsets=[0, 128]];
// DEFAULT-NEXT:     fn %8 @get_cmap_procs(%26 <unnamed>: ptr<ptr<void>>, %27 <unnamed>: ptr<void>) -> ptr<@type1> [linkage=external];
// DEFAULT-NEXT:     fn %9 @image_render_color(%10 penum: ptr<@type5>, %11 buffer: ptr<u8>, %12 data_x: i32, %13 dev: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 clues: ptr<@type4> [storage=automatic] = array_decay<ptr<@type4>, length=Some(256)>(field6(deref(read<ptr<@type5>>(%10))));
// DEFAULT-NEXT:         let %15 pis: ptr<ptr<void>> [storage=automatic] = read<ptr<ptr<void>>>(field3(deref(read<ptr<@type5>>(%10))));
// DEFAULT-NEXT:         let %16 pnext: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %17 pcs: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(field4(deref(read<ptr<@type5>>(%10))));
// DEFAULT-NEXT:         let %18 cmap_procs: ptr<@type1> [storage=automatic] = call<ptr<@type1>, signature=fn(ptr<ptr<void>>, ptr<void>) -> ptr<@type1>>(%8, read<ptr<ptr<void>>>(%15), read<ptr<void>>(%13));
// DEFAULT-NEXT:         let %19 map_4: ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void> [storage=automatic] = conditional<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(ne<i32>(read<i32>(field1(deref(read<ptr<@type5>>(%10)))), const<i32>(0)), read<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(field1(deref(read<ptr<@type1>>(%18)))), read<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(field0(deref(read<ptr<@type1>>(%18)))));
// DEFAULT-NEXT:         let %20 mask: u32 [storage=automatic] = read<u32>(field1(field2(deref(read<ptr<@type5>>(%10)))));
// DEFAULT-NEXT:         let %21 test: u32 [storage=automatic] = read<u32>(field2(field2(deref(read<ptr<@type5>>(%10)))));
// DEFAULT-NEXT:         let %22 pic_next: ptr<@type4> [storage=automatic] = addr_of<ptr<@type4>>(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(read<ptr<@type4>>(%14), const<i32>(1))));
// DEFAULT-NEXT:         let %23 spp: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field0(deref(read<ptr<@type5>>(%10))))));
// DEFAULT-NEXT:         let %24 psrc: ptr<u8> [storage=automatic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%11), mul<i32, overflow=ub>(read<i32>(%12), read<i32>(%23)));
// DEFAULT-NEXT:         let %25 v: array<u8, 6> [storage=automatic];
// DEFAULT-NEXT:         write<@type3>(%16, copy<@type3, reason=assign>(read<@type3>(field1(field5(deref(read<ptr<@type5>>(%10)))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%35, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<u8, 6>>>(%25)), const<i32>(0), const<u64>(6));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<void>) -> void>(read<ptr<fn(ptr<void>, ptr<void>) -> void>>(field0(deref(read<ptr<@type0>>(%17)))), null<ptr<void>>, null<ptr<void>>);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%23), const<i32>(4))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%25), const<i32>(0))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%25), const<i32>(1))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1)))));
// DEFAULT-NEXT:                 if logical_and<bool>(eq<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%11), const<i32>(0))))))), read<u32>(%20)), read<u32>(%21)), ne<i32>(read<i32>(field3(field2(deref(read<ptr<@type5>>(%10))))), const<i32>(0)))
// DEFAULT-NEXT:                     write<i64>(field0(deref(read<ptr<@type4>>(%22))), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 call<void, signature=fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>(read<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(%19), reinterpret<i16, reason=arg, fits=unknown>(widen<u16, reason=arg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%25), const<i32>(0)))))), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(field0(deref(read<ptr<@type4>>(%22))))), read<ptr<ptr<void>>>(%15), read<ptr<void>>(%13));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @__builtin_memset(%32 <unnamed>: ptr<void>, %33 <unnamed>: i32, %34 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
