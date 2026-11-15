// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
void sink(int);

void folded_if(unsigned long *p) {
    if (sizeof(*p) == 4)
        sink(4);
    else
        sink(8);
}

void folded_switch(unsigned long *p) {
    switch (sizeof(*p)) {
    case 1:
        sink(1);
        break;
    case 4:
        sink(4);
        break;
    default:
        sink(0);
    }
}

void runtime_if(int x) {
    if (x)
        sink(1);
}

void label_in_dead_arm(int x) {
    if (x)
        goto inside;
    if (0) {
    inside:
        sink(2);
    }
}

void label_in_switch(int x) {
    if (x)
        goto inside;
    switch (sizeof(int)) {
    case 4:
        sink(4);
        break;
    case 8:
    inside:
        sink(8);
    }
}

void case_in_dead_arm(int x) {
    switch (x) {
    case 0:
        if (0) {
        case 1:
            sink(1);
        }
    }
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
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_folded_if:[0-9]+]] @folded_if(%[[VALUE_p:[0-9]+]] p: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(size_of<u64, u64>(8) [c="__size_t"], reinterpret<u64>(widen<i64>(const<i32>(4))) [c="__size_t"]) [folded=false]
// DEFAULT-NEXT:             call<void>(%[[VALUE_sink]], const<i32>(4));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void>(%[[VALUE_sink]], const<i32>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_folded_switch:[0-9]+]] @folded_switch(%[[VALUE_p_2:[0-9]+]] p: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] size_of<u64, u64>(8) [c="__size_t"] [folded=8]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<u64>(1):
// DEFAULT-NEXT:                     call<void>(%[[VALUE_sink]], const<i32>(1));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<u64>(4):
// DEFAULT-NEXT:                     call<void>(%[[VALUE_sink]], const<i32>(4));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     call<void>(%[[VALUE_sink]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_runtime_if:[0-9]+]] @runtime_if(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// DEFAULT-NEXT:             call<void>(%[[VALUE_sink]], const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_label_in_dead_arm:[0-9]+]] @label_in_dead_arm(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// DEFAULT-NEXT:             goto %[[VALUE_inside:[0-9]+]];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %[[VALUE_inside]] inside:
// DEFAULT-NEXT:                     call<void>(%[[VALUE_sink]], const<i32>(2));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_label_in_switch:[0-9]+]] @label_in_switch(%[[VALUE_x_3:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0))
// DEFAULT-NEXT:             goto %[[VALUE_inside_2:[0-9]+]];
// DEFAULT-NEXT:         switch %[[VALUE2:[0-9]+]] size_of<u64, i32>(4) [c="__size_t"]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE2]] const<u64>(4):
// DEFAULT-NEXT:                     call<void>(%[[VALUE_sink]], const<i32>(4));
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<u64>(8):
// DEFAULT-NEXT:                     label %[[VALUE_inside_2]] inside:
// DEFAULT-NEXT:                         call<void>(%[[VALUE_sink]], const<i32>(8));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_case_in_dead_arm:[0-9]+]] @case_in_dead_arm(%[[VALUE_x_4:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE3:[0-9]+]] read<i32>(%[[VALUE_x_4]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i32>(0):
// DEFAULT-NEXT:                     if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE3]] const<i32>(1):
// DEFAULT-NEXT:                                 call<void>(%[[VALUE_sink]], const<i32>(1));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
