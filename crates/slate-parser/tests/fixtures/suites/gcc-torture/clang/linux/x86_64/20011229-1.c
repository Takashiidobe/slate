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
// DEFAULT-NEXT:     type @type[[TYPE_s0:[0-9]+]] s0 = struct {
// DEFAULT-NEXT:         field0 init_color: ptr<fn(ptr<void>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:         field0 map_cmyk: ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>;
// DEFAULT-NEXT:         field1 map_rgb_alpha: ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_s5:[0-9]+]] s5 = struct {
// DEFAULT-NEXT:         field0 fill1: i64;
// DEFAULT-NEXT:         field1 fill2: i32;
// DEFAULT-NEXT:         field2 fill3: i64;
// DEFAULT-NEXT:         field3 fill4: u32;
// DEFAULT-NEXT:         field4 fill5: u32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24, 28]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 x: @type[[TYPE_s5]];
// DEFAULT-NEXT:         field1 y: @type[[TYPE_s5]];
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 32]];
// DEFAULT-NEXT:     type @type[[TYPE_s3:[0-9]+]] s3 = struct {
// DEFAULT-NEXT:         field0 dev_color: i64;
// DEFAULT-NEXT:         field1 key: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_s4:[0-9]+]] s4 = struct {
// DEFAULT-NEXT:         field0 spp: u8;
// DEFAULT-NEXT:         field1 alpha: i32;
// DEFAULT-NEXT:         field2 mask_color: @type[[TYPE_mc_:[0-9]+]];
// DEFAULT-NEXT:         field3 pis: ptr<ptr<void>>;
// DEFAULT-NEXT:         field4 pcs: ptr<@type[[TYPE_s0]]>;
// DEFAULT-NEXT:         field5 dda: @type[[TYPE_dd_:[0-9]+]];
// DEFAULT-NEXT:         field6 clues: array<@type[[TYPE_s3]], 256>;
// DEFAULT-NEXT:     } [size=4384, align=8, offsets=[0, 4, 8, 80, 88, 96, 288]];
// DEFAULT-NEXT:     type @type[[TYPE_mc_]] mc_ = struct {
// DEFAULT-NEXT:         field0 values: array<u32, 14>;
// DEFAULT-NEXT:         field1 mask: u32;
// DEFAULT-NEXT:         field2 test: u32;
// DEFAULT-NEXT:         field3 exact: i32;
// DEFAULT-NEXT:     } [size=68, align=4, offsets=[0, 56, 60, 64]];
// DEFAULT-NEXT:     type @type[[TYPE_dd_]] dd_ = struct {
// DEFAULT-NEXT:         field0 row: array<@type[[TYPE_s2]], 2>;
// DEFAULT-NEXT:         field1 pixel0: @type[[TYPE_s2]];
// DEFAULT-NEXT:     } [size=192, align=8, offsets=[0, 128]];
// DEFAULT-NEXT:     fn %[[VALUE_get_cmap_procs:[0-9]+]] @get_cmap_procs(%[[VALUE0:[0-9]+]] <unnamed>: ptr<ptr<void>>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<void>) -> ptr<@type[[TYPE_s1]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_image_render_color:[0-9]+]] @image_render_color(%[[VALUE_penum:[0-9]+]] penum: ptr<@type[[TYPE_s4]]>, %[[VALUE_buffer:[0-9]+]] buffer: ptr<u8>, %[[VALUE_data_x:[0-9]+]] data_x: i32, %[[VALUE_dev:[0-9]+]] dev: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_clues:[0-9]+]] clues: ptr<@type[[TYPE_s3]]> [storage=automatic] = array_decay<ptr<@type[[TYPE_s3]]>, length=Some(256)>(field6(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]]))));
// DEFAULT-NEXT:         let %[[VALUE_pis:[0-9]+]] pis: ptr<ptr<void>> [storage=automatic] = read<ptr<ptr<void>>>(field3(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]]))));
// DEFAULT-NEXT:         let %[[VALUE_pnext:[0-9]+]] pnext: @type[[TYPE_s2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pcs:[0-9]+]] pcs: ptr<@type[[TYPE_s0]]> [storage=automatic] = read<ptr<@type[[TYPE_s0]]>>(field4(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]]))));
// DEFAULT-NEXT:         let %[[VALUE_cmap_procs:[0-9]+]] cmap_procs: ptr<@type[[TYPE_s1]]> [storage=automatic] = call<ptr<@type[[TYPE_s1]]>, signature=fn(ptr<ptr<void>>, ptr<void>) -> ptr<@type[[TYPE_s1]]>>(%[[VALUE_get_cmap_procs]], read<ptr<ptr<void>>>(%[[VALUE_pis]]), read<ptr<void>>(%[[VALUE_dev]]));
// DEFAULT-NEXT:         let %[[VALUE_map_4:[0-9]+]] map_4: ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void> [storage=automatic] = conditional<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]])))), const<i32>(0)), read<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(field1(deref(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_cmap_procs]])))), read<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(field0(deref(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_cmap_procs]])))));
// DEFAULT-NEXT:         let %[[VALUE_mask:[0-9]+]] mask: u32 [storage=automatic] = read<u32>(field1(field2(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]])))));
// DEFAULT-NEXT:         let %[[VALUE_test:[0-9]+]] test: u32 [storage=automatic] = read<u32>(field2(field2(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]])))));
// DEFAULT-NEXT:         let %[[VALUE_pic_next:[0-9]+]] pic_next: ptr<@type[[TYPE_s3]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_s3]]>>(deref(ptr_offset<ptr<@type[[TYPE_s3]]>, subtract=false, element=@type[[TYPE_s3]], overflow=ub>(read<ptr<@type[[TYPE_s3]]>>(%[[VALUE_clues]]), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_spp:[0-9]+]] spp: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field0(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]]))))));
// DEFAULT-NEXT:         let %[[VALUE_psrc:[0-9]+]] psrc: ptr<u8> [storage=automatic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_buffer]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_data_x]]), read<i32>(%[[VALUE_spp]])));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: array<u8, 6> [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_s2]]>(%[[VALUE_pnext]], copy<@type[[TYPE_s2]], reason=assign>(read<@type[[TYPE_s2]]>(field1(field5(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]])))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset:[0-9]+]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<u8, 6>>>(%[[VALUE_v]])), const<i32>(0), const<u64>(6));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<void>) -> void>(read<ptr<fn(ptr<void>, ptr<void>) -> void>>(field0(deref(read<ptr<@type[[TYPE_s0]]>>(%[[VALUE_pcs]])))), null<ptr<void>>, null<ptr<void>>);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_spp]]), const<i32>(4))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%[[VALUE_v]]), const<i32>(0))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_psrc]]), const<i32>(0)))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%[[VALUE_v]]), const<i32>(1))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_psrc]]), const<i32>(1)))));
// DEFAULT-NEXT:                 if logical_and<bool>(eq<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_buffer]]), const<i32>(0))))))), read<u32>(%[[VALUE_mask]])), read<u32>(%[[VALUE_test]])), ne<i32>(read<i32>(field3(field2(deref(read<ptr<@type[[TYPE_s4]]>>(%[[VALUE_penum]]))))), const<i32>(0)))
// DEFAULT-NEXT:                     write<i64>(field0(deref(read<ptr<@type[[TYPE_s3]]>>(%[[VALUE_pic_next]]))), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 call<void, signature=fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>(read<ptr<fn(i16, ptr<void>, ptr<ptr<void>>, ptr<void>) -> void>>(%[[VALUE_map_4]]), reinterpret<i16, reason=arg, fits=unknown>(widen<u16, reason=arg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%[[VALUE_v]]), const<i32>(0)))))), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(field0(deref(read<ptr<@type[[TYPE_s3]]>>(%[[VALUE_pic_next]]))))), read<ptr<ptr<void>>>(%[[VALUE_pis]]), read<ptr<void>>(%[[VALUE_dev]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset]] @__builtin_memset(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE3:[0-9]+]] <unnamed>: i32, %[[VALUE4:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
