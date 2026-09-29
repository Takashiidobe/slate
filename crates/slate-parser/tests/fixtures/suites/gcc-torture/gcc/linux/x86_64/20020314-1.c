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
// DEFAULT-NEXT:     type @type[[TYPE_tux_req_struct:[0-9]+]] tux_req_struct = struct {
// DEFAULT-NEXT:         field0 sock: ptr<@type[[TYPE_socket:[0-9]+]]>;
// DEFAULT-NEXT:         field1 usermode: i8;
// DEFAULT-NEXT:         field2 userbuf: ptr<i8>;
// DEFAULT-NEXT:         field3 userlen: u32;
// DEFAULT-NEXT:         field4 error: i8;
// DEFAULT-NEXT:         field5 private: ptr<void>;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24, 28, 32]];
// DEFAULT-NEXT:     type @type[[TYPE_tux_req_t:[0-9]+]] tux_req_t = @type[[TYPE_tux_req_struct]];
// DEFAULT-NEXT:     type @type[[TYPE_socket]] socket = struct incomplete;
// DEFAULT-NEXT:     fn %[[VALUE_add_output_space_event:[0-9]+]] @add_output_space_event(%[[VALUE_req:[0-9]+]] req: ptr<@type[[TYPE_tux_req_struct]]>, %[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_socket]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_del_tux_atom:[0-9]+]] @del_tux_atom(%[[VALUE_req_2:[0-9]+]] req: ptr<@type[[TYPE_tux_req_struct]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_req_to_workqueue:[0-9]+]] @add_req_to_workqueue(%[[VALUE_req_3:[0-9]+]] req: ptr<@type[[TYPE_tux_req_struct]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_user_send_buffer:[0-9]+]] @user_send_buffer(%[[VALUE_req_4:[0-9]+]] req: ptr<@type[[TYPE_tux_req_struct]]>, %[[VALUE_cachemiss:[0-9]+]] cachemiss: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: i32 [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_repeat:[0-9]+]] repeat:
// DEFAULT-NEXT:             switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_ret]])
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     case %[[VALUE1]] const<i32>(-11):
// DEFAULT-NEXT:                         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_tux_req_struct]]>, ptr<@type[[TYPE_socket]]>) -> i32>(%[[VALUE_add_output_space_event]], read<ptr<@type[[TYPE_tux_req_struct]]>>(%[[VALUE_req_4]]), read<ptr<@type[[TYPE_socket]]>>(field0(deref(read<ptr<@type[[TYPE_tux_req_struct]]>>(%[[VALUE_req_4]]))))), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn(ptr<@type[[TYPE_tux_req_struct]]>) -> void>(%[[VALUE_del_tux_atom]], read<ptr<@type[[TYPE_tux_req_struct]]>>(%[[VALUE_req_4]]));
// DEFAULT-NEXT:                                 goto %[[VALUE_repeat]];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     break %[[VALUE1]];
// DEFAULT-NEXT:                     default %[[VALUE1]]:
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_tux_req_struct]]>) -> void>(%[[VALUE_add_req_to_workqueue]], read<ptr<@type[[TYPE_tux_req_struct]]>>(%[[VALUE_req_4]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
