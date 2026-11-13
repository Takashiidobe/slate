// SLATE-BUILD-TARGETS: x86_64-unknown-freebsd aarch64-unknown-freebsd
#include <sys/param.h>
#include <sys/cpuset.h>
#include <netinet/tcp.h>
#include <stddef.h>

_Static_assert(sizeof(cpuset_t) == 128, "cpuset size");
_Static_assert(CPU_SETSIZE == 1024, "cpu set capacity");
_Static_assert(sizeof(struct tcp_info) == 256, "tcp_info size");
_Static_assert(offsetof(struct tcp_info, tcpi_total_tlp_bytes) == 168, "tcp_info offset");

unsigned long cpu_set_size(cpuset_t *set) { return sizeof(*set); }
unsigned long tcp_info_size(struct tcp_info *info) { return sizeof(*info); }
unsigned long tcp_tlp_bytes(struct tcp_info *info) { return info->tcpi_total_tlp_bytes; }
unsigned int cpu_capacity(void) { return CPU_SETSIZE; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-freebsd aarch64-unknown-freebsd
// CHECK: {{^}}#[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _cpuset {
// CHECK-NEXT:     __bits: [u64; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tcp_info {
// CHECK-NEXT:     tcpi_state: u8,
// CHECK-NEXT:     __tcpi_ca_state: u8,
// CHECK-NEXT:     __tcpi_retransmits: u8,
// CHECK-NEXT:     __tcpi_probes: u8,
// CHECK-NEXT:     __tcpi_backoff: u8,
// CHECK-NEXT:     tcpi_options: u8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits8U0,
// CHECK-NEXT:     tcpi_rto: u32,
// CHECK-NEXT:     __tcpi_ato: u32,
// CHECK-NEXT:     tcpi_snd_mss: u32,
// CHECK-NEXT:     tcpi_rcv_mss: u32,
// CHECK-NEXT:     __tcpi_unacked: u32,
// CHECK-NEXT:     __tcpi_sacked: u32,
// CHECK-NEXT:     __tcpi_lost: u32,
// CHECK-NEXT:     __tcpi_retrans: u32,
// CHECK-NEXT:     __tcpi_fackets: u32,
// CHECK-NEXT:     __tcpi_last_data_sent: u32,
// CHECK-NEXT:     __tcpi_last_ack_sent: u32,
// CHECK-NEXT:     tcpi_last_data_recv: u32,
// CHECK-NEXT:     __tcpi_last_ack_recv: u32,
// CHECK-NEXT:     __tcpi_pmtu: u32,
// CHECK-NEXT:     __tcpi_rcv_ssthresh: u32,
// CHECK-NEXT:     tcpi_rtt: u32,
// CHECK-NEXT:     tcpi_rttvar: u32,
// CHECK-NEXT:     tcpi_snd_ssthresh: u32,
// CHECK-NEXT:     tcpi_snd_cwnd: u32,
// CHECK-NEXT:     __tcpi_advmss: u32,
// CHECK-NEXT:     __tcpi_reordering: u32,
// CHECK-NEXT:     __tcpi_rcv_rtt: u32,
// CHECK-NEXT:     tcpi_rcv_space: u32,
// CHECK-NEXT:     tcpi_snd_wnd: u32,
// CHECK-NEXT:     tcpi_snd_bwnd: u32,
// CHECK-NEXT:     tcpi_snd_nxt: u32,
// CHECK-NEXT:     tcpi_rcv_nxt: u32,
// CHECK-NEXT:     tcpi_toe_tid: u32,
// CHECK-NEXT:     tcpi_snd_rexmitpack: u32,
// CHECK-NEXT:     tcpi_rcv_ooopack: u32,
// CHECK-NEXT:     tcpi_snd_zerowin: u32,
// CHECK-NEXT:     tcpi_delivered_ce: u32,
// CHECK-NEXT:     tcpi_received_ce: u32,
// CHECK-NEXT:     __tcpi_delivered_e1_bytes: u32,
// CHECK-NEXT:     __tcpi_delivered_e0_bytes: u32,
// CHECK-NEXT:     __tcpi_delivered_ce_bytes: u32,
// CHECK-NEXT:     __tcpi_received_e1_bytes: u32,
// CHECK-NEXT:     __tcpi_received_e0_bytes: u32,
// CHECK-NEXT:     __tcpi_received_ce_bytes: u32,
// CHECK-NEXT:     tcpi_total_tlp: u32,
// CHECK-NEXT:     tcpi_total_tlp_bytes: u64,
// CHECK-NEXT:     tcpi_snd_una: u32,
// CHECK-NEXT:     tcpi_snd_max: u32,
// CHECK-NEXT:     tcpi_rcv_numsacks: u32,
// CHECK-NEXT:     tcpi_rcv_adv: u32,
// CHECK-NEXT:     tcpi_dupacks: u32,
// CHECK-NEXT:     tcpi_rttmin: u32,
// CHECK-NEXT:     __tcpi_pad: [u32; 14],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits8U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub tcpi_snd_wscale: u8,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub tcpi_rcv_wscale: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn cpu_set_size(set: *mut _cpuset) -> u64 {
// CHECK-NEXT:     std::mem::size_of::<_cpuset>() as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn tcp_info_size(info: *mut tcp_info) -> u64 {
// CHECK-NEXT:     std::mem::size_of::<tcp_info>() as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn tcp_tlp_bytes(info: *mut tcp_info) -> u64 {
// CHECK-NEXT:     unsafe { (*info).tcpi_total_tlp_bytes }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn cpu_capacity() -> u32 {
// CHECK-NEXT:     1024
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
