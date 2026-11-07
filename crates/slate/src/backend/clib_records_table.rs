pub const CLIB_RECORD_TYPES: &[CLibRecord] = &[
    CLibRecord { rust_path: "slate_support::DIR", aliases: &[
        CLibAliases { names: &["DIR"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_dirdesc"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
        CLibAliases { names: &["__dirstream"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::Dl_info", aliases: &[
        CLibAliases { names: &["Dl_info"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["dl_info"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::FILE", aliases: &[
        CLibAliases { names: &["FILE"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_arm_exception_state64", aliases: &[
        CLibAliases { names: &["__darwin_arm_exception_state64"], targets: &["aarch64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_arm_neon_state64", aliases: &[
        CLibAliases { names: &["__darwin_arm_neon_state64"], targets: &["aarch64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_arm_thread_state64", aliases: &[
        CLibAliases { names: &["__darwin_arm_thread_state64"], targets: &["aarch64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_mcontext64", aliases: &[
        CLibAliases { names: &["__darwin_mcontext64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_mmst_reg", aliases: &[
        CLibAliases { names: &["__darwin_mmst_reg"], targets: &["x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_ucontext", aliases: &[
        CLibAliases { names: &["__darwin_ucontext"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_x86_exception_state64", aliases: &[
        CLibAliases { names: &["__darwin_x86_exception_state64"], targets: &["x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_x86_float_state64", aliases: &[
        CLibAliases { names: &["__darwin_x86_float_state64"], targets: &["x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_x86_thread_state64", aliases: &[
        CLibAliases { names: &["__darwin_x86_thread_state64"], targets: &["x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__darwin_xmm_reg", aliases: &[
        CLibAliases { names: &["__darwin_xmm_reg"], targets: &["x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::__kernel_fsid_t", aliases: &[
        CLibAliases { names: &["__kernel_fsid_t"], targets: &["aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::_libc_fpreg", aliases: &[
        CLibAliases { names: &["_libc_fpreg"], targets: &["i686-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::_libc_fpstate", aliases: &[
        CLibAliases { names: &["_libc_fpstate"], targets: &["i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::_libc_fpxreg", aliases: &[
        CLibAliases { names: &["_libc_fpxreg"], targets: &["x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::_libc_xmmreg", aliases: &[
        CLibAliases { names: &["_libc_xmmreg"], targets: &["x86_64-linux-android", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::_sem", aliases: &[
        CLibAliases { names: &["_sem"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::_umtx_time", aliases: &[
        CLibAliases { names: &["_umtx_time"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::addrinfo", aliases: &[
        CLibAliases { names: &["addrinfo"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::bintime", aliases: &[
        CLibAliases { names: &["bintime"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::caddr_t", aliases: &[
        CLibAliases { names: &["caddr_t"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::cap_rights_t", aliases: &[
        CLibAliases { names: &["cap_rights", "cap_rights_t"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::clockinfo", aliases: &[
        CLibAliases { names: &["clockinfo"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::cmsgcred", aliases: &[
        CLibAliases { names: &["cmsgcred"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::cmsghdr", aliases: &[
        CLibAliases { names: &["cmsghdr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::cpu_set_t", aliases: &[
        CLibAliases { names: &["cpu_set_t"], targets: &["aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::cpuset_t", aliases: &[
        CLibAliases { names: &["_cpuset", "cpuset_t"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::dirent", aliases: &[
        CLibAliases { names: &["dirent"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-linux-android", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::dirent64", aliases: &[
        CLibAliases { names: &["dirent64"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::epoll_params", aliases: &[
        CLibAliases { names: &["epoll_params"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::fd_set", aliases: &[
        CLibAliases { names: &["fd_set"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::flock", aliases: &[
        CLibAliases { names: &["flock"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::flock64", aliases: &[
        CLibAliases { names: &["flock64"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::fpos_t", aliases: &[
        CLibAliases { names: &["fpos_t"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_G_fpos_t"], targets: &["aarch64-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
        CLibAliases { names: &["_G_fpos64_t"], targets: &["aarch64-unknown-linux-musl", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::fpregs", aliases: &[
        CLibAliases { names: &["fpregs"], targets: &["aarch64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::fpunchhole_t", aliases: &[
        CLibAliases { names: &["fpunchhole", "fpunchhole_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::fsid_t", aliases: &[
        CLibAliases { names: &["fsid_t"], targets: &["aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::fspecread_t", aliases: &[
        CLibAliases { names: &["fspecread", "fspecread_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::fstore_t", aliases: &[
        CLibAliases { names: &["fstore", "fstore_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::ftrimactivefile_t", aliases: &[
        CLibAliases { names: &["ftrimactivefile", "ftrimactivefile_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::glob_t", aliases: &[
        CLibAliases { names: &["glob_t"], targets: &["aarch64-unknown-linux-musl", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::gpregs", aliases: &[
        CLibAliases { names: &["gpregs"], targets: &["aarch64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::group", aliases: &[
        CLibAliases { names: &["group"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::group_req", aliases: &[
        CLibAliases { names: &["group_req"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::group_source_req", aliases: &[
        CLibAliases { names: &["group_source_req"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::host_cpu_load_info", aliases: &[
        CLibAliases { names: &["host_cpu_load_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::host_cpu_load_info_data_t", aliases: &[
        CLibAliases { names: &["host_cpu_load_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::host_cpu_load_info_t", aliases: &[
        CLibAliases { names: &["host_cpu_load_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::host_info64_t", aliases: &[
        CLibAliases { names: &["host_info64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::host_info_t", aliases: &[
        CLibAliases { names: &["host_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::hostent", aliases: &[
        CLibAliases { names: &["hostent"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_announcemsghdr", aliases: &[
        CLibAliases { names: &["if_announcemsghdr"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_clonereq", aliases: &[
        CLibAliases { names: &["if_clonereq"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_data", aliases: &[
        CLibAliases { names: &["if_data"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_data64", aliases: &[
        CLibAliases { names: &["if_data64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_msghdr", aliases: &[
        CLibAliases { names: &["if_msghdr"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_msghdr2", aliases: &[
        CLibAliases { names: &["if_msghdr2"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_msghdrl", aliases: &[
        CLibAliases { names: &["if_msghdrl"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::if_nameindex", aliases: &[
        CLibAliases { names: &["if_nameindex"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifa_msghdr", aliases: &[
        CLibAliases { names: &["ifa_msghdr"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifa_msghdrl", aliases: &[
        CLibAliases { names: &["ifa_msghdrl"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifaddrs", aliases: &[
        CLibAliases { names: &["ifaddrs"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifaliasreq", aliases: &[
        CLibAliases { names: &["ifaliasreq"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifconf", aliases: &[
        CLibAliases { names: &["ifconf"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifdevmtu", aliases: &[
        CLibAliases { names: &["ifdevmtu"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifdownreason", aliases: &[
        CLibAliases { names: &["ifdownreason"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifdrv", aliases: &[
        CLibAliases { names: &["ifdrv"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifi2creq", aliases: &[
        CLibAliases { names: &["ifi2creq"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifkpi", aliases: &[
        CLibAliases { names: &["ifkpi"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifma_msghdr", aliases: &[
        CLibAliases { names: &["ifma_msghdr"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifma_msghdr2", aliases: &[
        CLibAliases { names: &["ifma_msghdr2"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifmediareq", aliases: &[
        CLibAliases { names: &["ifmediareq"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifreq", aliases: &[
        CLibAliases { names: &["ifreq"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifreq_buffer", aliases: &[
        CLibAliases { names: &["ifreq_buffer"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifrsshash", aliases: &[
        CLibAliases { names: &["ifrsshash"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifrsskey", aliases: &[
        CLibAliases { names: &["ifrsskey"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::ifstat", aliases: &[
        CLibAliases { names: &["ifstat"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::in6_addr", aliases: &[
        CLibAliases { names: &["in6_addr"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::in6_ifreq", aliases: &[
        CLibAliases { names: &["in6_ifreq"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::in6_pktinfo", aliases: &[
        CLibAliases { names: &["in6_pktinfo"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::in_addr", aliases: &[
        CLibAliases { names: &["in_addr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::in_pktinfo", aliases: &[
        CLibAliases { names: &["in_pktinfo"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::iovec", aliases: &[
        CLibAliases { names: &["iovec"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ip_mreq", aliases: &[
        CLibAliases { names: &["ip_mreq"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ip_mreq_source", aliases: &[
        CLibAliases { names: &["ip_mreq_source"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ip_mreqn", aliases: &[
        CLibAliases { names: &["ip_mreqn"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ipc_perm", aliases: &[
        CLibAliases { names: &["ipc_perm"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::ipv6_mreq", aliases: &[
        CLibAliases { names: &["ipv6_mreq"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::itimerspec", aliases: &[
        CLibAliases { names: &["itimerspec"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::itimerval", aliases: &[
        CLibAliases { names: &["itimerval"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::kevent", aliases: &[
        CLibAliases { names: &["kevent"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::kevent64_s", aliases: &[
        CLibAliases { names: &["kevent64_s"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::lconv", aliases: &[
        CLibAliases { names: &["lconv"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::linger", aliases: &[
        CLibAliases { names: &["linger"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::load_command", aliases: &[
        CLibAliases { names: &["load_command"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::locale_t", aliases: &[
        CLibAliases { names: &["locale_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::log2phys", aliases: &[
        CLibAliases { names: &["log2phys"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::bootstrap::bool_array_t", aliases: &[
        CLibAliases { names: &["bool_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::bootstrap::bootstrap_property_array_t", aliases: &[
        CLibAliases { names: &["bootstrap_property_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::bootstrap::bootstrap_status_array_t", aliases: &[
        CLibAliases { names: &["bootstrap_status_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::bootstrap::name_array_t", aliases: &[
        CLibAliases { names: &["name_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::clock_types::clock_attr_t", aliases: &[
        CLibAliases { names: &["clock_attr_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::clock_types::mach_timespec", aliases: &[
        CLibAliases { names: &["mach_timespec"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::clock_types::mach_timespec_t", aliases: &[
        CLibAliases { names: &["mach_timespec_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::dyld_kernel::dyld_kernel_image_info", aliases: &[
        CLibAliases { names: &["dyld_kernel_image_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::dyld_kernel::dyld_kernel_image_info_array_t", aliases: &[
        CLibAliases { names: &["dyld_kernel_image_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::dyld_kernel::dyld_kernel_image_info_t", aliases: &[
        CLibAliases { names: &["dyld_kernel_image_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::dyld_kernel::dyld_kernel_process_info", aliases: &[
        CLibAliases { names: &["dyld_kernel_process_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::dyld_kernel::dyld_kernel_process_info_t", aliases: &[
        CLibAliases { names: &["dyld_kernel_process_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::error::mach_error_fn_t", aliases: &[
        CLibAliases { names: &["mach_error_fn_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exc::__Reply__exception_raise_t", aliases: &[
        CLibAliases { names: &["__Reply__exception_raise_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exc::__Request__exception_raise_t", aliases: &[
        CLibAliases { names: &["__Request__exception_raise_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exception_types::exception_behavior_array_t", aliases: &[
        CLibAliases { names: &["exception_behavior_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exception_types::exception_data_t", aliases: &[
        CLibAliases { names: &["exception_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exception_types::exception_flavor_array_t", aliases: &[
        CLibAliases { names: &["exception_flavor_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exception_types::exception_mask_array_t", aliases: &[
        CLibAliases { names: &["exception_mask_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exception_types::exception_port_array_t", aliases: &[
        CLibAliases { names: &["exception_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::exception_types::mach_exception_data_t", aliases: &[
        CLibAliases { names: &["mach_exception_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::loader::mach_header", aliases: &[
        CLibAliases { names: &["mach_header"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::exception_handler_info_array_t", aliases: &[
        CLibAliases { names: &["exception_handler_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::exception_port_info_array_t", aliases: &[
        CLibAliases { names: &["exception_port_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_name", aliases: &[
        CLibAliases { names: &["ipc_info_name"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_name_array_t", aliases: &[
        CLibAliases { names: &["ipc_info_name_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_name_t", aliases: &[
        CLibAliases { names: &["ipc_info_name_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_port", aliases: &[
        CLibAliases { names: &["ipc_info_port"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_port_t", aliases: &[
        CLibAliases { names: &["ipc_info_port_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_space", aliases: &[
        CLibAliases { names: &["ipc_info_space"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_space_basic", aliases: &[
        CLibAliases { names: &["ipc_info_space_basic"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_space_basic_t", aliases: &[
        CLibAliases { names: &["ipc_info_space_basic_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_space_t", aliases: &[
        CLibAliases { names: &["ipc_info_space_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_tree_name", aliases: &[
        CLibAliases { names: &["ipc_info_tree_name"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_tree_name_array_t", aliases: &[
        CLibAliases { names: &["ipc_info_tree_name_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::ipc_info::ipc_info_tree_name_t", aliases: &[
        CLibAliases { names: &["ipc_info_tree_name_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_memory_info", aliases: &[
        CLibAliases { names: &["mach_memory_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_memory_info_array_t", aliases: &[
        CLibAliases { names: &["mach_memory_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_memory_info_t", aliases: &[
        CLibAliases { names: &["mach_memory_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_zone_info_array_t", aliases: &[
        CLibAliases { names: &["mach_zone_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_zone_info_data", aliases: &[
        CLibAliases { names: &["mach_zone_info_data"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_zone_info_t", aliases: &[
        CLibAliases { names: &["mach_zone_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_zone_name", aliases: &[
        CLibAliases { names: &["mach_zone_name"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_zone_name_array_t", aliases: &[
        CLibAliases { names: &["mach_zone_name_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::mach_zone_name_t", aliases: &[
        CLibAliases { names: &["mach_zone_name_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::task_zone_info_array_t", aliases: &[
        CLibAliases { names: &["task_zone_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::task_zone_info_data", aliases: &[
        CLibAliases { names: &["task_zone_info_data"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::task_zone_info_t", aliases: &[
        CLibAliases { names: &["task_zone_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_btrecord", aliases: &[
        CLibAliases { names: &["zone_btrecord"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_btrecord_array_t", aliases: &[
        CLibAliases { names: &["zone_btrecord_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_btrecord_t", aliases: &[
        CLibAliases { names: &["zone_btrecord_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_info", aliases: &[
        CLibAliases { names: &["zone_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_info_array_t", aliases: &[
        CLibAliases { names: &["zone_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_info_t", aliases: &[
        CLibAliases { names: &["zone_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_name", aliases: &[
        CLibAliases { names: &["zone_name"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_name_array_t", aliases: &[
        CLibAliases { names: &["zone_name_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_debug::zone_info::zone_name_t", aliases: &[
        CLibAliases { names: &["zone_name_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_time::mach_timebase_info", aliases: &[
        CLibAliases { names: &["mach_timebase_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_time::mach_timebase_info_data_t", aliases: &[
        CLibAliases { names: &["mach_timebase_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_time::mach_timebase_info_t", aliases: &[
        CLibAliases { names: &["mach_timebase_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::exception_handler_array_t", aliases: &[
        CLibAliases { names: &["exception_handler_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::exception_port_arrary_t", aliases: &[
        CLibAliases { names: &["exception_port_arrary_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::fsid", aliases: &[
        CLibAliases { names: &["fsid"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::fsid_t", aliases: &[
        CLibAliases { names: &["fsid_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::fsobj_id", aliases: &[
        CLibAliases { names: &["fsobj_id"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::fsobj_id_t", aliases: &[
        CLibAliases { names: &["fsobj_id_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::ledger_array_t", aliases: &[
        CLibAliases { names: &["ledger_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::ledger_port_array_t", aliases: &[
        CLibAliases { names: &["ledger_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::processor_array_t", aliases: &[
        CLibAliases { names: &["processor_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::processor_port_array_t", aliases: &[
        CLibAliases { names: &["processor_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::processor_set_array_t", aliases: &[
        CLibAliases { names: &["processor_set_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::processor_set_name_array_t", aliases: &[
        CLibAliases { names: &["processor_set_name_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::processor_set_name_port_array_t", aliases: &[
        CLibAliases { names: &["processor_set_name_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::task_array_t", aliases: &[
        CLibAliases { names: &["task_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::task_port_array_t", aliases: &[
        CLibAliases { names: &["task_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::thread_act_array_t", aliases: &[
        CLibAliases { names: &["thread_act_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::thread_act_port_array_t", aliases: &[
        CLibAliases { names: &["thread_act_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::thread_array_t", aliases: &[
        CLibAliases { names: &["thread_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_types::thread_port_array_t", aliases: &[
        CLibAliases { names: &["thread_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_voucher_types::mach_voucher_attr_content_t", aliases: &[
        CLibAliases { names: &["mach_voucher_attr_content_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_voucher_types::mach_voucher_attr_key_array_t", aliases: &[
        CLibAliases { names: &["mach_voucher_attr_key_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_voucher_types::mach_voucher_attr_raw_recipe_array_t", aliases: &[
        CLibAliases { names: &["mach_voucher_attr_raw_recipe_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_voucher_types::mach_voucher_attr_raw_recipe_t", aliases: &[
        CLibAliases { names: &["mach_voucher_attr_raw_recipe_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_voucher_types::mach_voucher_attr_recipe_command_array_t", aliases: &[
        CLibAliases { names: &["mach_voucher_attr_recipe_command_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_voucher_types::mach_voucher_attr_value_handle_array_t", aliases: &[
        CLibAliases { names: &["mach_voucher_attr_value_handle_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mach_voucher_types::mach_voucher_name_array_t", aliases: &[
        CLibAliases { names: &["mach_voucher_name_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::memory_object_types::memory_object_fault_info_t", aliases: &[
        CLibAliases { names: &["memory_object_fault_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::audit_token_t", aliases: &[
        CLibAliases { names: &["audit_token_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::mach_msg_audit_trailer_t", aliases: &[
        CLibAliases { names: &["mach_msg_audit_trailer_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::mach_msg_base_t", aliases: &[
        CLibAliases { names: &["mach_msg_base_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::mach_msg_body_t", aliases: &[
        CLibAliases { names: &["mach_msg_body_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::mach_msg_header_t", aliases: &[
        CLibAliases { names: &["mach_msg_header_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::mach_msg_security_trailer_t", aliases: &[
        CLibAliases { names: &["mach_msg_security_trailer_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::mach_msg_seqno_trailer_t", aliases: &[
        CLibAliases { names: &["mach_msg_seqno_trailer_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::mach_msg_trailer_t", aliases: &[
        CLibAliases { names: &["mach_msg_trailer_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::message::security_token_t", aliases: &[
        CLibAliases { names: &["security_token_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_impl_routine_t", aliases: &[
        CLibAliases { names: &["mig_impl_routine_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_routine_arg_descriptor_t", aliases: &[
        CLibAliases { names: &["mig_routine_arg_descriptor_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_routine_descriptor", aliases: &[
        CLibAliases { names: &["mig_routine_descriptor"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_routine_descriptor_t", aliases: &[
        CLibAliases { names: &["mig_routine_descriptor_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_routine_t", aliases: &[
        CLibAliases { names: &["mig_routine_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_server_routine_t", aliases: &[
        CLibAliases { names: &["mig_server_routine_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_stub_routine_t", aliases: &[
        CLibAliases { names: &["mig_stub_routine_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_subsystem", aliases: &[
        CLibAliases { names: &["mig_subsystem"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_subsystem_t", aliases: &[
        CLibAliases { names: &["mig_subsystem_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_symtab", aliases: &[
        CLibAliases { names: &["mig_symtab"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::mig_symtab_t", aliases: &[
        CLibAliases { names: &["mig_symtab_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::routine_arg_descriptor_t", aliases: &[
        CLibAliases { names: &["routine_arg_descriptor_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::routine_descriptor", aliases: &[
        CLibAliases { names: &["routine_descriptor"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::mig::routine_descriptor_t", aliases: &[
        CLibAliases { names: &["routine_descriptor_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::notify::mach_dead_name_notification_t", aliases: &[
        CLibAliases { names: &["mach_dead_name_notification_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::notify::mach_no_senders_notification_t", aliases: &[
        CLibAliases { names: &["mach_no_senders_notification_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::notify::mach_port_deleted_notification_t", aliases: &[
        CLibAliases { names: &["mach_port_deleted_notification_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::notify::mach_port_destroyed_notification_t", aliases: &[
        CLibAliases { names: &["mach_port_destroyed_notification_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::notify::mach_send_once_notification_t", aliases: &[
        CLibAliases { names: &["mach_send_once_notification_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::notify::mach_send_possible_notification_t", aliases: &[
        CLibAliases { names: &["mach_send_possible_notification_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_base_t", aliases: &[
        CLibAliases { names: &["policy_base_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_base", aliases: &[
        CLibAliases { names: &["policy_fifo_base"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_base_data_t", aliases: &[
        CLibAliases { names: &["policy_fifo_base_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_base_t", aliases: &[
        CLibAliases { names: &["policy_fifo_base_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_info", aliases: &[
        CLibAliases { names: &["policy_fifo_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_info_data_t", aliases: &[
        CLibAliases { names: &["policy_fifo_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_info_t", aliases: &[
        CLibAliases { names: &["policy_fifo_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_limit", aliases: &[
        CLibAliases { names: &["policy_fifo_limit"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_limit_data_t", aliases: &[
        CLibAliases { names: &["policy_fifo_limit_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_fifo_limit_t", aliases: &[
        CLibAliases { names: &["policy_fifo_limit_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_info_t", aliases: &[
        CLibAliases { names: &["policy_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_limit_t", aliases: &[
        CLibAliases { names: &["policy_limit_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_base", aliases: &[
        CLibAliases { names: &["policy_rr_base"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_base_data_t", aliases: &[
        CLibAliases { names: &["policy_rr_base_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_base_t", aliases: &[
        CLibAliases { names: &["policy_rr_base_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_info", aliases: &[
        CLibAliases { names: &["policy_rr_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_info_data_t", aliases: &[
        CLibAliases { names: &["policy_rr_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_info_t", aliases: &[
        CLibAliases { names: &["policy_rr_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_limit", aliases: &[
        CLibAliases { names: &["policy_rr_limit"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_limit_data_t", aliases: &[
        CLibAliases { names: &["policy_rr_limit_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_rr_limit_t", aliases: &[
        CLibAliases { names: &["policy_rr_limit_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_base", aliases: &[
        CLibAliases { names: &["policy_timeshare_base"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_base_data_t", aliases: &[
        CLibAliases { names: &["policy_timeshare_base_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_base_t", aliases: &[
        CLibAliases { names: &["policy_timeshare_base_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_info", aliases: &[
        CLibAliases { names: &["policy_timeshare_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_info_data_t", aliases: &[
        CLibAliases { names: &["policy_timeshare_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_info_t", aliases: &[
        CLibAliases { names: &["policy_timeshare_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_limit", aliases: &[
        CLibAliases { names: &["policy_timeshare_limit"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_limit_data_t", aliases: &[
        CLibAliases { names: &["policy_timeshare_limit_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::policy::policy_timeshare_limit_t", aliases: &[
        CLibAliases { names: &["policy_timeshare_limit_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::port::mach_port_array_t", aliases: &[
        CLibAliases { names: &["mach_port_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::port::mach_port_info_t", aliases: &[
        CLibAliases { names: &["mach_port_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::port::mach_port_limits_t", aliases: &[
        CLibAliases { names: &["mach_port_limits", "mach_port_limits_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::port::mach_port_options_t", aliases: &[
        CLibAliases { names: &["mach_port_options", "mach_port_options_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::structs::arm_thread_state64_t", aliases: &[
        CLibAliases { names: &["arm_thread_state64_t"], targets: &["aarch64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::structs::x86_thread_state64_t", aliases: &[
        CLibAliases { names: &["x86_thread_state64_t"], targets: &["x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__lock_set_create_t", aliases: &[
        CLibAliases { names: &["__Reply__lock_set_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__lock_set_destroy_t", aliases: &[
        CLibAliases { names: &["__Reply__lock_set_destroy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__mach_ports_lookup_t", aliases: &[
        CLibAliases { names: &["__Reply__mach_ports_lookup_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__mach_ports_register_t", aliases: &[
        CLibAliases { names: &["__Reply__mach_ports_register_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__semaphore_create_t", aliases: &[
        CLibAliases { names: &["__Reply__semaphore_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_assign_default_t", aliases: &[
        CLibAliases { names: &["__Reply__task_assign_default_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_assign_t", aliases: &[
        CLibAliases { names: &["__Reply__task_assign_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_create_t", aliases: &[
        CLibAliases { names: &["__Reply__task_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_get_assignment_t", aliases: &[
        CLibAliases { names: &["__Reply__task_get_assignment_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_get_emulation_vector_t", aliases: &[
        CLibAliases { names: &["__Reply__task_get_emulation_vector_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_get_exception_ports_t", aliases: &[
        CLibAliases { names: &["__Reply__task_get_exception_ports_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_get_special_port_t", aliases: &[
        CLibAliases { names: &["__Reply__task_get_special_port_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_get_state_t", aliases: &[
        CLibAliases { names: &["__Reply__task_get_state_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_info_t", aliases: &[
        CLibAliases { names: &["__Reply__task_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_policy_get_t", aliases: &[
        CLibAliases { names: &["__Reply__task_policy_get_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_policy_set_t", aliases: &[
        CLibAliases { names: &["__Reply__task_policy_set_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_policy_t", aliases: &[
        CLibAliases { names: &["__Reply__task_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_resume_t", aliases: &[
        CLibAliases { names: &["__Reply__task_resume_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_sample_t", aliases: &[
        CLibAliases { names: &["__Reply__task_sample_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_emulation_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_emulation_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_emulation_vector_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_emulation_vector_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_exception_ports_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_exception_ports_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_info_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_policy_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_ras_pc_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_ras_pc_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_special_port_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_special_port_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_set_state_t", aliases: &[
        CLibAliases { names: &["__Reply__task_set_state_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_suspend_t", aliases: &[
        CLibAliases { names: &["__Reply__task_suspend_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_swap_exception_ports_t", aliases: &[
        CLibAliases { names: &["__Reply__task_swap_exception_ports_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_terminate_t", aliases: &[
        CLibAliases { names: &["__Reply__task_terminate_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__task_threads_t", aliases: &[
        CLibAliases { names: &["__Reply__task_threads_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__thread_create_running_t", aliases: &[
        CLibAliases { names: &["__Reply__thread_create_running_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Reply__thread_create_t", aliases: &[
        CLibAliases { names: &["__Reply__thread_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__lock_set_create_t", aliases: &[
        CLibAliases { names: &["__Request__lock_set_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__lock_set_destroy_t", aliases: &[
        CLibAliases { names: &["__Request__lock_set_destroy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__mach_ports_lookup_t", aliases: &[
        CLibAliases { names: &["__Request__mach_ports_lookup_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__mach_ports_register_t", aliases: &[
        CLibAliases { names: &["__Request__mach_ports_register_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__semaphore_create_t", aliases: &[
        CLibAliases { names: &["__Request__semaphore_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_assign_default_t", aliases: &[
        CLibAliases { names: &["__Request__task_assign_default_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_assign_t", aliases: &[
        CLibAliases { names: &["__Request__task_assign_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_create_t", aliases: &[
        CLibAliases { names: &["__Request__task_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_get_assignment_t", aliases: &[
        CLibAliases { names: &["__Request__task_get_assignment_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_get_emulation_vector_t", aliases: &[
        CLibAliases { names: &["__Request__task_get_emulation_vector_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_get_exception_ports_t", aliases: &[
        CLibAliases { names: &["__Request__task_get_exception_ports_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_get_special_port_t", aliases: &[
        CLibAliases { names: &["__Request__task_get_special_port_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_get_state_t", aliases: &[
        CLibAliases { names: &["__Request__task_get_state_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_info_t", aliases: &[
        CLibAliases { names: &["__Request__task_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_policy_get_t", aliases: &[
        CLibAliases { names: &["__Request__task_policy_get_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_policy_set_t", aliases: &[
        CLibAliases { names: &["__Request__task_policy_set_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_policy_t", aliases: &[
        CLibAliases { names: &["__Request__task_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_resume_t", aliases: &[
        CLibAliases { names: &["__Request__task_resume_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_sample_t", aliases: &[
        CLibAliases { names: &["__Request__task_sample_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_emulation_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_emulation_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_emulation_vector_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_emulation_vector_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_exception_ports_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_exception_ports_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_info_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_policy_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_ras_pc_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_ras_pc_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_special_port_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_special_port_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_set_state_t", aliases: &[
        CLibAliases { names: &["__Request__task_set_state_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_suspend_t", aliases: &[
        CLibAliases { names: &["__Request__task_suspend_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_swap_exception_ports_t", aliases: &[
        CLibAliases { names: &["__Request__task_swap_exception_ports_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_terminate_t", aliases: &[
        CLibAliases { names: &["__Request__task_terminate_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__task_threads_t", aliases: &[
        CLibAliases { names: &["__Request__task_threads_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__thread_create_running_t", aliases: &[
        CLibAliases { names: &["__Request__thread_create_running_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::__Request__thread_create_t", aliases: &[
        CLibAliases { names: &["__Request__thread_create_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::emulation_vector_t", aliases: &[
        CLibAliases { names: &["emulation_vector_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task::task_policy_t", aliases: &[
        CLibAliases { names: &["task_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::gpu_energy_data", aliases: &[
        CLibAliases { names: &["gpu_energy_data"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::mach_task_basic_info", aliases: &[
        CLibAliases { names: &["mach_task_basic_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_absolutetime_info", aliases: &[
        CLibAliases { names: &["task_absolutetime_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_affinity_tag_info", aliases: &[
        CLibAliases { names: &["task_affinity_tag_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_basic_info", aliases: &[
        CLibAliases { names: &["task_basic_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_basic_info_32", aliases: &[
        CLibAliases { names: &["task_basic_info_32"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_basic_info_64", aliases: &[
        CLibAliases { names: &["task_basic_info_64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_dyld_info", aliases: &[
        CLibAliases { names: &["task_dyld_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_events_info", aliases: &[
        CLibAliases { names: &["task_events_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_extmod_info", aliases: &[
        CLibAliases { names: &["task_extmod_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_flags_info", aliases: &[
        CLibAliases { names: &["task_flags_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_info_t", aliases: &[
        CLibAliases { names: &["task_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_kernelmemory_info", aliases: &[
        CLibAliases { names: &["task_kernelmemory_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_power_info", aliases: &[
        CLibAliases { names: &["task_power_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_power_info_v2", aliases: &[
        CLibAliases { names: &["task_power_info_v2"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_purgable_info_t", aliases: &[
        CLibAliases { names: &["task_purgable_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_thread_times_info", aliases: &[
        CLibAliases { names: &["task_thread_times_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_trace_memory_info", aliases: &[
        CLibAliases { names: &["task_trace_memory_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_vm_info", aliases: &[
        CLibAliases { names: &["task_vm_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_info::task_wait_state_info", aliases: &[
        CLibAliases { names: &["task_wait_state_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_inspect::task_inspect_basic_counts", aliases: &[
        CLibAliases { names: &["task_inspect_basic_counts"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_inspect::task_inspect_basic_counts_data_t", aliases: &[
        CLibAliases { names: &["task_inspect_basic_counts_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_inspect::task_inspect_basic_counts_t", aliases: &[
        CLibAliases { names: &["task_inspect_basic_counts_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::task_inspect::task_inspect_info_t", aliases: &[
        CLibAliases { names: &["task_inspect_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_affinity_policy", aliases: &[
        CLibAliases { names: &["thread_affinity_policy"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_affinity_policy_data_t", aliases: &[
        CLibAliases { names: &["thread_affinity_policy_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_background_policy", aliases: &[
        CLibAliases { names: &["thread_background_policy"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_background_policy_data_t", aliases: &[
        CLibAliases { names: &["thread_background_policy_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_extended_policy", aliases: &[
        CLibAliases { names: &["thread_extended_policy"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_extended_policy_data_t", aliases: &[
        CLibAliases { names: &["thread_extended_policy_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_policy_t", aliases: &[
        CLibAliases { names: &["thread_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_precedence_policy", aliases: &[
        CLibAliases { names: &["thread_precedence_policy"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_precedence_policy_data_t", aliases: &[
        CLibAliases { names: &["thread_precedence_policy_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_standard_policy", aliases: &[
        CLibAliases { names: &["thread_standard_policy"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_standard_policy_data_t", aliases: &[
        CLibAliases { names: &["thread_standard_policy_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_time_constraint_policy", aliases: &[
        CLibAliases { names: &["thread_time_constraint_policy"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_policy::thread_time_constraint_policy_data_t", aliases: &[
        CLibAliases { names: &["thread_time_constraint_policy_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::thread_status::thread_state_t", aliases: &[
        CLibAliases { names: &["thread_state_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::time_value::time_value", aliases: &[
        CLibAliases { names: &["time_value"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::time_value::time_value_t", aliases: &[
        CLibAliases { names: &["time_value_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::mach_vm_read_entry", aliases: &[
        CLibAliases { names: &["mach_vm_read_entry"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_page_info_basic", aliases: &[
        CLibAliases { names: &["vm_page_info_basic"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_page_info_basic_data_t", aliases: &[
        CLibAliases { names: &["vm_page_info_basic_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_page_info_basic_t", aliases: &[
        CLibAliases { names: &["vm_page_info_basic_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_page_info_t", aliases: &[
        CLibAliases { names: &["vm_page_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_basic_info", aliases: &[
        CLibAliases { names: &["vm_region_basic_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_basic_info_64", aliases: &[
        CLibAliases { names: &["vm_region_basic_info_64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_basic_info_64_t", aliases: &[
        CLibAliases { names: &["vm_region_basic_info_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_basic_info_data_64_t", aliases: &[
        CLibAliases { names: &["vm_region_basic_info_data_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_basic_info_data_t", aliases: &[
        CLibAliases { names: &["vm_region_basic_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_basic_info_t", aliases: &[
        CLibAliases { names: &["vm_region_basic_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_extended_info", aliases: &[
        CLibAliases { names: &["vm_region_extended_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_extended_info_data_t", aliases: &[
        CLibAliases { names: &["vm_region_extended_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_extended_info_t", aliases: &[
        CLibAliases { names: &["vm_region_extended_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_info_64_t", aliases: &[
        CLibAliases { names: &["vm_region_info_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_info_t", aliases: &[
        CLibAliases { names: &["vm_region_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_recurse_info_64_t", aliases: &[
        CLibAliases { names: &["vm_region_recurse_info_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_recurse_info_t", aliases: &[
        CLibAliases { names: &["vm_region_recurse_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_info", aliases: &[
        CLibAliases { names: &["vm_region_submap_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_info_64", aliases: &[
        CLibAliases { names: &["vm_region_submap_info_64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_info_64_t", aliases: &[
        CLibAliases { names: &["vm_region_submap_info_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_info_data_64_t", aliases: &[
        CLibAliases { names: &["vm_region_submap_info_data_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_info_data_t", aliases: &[
        CLibAliases { names: &["vm_region_submap_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_info_t", aliases: &[
        CLibAliases { names: &["vm_region_submap_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_short_info_64", aliases: &[
        CLibAliases { names: &["vm_region_submap_short_info_64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_short_info_64_t", aliases: &[
        CLibAliases { names: &["vm_region_submap_short_info_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_submap_short_info_data_64_t", aliases: &[
        CLibAliases { names: &["vm_region_submap_short_info_data_64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_top_info", aliases: &[
        CLibAliases { names: &["vm_region_top_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_top_info_data_t", aliases: &[
        CLibAliases { names: &["vm_region_top_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_region::vm_region_top_info_t", aliases: &[
        CLibAliases { names: &["vm_region_top_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_extmod_statistics", aliases: &[
        CLibAliases { names: &["vm_extmod_statistics"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_extmod_statistics_data_t", aliases: &[
        CLibAliases { names: &["vm_extmod_statistics_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_extmod_statistics_t", aliases: &[
        CLibAliases { names: &["vm_extmod_statistics_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_purgeable_info", aliases: &[
        CLibAliases { names: &["vm_purgeable_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_purgeable_info_t", aliases: &[
        CLibAliases { names: &["vm_purgeable_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_purgeable_stat", aliases: &[
        CLibAliases { names: &["vm_purgeable_stat"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_purgeable_stat_t", aliases: &[
        CLibAliases { names: &["vm_purgeable_stat_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_statistics", aliases: &[
        CLibAliases { names: &["vm_statistics"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_statistics64_t", aliases: &[
        CLibAliases { names: &["vm_statistics64_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_statistics_data_t", aliases: &[
        CLibAliases { names: &["vm_statistics_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach::vm_statistics::vm_statistics_t", aliases: &[
        CLibAliases { names: &["vm_statistics_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach_header_64", aliases: &[
        CLibAliases { names: &["mach_header_64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach_task_basic_info_data_t", aliases: &[
        CLibAliases { names: &["mach_task_basic_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mach_task_basic_info_t", aliases: &[
        CLibAliases { names: &["mach_task_basic_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::mallinfo", aliases: &[
        CLibAliases { names: &["mallinfo"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::malloc_zone_t", aliases: &[
        CLibAliases { names: &["_malloc_zone_t", "malloc_zone_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::max_align_t", aliases: &[
        CLibAliases { names: &["max_align_t"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "x86_64-linux-android", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::mbstate_t", aliases: &[
        CLibAliases { names: &["mbstate_t"], targets: &["aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::mcontext_t", aliases: &[
        CLibAliases { names: &["mcontext_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
        CLibAliases { names: &["sigcontext"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-musl"] },
        CLibAliases { names: &["__mcontext"], targets: &["x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::mmsghdr", aliases: &[
        CLibAliases { names: &["mmsghdr"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "x86_64-linux-android", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::mq_attr", aliases: &[
        CLibAliases { names: &["mq_attr"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::mqd_t", aliases: &[
        CLibAliases { names: &["mqd_t"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::msghdr", aliases: &[
        CLibAliases { names: &["msghdr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::msginfo", aliases: &[
        CLibAliases { names: &["msginfo"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::msqid_ds", aliases: &[
        CLibAliases { names: &["msqid_ds"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::netent", aliases: &[
        CLibAliases { names: &["netent"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::oifaliasreq", aliases: &[
        CLibAliases { names: &["oifaliasreq"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::option", aliases: &[
        CLibAliases { names: &["option"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::passwd", aliases: &[
        CLibAliases { names: &["passwd"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::pollfd", aliases: &[
        CLibAliases { names: &["pollfd"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::posix_spawn_file_actions_t", aliases: &[
        CLibAliases { names: &["posix_spawn_file_actions_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::posix_spawnattr_t", aliases: &[
        CLibAliases { names: &["posix_spawnattr_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_basic_info", aliases: &[
        CLibAliases { names: &["processor_basic_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_basic_info_data_t", aliases: &[
        CLibAliases { names: &["processor_basic_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_basic_info_t", aliases: &[
        CLibAliases { names: &["processor_basic_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_cpu_load_info", aliases: &[
        CLibAliases { names: &["processor_cpu_load_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_cpu_load_info_data_t", aliases: &[
        CLibAliases { names: &["processor_cpu_load_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_cpu_load_info_t", aliases: &[
        CLibAliases { names: &["processor_cpu_load_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_info_array_t", aliases: &[
        CLibAliases { names: &["processor_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_info_t", aliases: &[
        CLibAliases { names: &["processor_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_set_basic_info", aliases: &[
        CLibAliases { names: &["processor_set_basic_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_set_basic_info_data_t", aliases: &[
        CLibAliases { names: &["processor_set_basic_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_set_basic_info_t", aliases: &[
        CLibAliases { names: &["processor_set_basic_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_set_load_info", aliases: &[
        CLibAliases { names: &["processor_set_load_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_set_load_info_data_t", aliases: &[
        CLibAliases { names: &["processor_set_load_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_set_load_info_t", aliases: &[
        CLibAliases { names: &["processor_set_load_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::protoent", aliases: &[
        CLibAliases { names: &["protoent"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_attr_t", aliases: &[
        CLibAliases { names: &["pthread_attr_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_attr_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_barrier_t", aliases: &[
        CLibAliases { names: &["pthread_barrier_t"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_barrierattr_t", aliases: &[
        CLibAliases { names: &["pthread_barrierattr_t"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_cond_t", aliases: &[
        CLibAliases { names: &["pthread_cond_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_cond_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_condattr_t", aliases: &[
        CLibAliases { names: &["pthread_condattr_t"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_condattr_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_jit_write_callback_t", aliases: &[
        CLibAliases { names: &["pthread_jit_write_callback_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_mutex_t", aliases: &[
        CLibAliases { names: &["pthread_mutex_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_mutex_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_mutexattr_t", aliases: &[
        CLibAliases { names: &["pthread_mutexattr_t"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_mutexattr_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_once_t", aliases: &[
        CLibAliases { names: &["_opaque_pthread_once_t", "pthread_once_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_rwlock_t", aliases: &[
        CLibAliases { names: &["pthread_rwlock_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_rwlock_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_rwlockattr_t", aliases: &[
        CLibAliases { names: &["pthread_rwlockattr_t"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_rwlockattr_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_spinlock_t", aliases: &[
        CLibAliases { names: &["pthread_spinlock_t"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "x86_64-linux-android", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_t", aliases: &[
        CLibAliases { names: &["pthread_t"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::radvisory", aliases: &[
        CLibAliases { names: &["radvisory"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::regmatch_t", aliases: &[
        CLibAliases { names: &["regmatch_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::rlimit", aliases: &[
        CLibAliases { names: &["rlimit"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::rlimit64", aliases: &[
        CLibAliases { names: &["rlimit64"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::rusage", aliases: &[
        CLibAliases { names: &["rusage"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::rusage_info_t", aliases: &[
        CLibAliases { names: &["rusage_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::rusage_info_v0", aliases: &[
        CLibAliases { names: &["rusage_info_v0"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::rusage_info_v1", aliases: &[
        CLibAliases { names: &["rusage_info_v1"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::rusage_info_v2", aliases: &[
        CLibAliases { names: &["rusage_info_v2"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::rusage_info_v3", aliases: &[
        CLibAliases { names: &["rusage_info_v3"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::rusage_info_v4", aliases: &[
        CLibAliases { names: &["rusage_info_v4"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::sa_endpoints_t", aliases: &[
        CLibAliases { names: &["sa_endpoints", "sa_endpoints_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::sched_param", aliases: &[
        CLibAliases { names: &["sched_param"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::segment_command", aliases: &[
        CLibAliases { names: &["segment_command"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::segment_command_64", aliases: &[
        CLibAliases { names: &["segment_command_64"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::sem_t", aliases: &[
        CLibAliases { names: &["sem_t"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sembuf", aliases: &[
        CLibAliases { names: &["sembuf"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::semid_ds", aliases: &[
        CLibAliases { names: &["semid_ds"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::seminfo", aliases: &[
        CLibAliases { names: &["seminfo"], targets: &["aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::semun", aliases: &[
        CLibAliases { names: &["semun"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::servent", aliases: &[
        CLibAliases { names: &["servent"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sf_hdtr", aliases: &[
        CLibAliases { names: &["sf_hdtr"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::shmid_ds", aliases: &[
        CLibAliases { names: &["shmid_ds"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sigaction", aliases: &[
        CLibAliases { names: &["sigaction"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::sigevent", aliases: &[
        CLibAliases { names: &["sigevent"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sighandler_t", aliases: &[
        CLibAliases { names: &["sighandler_t"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::siginfo_t", aliases: &[
        CLibAliases { names: &["siginfo", "siginfo_t"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::sigset_t", aliases: &[
        CLibAliases { names: &["sigset_t"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["__sigset"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
        CLibAliases { names: &["__sigset_t"], targets: &["aarch64-unknown-linux-musl", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr", aliases: &[
        CLibAliases { names: &["sockaddr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_in", aliases: &[
        CLibAliases { names: &["sockaddr_in"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_in6", aliases: &[
        CLibAliases { names: &["sockaddr_in6"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_storage", aliases: &[
        CLibAliases { names: &["sockaddr_storage"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_un", aliases: &[
        CLibAliases { names: &["sockaddr_un"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockcred", aliases: &[
        CLibAliases { names: &["sockcred"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockcred2", aliases: &[
        CLibAliases { names: &["sockcred2"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::spacectl_range", aliases: &[
        CLibAliases { names: &["spacectl_range"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::splice", aliases: &[
        CLibAliases { names: &["splice"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::stack_t", aliases: &[
        CLibAliases { names: &["stack_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["__darwin_sigaltstack"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
        CLibAliases { names: &["sigaltstack"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::stat", aliases: &[
        CLibAliases { names: &["stat"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::statvfs", aliases: &[
        CLibAliases { names: &["statvfs"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::statvfs64", aliases: &[
        CLibAliases { names: &["statvfs64"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::statx_timestamp", aliases: &[
        CLibAliases { names: &["statx_timestamp"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::sysinfo", aliases: &[
        CLibAliases { names: &["sysinfo"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-linux-android", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::task_thread_times_info_data_t", aliases: &[
        CLibAliases { names: &["task_thread_times_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::task_thread_times_info_t", aliases: &[
        CLibAliases { names: &["task_thread_times_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::tcp_fastopen", aliases: &[
        CLibAliases { names: &["tcp_fastopen"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::tcp_function_set", aliases: &[
        CLibAliases { names: &["tcp_function_set"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::termios", aliases: &[
        CLibAliases { names: &["termios"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::termios2", aliases: &[
        CLibAliases { names: &["termios2"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_affinity_policy_t", aliases: &[
        CLibAliases { names: &["thread_affinity_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_background_policy_t", aliases: &[
        CLibAliases { names: &["thread_background_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_basic_info", aliases: &[
        CLibAliases { names: &["thread_basic_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_basic_info_data_t", aliases: &[
        CLibAliases { names: &["thread_basic_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_basic_info_t", aliases: &[
        CLibAliases { names: &["thread_basic_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_extended_info", aliases: &[
        CLibAliases { names: &["thread_extended_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_extended_info_data_t", aliases: &[
        CLibAliases { names: &["thread_extended_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_extended_info_t", aliases: &[
        CLibAliases { names: &["thread_extended_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_extended_policy_t", aliases: &[
        CLibAliases { names: &["thread_extended_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_identifier_info", aliases: &[
        CLibAliases { names: &["thread_identifier_info"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_identifier_info_data_t", aliases: &[
        CLibAliases { names: &["thread_identifier_info_data_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_identifier_info_t", aliases: &[
        CLibAliases { names: &["thread_identifier_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_info_t", aliases: &[
        CLibAliases { names: &["thread_info_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_latency_qos_policy_t", aliases: &[
        CLibAliases { names: &["thread_latency_qos_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_precedence_policy_t", aliases: &[
        CLibAliases { names: &["thread_precedence_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_standard_policy_t", aliases: &[
        CLibAliases { names: &["thread_standard_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_throughput_qos_policy_t", aliases: &[
        CLibAliases { names: &["thread_throughput_qos_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_time_constraint_policy_t", aliases: &[
        CLibAliases { names: &["thread_time_constraint_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::timer_t", aliases: &[
        CLibAliases { names: &["timer_t"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timespec", aliases: &[
        CLibAliases { names: &["timespec"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "i686-pc-windows-msvc", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timeval", aliases: &[
        CLibAliases { names: &["timeval"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timeval32", aliases: &[
        CLibAliases { names: &["timeval32"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::tm", aliases: &[
        CLibAliases { names: &["tm"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::tms", aliases: &[
        CLibAliases { names: &["tms"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ucontext_t", aliases: &[
        CLibAliases { names: &["ucontext_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["ucontext"], targets: &["aarch64-linux-android"] },
        CLibAliases { names: &["__ucontext"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ucred", aliases: &[
        CLibAliases { names: &["ucred"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::user_fpsimd_struct", aliases: &[
        CLibAliases { names: &["user_fpsimd_struct"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::user_regs_struct", aliases: &[
        CLibAliases { names: &["user_regs_struct"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::utimbuf", aliases: &[
        CLibAliases { names: &["utimbuf"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::utsname", aliases: &[
        CLibAliases { names: &["utsname"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::winsize", aliases: &[
        CLibAliases { names: &["winsize"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
];
