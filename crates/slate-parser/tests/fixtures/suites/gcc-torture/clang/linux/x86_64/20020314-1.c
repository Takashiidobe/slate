// SLATE-FILECHECK-DEFINES DEFAULT

typedef struct tux_req_struct tux_req_t;
struct tux_req_struct
{
        struct socket *sock;
        char usermode;
        char *userbuf;
        unsigned int userlen;
        char error;
        void *private;
};
int add_output_space_event(tux_req_t *req, struct socket *);
void del_tux_atom(tux_req_t *req);
void add_req_to_workqueue(tux_req_t *req);
void user_send_buffer (tux_req_t *req, int cachemiss)
{
        int ret;
repeat:
        switch (ret) {
                case -11:
                        if (add_output_space_event(req, req->sock)) {
                                del_tux_atom(req);
                                goto repeat;
                        }
                        do { } while (0);
                        break;
                default:
                        add_req_to_workqueue(req);
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
// DEFAULT-NEXT:     type @type0 tux_req_struct = struct incomplete;
// DEFAULT-NEXT:     type @type1 tux_req_t = @type0;
// DEFAULT-NEXT:     type @type2 socket = struct incomplete;
// DEFAULT-NEXT:     fn %4 @add_output_space_event(%14 req: ptr<@type0>, %15 <unnamed>: ptr<@type2>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @del_tux_atom(%16 req: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @add_req_to_workqueue(%17 req: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @user_send_buffer(%11 req: ptr<@type0>, %12 cachemiss: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 ret: i32 [storage=automatic];
// DEFAULT-NEXT:         label %10 repeat:
// DEFAULT-NEXT:             switch %18 read<i32>(%13)
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     case %18 const<i32>(-11):
// DEFAULT-NEXT:                         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type2>) -> i32>(%4, read<ptr<@type0>>(%11), read<ptr<@type2>>(field0(deref(read<ptr<@type0>>(%11))))), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn(ptr<@type0>) -> void>(%6, read<ptr<@type0>>(%11));
// DEFAULT-NEXT:                                 goto %10;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     do %19
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     break %18;
// DEFAULT-NEXT:                     default %18:
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type0>) -> void>(%8, read<ptr<@type0>>(%11));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
