/* { dg-do compile } */
/* { dg-skip-if "No section attribute" { { hppa*-*-hpux* } && { ! lp64 } } } */
/* { dg-options "-fpic -Os -g" } */
/* { dg-require-effective-target fpic } */
/* { dg-require-effective-target named_sections } */

struct fit_loadable_tbl {
	int type;
	void (*handler)(int data, int size);
};

#define ll_entry_start(_type, _list)					\
({									\
	static char start[0] __attribute__((aligned(4)))		\
		__attribute__((unused, section(".u_boot_list_2_"#_list"_1")));	\
	(_type *)&start;						\
})

#define ll_entry_end(_type, _list)					\
({									\
	static char end[0] __attribute__((aligned(4)))			\
		__attribute__((unused, section(".u_boot_list_2_"#_list"_3")));	\
	(_type *)&end;							\
})

#define ll_entry_count(_type, _list)					\
	({								\
		_type *start = ll_entry_start(_type, _list);		\
		_type *end = ll_entry_end(_type, _list);		\
		unsigned int _ll_result = end - start;			\
		_ll_result;						\
	})

void test(int img_type, int img_data, int img_len)
{
	int i;
	const unsigned int count =
		ll_entry_count(struct fit_loadable_tbl, fit_loadable);
	struct fit_loadable_tbl *fit_loadable_handler =
		ll_entry_start(struct fit_loadable_tbl, fit_loadable);

	for (i = 0; i < count; i++, fit_loadable_handler++)
		if (fit_loadable_handler->type == img_type)
			fit_loadable_handler->handler(img_data, img_len);
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_fit_loadable_tbl:[0-9]+]] fit_loadable_tbl = struct {
// DEFAULT-NEXT:         field0 type: i32;
// DEFAULT-NEXT:         field1 handler: ptr<fn(i32, i32) -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_start:[0-9]+]] start: array<i8, 0> [storage=static] [align=4] [linkage=internal] [section=".u_boot_list_2_fit_loadable_1"];
// DEFAULT-NEXT:     global %[[VALUE_end:[0-9]+]] end: array<i8, 0> [storage=static] [align=4] [linkage=internal] [section=".u_boot_list_2_fit_loadable_3"];
// DEFAULT-NEXT:     global %[[VALUE_start_2:[0-9]+]] start: array<i8, 0> [storage=static] [align=4] [linkage=internal] [section=".u_boot_list_2_fit_loadable_1"];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_img_type:[0-9]+]] img_type: i32, %[[VALUE_img_data:[0-9]+]] img_data: i32, %[[VALUE_img_len:[0-9]+]] img_len: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: u32 [storage=automatic] [const];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_start_3:[0-9]+]] start: ptr<@type[[TYPE_fit_loadable_tbl]]> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE_fit_loadable_tbl]]> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE1]], pointer_cast<ptr<@type[[TYPE_fit_loadable_tbl]]>, reason=explicit>(addr_of<ptr<array<i8, 0>>>(%[[VALUE_start]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_start_3]], read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE1]]));
// DEFAULT-NEXT:             let %[[VALUE_end_2:[0-9]+]] end: ptr<@type[[TYPE_fit_loadable_tbl]]> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<@type[[TYPE_fit_loadable_tbl]]> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE2]], pointer_cast<ptr<@type[[TYPE_fit_loadable_tbl]]>, reason=explicit>(addr_of<ptr<array<i8, 0>>>(%[[VALUE_end]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_end_2]], read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE2]]));
// DEFAULT-NEXT:             let %[[VALUE__ll_result:[0-9]+]] _ll_result: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=@type[[TYPE_fit_loadable_tbl]], same_array=required, overflow=ub>(read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_end_2]]), read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_start_3]]))));
// DEFAULT-NEXT:             write<u32>(%[[VALUE0]], read<u32>(%[[VALUE__ll_result]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<u32>(%[[VALUE_count]], read<u32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_fit_loadable_handler:[0-9]+]] fit_loadable_handler: ptr<@type[[TYPE_fit_loadable_tbl]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE_fit_loadable_tbl]]> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE3]], pointer_cast<ptr<@type[[TYPE_fit_loadable_tbl]]>, reason=explicit>(addr_of<ptr<array<i8, 0>>>(%[[VALUE_start_2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_fit_loadable_handler]], read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE3]]));
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_i]])), read<u32>(%[[VALUE_count]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<@type[[TYPE_fit_loadable_tbl]]> [synthetic] = read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_fit_loadable_handler]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<@type[[TYPE_fit_loadable_tbl]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_fit_loadable_tbl]]>, subtract=false, element=@type[[TYPE_fit_loadable_tbl]], overflow=ub>(read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_fit_loadable_handler]], read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_fit_loadable_handler]])))), read<i32>(%[[VALUE_img_type]]))
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32) -> void>(read<ptr<fn(i32, i32) -> void>>(field1(deref(read<ptr<@type[[TYPE_fit_loadable_tbl]]>>(%[[VALUE_fit_loadable_handler]])))), read<i32>(%[[VALUE_img_data]]), read<i32>(%[[VALUE_img_len]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
