
// This verifies that global variable redirection works correctly when using hotpatching.
//

#ifdef __clang__
#define NO_TAIL __attribute__((disable_tail_calls))
#else
#define NO_TAIL
#endif

extern int g_data[10];

struct SomeData {
    int x;
    int y;
};

const struct SomeData g_this_is_const = { 100, 200 };

struct HasPointers {
    int* ptr;
    int x;
};

extern struct HasPointers g_has_pointers;

void take_data(const void* p);

void do_side_effects();
void do_other_side_effects();

void hp1() NO_TAIL {
    take_data(&g_data[5]);
}


void hp2() NO_TAIL {
    // We do not expect string literals to be redirected.
    take_data("hello, world!");
}


void hp3() NO_TAIL {
    // We do not expect g_this_is_const to be redirected because it is const
    // and contains no pointers.
    take_data(&g_this_is_const);
}


void hp4() NO_TAIL {
    take_data(&g_has_pointers);
    // We expect &g_has_pointers to be redirected.
}


// This case checks that global variable redirection interacts correctly with PHI nodes.
// The IR for this generates a "phi ptr g_has_pointers, g_this_is_const" node.
// We expect g_has_pointers to be redirected, but not g_this_is_const.
void hp5_phi_ptr_mixed(int x) NO_TAIL {
    const void* y;
    if (x) {
        y = &g_has_pointers;
        do_side_effects();
    } else {
        y = &g_this_is_const;
        do_other_side_effects();
    }
    take_data(y);
}


// This case tests that global variable redirection interacts correctly with PHI nodes,
// where two (all) operands of a given PHI node are globabl variables that redirect.
void hp_phi_ptr_both(int x) NO_TAIL {
    const void* y;
    if (x) {
        y = &g_has_pointers;
        do_side_effects();
    } else {
        y = &g_data[5];
        do_other_side_effects();
    }
    take_data(y);
}


// Test a constant expression which references global variable addresses.
size_t hp_const_ptr_sub() NO_TAIL {
    return (unsigned char*)&g_has_pointers - (unsigned char*)&g_data;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_SomeData:[0-9]+]] SomeData = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_HasPointers:[0-9]+]] HasPointers = struct {
// DEFAULT-NEXT:         field0 ptr: ptr<i32>;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     extern %[[VALUE_g_data:[0-9]+]] g_data: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g_this_is_const:[0-9]+]] g_this_is_const: @type[[TYPE_SomeData]] [storage=static] [const] = aggregate<@type[[TYPE_SomeData]], zero_fill=false>(field0 = const<i32>(100), field1 = const<i32>(200)) [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_g_has_pointers:[0-9]+]] g_has_pointers: @type[[TYPE_HasPointers]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([104, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_take_data:[0-9]+]] @take_data(%[[VALUE_p:[0-9]+]] p: ptr<const void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_do_side_effects:[0-9]+]] @do_side_effects(unprototyped) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_do_other_side_effects:[0-9]+]] @do_other_side_effects(unprototyped) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hp1:[0-9]+]] @hp1(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>) -> void>(%[[VALUE_take_data]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_g_data]]), const<i32>(5))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_hp2:[0-9]+]] @hp2(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>) -> void>(%[[VALUE_take_data]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_hp3:[0-9]+]] @hp3(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>) -> void>(%[[VALUE_take_data]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const @type[[TYPE_SomeData]]>>(%[[VALUE_g_this_is_const]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_hp4:[0-9]+]] @hp4(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>) -> void>(%[[VALUE_take_data]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_HasPointers]]>>(%[[VALUE_g_has_pointers]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_hp5_phi_ptr_mixed:[0-9]+]] @hp5_phi_ptr_mixed(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: ptr<const void> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<const void>>(%[[VALUE_y]], pointer_cast<ptr<const void>, reason=assign>(addr_of<ptr<@type[[TYPE_HasPointers]]>>(%[[VALUE_g_has_pointers]])));
// DEFAULT-NEXT:                 call<void, signature=fn(unprototyped) -> void>(%[[VALUE_do_side_effects]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<const void>>(%[[VALUE_y]], pointer_cast<ptr<const void>, reason=assign>(addr_of<ptr<const @type[[TYPE_SomeData]]>>(%[[VALUE_g_this_is_const]])));
// DEFAULT-NEXT:                 call<void, signature=fn(unprototyped) -> void>(%[[VALUE_do_other_side_effects]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>) -> void>(%[[VALUE_take_data]], read<ptr<const void>>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_hp_phi_ptr_both:[0-9]+]] @hp_phi_ptr_both(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: ptr<const void> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<const void>>(%[[VALUE_y_2]], pointer_cast<ptr<const void>, reason=assign>(addr_of<ptr<@type[[TYPE_HasPointers]]>>(%[[VALUE_g_has_pointers]])));
// DEFAULT-NEXT:                 call<void, signature=fn(unprototyped) -> void>(%[[VALUE_do_side_effects]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<const void>>(%[[VALUE_y_2]], pointer_cast<ptr<const void>, reason=assign>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_g_data]]), const<i32>(5))))));
// DEFAULT-NEXT:                 call<void, signature=fn(unprototyped) -> void>(%[[VALUE_do_other_side_effects]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>) -> void>(%[[VALUE_take_data]], read<ptr<const void>>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_hp_const_ptr_sub:[0-9]+]] @hp_const_ptr_sub(unprototyped) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(ptr_diff<i64, element=u8, same_array=required, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE_HasPointers]]>>(%[[VALUE_g_has_pointers]])), pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<array<i32, 10>>>(%[[VALUE_g_data]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
