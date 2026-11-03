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
        CLibAliases { names: &["FILE"], targets: &["aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
    CLibRecord { rust_path: "slate_support::__darwin_mmst_reg", aliases: &[
        CLibAliases { names: &["__darwin_mmst_reg"], targets: &["x86_64-apple-darwin"] },
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
        CLibAliases { names: &["cmsghdr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::cpuset_t", aliases: &[
        CLibAliases { names: &["_cpuset", "cpuset_t"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::dirent", aliases: &[
        CLibAliases { names: &["dirent"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["flock"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::fpos_t", aliases: &[
        CLibAliases { names: &["fpos_t"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_G_fpos_t"], targets: &["aarch64-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
        CLibAliases { names: &["_G_fpos64_t"], targets: &["aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::fpregs", aliases: &[
        CLibAliases { names: &["fpregs"], targets: &["aarch64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::fsid_t", aliases: &[
        CLibAliases { names: &["fsid_t"], targets: &["aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
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
        CLibAliases { names: &["group_req"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::group_source_req", aliases: &[
        CLibAliases { names: &["group_source_req"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["ifconf"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["in6_addr"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::in6_pktinfo", aliases: &[
        CLibAliases { names: &["in6_pktinfo"], targets: &["aarch64-unknown-linux-musl", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::in_addr", aliases: &[
        CLibAliases { names: &["in_addr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::in_pktinfo", aliases: &[
        CLibAliases { names: &["in_pktinfo"], targets: &["aarch64-apple-darwin", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::iovec", aliases: &[
        CLibAliases { names: &["iovec"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ip_mreq", aliases: &[
        CLibAliases { names: &["ip_mreq"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ip_mreq_source", aliases: &[
        CLibAliases { names: &["ip_mreq_source"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ip_mreqn", aliases: &[
        CLibAliases { names: &["ip_mreqn"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ipc_perm", aliases: &[
        CLibAliases { names: &["ipc_perm"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::ipv6_mreq", aliases: &[
        CLibAliases { names: &["ipv6_mreq"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::itimerspec", aliases: &[
        CLibAliases { names: &["itimerspec"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::itimerval", aliases: &[
        CLibAliases { names: &["itimerval"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::kevent", aliases: &[
        CLibAliases { names: &["kevent"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "x86_64-apple-darwin", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::kevent64_s", aliases: &[
        CLibAliases { names: &["kevent64_s"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::lconv", aliases: &[
        CLibAliases { names: &["lconv"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::linger", aliases: &[
        CLibAliases { names: &["linger"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::load_command", aliases: &[
        CLibAliases { names: &["load_command"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::locale_t", aliases: &[
        CLibAliases { names: &["locale_t"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
    CLibRecord { rust_path: "slate_support::mbstate_t", aliases: &[
        CLibAliases { names: &["mbstate_t"], targets: &["aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::mcontext_t", aliases: &[
        CLibAliases { names: &["mcontext_t"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
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
        CLibAliases { names: &["msginfo"], targets: &["aarch64-unknown-linux-musl", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::msqid_ds", aliases: &[
        CLibAliases { names: &["msqid_ds"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::netent", aliases: &[
        CLibAliases { names: &["netent"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::oifaliasreq", aliases: &[
        CLibAliases { names: &["oifaliasreq"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::option", aliases: &[
        CLibAliases { names: &["option"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::passwd", aliases: &[
        CLibAliases { names: &["passwd"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::pollfd", aliases: &[
        CLibAliases { names: &["pollfd"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::posix_spawn_file_actions_t", aliases: &[
        CLibAliases { names: &["posix_spawn_file_actions_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::posix_spawnattr_t", aliases: &[
        CLibAliases { names: &["posix_spawnattr_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::processor_info_array_t", aliases: &[
        CLibAliases { names: &["processor_info_array_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::protoent", aliases: &[
        CLibAliases { names: &["protoent"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_attr_t", aliases: &[
        CLibAliases { names: &["pthread_attr_t"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["_opaque_pthread_attr_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_barrier_t", aliases: &[
        CLibAliases { names: &["pthread_barrier_t"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["pthread_spinlock_t"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::pthread_t", aliases: &[
        CLibAliases { names: &["pthread_t"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::regmatch_t", aliases: &[
        CLibAliases { names: &["regmatch_t"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::rlimit", aliases: &[
        CLibAliases { names: &["rlimit"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["semid_ds"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
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
        CLibAliases { names: &["shmid_ds"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["sockaddr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_in", aliases: &[
        CLibAliases { names: &["sockaddr_in"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_in6", aliases: &[
        CLibAliases { names: &["sockaddr_in6"], targets: &["aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_storage", aliases: &[
        CLibAliases { names: &["sockaddr_storage"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::sockaddr_un", aliases: &[
        CLibAliases { names: &["sockaddr_un"], targets: &["aarch64-apple-darwin", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["statvfs"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::statvfs64", aliases: &[
        CLibAliases { names: &["statvfs64"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
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
        CLibAliases { names: &["termios"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_affinity_policy_t", aliases: &[
        CLibAliases { names: &["thread_affinity_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_background_policy_t", aliases: &[
        CLibAliases { names: &["thread_background_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::thread_extended_policy_t", aliases: &[
        CLibAliases { names: &["thread_extended_policy_t"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
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
        CLibAliases { names: &["timespec"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timeval", aliases: &[
        CLibAliases { names: &["timeval"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timeval32", aliases: &[
        CLibAliases { names: &["timeval32"], targets: &["aarch64-apple-darwin", "x86_64-apple-darwin"] },
    ] },
    CLibRecord { rust_path: "slate_support::tm", aliases: &[
        CLibAliases { names: &["tm"], targets: &["aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::tms", aliases: &[
        CLibAliases { names: &["tms"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ucontext_t", aliases: &[
        CLibAliases { names: &["ucontext"], targets: &["aarch64-linux-android"] },
        CLibAliases { names: &["ucontext_t"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
        CLibAliases { names: &["__ucontext"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::ucred", aliases: &[
        CLibAliases { names: &["ucred"], targets: &["aarch64-linux-android", "x86_64-linux-android"] },
    ] },
    CLibRecord { rust_path: "slate_support::user_fpsimd_struct", aliases: &[
        CLibAliases { names: &["user_fpsimd_struct"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::user_regs_struct", aliases: &[
        CLibAliases { names: &["user_regs_struct"], targets: &["aarch64-unknown-linux-gnu"] },
    ] },
    CLibRecord { rust_path: "slate_support::utimbuf", aliases: &[
        CLibAliases { names: &["utimbuf"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::utsname", aliases: &[
        CLibAliases { names: &["utsname"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Communication::COMMCONFIG", aliases: &[
        CLibAliases { names: &["COMMCONFIG", "_COMMCONFIG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Communication::COMMPROP", aliases: &[
        CLibAliases { names: &["COMMPROP", "_COMMPROP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Communication::COMMTIMEOUTS", aliases: &[
        CLibAliases { names: &["COMMTIMEOUTS", "_COMMTIMEOUTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Communication::MODEMDEVCAPS", aliases: &[
        CLibAliases { names: &["MODEMDEVCAPS", "_MODEMDEVCAPS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Communication::MODEMSETTINGS", aliases: &[
        CLibAliases { names: &["MODEMSETTINGS", "_MODEMSETTINGS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_2DREGION", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_2DREGION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_ADAPTER_NAME", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_ADAPTER_NAME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_DESKTOP_IMAGE_INFO", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_DESKTOP_IMAGE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_DEVICE_INFO_HEADER", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_DEVICE_INFO_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO", "_DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_GET_MONITOR_SPECIALIZATION", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_GET_MONITOR_SPECIALIZATION", "_DISPLAYCONFIG_GET_MONITOR_SPECIALIZATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_MODE_INFO", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_MODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_PATH_SOURCE_INFO", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_PATH_SOURCE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_RATIONAL", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_RATIONAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_SDR_WHITE_LEVEL", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_SDR_WHITE_LEVEL", "_DISPLAYCONFIG_SDR_WHITE_LEVEL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_SET_ADVANCED_COLOR_STATE", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_SET_ADVANCED_COLOR_STATE", "_DISPLAYCONFIG_SET_ADVANCED_COLOR_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_SET_MONITOR_SPECIALIZATION", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_SET_MONITOR_SPECIALIZATION", "_DISPLAYCONFIG_SET_MONITOR_SPECIALIZATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_SET_TARGET_PERSISTENCE", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_SET_TARGET_PERSISTENCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_SOURCE_DEVICE_NAME", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_SOURCE_DEVICE_NAME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_SOURCE_MODE", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_SOURCE_MODE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_SUPPORT_VIRTUAL_RESOLUTION", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_SUPPORT_VIRTUAL_RESOLUTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_TARGET_DEVICE_NAME_FLAGS", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_TARGET_DEVICE_NAME_FLAGS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_TARGET_MODE", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_TARGET_MODE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_TARGET_PREFERRED_MODE", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_TARGET_PREFERRED_MODE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::DISPLAYCONFIG_VIDEO_SIGNAL_INFO", aliases: &[
        CLibAliases { names: &["DISPLAYCONFIG_VIDEO_SIGNAL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Devices::Display::VIDEOPARAMETERS", aliases: &[
        CLibAliases { names: &["VIDEOPARAMETERS", "_VIDEOPARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::APP_LOCAL_DEVICE_ID", aliases: &[
        CLibAliases { names: &["APP_LOCAL_DEVICE_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::DECIMAL", aliases: &[
        CLibAliases { names: &["DECIMAL", "tagDEC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::FARPROC", aliases: &[
        CLibAliases { names: &["FARPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::FILETIME", aliases: &[
        CLibAliases { names: &["FILETIME", "_FILETIME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::FLOAT128", aliases: &[
        CLibAliases { names: &["FLOAT128", "_FLOAT128"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HANDLE", aliases: &[
        CLibAliases { names: &["HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HGLOBAL", aliases: &[
        CLibAliases { names: &["HGLOBAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HINSTANCE", aliases: &[
        CLibAliases { names: &["HINSTANCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HLOCAL", aliases: &[
        CLibAliases { names: &["HLOCAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HLSURF", aliases: &[
        CLibAliases { names: &["HLSURF"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HMODULE", aliases: &[
        CLibAliases { names: &["HMODULE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HRSRC", aliases: &[
        CLibAliases { names: &["HRSRC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HSPRITE", aliases: &[
        CLibAliases { names: &["HSPRITE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HSTR", aliases: &[
        CLibAliases { names: &["HSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HUMPD", aliases: &[
        CLibAliases { names: &["HUMPD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::HWND", aliases: &[
        CLibAliases { names: &["HWND"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::LUID", aliases: &[
        CLibAliases { names: &["LUID", "_LUID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::NEARPROC", aliases: &[
        CLibAliases { names: &["NEARPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::PAPCFUNC", aliases: &[
        CLibAliases { names: &["PAPCFUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::POINT", aliases: &[
        CLibAliases { names: &["POINT", "tagPOINT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::POINTL", aliases: &[
        CLibAliases { names: &["POINTL", "_POINTL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::POINTS", aliases: &[
        CLibAliases { names: &["POINTS", "tagPOINTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::PROC", aliases: &[
        CLibAliases { names: &["PROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::PROPERTYKEY", aliases: &[
        CLibAliases { names: &["PROPERTYKEY", "_tagpropertykey"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::RECT", aliases: &[
        CLibAliases { names: &["RECT", "tagRECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::RECTL", aliases: &[
        CLibAliases { names: &["RECTL", "_RECTL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::SIZE", aliases: &[
        CLibAliases { names: &["SIZE", "tagSIZE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Foundation::SYSTEMTIME", aliases: &[
        CLibAliases { names: &["SYSTEMTIME", "_SYSTEMTIME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CALINFO_ENUMPROCA", aliases: &[
        CLibAliases { names: &["CALINFO_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CALINFO_ENUMPROCEXA", aliases: &[
        CLibAliases { names: &["CALINFO_ENUMPROCEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CALINFO_ENUMPROCEXEX", aliases: &[
        CLibAliases { names: &["CALINFO_ENUMPROCEXEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CALINFO_ENUMPROCEXW", aliases: &[
        CLibAliases { names: &["CALINFO_ENUMPROCEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CALINFO_ENUMPROCW", aliases: &[
        CLibAliases { names: &["CALINFO_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CHARSETINFO", aliases: &[
        CLibAliases { names: &["CHARSETINFO", "tagCHARSETINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CODEPAGE_ENUMPROCA", aliases: &[
        CLibAliases { names: &["CODEPAGE_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CODEPAGE_ENUMPROCW", aliases: &[
        CLibAliases { names: &["CODEPAGE_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CPINFO", aliases: &[
        CLibAliases { names: &["CPINFO", "_cpinfo"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CPINFOEXA", aliases: &[
        CLibAliases { names: &["CPINFOEXA", "_cpinfoexA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CPINFOEXW", aliases: &[
        CLibAliases { names: &["CPINFOEXW", "_cpinfoexW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CURRENCYFMTA", aliases: &[
        CLibAliases { names: &["CURRENCYFMTA", "_currencyfmtA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::CURRENCYFMTW", aliases: &[
        CLibAliases { names: &["CURRENCYFMTW", "_currencyfmtW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::DATEFMT_ENUMPROCA", aliases: &[
        CLibAliases { names: &["DATEFMT_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::DATEFMT_ENUMPROCEXA", aliases: &[
        CLibAliases { names: &["DATEFMT_ENUMPROCEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::DATEFMT_ENUMPROCEXEX", aliases: &[
        CLibAliases { names: &["DATEFMT_ENUMPROCEXEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::DATEFMT_ENUMPROCEXW", aliases: &[
        CLibAliases { names: &["DATEFMT_ENUMPROCEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::DATEFMT_ENUMPROCW", aliases: &[
        CLibAliases { names: &["DATEFMT_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::ENUMTEXTMETRICA", aliases: &[
        CLibAliases { names: &["ENUMTEXTMETRICA", "tagENUMTEXTMETRICA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::ENUMTEXTMETRICW", aliases: &[
        CLibAliases { names: &["ENUMTEXTMETRICW", "tagENUMTEXTMETRICW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::FILEMUIINFO", aliases: &[
        CLibAliases { names: &["FILEMUIINFO", "_FILEMUIINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::FONTSIGNATURE", aliases: &[
        CLibAliases { names: &["FONTSIGNATURE", "tagFONTSIGNATURE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::GEO_ENUMNAMEPROC", aliases: &[
        CLibAliases { names: &["GEO_ENUMNAMEPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::GEO_ENUMPROC", aliases: &[
        CLibAliases { names: &["GEO_ENUMPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::HSAVEDUILANGUAGES", aliases: &[
        CLibAliases { names: &["HSAVEDUILANGUAGES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LANGGROUPLOCALE_ENUMPROCA", aliases: &[
        CLibAliases { names: &["LANGGROUPLOCALE_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LANGGROUPLOCALE_ENUMPROCW", aliases: &[
        CLibAliases { names: &["LANGGROUPLOCALE_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LANGUAGEGROUP_ENUMPROCA", aliases: &[
        CLibAliases { names: &["LANGUAGEGROUP_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LANGUAGEGROUP_ENUMPROCW", aliases: &[
        CLibAliases { names: &["LANGUAGEGROUP_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LOCALESIGNATURE", aliases: &[
        CLibAliases { names: &["LOCALESIGNATURE", "tagLOCALESIGNATURE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LOCALE_ENUMPROCA", aliases: &[
        CLibAliases { names: &["LOCALE_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LOCALE_ENUMPROCEX", aliases: &[
        CLibAliases { names: &["LOCALE_ENUMPROCEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::LOCALE_ENUMPROCW", aliases: &[
        CLibAliases { names: &["LOCALE_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::NEWTEXTMETRICEXA", aliases: &[
        CLibAliases { names: &["NEWTEXTMETRICEXA", "tagNEWTEXTMETRICEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::NEWTEXTMETRICEXW", aliases: &[
        CLibAliases { names: &["NEWTEXTMETRICEXW", "tagNEWTEXTMETRICEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::NLSVERSIONINFO", aliases: &[
        CLibAliases { names: &["NLSVERSIONINFO", "_nlsversioninfo"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::NLSVERSIONINFOEX", aliases: &[
        CLibAliases { names: &["NLSVERSIONINFOEX", "_nlsversioninfoex"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::NUMBERFMTA", aliases: &[
        CLibAliases { names: &["NUMBERFMTA", "_numberfmtA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::NUMBERFMTW", aliases: &[
        CLibAliases { names: &["NUMBERFMTW", "_numberfmtW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::TIMEFMT_ENUMPROCA", aliases: &[
        CLibAliases { names: &["TIMEFMT_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::TIMEFMT_ENUMPROCEX", aliases: &[
        CLibAliases { names: &["TIMEFMT_ENUMPROCEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::TIMEFMT_ENUMPROCW", aliases: &[
        CLibAliases { names: &["TIMEFMT_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::UILANGUAGE_ENUMPROCA", aliases: &[
        CLibAliases { names: &["UILANGUAGE_ENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Globalization::UILANGUAGE_ENUMPROCW", aliases: &[
        CLibAliases { names: &["UILANGUAGE_ENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ABC", aliases: &[
        CLibAliases { names: &["ABC", "_ABC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ABCFLOAT", aliases: &[
        CLibAliases { names: &["ABCFLOAT", "_ABCFLOAT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::AXESLISTA", aliases: &[
        CLibAliases { names: &["AXESLISTA", "tagAXESLISTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::AXESLISTW", aliases: &[
        CLibAliases { names: &["AXESLISTW", "tagAXESLISTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::AXISINFOA", aliases: &[
        CLibAliases { names: &["AXISINFOA", "tagAXISINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::AXISINFOW", aliases: &[
        CLibAliases { names: &["AXISINFOW", "tagAXISINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAP", aliases: &[
        CLibAliases { names: &["BITMAP", "tagBITMAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAPCOREHEADER", aliases: &[
        CLibAliases { names: &["BITMAPCOREHEADER", "tagBITMAPCOREHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAPCOREINFO", aliases: &[
        CLibAliases { names: &["BITMAPCOREINFO", "tagBITMAPCOREINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAPFILEHEADER", aliases: &[
        CLibAliases { names: &["BITMAPFILEHEADER", "tagBITMAPFILEHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAPINFO", aliases: &[
        CLibAliases { names: &["BITMAPINFO", "tagBITMAPINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAPINFOHEADER", aliases: &[
        CLibAliases { names: &["BITMAPINFOHEADER", "tagBITMAPINFOHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAPV4HEADER", aliases: &[
        CLibAliases { names: &["BITMAPV4HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BITMAPV5HEADER", aliases: &[
        CLibAliases { names: &["BITMAPV5HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::BLENDFUNCTION", aliases: &[
        CLibAliases { names: &["BLENDFUNCTION", "_BLENDFUNCTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::CIEXYZ", aliases: &[
        CLibAliases { names: &["CIEXYZ", "tagCIEXYZ"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::CIEXYZTRIPLE", aliases: &[
        CLibAliases { names: &["CIEXYZTRIPLE", "tagICEXYZTRIPLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::COLORADJUSTMENT", aliases: &[
        CLibAliases { names: &["COLORADJUSTMENT", "tagCOLORADJUSTMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DESIGNVECTOR", aliases: &[
        CLibAliases { names: &["DESIGNVECTOR", "tagDESIGNVECTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DEVMODEA", aliases: &[
        CLibAliases { names: &["DEVMODEA", "_devicemodeA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DEVMODEW", aliases: &[
        CLibAliases { names: &["DEVMODEW", "_devicemodeW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DIBSECTION", aliases: &[
        CLibAliases { names: &["DIBSECTION", "tagDIBSECTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DISPLAY_DEVICEA", aliases: &[
        CLibAliases { names: &["DISPLAY_DEVICEA", "_DISPLAY_DEVICEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DISPLAY_DEVICEW", aliases: &[
        CLibAliases { names: &["DISPLAY_DEVICEW", "_DISPLAY_DEVICEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DRAWSTATEPROC", aliases: &[
        CLibAliases { names: &["DRAWSTATEPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::DRAWTEXTPARAMS", aliases: &[
        CLibAliases { names: &["DRAWTEXTPARAMS", "tagDRAWTEXTPARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMR", aliases: &[
        CLibAliases { names: &["EMR", "tagEMR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRALPHABLEND", aliases: &[
        CLibAliases { names: &["EMRALPHABLEND", "tagEMRALPHABLEND"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRANGLEARC", aliases: &[
        CLibAliases { names: &["EMRANGLEARC", "tagEMRANGLEARC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRARC", aliases: &[
        CLibAliases { names: &["EMRARC", "tagEMRARC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRBITBLT", aliases: &[
        CLibAliases { names: &["EMRBITBLT", "tagEMRBITBLT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRCOLORCORRECTPALETTE", aliases: &[
        CLibAliases { names: &["EMRCOLORCORRECTPALETTE", "tagCOLORCORRECTPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRCOLORMATCHTOTARGET", aliases: &[
        CLibAliases { names: &["EMRCOLORMATCHTOTARGET", "tagCOLORMATCHTOTARGET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRCREATEBRUSHINDIRECT", aliases: &[
        CLibAliases { names: &["EMRCREATEBRUSHINDIRECT", "tagEMRCREATEBRUSHINDIRECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRCREATEDIBPATTERNBRUSHPT", aliases: &[
        CLibAliases { names: &["EMRCREATEDIBPATTERNBRUSHPT", "tagEMRCREATEDIBPATTERNBRUSHPT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRCREATEMONOBRUSH", aliases: &[
        CLibAliases { names: &["EMRCREATEMONOBRUSH", "tagEMRCREATEMONOBRUSH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRCREATEPALETTE", aliases: &[
        CLibAliases { names: &["EMRCREATEPALETTE", "tagEMRCREATEPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRCREATEPEN", aliases: &[
        CLibAliases { names: &["EMRCREATEPEN", "tagEMRCREATEPEN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRELLIPSE", aliases: &[
        CLibAliases { names: &["EMRELLIPSE", "tagEMRELLIPSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREOF", aliases: &[
        CLibAliases { names: &["EMREOF", "tagEMREOF"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREXCLUDECLIPRECT", aliases: &[
        CLibAliases { names: &["EMREXCLUDECLIPRECT", "tagEMREXCLUDECLIPRECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREXTCREATEFONTINDIRECTW", aliases: &[
        CLibAliases { names: &["EMREXTCREATEFONTINDIRECTW", "tagEMREXTCREATEFONTINDIRECTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREXTCREATEPEN", aliases: &[
        CLibAliases { names: &["EMREXTCREATEPEN", "tagEMREXTCREATEPEN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREXTESCAPE", aliases: &[
        CLibAliases { names: &["EMREXTESCAPE", "tagEMREXTESCAPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREXTFLOODFILL", aliases: &[
        CLibAliases { names: &["EMREXTFLOODFILL", "tagEMREXTFLOODFILL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREXTSELECTCLIPRGN", aliases: &[
        CLibAliases { names: &["EMREXTSELECTCLIPRGN", "tagEMREXTSELECTCLIPRGN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMREXTTEXTOUTA", aliases: &[
        CLibAliases { names: &["EMREXTTEXTOUTA", "tagEMREXTTEXTOUTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRFILLPATH", aliases: &[
        CLibAliases { names: &["EMRFILLPATH", "tagEMRFILLPATH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRFILLRGN", aliases: &[
        CLibAliases { names: &["EMRFILLRGN", "tagEMRFILLRGN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRFORMAT", aliases: &[
        CLibAliases { names: &["EMRFORMAT", "tagEMRFORMAT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRFRAMERGN", aliases: &[
        CLibAliases { names: &["EMRFRAMERGN", "tagEMRFRAMERGN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRGDICOMMENT", aliases: &[
        CLibAliases { names: &["EMRGDICOMMENT", "tagEMRGDICOMMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRGLSBOUNDEDRECORD", aliases: &[
        CLibAliases { names: &["EMRGLSBOUNDEDRECORD", "tagEMRGLSBOUNDEDRECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRGLSRECORD", aliases: &[
        CLibAliases { names: &["EMRGLSRECORD", "tagEMRGLSRECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRGRADIENTFILL", aliases: &[
        CLibAliases { names: &["EMRGRADIENTFILL", "tagEMRGRADIENTFILL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRINVERTRGN", aliases: &[
        CLibAliases { names: &["EMRINVERTRGN", "tagEMRINVERTRGN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRLINETO", aliases: &[
        CLibAliases { names: &["EMRLINETO", "tagEMRLINETO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRMASKBLT", aliases: &[
        CLibAliases { names: &["EMRMASKBLT", "tagEMRMASKBLT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRMODIFYWORLDTRANSFORM", aliases: &[
        CLibAliases { names: &["EMRMODIFYWORLDTRANSFORM", "tagEMRMODIFYWORLDTRANSFORM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRNAMEDESCAPE", aliases: &[
        CLibAliases { names: &["EMRNAMEDESCAPE", "tagEMRNAMEDESCAPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMROFFSETCLIPRGN", aliases: &[
        CLibAliases { names: &["EMROFFSETCLIPRGN", "tagEMROFFSETCLIPRGN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPLGBLT", aliases: &[
        CLibAliases { names: &["EMRPLGBLT", "tagEMRPLGBLT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPOLYDRAW", aliases: &[
        CLibAliases { names: &["EMRPOLYDRAW", "tagEMRPOLYDRAW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPOLYDRAW16", aliases: &[
        CLibAliases { names: &["EMRPOLYDRAW16", "tagEMRPOLYDRAW16"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPOLYLINE", aliases: &[
        CLibAliases { names: &["EMRPOLYLINE", "tagEMRPOLYLINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPOLYLINE16", aliases: &[
        CLibAliases { names: &["EMRPOLYLINE16", "tagEMRPOLYLINE16"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPOLYPOLYLINE", aliases: &[
        CLibAliases { names: &["EMRPOLYPOLYLINE", "tagEMRPOLYPOLYLINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPOLYPOLYLINE16", aliases: &[
        CLibAliases { names: &["EMRPOLYPOLYLINE16", "tagEMRPOLYPOLYLINE16"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRPOLYTEXTOUTA", aliases: &[
        CLibAliases { names: &["EMRPOLYTEXTOUTA", "tagEMRPOLYTEXTOUTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRRESIZEPALETTE", aliases: &[
        CLibAliases { names: &["EMRRESIZEPALETTE", "tagEMRRESIZEPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRRESTOREDC", aliases: &[
        CLibAliases { names: &["EMRRESTOREDC", "tagEMRRESTOREDC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRROUNDRECT", aliases: &[
        CLibAliases { names: &["EMRROUNDRECT", "tagEMRROUNDRECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSCALEVIEWPORTEXTEX", aliases: &[
        CLibAliases { names: &["EMRSCALEVIEWPORTEXTEX", "tagEMRSCALEVIEWPORTEXTEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSELECTCLIPPATH", aliases: &[
        CLibAliases { names: &["EMRSELECTCLIPPATH", "tagEMRSELECTCLIPPATH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSELECTOBJECT", aliases: &[
        CLibAliases { names: &["EMRSELECTOBJECT", "tagEMRSELECTOBJECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSELECTPALETTE", aliases: &[
        CLibAliases { names: &["EMRSELECTPALETTE", "tagEMRSELECTPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETARCDIRECTION", aliases: &[
        CLibAliases { names: &["EMRSETARCDIRECTION", "tagEMRSETARCDIRECTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETCOLORADJUSTMENT", aliases: &[
        CLibAliases { names: &["EMRSETCOLORADJUSTMENT", "tagEMRSETCOLORADJUSTMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETCOLORSPACE", aliases: &[
        CLibAliases { names: &["EMRSETCOLORSPACE", "tagEMRSETCOLORSPACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETDIBITSTODEVICE", aliases: &[
        CLibAliases { names: &["EMRSETDIBITSTODEVICE", "tagEMRSETDIBITSTODEVICE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETICMPROFILE", aliases: &[
        CLibAliases { names: &["EMRSETICMPROFILE", "tagEMRSETICMPROFILE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETMAPPERFLAGS", aliases: &[
        CLibAliases { names: &["EMRSETMAPPERFLAGS", "tagEMRSETMAPPERFLAGS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETMITERLIMIT", aliases: &[
        CLibAliases { names: &["EMRSETMITERLIMIT", "tagEMRSETMITERLIMIT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETPALETTEENTRIES", aliases: &[
        CLibAliases { names: &["EMRSETPALETTEENTRIES", "tagEMRSETPALETTEENTRIES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETPIXELV", aliases: &[
        CLibAliases { names: &["EMRSETPIXELV", "tagEMRSETPIXELV"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETTEXTCOLOR", aliases: &[
        CLibAliases { names: &["EMRSETTEXTCOLOR", "tagEMRSETTEXTCOLOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETVIEWPORTEXTEX", aliases: &[
        CLibAliases { names: &["EMRSETVIEWPORTEXTEX", "tagEMRSETVIEWPORTEXTEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETVIEWPORTORGEX", aliases: &[
        CLibAliases { names: &["EMRSETVIEWPORTORGEX", "tagEMRSETVIEWPORTORGEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSETWORLDTRANSFORM", aliases: &[
        CLibAliases { names: &["EMRSETWORLDTRANSFORM", "tagEMRSETWORLDTRANSFORM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSTRETCHBLT", aliases: &[
        CLibAliases { names: &["EMRSTRETCHBLT", "tagEMRSTRETCHBLT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRSTRETCHDIBITS", aliases: &[
        CLibAliases { names: &["EMRSTRETCHDIBITS", "tagEMRSTRETCHDIBITS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRTEXT", aliases: &[
        CLibAliases { names: &["EMRTEXT", "tagEMRTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EMRTRANSPARENTBLT", aliases: &[
        CLibAliases { names: &["EMRTRANSPARENTBLT", "tagEMRTRANSPARENTBLT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENHMETAHEADER", aliases: &[
        CLibAliases { names: &["ENHMETAHEADER", "tagENHMETAHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENHMETARECORD", aliases: &[
        CLibAliases { names: &["ENHMETARECORD", "tagENHMETARECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENHMFENUMPROC", aliases: &[
        CLibAliases { names: &["ENHMFENUMPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENUMLOGFONTA", aliases: &[
        CLibAliases { names: &["ENUMLOGFONTA", "tagENUMLOGFONTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENUMLOGFONTEXA", aliases: &[
        CLibAliases { names: &["ENUMLOGFONTEXA", "tagENUMLOGFONTEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENUMLOGFONTEXDVA", aliases: &[
        CLibAliases { names: &["ENUMLOGFONTEXDVA", "tagENUMLOGFONTEXDVA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENUMLOGFONTEXDVW", aliases: &[
        CLibAliases { names: &["ENUMLOGFONTEXDVW", "tagENUMLOGFONTEXDVW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENUMLOGFONTEXW", aliases: &[
        CLibAliases { names: &["ENUMLOGFONTEXW", "tagENUMLOGFONTEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::ENUMLOGFONTW", aliases: &[
        CLibAliases { names: &["ENUMLOGFONTW", "tagENUMLOGFONTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EXTLOGFONTA", aliases: &[
        CLibAliases { names: &["EXTLOGFONTA", "tagEXTLOGFONTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EXTLOGFONTW", aliases: &[
        CLibAliases { names: &["EXTLOGFONTW", "tagEXTLOGFONTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EXTLOGPEN", aliases: &[
        CLibAliases { names: &["EXTLOGPEN", "tagEXTLOGPEN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::EXTLOGPEN32", aliases: &[
        CLibAliases { names: &["EXTLOGPEN32", "tagEXTLOGPEN32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::FIXED", aliases: &[
        CLibAliases { names: &["FIXED", "_FIXED"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::FONTENUMPROCA", aliases: &[
        CLibAliases { names: &["FONTENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::FONTENUMPROCW", aliases: &[
        CLibAliases { names: &["FONTENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GCP_RESULTSA", aliases: &[
        CLibAliases { names: &["GCP_RESULTSA", "tagGCP_RESULTSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GCP_RESULTSW", aliases: &[
        CLibAliases { names: &["GCP_RESULTSW", "tagGCP_RESULTSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GLYPHMETRICS", aliases: &[
        CLibAliases { names: &["GLYPHMETRICS", "_GLYPHMETRICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GLYPHSET", aliases: &[
        CLibAliases { names: &["GLYPHSET", "tagGLYPHSET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GOBJENUMPROC", aliases: &[
        CLibAliases { names: &["GOBJENUMPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GRADIENT_RECT", aliases: &[
        CLibAliases { names: &["GRADIENT_RECT", "_GRADIENT_RECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GRADIENT_TRIANGLE", aliases: &[
        CLibAliases { names: &["GRADIENT_TRIANGLE", "_GRADIENT_TRIANGLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::GRAYSTRINGPROC", aliases: &[
        CLibAliases { names: &["GRAYSTRINGPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HANDLETABLE", aliases: &[
        CLibAliases { names: &["HANDLETABLE", "tagHANDLETABLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HBITMAP", aliases: &[
        CLibAliases { names: &["HBITMAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HBRUSH", aliases: &[
        CLibAliases { names: &["HBRUSH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HDC", aliases: &[
        CLibAliases { names: &["HDC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HENHMETAFILE", aliases: &[
        CLibAliases { names: &["HENHMETAFILE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HFONT", aliases: &[
        CLibAliases { names: &["HFONT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HGDIOBJ", aliases: &[
        CLibAliases { names: &["HGDIOBJ"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HMETAFILE", aliases: &[
        CLibAliases { names: &["HMETAFILE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HMONITOR", aliases: &[
        CLibAliases { names: &["HMONITOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HPALETTE", aliases: &[
        CLibAliases { names: &["HPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HPEN", aliases: &[
        CLibAliases { names: &["HPEN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::HRGN", aliases: &[
        CLibAliases { names: &["HRGN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::KERNINGPAIR", aliases: &[
        CLibAliases { names: &["KERNINGPAIR", "tagKERNINGPAIR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LINEDDAPROC", aliases: &[
        CLibAliases { names: &["LINEDDAPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LOGBRUSH", aliases: &[
        CLibAliases { names: &["LOGBRUSH", "tagLOGBRUSH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LOGBRUSH32", aliases: &[
        CLibAliases { names: &["LOGBRUSH32", "tagLOGBRUSH32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LOGFONTA", aliases: &[
        CLibAliases { names: &["LOGFONTA", "tagLOGFONTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LOGFONTW", aliases: &[
        CLibAliases { names: &["LOGFONTW", "tagLOGFONTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LOGPALETTE", aliases: &[
        CLibAliases { names: &["LOGPALETTE", "tagLOGPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LOGPEN", aliases: &[
        CLibAliases { names: &["LOGPEN", "tagLOGPEN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LPFNDEVCAPS", aliases: &[
        CLibAliases { names: &["LPFNDEVCAPS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::LPFNDEVMODE", aliases: &[
        CLibAliases { names: &["LPFNDEVMODE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::MAT2", aliases: &[
        CLibAliases { names: &["MAT2", "_MAT2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::METAHEADER", aliases: &[
        CLibAliases { names: &["METAHEADER", "tagMETAHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::METARECORD", aliases: &[
        CLibAliases { names: &["METARECORD", "tagMETARECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::MFENUMPROC", aliases: &[
        CLibAliases { names: &["MFENUMPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::MONITORENUMPROC", aliases: &[
        CLibAliases { names: &["MONITORENUMPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::MONITORINFO", aliases: &[
        CLibAliases { names: &["MONITORINFO", "tagMONITORINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::MONITORINFOEXA", aliases: &[
        CLibAliases { names: &["MONITORINFOEXA", "tagMONITORINFOEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::MONITORINFOEXW", aliases: &[
        CLibAliases { names: &["MONITORINFOEXW", "tagMONITORINFOEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::NEWTEXTMETRICA", aliases: &[
        CLibAliases { names: &["NEWTEXTMETRICA", "tagNEWTEXTMETRICA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::NEWTEXTMETRICW", aliases: &[
        CLibAliases { names: &["NEWTEXTMETRICW", "tagNEWTEXTMETRICW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::OUTLINETEXTMETRICA", aliases: &[
        CLibAliases { names: &["OUTLINETEXTMETRICA", "_OUTLINETEXTMETRICA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::OUTLINETEXTMETRICW", aliases: &[
        CLibAliases { names: &["OUTLINETEXTMETRICW", "_OUTLINETEXTMETRICW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::PAINTSTRUCT", aliases: &[
        CLibAliases { names: &["PAINTSTRUCT", "tagPAINTSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::PALETTEENTRY", aliases: &[
        CLibAliases { names: &["PALETTEENTRY", "tagPALETTEENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::PANOSE", aliases: &[
        CLibAliases { names: &["PANOSE", "tagPANOSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::PELARRAY", aliases: &[
        CLibAliases { names: &["PELARRAY", "tagPELARRAY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::POINTFX", aliases: &[
        CLibAliases { names: &["POINTFX", "tagPOINTFX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::POLYTEXTA", aliases: &[
        CLibAliases { names: &["POLYTEXTA", "tagPOLYTEXTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::POLYTEXTW", aliases: &[
        CLibAliases { names: &["POLYTEXTW", "tagPOLYTEXTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::RASTERIZER_STATUS", aliases: &[
        CLibAliases { names: &["RASTERIZER_STATUS", "_RASTERIZER_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::RGBQUAD", aliases: &[
        CLibAliases { names: &["RGBQUAD", "tagRGBQUAD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::RGBTRIPLE", aliases: &[
        CLibAliases { names: &["RGBTRIPLE", "tagRGBTRIPLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::RGNDATA", aliases: &[
        CLibAliases { names: &["RGNDATA", "_RGNDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::RGNDATAHEADER", aliases: &[
        CLibAliases { names: &["RGNDATAHEADER", "_RGNDATAHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::TEXTMETRICA", aliases: &[
        CLibAliases { names: &["TEXTMETRICA", "tagTEXTMETRICA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::TEXTMETRICW", aliases: &[
        CLibAliases { names: &["TEXTMETRICW", "tagTEXTMETRICW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::TRIVERTEX", aliases: &[
        CLibAliases { names: &["TRIVERTEX", "_TRIVERTEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::TTPOLYCURVE", aliases: &[
        CLibAliases { names: &["TTPOLYCURVE", "tagTTPOLYCURVE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::TTPOLYGONHEADER", aliases: &[
        CLibAliases { names: &["TTPOLYGONHEADER", "tagTTPOLYGONHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::WCRANGE", aliases: &[
        CLibAliases { names: &["WCRANGE", "tagWCRANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::WGLSWAP", aliases: &[
        CLibAliases { names: &["WGLSWAP", "_WGLSWAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Gdi::XFORM", aliases: &[
        CLibAliases { names: &["XFORM", "tagXFORM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::OpenGL::EMRPIXELFORMAT", aliases: &[
        CLibAliases { names: &["EMRPIXELFORMAT", "tagEMRPIXELFORMAT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::OpenGL::GLYPHMETRICSFLOAT", aliases: &[
        CLibAliases { names: &["GLYPHMETRICSFLOAT", "_GLYPHMETRICSFLOAT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::OpenGL::HGLRC", aliases: &[
        CLibAliases { names: &["HGLRC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::OpenGL::LAYERPLANEDESCRIPTOR", aliases: &[
        CLibAliases { names: &["LAYERPLANEDESCRIPTOR", "tagLAYERPLANEDESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::OpenGL::PIXELFORMATDESCRIPTOR", aliases: &[
        CLibAliases { names: &["PIXELFORMATDESCRIPTOR", "tagPIXELFORMATDESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::OpenGL::POINTFLOAT", aliases: &[
        CLibAliases { names: &["POINTFLOAT", "_POINTFLOAT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::ADDJOB_INFO_1A", aliases: &[
        CLibAliases { names: &["ADDJOB_INFO_1A", "_ADDJOB_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::ADDJOB_INFO_1W", aliases: &[
        CLibAliases { names: &["ADDJOB_INFO_1W", "_ADDJOB_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::BIDI_DATA", aliases: &[
        CLibAliases { names: &["BIDI_DATA", "_BIDI_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::BIDI_REQUEST_CONTAINER", aliases: &[
        CLibAliases { names: &["BIDI_REQUEST_CONTAINER", "_BIDI_REQUEST_CONTAINER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::BIDI_REQUEST_DATA", aliases: &[
        CLibAliases { names: &["BIDI_REQUEST_DATA", "_BIDI_REQUEST_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::BIDI_RESPONSE_CONTAINER", aliases: &[
        CLibAliases { names: &["BIDI_RESPONSE_CONTAINER", "_BIDI_RESPONSE_CONTAINER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::BIDI_RESPONSE_DATA", aliases: &[
        CLibAliases { names: &["BIDI_RESPONSE_DATA", "_BIDI_RESPONSE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::BINARY_CONTAINER", aliases: &[
        CLibAliases { names: &["BINARY_CONTAINER", "_BINARY_CONTAINER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::CORE_PRINTER_DRIVERA", aliases: &[
        CLibAliases { names: &["CORE_PRINTER_DRIVERA", "_CORE_PRINTER_DRIVERA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::CORE_PRINTER_DRIVERW", aliases: &[
        CLibAliases { names: &["CORE_PRINTER_DRIVERW", "_CORE_PRINTER_DRIVERW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DATATYPES_INFO_1A", aliases: &[
        CLibAliases { names: &["DATATYPES_INFO_1A", "_DATATYPES_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DATATYPES_INFO_1W", aliases: &[
        CLibAliases { names: &["DATATYPES_INFO_1W", "_DATATYPES_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DOC_INFO_1A", aliases: &[
        CLibAliases { names: &["DOC_INFO_1A", "_DOC_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DOC_INFO_1W", aliases: &[
        CLibAliases { names: &["DOC_INFO_1W", "_DOC_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DOC_INFO_2A", aliases: &[
        CLibAliases { names: &["DOC_INFO_2A", "_DOC_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DOC_INFO_2W", aliases: &[
        CLibAliases { names: &["DOC_INFO_2W", "_DOC_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DOC_INFO_3A", aliases: &[
        CLibAliases { names: &["DOC_INFO_3A", "_DOC_INFO_3A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DOC_INFO_3W", aliases: &[
        CLibAliases { names: &["DOC_INFO_3W", "_DOC_INFO_3W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_1A", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_1A", "_DRIVER_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_1W", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_1W", "_DRIVER_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_2A", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_2A", "_DRIVER_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_2W", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_2W", "_DRIVER_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_3A", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_3A", "_DRIVER_INFO_3A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_3W", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_3W", "_DRIVER_INFO_3W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_4A", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_4A", "_DRIVER_INFO_4A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_4W", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_4W", "_DRIVER_INFO_4W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_5A", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_5A", "_DRIVER_INFO_5A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_5W", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_5W", "_DRIVER_INFO_5W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_6A", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_6A", "_DRIVER_INFO_6A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_6W", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_6W", "_DRIVER_INFO_6W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_8A", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_8A", "_DRIVER_INFO_8A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::DRIVER_INFO_8W", aliases: &[
        CLibAliases { names: &["DRIVER_INFO_8W", "_DRIVER_INFO_8W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::FORM_INFO_1A", aliases: &[
        CLibAliases { names: &["FORM_INFO_1A", "_FORM_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::FORM_INFO_1W", aliases: &[
        CLibAliases { names: &["FORM_INFO_1W", "_FORM_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::FORM_INFO_2A", aliases: &[
        CLibAliases { names: &["FORM_INFO_2A", "_FORM_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::FORM_INFO_2W", aliases: &[
        CLibAliases { names: &["FORM_INFO_2W", "_FORM_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::JOB_INFO_1A", aliases: &[
        CLibAliases { names: &["JOB_INFO_1A", "_JOB_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::JOB_INFO_1W", aliases: &[
        CLibAliases { names: &["JOB_INFO_1W", "_JOB_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::JOB_INFO_2A", aliases: &[
        CLibAliases { names: &["JOB_INFO_2A", "_JOB_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::JOB_INFO_2W", aliases: &[
        CLibAliases { names: &["JOB_INFO_2W", "_JOB_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::JOB_INFO_3", aliases: &[
        CLibAliases { names: &["JOB_INFO_3", "_JOB_INFO_3"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::JOB_INFO_4A", aliases: &[
        CLibAliases { names: &["JOB_INFO_4A", "_JOB_INFO_4A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::JOB_INFO_4W", aliases: &[
        CLibAliases { names: &["JOB_INFO_4W", "_JOB_INFO_4W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::MONITOR_INFO_1A", aliases: &[
        CLibAliases { names: &["MONITOR_INFO_1A", "_MONITOR_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::MONITOR_INFO_1W", aliases: &[
        CLibAliases { names: &["MONITOR_INFO_1W", "_MONITOR_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::MONITOR_INFO_2A", aliases: &[
        CLibAliases { names: &["MONITOR_INFO_2A", "_MONITOR_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::MONITOR_INFO_2W", aliases: &[
        CLibAliases { names: &["MONITOR_INFO_2W", "_MONITOR_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PORT_INFO_1A", aliases: &[
        CLibAliases { names: &["PORT_INFO_1A", "_PORT_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PORT_INFO_1W", aliases: &[
        CLibAliases { names: &["PORT_INFO_1W", "_PORT_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PORT_INFO_2A", aliases: &[
        CLibAliases { names: &["PORT_INFO_2A", "_PORT_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PORT_INFO_2W", aliases: &[
        CLibAliases { names: &["PORT_INFO_2W", "_PORT_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PORT_INFO_3A", aliases: &[
        CLibAliases { names: &["PORT_INFO_3A", "_PORT_INFO_3A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PORT_INFO_3W", aliases: &[
        CLibAliases { names: &["PORT_INFO_3W", "_PORT_INFO_3W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_CONNECTION_INFO_1A", aliases: &[
        CLibAliases { names: &["PRINTER_CONNECTION_INFO_1A", "_PRINTER_CONNECTION_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_CONNECTION_INFO_1W", aliases: &[
        CLibAliases { names: &["PRINTER_CONNECTION_INFO_1W", "_PRINTER_CONNECTION_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_DEFAULTSA", aliases: &[
        CLibAliases { names: &["PRINTER_DEFAULTSA", "_PRINTER_DEFAULTSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_DEFAULTSW", aliases: &[
        CLibAliases { names: &["PRINTER_DEFAULTSW", "_PRINTER_DEFAULTSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_ENUM_VALUESA", aliases: &[
        CLibAliases { names: &["PRINTER_ENUM_VALUESA", "_PRINTER_ENUM_VALUESA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_ENUM_VALUESW", aliases: &[
        CLibAliases { names: &["PRINTER_ENUM_VALUESW", "_PRINTER_ENUM_VALUESW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_1A", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_1A", "_PRINTER_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_1W", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_1W", "_PRINTER_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_2A", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_2A", "_PRINTER_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_2W", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_2W", "_PRINTER_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_3", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_3", "_PRINTER_INFO_3"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_4A", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_4A", "_PRINTER_INFO_4A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_4W", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_4W", "_PRINTER_INFO_4W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_5A", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_5A", "_PRINTER_INFO_5A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_5W", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_5W", "_PRINTER_INFO_5W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_6", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_6", "_PRINTER_INFO_6"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_7A", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_7A", "_PRINTER_INFO_7A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_7W", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_7W", "_PRINTER_INFO_7W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_8A", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_8A", "_PRINTER_INFO_8A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_8W", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_8W", "_PRINTER_INFO_8W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_9A", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_9A", "_PRINTER_INFO_9A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_INFO_9W", aliases: &[
        CLibAliases { names: &["PRINTER_INFO_9W", "_PRINTER_INFO_9W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_NOTIFY_INFO", aliases: &[
        CLibAliases { names: &["PRINTER_NOTIFY_INFO", "_PRINTER_NOTIFY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_NOTIFY_INFO_DATA", aliases: &[
        CLibAliases { names: &["PRINTER_NOTIFY_INFO_DATA", "_PRINTER_NOTIFY_INFO_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_NOTIFY_OPTIONS", aliases: &[
        CLibAliases { names: &["PRINTER_NOTIFY_OPTIONS", "_PRINTER_NOTIFY_OPTIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_NOTIFY_OPTIONS_TYPE", aliases: &[
        CLibAliases { names: &["PRINTER_NOTIFY_OPTIONS_TYPE", "_PRINTER_NOTIFY_OPTIONS_TYPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_OPTIONSA", aliases: &[
        CLibAliases { names: &["PRINTER_OPTIONSA", "_PRINTER_OPTIONSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTER_OPTIONSW", aliases: &[
        CLibAliases { names: &["PRINTER_OPTIONSW", "_PRINTER_OPTIONSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTPROCESSOR_CAPS_1", aliases: &[
        CLibAliases { names: &["PRINTPROCESSOR_CAPS_1", "_PRINTPROCESSOR_CAPS_1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTPROCESSOR_CAPS_2", aliases: &[
        CLibAliases { names: &["PRINTPROCESSOR_CAPS_2", "_PRINTPROCESSOR_CAPS_2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTPROCESSOR_INFO_1A", aliases: &[
        CLibAliases { names: &["PRINTPROCESSOR_INFO_1A", "_PRINTPROCESSOR_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINTPROCESSOR_INFO_1W", aliases: &[
        CLibAliases { names: &["PRINTPROCESSOR_INFO_1W", "_PRINTPROCESSOR_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PRINT_EXECUTION_DATA", aliases: &[
        CLibAliases { names: &["PRINT_EXECUTION_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PROVIDOR_INFO_1A", aliases: &[
        CLibAliases { names: &["PROVIDOR_INFO_1A", "_PROVIDOR_INFO_1A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PROVIDOR_INFO_1W", aliases: &[
        CLibAliases { names: &["PROVIDOR_INFO_1W", "_PROVIDOR_INFO_1W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PROVIDOR_INFO_2A", aliases: &[
        CLibAliases { names: &["PROVIDOR_INFO_2A", "_PROVIDOR_INFO_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PROVIDOR_INFO_2W", aliases: &[
        CLibAliases { names: &["PROVIDOR_INFO_2W", "_PROVIDOR_INFO_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PrintNamedProperty", aliases: &[
        CLibAliases { names: &["PrintNamedProperty"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PrintPropertiesCollection", aliases: &[
        CLibAliases { names: &["PrintPropertiesCollection"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Graphics::Printing::PrintPropertyValue", aliases: &[
        CLibAliases { names: &["PrintPropertyValue"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::AUXCAPS2A", aliases: &[
        CLibAliases { names: &["AUXCAPS2A", "tagAUXCAPS2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::AUXCAPS2W", aliases: &[
        CLibAliases { names: &["AUXCAPS2W", "tagAUXCAPS2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::AUXCAPSA", aliases: &[
        CLibAliases { names: &["AUXCAPSA", "tagAUXCAPSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::AUXCAPSW", aliases: &[
        CLibAliases { names: &["AUXCAPSW", "tagAUXCAPSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HMIDI", aliases: &[
        CLibAliases { names: &["HMIDI"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HMIDIIN", aliases: &[
        CLibAliases { names: &["HMIDIIN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HMIDIOUT", aliases: &[
        CLibAliases { names: &["HMIDIOUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HMIDISTRM", aliases: &[
        CLibAliases { names: &["HMIDISTRM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HMIXER", aliases: &[
        CLibAliases { names: &["HMIXER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HMIXEROBJ", aliases: &[
        CLibAliases { names: &["HMIXEROBJ"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HWAVE", aliases: &[
        CLibAliases { names: &["HWAVE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HWAVEIN", aliases: &[
        CLibAliases { names: &["HWAVEIN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::HWAVEOUT", aliases: &[
        CLibAliases { names: &["HWAVEOUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::LPMIDICALLBACK", aliases: &[
        CLibAliases { names: &["LPMIDICALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::LPWAVECALLBACK", aliases: &[
        CLibAliases { names: &["LPWAVECALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIEVENT", aliases: &[
        CLibAliases { names: &["MIDIEVENT", "midievent_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIHDR", aliases: &[
        CLibAliases { names: &["MIDIHDR", "midihdr_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIINCAPS2A", aliases: &[
        CLibAliases { names: &["MIDIINCAPS2A", "tagMIDIINCAPS2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIINCAPS2W", aliases: &[
        CLibAliases { names: &["MIDIINCAPS2W", "tagMIDIINCAPS2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIINCAPSA", aliases: &[
        CLibAliases { names: &["MIDIINCAPSA", "tagMIDIINCAPSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIINCAPSW", aliases: &[
        CLibAliases { names: &["MIDIINCAPSW", "tagMIDIINCAPSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIOUTCAPS2A", aliases: &[
        CLibAliases { names: &["MIDIOUTCAPS2A", "tagMIDIOUTCAPS2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIOUTCAPS2W", aliases: &[
        CLibAliases { names: &["MIDIOUTCAPS2W", "tagMIDIOUTCAPS2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIOUTCAPSA", aliases: &[
        CLibAliases { names: &["MIDIOUTCAPSA", "tagMIDIOUTCAPSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIOUTCAPSW", aliases: &[
        CLibAliases { names: &["MIDIOUTCAPSW", "tagMIDIOUTCAPSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIPROPTEMPO", aliases: &[
        CLibAliases { names: &["MIDIPROPTEMPO", "midiproptempo_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDIPROPTIMEDIV", aliases: &[
        CLibAliases { names: &["MIDIPROPTIMEDIV", "midiproptimediv_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIDISTRMBUFFVER", aliases: &[
        CLibAliases { names: &["MIDISTRMBUFFVER", "midistrmbuffver_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCAPS2A", aliases: &[
        CLibAliases { names: &["MIXERCAPS2A", "tagMIXERCAPS2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCAPS2W", aliases: &[
        CLibAliases { names: &["MIXERCAPS2W", "tagMIXERCAPS2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCAPSA", aliases: &[
        CLibAliases { names: &["MIXERCAPSA", "tagMIXERCAPSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCAPSW", aliases: &[
        CLibAliases { names: &["MIXERCAPSW", "tagMIXERCAPSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLA", aliases: &[
        CLibAliases { names: &["MIXERCONTROLA", "tagMIXERCONTROLA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLDETAILS", aliases: &[
        CLibAliases { names: &["MIXERCONTROLDETAILS", "tMIXERCONTROLDETAILS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLDETAILS_BOOLEAN", aliases: &[
        CLibAliases { names: &["MIXERCONTROLDETAILS_BOOLEAN", "tMIXERCONTROLDETAILS_BOOLEAN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLDETAILS_LISTTEXTA", aliases: &[
        CLibAliases { names: &["MIXERCONTROLDETAILS_LISTTEXTA", "tagMIXERCONTROLDETAILS_LISTTEXTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLDETAILS_LISTTEXTW", aliases: &[
        CLibAliases { names: &["MIXERCONTROLDETAILS_LISTTEXTW", "tagMIXERCONTROLDETAILS_LISTTEXTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLDETAILS_SIGNED", aliases: &[
        CLibAliases { names: &["MIXERCONTROLDETAILS_SIGNED", "tMIXERCONTROLDETAILS_SIGNED"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLDETAILS_UNSIGNED", aliases: &[
        CLibAliases { names: &["MIXERCONTROLDETAILS_UNSIGNED", "tMIXERCONTROLDETAILS_UNSIGNED"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERCONTROLW", aliases: &[
        CLibAliases { names: &["MIXERCONTROLW", "tagMIXERCONTROLW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERLINEA", aliases: &[
        CLibAliases { names: &["MIXERLINEA", "tagMIXERLINEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERLINECONTROLSA", aliases: &[
        CLibAliases { names: &["MIXERLINECONTROLSA", "tagMIXERLINECONTROLSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERLINECONTROLSW", aliases: &[
        CLibAliases { names: &["MIXERLINECONTROLSW", "tagMIXERLINECONTROLSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::MIXERLINEW", aliases: &[
        CLibAliases { names: &["MIXERLINEW", "tagMIXERLINEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::PCMWAVEFORMAT", aliases: &[
        CLibAliases { names: &["PCMWAVEFORMAT", "pcmwaveformat_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEFORMAT", aliases: &[
        CLibAliases { names: &["WAVEFORMAT", "waveformat_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEFORMATEX", aliases: &[
        CLibAliases { names: &["WAVEFORMATEX", "tWAVEFORMATEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEHDR", aliases: &[
        CLibAliases { names: &["WAVEHDR", "wavehdr_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEINCAPS2A", aliases: &[
        CLibAliases { names: &["WAVEINCAPS2A", "tagWAVEINCAPS2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEINCAPS2W", aliases: &[
        CLibAliases { names: &["WAVEINCAPS2W", "tagWAVEINCAPS2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEINCAPSA", aliases: &[
        CLibAliases { names: &["WAVEINCAPSA", "tagWAVEINCAPSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEINCAPSW", aliases: &[
        CLibAliases { names: &["WAVEINCAPSW", "tagWAVEINCAPSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEOUTCAPS2A", aliases: &[
        CLibAliases { names: &["WAVEOUTCAPS2A", "tagWAVEOUTCAPS2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEOUTCAPS2W", aliases: &[
        CLibAliases { names: &["WAVEOUTCAPS2W", "tagWAVEOUTCAPS2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEOUTCAPSA", aliases: &[
        CLibAliases { names: &["WAVEOUTCAPSA", "tagWAVEOUTCAPSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Audio::WAVEOUTCAPSW", aliases: &[
        CLibAliases { names: &["WAVEOUTCAPSW", "tagWAVEOUTCAPSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::HTASK", aliases: &[
        CLibAliases { names: &["HTASK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::LPDRVCALLBACK", aliases: &[
        CLibAliases { names: &["LPDRVCALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::LPTIMECALLBACK", aliases: &[
        CLibAliases { names: &["LPTIMECALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::MMTIME", aliases: &[
        CLibAliases { names: &["MMTIME", "mmtime_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::DRIVERMSGPROC", aliases: &[
        CLibAliases { names: &["DRIVERMSGPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::DRIVERPROC", aliases: &[
        CLibAliases { names: &["DRIVERPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::DRVCONFIGINFO", aliases: &[
        CLibAliases { names: &["DRVCONFIGINFO", "tagDRVCONFIGINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::DRVCONFIGINFOEX", aliases: &[
        CLibAliases { names: &["DRVCONFIGINFOEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::HDRVR", aliases: &[
        CLibAliases { names: &["HDRVR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::HMMIO", aliases: &[
        CLibAliases { names: &["HMMIO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::JOYCAPS2A", aliases: &[
        CLibAliases { names: &["JOYCAPS2A", "tagJOYCAPS2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::JOYCAPS2W", aliases: &[
        CLibAliases { names: &["JOYCAPS2W", "tagJOYCAPS2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::JOYCAPSA", aliases: &[
        CLibAliases { names: &["JOYCAPSA", "tagJOYCAPSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::JOYCAPSW", aliases: &[
        CLibAliases { names: &["JOYCAPSW", "tagJOYCAPSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::JOYINFO", aliases: &[
        CLibAliases { names: &["JOYINFO", "joyinfo_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::JOYINFOEX", aliases: &[
        CLibAliases { names: &["JOYINFOEX", "joyinfoex_tag"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::LPMMIOPROC", aliases: &[
        CLibAliases { names: &["LPMMIOPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_OPEN_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_ANIM_OPEN_PARMSA", "tagMCI_ANIM_OPEN_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_OPEN_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_ANIM_OPEN_PARMSW", "tagMCI_ANIM_OPEN_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_PLAY_PARMS", aliases: &[
        CLibAliases { names: &["MCI_ANIM_PLAY_PARMS", "tagMCI_ANIM_PLAY_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_RECT_PARMS", aliases: &[
        CLibAliases { names: &["MCI_ANIM_RECT_PARMS", "tagMCI_ANIM_RECT_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_STEP_PARMS", aliases: &[
        CLibAliases { names: &["MCI_ANIM_STEP_PARMS", "tagMCI_ANIM_STEP_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_UPDATE_PARMS", aliases: &[
        CLibAliases { names: &["MCI_ANIM_UPDATE_PARMS", "tagMCI_ANIM_UPDATE_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_WINDOW_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_ANIM_WINDOW_PARMSA", "tagMCI_ANIM_WINDOW_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_ANIM_WINDOW_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_ANIM_WINDOW_PARMSW", "tagMCI_ANIM_WINDOW_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_BREAK_PARMS", aliases: &[
        CLibAliases { names: &["MCI_BREAK_PARMS", "tagMCI_BREAK_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_GENERIC_PARMS", aliases: &[
        CLibAliases { names: &["MCI_GENERIC_PARMS", "tagMCI_GENERIC_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_GETDEVCAPS_PARMS", aliases: &[
        CLibAliases { names: &["MCI_GETDEVCAPS_PARMS", "tagMCI_GETDEVCAPS_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_INFO_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_INFO_PARMSA", "tagMCI_INFO_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_INFO_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_INFO_PARMSW", "tagMCI_INFO_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_LOAD_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_LOAD_PARMSA", "tagMCI_LOAD_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_LOAD_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_LOAD_PARMSW", "tagMCI_LOAD_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OPEN_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_OPEN_PARMSA", "tagMCI_OPEN_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OPEN_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_OPEN_PARMSW", "tagMCI_OPEN_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_LOAD_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_OVLY_LOAD_PARMSA", "tagMCI_OVLY_LOAD_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_LOAD_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_OVLY_LOAD_PARMSW", "tagMCI_OVLY_LOAD_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_OPEN_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_OVLY_OPEN_PARMSA", "tagMCI_OVLY_OPEN_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_OPEN_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_OVLY_OPEN_PARMSW", "tagMCI_OVLY_OPEN_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_RECT_PARMS", aliases: &[
        CLibAliases { names: &["MCI_OVLY_RECT_PARMS", "tagMCI_OVLY_RECT_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_SAVE_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_OVLY_SAVE_PARMSA", "tagMCI_OVLY_SAVE_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_SAVE_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_OVLY_SAVE_PARMSW", "tagMCI_OVLY_SAVE_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_WINDOW_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_OVLY_WINDOW_PARMSA", "tagMCI_OVLY_WINDOW_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_OVLY_WINDOW_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_OVLY_WINDOW_PARMSW", "tagMCI_OVLY_WINDOW_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_PLAY_PARMS", aliases: &[
        CLibAliases { names: &["MCI_PLAY_PARMS", "tagMCI_PLAY_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_RECORD_PARMS", aliases: &[
        CLibAliases { names: &["MCI_RECORD_PARMS", "tagMCI_RECORD_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_SAVE_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_SAVE_PARMSA", "tagMCI_SAVE_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_SAVE_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_SAVE_PARMSW", "tagMCI_SAVE_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_SEEK_PARMS", aliases: &[
        CLibAliases { names: &["MCI_SEEK_PARMS", "tagMCI_SEEK_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_SEQ_SET_PARMS", aliases: &[
        CLibAliases { names: &["MCI_SEQ_SET_PARMS", "tagMCI_SEQ_SET_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_SET_PARMS", aliases: &[
        CLibAliases { names: &["MCI_SET_PARMS", "tagMCI_SET_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_STATUS_PARMS", aliases: &[
        CLibAliases { names: &["MCI_STATUS_PARMS", "tagMCI_STATUS_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_SYSINFO_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_SYSINFO_PARMSA", "tagMCI_SYSINFO_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_SYSINFO_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_SYSINFO_PARMSW", "tagMCI_SYSINFO_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_VD_ESCAPE_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_VD_ESCAPE_PARMSA", "tagMCI_VD_ESCAPE_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_VD_ESCAPE_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_VD_ESCAPE_PARMSW", "tagMCI_VD_ESCAPE_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_VD_PLAY_PARMS", aliases: &[
        CLibAliases { names: &["MCI_VD_PLAY_PARMS", "tagMCI_VD_PLAY_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_VD_STEP_PARMS", aliases: &[
        CLibAliases { names: &["MCI_VD_STEP_PARMS", "tagMCI_VD_STEP_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_WAVE_DELETE_PARMS", aliases: &[
        CLibAliases { names: &["MCI_WAVE_DELETE_PARMS", "tagMCI_WAVE_DELETE_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_WAVE_OPEN_PARMSA", aliases: &[
        CLibAliases { names: &["MCI_WAVE_OPEN_PARMSA", "tagMCI_WAVE_OPEN_PARMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_WAVE_OPEN_PARMSW", aliases: &[
        CLibAliases { names: &["MCI_WAVE_OPEN_PARMSW", "tagMCI_WAVE_OPEN_PARMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MCI_WAVE_SET_PARMS", aliases: &[
        CLibAliases { names: &["MCI_WAVE_SET_PARMS", "tagMCI_WAVE_SET_PARMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MMCKINFO", aliases: &[
        CLibAliases { names: &["MMCKINFO", "_MMCKINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::MMIOINFO", aliases: &[
        CLibAliases { names: &["MMIOINFO", "_MMIOINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Media::Multimedia::YIELDPROC", aliases: &[
        CLibAliases { names: &["YIELDPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::ACTION_HEADER", aliases: &[
        CLibAliases { names: &["ACTION_HEADER", "_ACTION_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::ADAPTER_STATUS", aliases: &[
        CLibAliases { names: &["ADAPTER_STATUS", "_ADAPTER_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::FIND_NAME_BUFFER", aliases: &[
        CLibAliases { names: &["FIND_NAME_BUFFER", "_FIND_NAME_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::FIND_NAME_HEADER", aliases: &[
        CLibAliases { names: &["FIND_NAME_HEADER", "_FIND_NAME_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::LANA_ENUM", aliases: &[
        CLibAliases { names: &["LANA_ENUM", "_LANA_ENUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::NAME_BUFFER", aliases: &[
        CLibAliases { names: &["NAME_BUFFER", "_NAME_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::NCB", aliases: &[
        CLibAliases { names: &["NCB", "_NCB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::SESSION_BUFFER", aliases: &[
        CLibAliases { names: &["SESSION_BUFFER", "_SESSION_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::NetBios::SESSION_HEADER", aliases: &[
        CLibAliases { names: &["SESSION_HEADER", "_SESSION_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::CONNECTDLGSTRUCTA", aliases: &[
        CLibAliases { names: &["CONNECTDLGSTRUCTA", "_CONNECTDLGSTRUCTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::CONNECTDLGSTRUCTW", aliases: &[
        CLibAliases { names: &["CONNECTDLGSTRUCTW", "_CONNECTDLGSTRUCTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::DISCDLGSTRUCTA", aliases: &[
        CLibAliases { names: &["DISCDLGSTRUCTA", "_DISCDLGSTRUCTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::DISCDLGSTRUCTW", aliases: &[
        CLibAliases { names: &["DISCDLGSTRUCTW", "_DISCDLGSTRUCTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::NETCONNECTINFOSTRUCT", aliases: &[
        CLibAliases { names: &["NETCONNECTINFOSTRUCT", "_NETCONNECTINFOSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::NETINFOSTRUCT", aliases: &[
        CLibAliases { names: &["NETINFOSTRUCT", "_NETINFOSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::NETRESOURCEA", aliases: &[
        CLibAliases { names: &["NETRESOURCEA", "_NETRESOURCEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::NETRESOURCEW", aliases: &[
        CLibAliases { names: &["NETRESOURCEW", "_NETRESOURCEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::REMOTE_NAME_INFOA", aliases: &[
        CLibAliases { names: &["REMOTE_NAME_INFOA", "_REMOTE_NAME_INFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::REMOTE_NAME_INFOW", aliases: &[
        CLibAliases { names: &["REMOTE_NAME_INFOW", "_REMOTE_NAME_INFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::UNIVERSAL_NAME_INFOA", aliases: &[
        CLibAliases { names: &["UNIVERSAL_NAME_INFOA", "_UNIVERSAL_NAME_INFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::NetworkManagement::WNet::UNIVERSAL_NAME_INFOW", aliases: &[
        CLibAliases { names: &["UNIVERSAL_NAME_INFOW", "_UNIVERSAL_NAME_INFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::FD_SET", aliases: &[
        CLibAliases { names: &["FD_SET", "fd_set"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::HOSTENT", aliases: &[
        CLibAliases { names: &["HOSTENT", "hostent"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::IN_ADDR", aliases: &[
        CLibAliases { names: &["IN_ADDR", "in_addr"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::LINGER", aliases: &[
        CLibAliases { names: &["LINGER", "linger"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::PROTOENT", aliases: &[
        CLibAliases { names: &["PROTOENT", "protoent"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::SERVENT", aliases: &[
        CLibAliases { names: &["SERVENT", "servent"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::SOCKADDR", aliases: &[
        CLibAliases { names: &["SOCKADDR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::SOCKADDR_IN", aliases: &[
        CLibAliases { names: &["SOCKADDR_IN", "sockaddr_in"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::TIMEVAL", aliases: &[
        CLibAliases { names: &["TIMEVAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::TRANSMIT_FILE_BUFFERS", aliases: &[
        CLibAliases { names: &["TRANSMIT_FILE_BUFFERS", "_TRANSMIT_FILE_BUFFERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::WSADATA", aliases: &[
        CLibAliases { names: &["WSADATA", "WSAData"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::netent", aliases: &[
        CLibAliases { names: &["netent"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Networking::WinSock::sockproto", aliases: &[
        CLibAliases { names: &["sockproto"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_ALLOWED_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_ALLOWED_ACE", "_ACCESS_ALLOWED_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_ALLOWED_CALLBACK_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_ALLOWED_CALLBACK_ACE", "_ACCESS_ALLOWED_CALLBACK_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_ALLOWED_CALLBACK_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_ALLOWED_CALLBACK_OBJECT_ACE", "_ACCESS_ALLOWED_CALLBACK_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_ALLOWED_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_ALLOWED_OBJECT_ACE", "_ACCESS_ALLOWED_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_DENIED_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_DENIED_ACE", "_ACCESS_DENIED_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_DENIED_CALLBACK_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_DENIED_CALLBACK_ACE", "_ACCESS_DENIED_CALLBACK_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_DENIED_CALLBACK_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_DENIED_CALLBACK_OBJECT_ACE", "_ACCESS_DENIED_CALLBACK_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_DENIED_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["ACCESS_DENIED_OBJECT_ACE", "_ACCESS_DENIED_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACCESS_REASONS", aliases: &[
        CLibAliases { names: &["ACCESS_REASONS", "_ACCESS_REASONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACE_HEADER", aliases: &[
        CLibAliases { names: &["ACE_HEADER", "_ACE_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACL", aliases: &[
        CLibAliases { names: &["ACL", "_ACL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACL_REVISION_INFORMATION", aliases: &[
        CLibAliases { names: &["ACL_REVISION_INFORMATION", "_ACL_REVISION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::ACL_SIZE_INFORMATION", aliases: &[
        CLibAliases { names: &["ACL_SIZE_INFORMATION", "_ACL_SIZE_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::CLAIM_SECURITY_ATTRIBUTES_INFORMATION", aliases: &[
        CLibAliases { names: &["CLAIM_SECURITY_ATTRIBUTES_INFORMATION", "_CLAIM_SECURITY_ATTRIBUTES_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE", aliases: &[
        CLibAliases { names: &["CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE", "_CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE", aliases: &[
        CLibAliases { names: &["CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE", "_CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::CLAIM_SECURITY_ATTRIBUTE_RELATIVE_V1", aliases: &[
        CLibAliases { names: &["CLAIM_SECURITY_ATTRIBUTE_RELATIVE_V1", "_CLAIM_SECURITY_ATTRIBUTE_RELATIVE_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::CLAIM_SECURITY_ATTRIBUTE_V1", aliases: &[
        CLibAliases { names: &["CLAIM_SECURITY_ATTRIBUTE_V1", "_CLAIM_SECURITY_ATTRIBUTE_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::LPOCNCHKPROC", aliases: &[
        CLibAliases { names: &["LPOCNCHKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::LPOCNCONNPROCA", aliases: &[
        CLibAliases { names: &["LPOCNCONNPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::LPOCNCONNPROCW", aliases: &[
        CLibAliases { names: &["LPOCNCONNPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::LPOCNDSCPROC", aliases: &[
        CLibAliases { names: &["LPOCNDSCPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::OPENCARDNAMEA", aliases: &[
        CLibAliases { names: &["OPENCARDNAMEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::OPENCARDNAMEW", aliases: &[
        CLibAliases { names: &["OPENCARDNAMEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::OPENCARDNAME_EXA", aliases: &[
        CLibAliases { names: &["OPENCARDNAME_EXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::OPENCARDNAME_EXW", aliases: &[
        CLibAliases { names: &["OPENCARDNAME_EXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::OPENCARD_SEARCH_CRITERIAA", aliases: &[
        CLibAliases { names: &["OPENCARD_SEARCH_CRITERIAA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::OPENCARD_SEARCH_CRITERIAW", aliases: &[
        CLibAliases { names: &["OPENCARD_SEARCH_CRITERIAW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::READER_SEL_REQUEST", aliases: &[
        CLibAliases { names: &["READER_SEL_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::READER_SEL_RESPONSE", aliases: &[
        CLibAliases { names: &["READER_SEL_RESPONSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::SCARD_ATRMASK", aliases: &[
        CLibAliases { names: &["SCARD_ATRMASK", "_SCARD_ATRMASK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::SCARD_IO_REQUEST", aliases: &[
        CLibAliases { names: &["SCARD_IO_REQUEST", "_SCARD_IO_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::SCARD_READERSTATEA", aliases: &[
        CLibAliases { names: &["SCARD_READERSTATEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::SCARD_READERSTATEW", aliases: &[
        CLibAliases { names: &["SCARD_READERSTATEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::SCARD_T0_COMMAND", aliases: &[
        CLibAliases { names: &["SCARD_T0_COMMAND", "_SCARD_T0_COMMAND"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::SCARD_T0_REQUEST", aliases: &[
        CLibAliases { names: &["SCARD_T0_REQUEST", "_SCARD_T0_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Credentials::SCARD_T1_REQUEST", aliases: &[
        CLibAliases { names: &["SCARD_T1_REQUEST", "_SCARD_T1_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::AUTHENTICODE_EXTRA_CERT_CHAIN_POLICY_PARA", aliases: &[
        CLibAliases { names: &["AUTHENTICODE_EXTRA_CERT_CHAIN_POLICY_PARA", "_AUTHENTICODE_EXTRA_CERT_CHAIN_POLICY_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::AUTHENTICODE_EXTRA_CERT_CHAIN_POLICY_STATUS", aliases: &[
        CLibAliases { names: &["AUTHENTICODE_EXTRA_CERT_CHAIN_POLICY_STATUS", "_AUTHENTICODE_EXTRA_CERT_CHAIN_POLICY_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::AUTHENTICODE_TS_EXTRA_CERT_CHAIN_POLICY_PARA", aliases: &[
        CLibAliases { names: &["AUTHENTICODE_TS_EXTRA_CERT_CHAIN_POLICY_PARA", "_AUTHENTICODE_TS_EXTRA_CERT_CHAIN_POLICY_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_ALGORITHM_IDENTIFIER", aliases: &[
        CLibAliases { names: &["BCRYPT_ALGORITHM_IDENTIFIER", "_BCRYPT_ALGORITHM_IDENTIFIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_ALG_HANDLE", aliases: &[
        CLibAliases { names: &["BCRYPT_ALG_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO", aliases: &[
        CLibAliases { names: &["BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO", "_BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_DH_KEY_BLOB", aliases: &[
        CLibAliases { names: &["BCRYPT_DH_KEY_BLOB", "_BCRYPT_DH_KEY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_DH_PARAMETER_HEADER", aliases: &[
        CLibAliases { names: &["BCRYPT_DH_PARAMETER_HEADER", "_BCRYPT_DH_PARAMETER_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_DSA_KEY_BLOB", aliases: &[
        CLibAliases { names: &["BCRYPT_DSA_KEY_BLOB", "_BCRYPT_DSA_KEY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_DSA_KEY_BLOB_V2", aliases: &[
        CLibAliases { names: &["BCRYPT_DSA_KEY_BLOB_V2", "_BCRYPT_DSA_KEY_BLOB_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_DSA_PARAMETER_HEADER", aliases: &[
        CLibAliases { names: &["BCRYPT_DSA_PARAMETER_HEADER", "_BCRYPT_DSA_PARAMETER_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_DSA_PARAMETER_HEADER_V2", aliases: &[
        CLibAliases { names: &["BCRYPT_DSA_PARAMETER_HEADER_V2", "_BCRYPT_DSA_PARAMETER_HEADER_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_ECCFULLKEY_BLOB", aliases: &[
        CLibAliases { names: &["BCRYPT_ECCFULLKEY_BLOB", "_BCRYPT_ECCFULLKEY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_ECCKEY_BLOB", aliases: &[
        CLibAliases { names: &["BCRYPT_ECCKEY_BLOB", "_BCRYPT_ECCKEY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_ECC_CURVE_NAMES", aliases: &[
        CLibAliases { names: &["BCRYPT_ECC_CURVE_NAMES", "_BCRYPT_ECC_CURVE_NAMES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_HANDLE", aliases: &[
        CLibAliases { names: &["BCRYPT_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_HASH_HANDLE", aliases: &[
        CLibAliases { names: &["BCRYPT_HASH_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_INTERFACE_VERSION", aliases: &[
        CLibAliases { names: &["BCRYPT_INTERFACE_VERSION", "_BCRYPT_INTERFACE_VERSION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_KEY_BLOB", aliases: &[
        CLibAliases { names: &["BCRYPT_KEY_BLOB", "_BCRYPT_KEY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_KEY_DATA_BLOB_HEADER", aliases: &[
        CLibAliases { names: &["BCRYPT_KEY_DATA_BLOB_HEADER", "_BCRYPT_KEY_DATA_BLOB_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_KEY_HANDLE", aliases: &[
        CLibAliases { names: &["BCRYPT_KEY_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_KEY_LENGTHS_STRUCT", aliases: &[
        CLibAliases { names: &["BCRYPT_KEY_LENGTHS_STRUCT", "__BCRYPT_KEY_LENGTHS_STRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_MULTI_HASH_OPERATION", aliases: &[
        CLibAliases { names: &["BCRYPT_MULTI_HASH_OPERATION", "_BCRYPT_MULTI_HASH_OPERATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_MULTI_OBJECT_LENGTH_STRUCT", aliases: &[
        CLibAliases { names: &["BCRYPT_MULTI_OBJECT_LENGTH_STRUCT", "_BCRYPT_MULTI_OBJECT_LENGTH_STRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_OAEP_PADDING_INFO", aliases: &[
        CLibAliases { names: &["BCRYPT_OAEP_PADDING_INFO", "_BCRYPT_OAEP_PADDING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_OID", aliases: &[
        CLibAliases { names: &["BCRYPT_OID", "_BCRYPT_OID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_OID_LIST", aliases: &[
        CLibAliases { names: &["BCRYPT_OID_LIST", "_BCRYPT_OID_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_PKCS1_PADDING_INFO", aliases: &[
        CLibAliases { names: &["BCRYPT_PKCS1_PADDING_INFO", "_BCRYPT_PKCS1_PADDING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_PROVIDER_NAME", aliases: &[
        CLibAliases { names: &["BCRYPT_PROVIDER_NAME", "_BCRYPT_PROVIDER_NAME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_PSS_PADDING_INFO", aliases: &[
        CLibAliases { names: &["BCRYPT_PSS_PADDING_INFO", "_BCRYPT_PSS_PADDING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_RSAKEY_BLOB", aliases: &[
        CLibAliases { names: &["BCRYPT_RSAKEY_BLOB", "_BCRYPT_RSAKEY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCRYPT_SECRET_HANDLE", aliases: &[
        CLibAliases { names: &["BCRYPT_SECRET_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCryptBuffer", aliases: &[
        CLibAliases { names: &["BCryptBuffer", "_BCryptBuffer"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::BCryptBufferDesc", aliases: &[
        CLibAliases { names: &["BCryptBufferDesc", "_BCryptBufferDesc"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_ACCESS_DESCRIPTION", aliases: &[
        CLibAliases { names: &["CERT_ACCESS_DESCRIPTION", "_CERT_ACCESS_DESCRIPTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_ALT_NAME_ENTRY", aliases: &[
        CLibAliases { names: &["CERT_ALT_NAME_ENTRY", "_CERT_ALT_NAME_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_ALT_NAME_INFO", aliases: &[
        CLibAliases { names: &["CERT_ALT_NAME_INFO", "_CERT_ALT_NAME_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_AUTHORITY_INFO_ACCESS", aliases: &[
        CLibAliases { names: &["CERT_AUTHORITY_INFO_ACCESS", "_CERT_AUTHORITY_INFO_ACCESS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_AUTHORITY_KEY_ID2_INFO", aliases: &[
        CLibAliases { names: &["CERT_AUTHORITY_KEY_ID2_INFO", "_CERT_AUTHORITY_KEY_ID2_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_AUTHORITY_KEY_ID_INFO", aliases: &[
        CLibAliases { names: &["CERT_AUTHORITY_KEY_ID_INFO", "_CERT_AUTHORITY_KEY_ID_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_BASIC_CONSTRAINTS2_INFO", aliases: &[
        CLibAliases { names: &["CERT_BASIC_CONSTRAINTS2_INFO", "_CERT_BASIC_CONSTRAINTS2_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_BASIC_CONSTRAINTS_INFO", aliases: &[
        CLibAliases { names: &["CERT_BASIC_CONSTRAINTS_INFO", "_CERT_BASIC_CONSTRAINTS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_BIOMETRIC_DATA", aliases: &[
        CLibAliases { names: &["CERT_BIOMETRIC_DATA", "_CERT_BIOMETRIC_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_BIOMETRIC_EXT_INFO", aliases: &[
        CLibAliases { names: &["CERT_BIOMETRIC_EXT_INFO", "_CERT_BIOMETRIC_EXT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CHAIN", aliases: &[
        CLibAliases { names: &["CERT_CHAIN", "_CERT_CHAIN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CHAIN_CONTEXT", aliases: &[
        CLibAliases { names: &["CERT_CHAIN_CONTEXT", "_CERT_CHAIN_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CHAIN_ELEMENT", aliases: &[
        CLibAliases { names: &["CERT_CHAIN_ELEMENT", "_CERT_CHAIN_ELEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CHAIN_ENGINE_CONFIG", aliases: &[
        CLibAliases { names: &["CERT_CHAIN_ENGINE_CONFIG", "_CERT_CHAIN_ENGINE_CONFIG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CHAIN_FIND_BY_ISSUER_PARA", aliases: &[
        CLibAliases { names: &["CERT_CHAIN_FIND_BY_ISSUER_PARA", "_CERT_CHAIN_FIND_BY_ISSUER_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CHAIN_POLICY_PARA", aliases: &[
        CLibAliases { names: &["CERT_CHAIN_POLICY_PARA", "_CERT_CHAIN_POLICY_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CHAIN_POLICY_STATUS", aliases: &[
        CLibAliases { names: &["CERT_CHAIN_POLICY_STATUS", "_CERT_CHAIN_POLICY_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CONTEXT", aliases: &[
        CLibAliases { names: &["CERT_CONTEXT", "_CERT_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CREATE_CONTEXT_PARA", aliases: &[
        CLibAliases { names: &["CERT_CREATE_CONTEXT_PARA", "_CERT_CREATE_CONTEXT_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_CRL_CONTEXT_PAIR", aliases: &[
        CLibAliases { names: &["CERT_CRL_CONTEXT_PAIR", "_CERT_CRL_CONTEXT_PAIR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_DH_PARAMETERS", aliases: &[
        CLibAliases { names: &["CERT_DH_PARAMETERS", "_CERT_DH_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_DSS_PARAMETERS", aliases: &[
        CLibAliases { names: &["CERT_DSS_PARAMETERS", "_CERT_DSS_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_ECC_SIGNATURE", aliases: &[
        CLibAliases { names: &["CERT_ECC_SIGNATURE", "_CERT_ECC_SIGNATURE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_EXTENSION", aliases: &[
        CLibAliases { names: &["CERT_EXTENSION", "_CERT_EXTENSION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_EXTENSIONS", aliases: &[
        CLibAliases { names: &["CERT_EXTENSIONS", "_CERT_EXTENSIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_FORTEZZA_DATA_PROP", aliases: &[
        CLibAliases { names: &["CERT_FORTEZZA_DATA_PROP", "_CERT_FORTEZZA_DATA_PROP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_GENERAL_SUBTREE", aliases: &[
        CLibAliases { names: &["CERT_GENERAL_SUBTREE", "_CERT_GENERAL_SUBTREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_HASHED_URL", aliases: &[
        CLibAliases { names: &["CERT_HASHED_URL", "_CERT_HASHED_URL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_ID", aliases: &[
        CLibAliases { names: &["CERT_ID", "_CERT_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_INFO", aliases: &[
        CLibAliases { names: &["CERT_INFO", "_CERT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_ISSUER_SERIAL_NUMBER", aliases: &[
        CLibAliases { names: &["CERT_ISSUER_SERIAL_NUMBER", "_CERT_ISSUER_SERIAL_NUMBER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_KEYGEN_REQUEST_INFO", aliases: &[
        CLibAliases { names: &["CERT_KEYGEN_REQUEST_INFO", "_CERT_KEYGEN_REQUEST_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_KEY_ATTRIBUTES_INFO", aliases: &[
        CLibAliases { names: &["CERT_KEY_ATTRIBUTES_INFO", "_CERT_KEY_ATTRIBUTES_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_KEY_CONTEXT", aliases: &[
        CLibAliases { names: &["CERT_KEY_CONTEXT", "_CERT_KEY_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_KEY_USAGE_RESTRICTION_INFO", aliases: &[
        CLibAliases { names: &["CERT_KEY_USAGE_RESTRICTION_INFO", "_CERT_KEY_USAGE_RESTRICTION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LDAP_STORE_OPENED_PARA", aliases: &[
        CLibAliases { names: &["CERT_LDAP_STORE_OPENED_PARA", "_CERT_LDAP_STORE_OPENED_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_AUDIO", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_AUDIO", "_CERT_LOGOTYPE_AUDIO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_AUDIO_INFO", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_AUDIO_INFO", "_CERT_LOGOTYPE_AUDIO_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_DATA", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_DATA", "_CERT_LOGOTYPE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_DETAILS", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_DETAILS", "_CERT_LOGOTYPE_DETAILS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_EXT_INFO", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_EXT_INFO", "_CERT_LOGOTYPE_EXT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_IMAGE", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_IMAGE", "_CERT_LOGOTYPE_IMAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_IMAGE_INFO", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_IMAGE_INFO", "_CERT_LOGOTYPE_IMAGE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_INFO", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_INFO", "_CERT_LOGOTYPE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_LOGOTYPE_REFERENCE", aliases: &[
        CLibAliases { names: &["CERT_LOGOTYPE_REFERENCE", "_CERT_LOGOTYPE_REFERENCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_NAME_CONSTRAINTS_INFO", aliases: &[
        CLibAliases { names: &["CERT_NAME_CONSTRAINTS_INFO", "_CERT_NAME_CONSTRAINTS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_NAME_INFO", aliases: &[
        CLibAliases { names: &["CERT_NAME_INFO", "_CERT_NAME_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_NAME_VALUE", aliases: &[
        CLibAliases { names: &["CERT_NAME_VALUE", "_CERT_NAME_VALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_OR_CRL_BLOB", aliases: &[
        CLibAliases { names: &["CERT_OR_CRL_BLOB", "_CERT_OR_CRL_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_OR_CRL_BUNDLE", aliases: &[
        CLibAliases { names: &["CERT_OR_CRL_BUNDLE", "_CERT_OR_CRL_BUNDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_OTHER_LOGOTYPE_INFO", aliases: &[
        CLibAliases { names: &["CERT_OTHER_LOGOTYPE_INFO", "_CERT_OTHER_LOGOTYPE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_OTHER_NAME", aliases: &[
        CLibAliases { names: &["CERT_OTHER_NAME", "_CERT_OTHER_NAME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_PAIR", aliases: &[
        CLibAliases { names: &["CERT_PAIR", "_CERT_PAIR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_PHYSICAL_STORE_INFO", aliases: &[
        CLibAliases { names: &["CERT_PHYSICAL_STORE_INFO", "_CERT_PHYSICAL_STORE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICIES_INFO", aliases: &[
        CLibAliases { names: &["CERT_POLICIES_INFO", "_CERT_POLICIES_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY95_QUALIFIER1", aliases: &[
        CLibAliases { names: &["CERT_POLICY95_QUALIFIER1", "_CERT_POLICY95_QUALIFIER1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_CONSTRAINTS_INFO", aliases: &[
        CLibAliases { names: &["CERT_POLICY_CONSTRAINTS_INFO", "_CERT_POLICY_CONSTRAINTS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_ID", aliases: &[
        CLibAliases { names: &["CERT_POLICY_ID", "_CERT_POLICY_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_INFO", aliases: &[
        CLibAliases { names: &["CERT_POLICY_INFO", "_CERT_POLICY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_MAPPING", aliases: &[
        CLibAliases { names: &["CERT_POLICY_MAPPING", "_CERT_POLICY_MAPPING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_MAPPINGS_INFO", aliases: &[
        CLibAliases { names: &["CERT_POLICY_MAPPINGS_INFO", "_CERT_POLICY_MAPPINGS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_QUALIFIER_INFO", aliases: &[
        CLibAliases { names: &["CERT_POLICY_QUALIFIER_INFO", "_CERT_POLICY_QUALIFIER_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_QUALIFIER_NOTICE_REFERENCE", aliases: &[
        CLibAliases { names: &["CERT_POLICY_QUALIFIER_NOTICE_REFERENCE", "_CERT_POLICY_QUALIFIER_NOTICE_REFERENCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_POLICY_QUALIFIER_USER_NOTICE", aliases: &[
        CLibAliases { names: &["CERT_POLICY_QUALIFIER_USER_NOTICE", "_CERT_POLICY_QUALIFIER_USER_NOTICE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_PRIVATE_KEY_VALIDITY", aliases: &[
        CLibAliases { names: &["CERT_PRIVATE_KEY_VALIDITY", "_CERT_PRIVATE_KEY_VALIDITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_PUBLIC_KEY_INFO", aliases: &[
        CLibAliases { names: &["CERT_PUBLIC_KEY_INFO", "_CERT_PUBLIC_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_QC_STATEMENT", aliases: &[
        CLibAliases { names: &["CERT_QC_STATEMENT", "_CERT_QC_STATEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_QC_STATEMENTS_EXT_INFO", aliases: &[
        CLibAliases { names: &["CERT_QC_STATEMENTS_EXT_INFO", "_CERT_QC_STATEMENTS_EXT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_RDN", aliases: &[
        CLibAliases { names: &["CERT_RDN", "_CERT_RDN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_RDN_ATTR", aliases: &[
        CLibAliases { names: &["CERT_RDN_ATTR", "_CERT_RDN_ATTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REGISTRY_STORE_CLIENT_GPT_PARA", aliases: &[
        CLibAliases { names: &["CERT_REGISTRY_STORE_CLIENT_GPT_PARA", "_CERT_REGISTRY_STORE_CLIENT_GPT_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REGISTRY_STORE_ROAMING_PARA", aliases: &[
        CLibAliases { names: &["CERT_REGISTRY_STORE_ROAMING_PARA", "_CERT_REGISTRY_STORE_ROAMING_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REQUEST_INFO", aliases: &[
        CLibAliases { names: &["CERT_REQUEST_INFO", "_CERT_REQUEST_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REVOCATION_CHAIN_PARA", aliases: &[
        CLibAliases { names: &["CERT_REVOCATION_CHAIN_PARA", "_CERT_REVOCATION_CHAIN_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REVOCATION_CRL_INFO", aliases: &[
        CLibAliases { names: &["CERT_REVOCATION_CRL_INFO", "_CERT_REVOCATION_CRL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REVOCATION_INFO", aliases: &[
        CLibAliases { names: &["CERT_REVOCATION_INFO", "_CERT_REVOCATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REVOCATION_PARA", aliases: &[
        CLibAliases { names: &["CERT_REVOCATION_PARA", "_CERT_REVOCATION_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_REVOCATION_STATUS", aliases: &[
        CLibAliases { names: &["CERT_REVOCATION_STATUS", "_CERT_REVOCATION_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SELECT_CHAIN_PARA", aliases: &[
        CLibAliases { names: &["CERT_SELECT_CHAIN_PARA", "_CERT_SELECT_CHAIN_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SELECT_CRITERIA", aliases: &[
        CLibAliases { names: &["CERT_SELECT_CRITERIA", "_CERT_SELECT_CRITERIA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SERVER_OCSP_RESPONSE_CONTEXT", aliases: &[
        CLibAliases { names: &["CERT_SERVER_OCSP_RESPONSE_CONTEXT", "_CERT_SERVER_OCSP_RESPONSE_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SERVER_OCSP_RESPONSE_OPEN_PARA", aliases: &[
        CLibAliases { names: &["CERT_SERVER_OCSP_RESPONSE_OPEN_PARA", "_CERT_SERVER_OCSP_RESPONSE_OPEN_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SIGNED_CONTENT_INFO", aliases: &[
        CLibAliases { names: &["CERT_SIGNED_CONTENT_INFO", "_CERT_SIGNED_CONTENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SIMPLE_CHAIN", aliases: &[
        CLibAliases { names: &["CERT_SIMPLE_CHAIN", "_CERT_SIMPLE_CHAIN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_STORE_PROV_FIND_INFO", aliases: &[
        CLibAliases { names: &["CERT_STORE_PROV_FIND_INFO", "_CERT_STORE_PROV_FIND_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_STORE_PROV_INFO", aliases: &[
        CLibAliases { names: &["CERT_STORE_PROV_INFO", "_CERT_STORE_PROV_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_STRONG_SIGN_PARA", aliases: &[
        CLibAliases { names: &["CERT_STRONG_SIGN_PARA", "_CERT_STRONG_SIGN_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_STRONG_SIGN_SERIALIZED_INFO", aliases: &[
        CLibAliases { names: &["CERT_STRONG_SIGN_SERIALIZED_INFO", "_CERT_STRONG_SIGN_SERIALIZED_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SUPPORTED_ALGORITHM_INFO", aliases: &[
        CLibAliases { names: &["CERT_SUPPORTED_ALGORITHM_INFO", "_CERT_SUPPORTED_ALGORITHM_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SYSTEM_STORE_INFO", aliases: &[
        CLibAliases { names: &["CERT_SYSTEM_STORE_INFO", "_CERT_SYSTEM_STORE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_SYSTEM_STORE_RELOCATE_PARA", aliases: &[
        CLibAliases { names: &["CERT_SYSTEM_STORE_RELOCATE_PARA", "_CERT_SYSTEM_STORE_RELOCATE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_TEMPLATE_EXT", aliases: &[
        CLibAliases { names: &["CERT_TEMPLATE_EXT", "_CERT_TEMPLATE_EXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_TPM_SPECIFICATION_INFO", aliases: &[
        CLibAliases { names: &["CERT_TPM_SPECIFICATION_INFO", "_CERT_TPM_SPECIFICATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_TRUST_LIST_INFO", aliases: &[
        CLibAliases { names: &["CERT_TRUST_LIST_INFO", "_CERT_TRUST_LIST_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_TRUST_STATUS", aliases: &[
        CLibAliases { names: &["CERT_TRUST_STATUS", "_CERT_TRUST_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_USAGE_MATCH", aliases: &[
        CLibAliases { names: &["CERT_USAGE_MATCH", "_CERT_USAGE_MATCH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_X942_DH_PARAMETERS", aliases: &[
        CLibAliases { names: &["CERT_X942_DH_PARAMETERS", "_CERT_X942_DH_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CERT_X942_DH_VALIDATION_PARAMS", aliases: &[
        CLibAliases { names: &["CERT_X942_DH_VALIDATION_PARAMS", "_CERT_X942_DH_VALIDATION_PARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_ADD_ATTRIBUTES_INFO", aliases: &[
        CLibAliases { names: &["CMC_ADD_ATTRIBUTES_INFO", "_CMC_ADD_ATTRIBUTES_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_ADD_EXTENSIONS_INFO", aliases: &[
        CLibAliases { names: &["CMC_ADD_EXTENSIONS_INFO", "_CMC_ADD_EXTENSIONS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_DATA_INFO", aliases: &[
        CLibAliases { names: &["CMC_DATA_INFO", "_CMC_DATA_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_PEND_INFO", aliases: &[
        CLibAliases { names: &["CMC_PEND_INFO", "_CMC_PEND_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_RESPONSE_INFO", aliases: &[
        CLibAliases { names: &["CMC_RESPONSE_INFO", "_CMC_RESPONSE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_STATUS_INFO", aliases: &[
        CLibAliases { names: &["CMC_STATUS_INFO", "_CMC_STATUS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_TAGGED_ATTRIBUTE", aliases: &[
        CLibAliases { names: &["CMC_TAGGED_ATTRIBUTE", "_CMC_TAGGED_ATTRIBUTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_TAGGED_CERT_REQUEST", aliases: &[
        CLibAliases { names: &["CMC_TAGGED_CERT_REQUEST", "_CMC_TAGGED_CERT_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_TAGGED_CONTENT_INFO", aliases: &[
        CLibAliases { names: &["CMC_TAGGED_CONTENT_INFO", "_CMC_TAGGED_CONTENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_TAGGED_OTHER_MSG", aliases: &[
        CLibAliases { names: &["CMC_TAGGED_OTHER_MSG", "_CMC_TAGGED_OTHER_MSG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMC_TAGGED_REQUEST", aliases: &[
        CLibAliases { names: &["CMC_TAGGED_REQUEST", "_CMC_TAGGED_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CMS_RECIPIENT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_CMS_RECIPIENT_INFO", "_CMSG_CMS_RECIPIENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CMS_SIGNER_INFO", aliases: &[
        CLibAliases { names: &["CMSG_CMS_SIGNER_INFO", "_CMSG_CMS_SIGNER_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CNG_CONTENT_DECRYPT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_CNG_CONTENT_DECRYPT_INFO", "_CMSG_CNG_CONTENT_DECRYPT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CONTENT_ENCRYPT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_CONTENT_ENCRYPT_INFO", "_CMSG_CONTENT_ENCRYPT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CTRL_ADD_SIGNER_UNAUTH_ATTR_PARA", aliases: &[
        CLibAliases { names: &["CMSG_CTRL_ADD_SIGNER_UNAUTH_ATTR_PARA", "_CMSG_CTRL_ADD_SIGNER_UNAUTH_ATTR_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CTRL_DECRYPT_PARA", aliases: &[
        CLibAliases { names: &["CMSG_CTRL_DECRYPT_PARA", "_CMSG_CTRL_DECRYPT_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CTRL_DEL_SIGNER_UNAUTH_ATTR_PARA", aliases: &[
        CLibAliases { names: &["CMSG_CTRL_DEL_SIGNER_UNAUTH_ATTR_PARA", "_CMSG_CTRL_DEL_SIGNER_UNAUTH_ATTR_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CTRL_KEY_AGREE_DECRYPT_PARA", aliases: &[
        CLibAliases { names: &["CMSG_CTRL_KEY_AGREE_DECRYPT_PARA", "_CMSG_CTRL_KEY_AGREE_DECRYPT_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CTRL_KEY_TRANS_DECRYPT_PARA", aliases: &[
        CLibAliases { names: &["CMSG_CTRL_KEY_TRANS_DECRYPT_PARA", "_CMSG_CTRL_KEY_TRANS_DECRYPT_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CTRL_MAIL_LIST_DECRYPT_PARA", aliases: &[
        CLibAliases { names: &["CMSG_CTRL_MAIL_LIST_DECRYPT_PARA", "_CMSG_CTRL_MAIL_LIST_DECRYPT_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_CTRL_VERIFY_SIGNATURE_EX_PARA", aliases: &[
        CLibAliases { names: &["CMSG_CTRL_VERIFY_SIGNATURE_EX_PARA", "_CMSG_CTRL_VERIFY_SIGNATURE_EX_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_ENCRYPTED_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_ENCRYPTED_ENCODE_INFO", "_CMSG_ENCRYPTED_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_ENVELOPED_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_ENVELOPED_ENCODE_INFO", "_CMSG_ENVELOPED_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_HASHED_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_HASHED_ENCODE_INFO", "_CMSG_HASHED_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_KEY_AGREE_ENCRYPT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_KEY_AGREE_ENCRYPT_INFO", "_CMSG_KEY_AGREE_ENCRYPT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_KEY_AGREE_KEY_ENCRYPT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_KEY_AGREE_KEY_ENCRYPT_INFO", "_CMSG_KEY_AGREE_KEY_ENCRYPT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_KEY_AGREE_RECIPIENT_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_KEY_AGREE_RECIPIENT_ENCODE_INFO", "_CMSG_KEY_AGREE_RECIPIENT_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_KEY_AGREE_RECIPIENT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_KEY_AGREE_RECIPIENT_INFO", "_CMSG_KEY_AGREE_RECIPIENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_KEY_TRANS_ENCRYPT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_KEY_TRANS_ENCRYPT_INFO", "_CMSG_KEY_TRANS_ENCRYPT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_KEY_TRANS_RECIPIENT_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_KEY_TRANS_RECIPIENT_ENCODE_INFO", "_CMSG_KEY_TRANS_RECIPIENT_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_KEY_TRANS_RECIPIENT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_KEY_TRANS_RECIPIENT_INFO", "_CMSG_KEY_TRANS_RECIPIENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_MAIL_LIST_ENCRYPT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_MAIL_LIST_ENCRYPT_INFO", "_CMSG_MAIL_LIST_ENCRYPT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_MAIL_LIST_RECIPIENT_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_MAIL_LIST_RECIPIENT_ENCODE_INFO", "_CMSG_MAIL_LIST_RECIPIENT_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_MAIL_LIST_RECIPIENT_INFO", aliases: &[
        CLibAliases { names: &["CMSG_MAIL_LIST_RECIPIENT_INFO", "_CMSG_MAIL_LIST_RECIPIENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_RC2_AUX_INFO", aliases: &[
        CLibAliases { names: &["CMSG_RC2_AUX_INFO", "_CMSG_RC2_AUX_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_RC4_AUX_INFO", aliases: &[
        CLibAliases { names: &["CMSG_RC4_AUX_INFO", "_CMSG_RC4_AUX_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_RECIPIENT_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_RECIPIENT_ENCODE_INFO", "_CMSG_RECIPIENT_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_RECIPIENT_ENCRYPTED_KEY_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_RECIPIENT_ENCRYPTED_KEY_ENCODE_INFO", "_CMSG_RECIPIENT_ENCRYPTED_KEY_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_RECIPIENT_ENCRYPTED_KEY_INFO", aliases: &[
        CLibAliases { names: &["CMSG_RECIPIENT_ENCRYPTED_KEY_INFO", "_CMSG_RECIPIENT_ENCRYPTED_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_SIGNED_AND_ENVELOPED_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_SIGNED_AND_ENVELOPED_ENCODE_INFO", "_CMSG_SIGNED_AND_ENVELOPED_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_SIGNED_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_SIGNED_ENCODE_INFO", "_CMSG_SIGNED_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_SIGNER_ENCODE_INFO", aliases: &[
        CLibAliases { names: &["CMSG_SIGNER_ENCODE_INFO", "_CMSG_SIGNER_ENCODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_SIGNER_INFO", aliases: &[
        CLibAliases { names: &["CMSG_SIGNER_INFO", "_CMSG_SIGNER_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_SP3_COMPATIBLE_AUX_INFO", aliases: &[
        CLibAliases { names: &["CMSG_SP3_COMPATIBLE_AUX_INFO", "_CMSG_SP3_COMPATIBLE_AUX_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMSG_STREAM_INFO", aliases: &[
        CLibAliases { names: &["CMSG_STREAM_INFO", "_CMSG_STREAM_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMS_DH_KEY_INFO", aliases: &[
        CLibAliases { names: &["CMS_DH_KEY_INFO", "_CMS_DH_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CMS_KEY_INFO", aliases: &[
        CLibAliases { names: &["CMS_KEY_INFO", "_CMS_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CPS_URLS", aliases: &[
        CLibAliases { names: &["CPS_URLS", "_CPS_URLS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_CONTEXT", aliases: &[
        CLibAliases { names: &["CRL_CONTEXT", "_CRL_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_DIST_POINT", aliases: &[
        CLibAliases { names: &["CRL_DIST_POINT", "_CRL_DIST_POINT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_DIST_POINTS_INFO", aliases: &[
        CLibAliases { names: &["CRL_DIST_POINTS_INFO", "_CRL_DIST_POINTS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_DIST_POINT_NAME", aliases: &[
        CLibAliases { names: &["CRL_DIST_POINT_NAME", "_CRL_DIST_POINT_NAME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_ENTRY", aliases: &[
        CLibAliases { names: &["CRL_ENTRY", "_CRL_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_FIND_ISSUED_FOR_PARA", aliases: &[
        CLibAliases { names: &["CRL_FIND_ISSUED_FOR_PARA", "_CRL_FIND_ISSUED_FOR_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_INFO", aliases: &[
        CLibAliases { names: &["CRL_INFO", "_CRL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_ISSUING_DIST_POINT", aliases: &[
        CLibAliases { names: &["CRL_ISSUING_DIST_POINT", "_CRL_ISSUING_DIST_POINT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRL_REVOCATION_INFO", aliases: &[
        CLibAliases { names: &["CRL_REVOCATION_INFO", "_CRL_REVOCATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CROSS_CERT_DIST_POINTS_INFO", aliases: &[
        CLibAliases { names: &["CROSS_CERT_DIST_POINTS_INFO", "_CROSS_CERT_DIST_POINTS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPTNET_URL_CACHE_FLUSH_INFO", aliases: &[
        CLibAliases { names: &["CRYPTNET_URL_CACHE_FLUSH_INFO", "_CRYPTNET_URL_CACHE_FLUSH_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPTNET_URL_CACHE_PRE_FETCH_INFO", aliases: &[
        CLibAliases { names: &["CRYPTNET_URL_CACHE_PRE_FETCH_INFO", "_CRYPTNET_URL_CACHE_PRE_FETCH_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPTNET_URL_CACHE_RESPONSE_INFO", aliases: &[
        CLibAliases { names: &["CRYPTNET_URL_CACHE_RESPONSE_INFO", "_CRYPTNET_URL_CACHE_RESPONSE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPTPROTECT_PROMPTSTRUCT", aliases: &[
        CLibAliases { names: &["CRYPTPROTECT_PROMPTSTRUCT", "_CRYPTPROTECT_PROMPTSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_3DES_KEY_STATE", aliases: &[
        CLibAliases { names: &["CRYPT_3DES_KEY_STATE", "_CRYPT_3DES_KEY_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_AES_128_KEY_STATE", aliases: &[
        CLibAliases { names: &["CRYPT_AES_128_KEY_STATE", "_CRYPT_AES_128_KEY_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_AES_256_KEY_STATE", aliases: &[
        CLibAliases { names: &["CRYPT_AES_256_KEY_STATE", "_CRYPT_AES_256_KEY_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ALGORITHM_IDENTIFIER", aliases: &[
        CLibAliases { names: &["CRYPT_ALGORITHM_IDENTIFIER", "_CRYPT_ALGORITHM_IDENTIFIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ASYNC_RETRIEVAL_COMPLETION", aliases: &[
        CLibAliases { names: &["CRYPT_ASYNC_RETRIEVAL_COMPLETION", "_CRYPT_ASYNC_RETRIEVAL_COMPLETION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ATTRIBUTE", aliases: &[
        CLibAliases { names: &["CRYPT_ATTRIBUTE", "_CRYPT_ATTRIBUTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["CRYPT_ATTRIBUTES", "_CRYPT_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ATTRIBUTE_TYPE_VALUE", aliases: &[
        CLibAliases { names: &["CRYPT_ATTRIBUTE_TYPE_VALUE", "_CRYPT_ATTRIBUTE_TYPE_VALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_BIT_BLOB", aliases: &[
        CLibAliases { names: &["CRYPT_BIT_BLOB", "_CRYPT_BIT_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_BLOB_ARRAY", aliases: &[
        CLibAliases { names: &["CRYPT_BLOB_ARRAY", "_CRYPT_BLOB_ARRAY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CONTENT_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_CONTENT_INFO", "_CRYPT_CONTENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CONTENT_INFO_SEQUENCE_OF_ANY", aliases: &[
        CLibAliases { names: &["CRYPT_CONTENT_INFO_SEQUENCE_OF_ANY", "_CRYPT_CONTENT_INFO_SEQUENCE_OF_ANY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CONTEXTS", aliases: &[
        CLibAliases { names: &["CRYPT_CONTEXTS", "_CRYPT_CONTEXTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CONTEXT_CONFIG", aliases: &[
        CLibAliases { names: &["CRYPT_CONTEXT_CONFIG", "_CRYPT_CONTEXT_CONFIG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CONTEXT_FUNCTIONS", aliases: &[
        CLibAliases { names: &["CRYPT_CONTEXT_FUNCTIONS", "_CRYPT_CONTEXT_FUNCTIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CONTEXT_FUNCTION_CONFIG", aliases: &[
        CLibAliases { names: &["CRYPT_CONTEXT_FUNCTION_CONFIG", "_CRYPT_CONTEXT_FUNCTION_CONFIG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CONTEXT_FUNCTION_PROVIDERS", aliases: &[
        CLibAliases { names: &["CRYPT_CONTEXT_FUNCTION_PROVIDERS", "_CRYPT_CONTEXT_FUNCTION_PROVIDERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CREDENTIALS", aliases: &[
        CLibAliases { names: &["CRYPT_CREDENTIALS", "_CRYPT_CREDENTIALS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_CSP_PROVIDER", aliases: &[
        CLibAliases { names: &["CRYPT_CSP_PROVIDER", "_CRYPT_CSP_PROVIDER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_DECODE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_DECODE_PARA", "_CRYPT_DECODE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_DECRYPT_MESSAGE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_DECRYPT_MESSAGE_PARA", "_CRYPT_DECRYPT_MESSAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_DEFAULT_CONTEXT_MULTI_OID_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_DEFAULT_CONTEXT_MULTI_OID_PARA", "_CRYPT_DEFAULT_CONTEXT_MULTI_OID_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_DES_KEY_STATE", aliases: &[
        CLibAliases { names: &["CRYPT_DES_KEY_STATE", "_CRYPT_DES_KEY_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ECC_CMS_SHARED_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_ECC_CMS_SHARED_INFO", "_CRYPT_ECC_CMS_SHARED_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ECC_PRIVATE_KEY_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_ECC_PRIVATE_KEY_INFO", "_CRYPT_ECC_PRIVATE_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ENCODE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_ENCODE_PARA", "_CRYPT_ENCODE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ENCRYPTED_PRIVATE_KEY_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_ENCRYPTED_PRIVATE_KEY_INFO", "_CRYPT_ENCRYPTED_PRIVATE_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ENCRYPT_MESSAGE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_ENCRYPT_MESSAGE_PARA", "_CRYPT_ENCRYPT_MESSAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_ENROLLMENT_NAME_VALUE_PAIR", aliases: &[
        CLibAliases { names: &["CRYPT_ENROLLMENT_NAME_VALUE_PAIR", "_CRYPT_ENROLLMENT_NAME_VALUE_PAIR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_GET_TIME_VALID_OBJECT_EXTRA_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_GET_TIME_VALID_OBJECT_EXTRA_INFO", "_CRYPT_GET_TIME_VALID_OBJECT_EXTRA_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_HASH_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_HASH_INFO", "_CRYPT_HASH_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_HASH_MESSAGE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_HASH_MESSAGE_PARA", "_CRYPT_HASH_MESSAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_IMAGE_REF", aliases: &[
        CLibAliases { names: &["CRYPT_IMAGE_REF", "_CRYPT_IMAGE_REF"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_IMAGE_REG", aliases: &[
        CLibAliases { names: &["CRYPT_IMAGE_REG", "_CRYPT_IMAGE_REG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_INTEGER_BLOB", aliases: &[
        CLibAliases { names: &["CRYPT_INTEGER_BLOB", "_CRYPTOAPI_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_INTERFACE_REG", aliases: &[
        CLibAliases { names: &["CRYPT_INTERFACE_REG", "_CRYPT_INTERFACE_REG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_KEY_PROV_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_KEY_PROV_INFO", "_CRYPT_KEY_PROV_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_KEY_PROV_PARAM", aliases: &[
        CLibAliases { names: &["CRYPT_KEY_PROV_PARAM", "_CRYPT_KEY_PROV_PARAM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_KEY_SIGN_MESSAGE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_KEY_SIGN_MESSAGE_PARA", "_CRYPT_KEY_SIGN_MESSAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_KEY_VERIFY_MESSAGE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_KEY_VERIFY_MESSAGE_PARA", "_CRYPT_KEY_VERIFY_MESSAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_MASK_GEN_ALGORITHM", aliases: &[
        CLibAliases { names: &["CRYPT_MASK_GEN_ALGORITHM", "_CRYPT_MASK_GEN_ALGORITHM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_OBJECT_LOCATOR_PROVIDER_TABLE", aliases: &[
        CLibAliases { names: &["CRYPT_OBJECT_LOCATOR_PROVIDER_TABLE", "_CRYPT_OBJECT_LOCATOR_PROVIDER_TABLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_OBJID_TABLE", aliases: &[
        CLibAliases { names: &["CRYPT_OBJID_TABLE", "_CRYPT_OBJID_TABLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_OID_FUNC_ENTRY", aliases: &[
        CLibAliases { names: &["CRYPT_OID_FUNC_ENTRY", "_CRYPT_OID_FUNC_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_OID_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_OID_INFO", "_CRYPT_OID_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PASSWORD_CREDENTIALSA", aliases: &[
        CLibAliases { names: &["CRYPT_PASSWORD_CREDENTIALSA", "_CRYPT_PASSWORD_CREDENTIALSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PASSWORD_CREDENTIALSW", aliases: &[
        CLibAliases { names: &["CRYPT_PASSWORD_CREDENTIALSW", "_CRYPT_PASSWORD_CREDENTIALSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PKCS12_PBE_PARAMS", aliases: &[
        CLibAliases { names: &["CRYPT_PKCS12_PBE_PARAMS", "_CRYPT_PKCS12_PBE_PARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PKCS8_EXPORT_PARAMS", aliases: &[
        CLibAliases { names: &["CRYPT_PKCS8_EXPORT_PARAMS", "_CRYPT_PKCS8_EXPORT_PARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PKCS8_IMPORT_PARAMS", aliases: &[
        CLibAliases { names: &["CRYPT_PKCS8_IMPORT_PARAMS", "_CRYPT_PKCS8_IMPORT_PARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PRIVATE_KEY_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_PRIVATE_KEY_INFO", "_CRYPT_PRIVATE_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PROPERTY_REF", aliases: &[
        CLibAliases { names: &["CRYPT_PROPERTY_REF", "_CRYPT_PROPERTY_REF"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PROVIDERS", aliases: &[
        CLibAliases { names: &["CRYPT_PROVIDERS", "_CRYPT_PROVIDERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PROVIDER_REF", aliases: &[
        CLibAliases { names: &["CRYPT_PROVIDER_REF", "_CRYPT_PROVIDER_REF"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PROVIDER_REFS", aliases: &[
        CLibAliases { names: &["CRYPT_PROVIDER_REFS", "_CRYPT_PROVIDER_REFS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PROVIDER_REG", aliases: &[
        CLibAliases { names: &["CRYPT_PROVIDER_REG", "_CRYPT_PROVIDER_REG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_PSOURCE_ALGORITHM", aliases: &[
        CLibAliases { names: &["CRYPT_PSOURCE_ALGORITHM", "_CRYPT_PSOURCE_ALGORITHM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_RC2_CBC_PARAMETERS", aliases: &[
        CLibAliases { names: &["CRYPT_RC2_CBC_PARAMETERS", "_CRYPT_RC2_CBC_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_RC4_KEY_STATE", aliases: &[
        CLibAliases { names: &["CRYPT_RC4_KEY_STATE", "_CRYPT_RC4_KEY_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_RETRIEVE_AUX_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_RETRIEVE_AUX_INFO", "_CRYPT_RETRIEVE_AUX_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_RSAES_OAEP_PARAMETERS", aliases: &[
        CLibAliases { names: &["CRYPT_RSAES_OAEP_PARAMETERS", "_CRYPT_RSAES_OAEP_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_RSA_SSA_PSS_PARAMETERS", aliases: &[
        CLibAliases { names: &["CRYPT_RSA_SSA_PSS_PARAMETERS", "_CRYPT_RSA_SSA_PSS_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_SEQUENCE_OF_ANY", aliases: &[
        CLibAliases { names: &["CRYPT_SEQUENCE_OF_ANY", "_CRYPT_SEQUENCE_OF_ANY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_SIGN_MESSAGE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_SIGN_MESSAGE_PARA", "_CRYPT_SIGN_MESSAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_SMART_CARD_ROOT_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_SMART_CARD_ROOT_INFO", "_CRYPT_SMART_CARD_ROOT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_SMIME_CAPABILITIES", aliases: &[
        CLibAliases { names: &["CRYPT_SMIME_CAPABILITIES", "_CRYPT_SMIME_CAPABILITIES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_SMIME_CAPABILITY", aliases: &[
        CLibAliases { names: &["CRYPT_SMIME_CAPABILITY", "_CRYPT_SMIME_CAPABILITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_TIMESTAMP_ACCURACY", aliases: &[
        CLibAliases { names: &["CRYPT_TIMESTAMP_ACCURACY", "_CRYPT_TIMESTAMP_ACCURACY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_TIMESTAMP_CONTEXT", aliases: &[
        CLibAliases { names: &["CRYPT_TIMESTAMP_CONTEXT", "_CRYPT_TIMESTAMP_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_TIMESTAMP_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_TIMESTAMP_INFO", "_CRYPT_TIMESTAMP_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_TIMESTAMP_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_TIMESTAMP_PARA", "_CRYPT_TIMESTAMP_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_TIMESTAMP_REQUEST", aliases: &[
        CLibAliases { names: &["CRYPT_TIMESTAMP_REQUEST", "_CRYPT_TIMESTAMP_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_TIMESTAMP_RESPONSE", aliases: &[
        CLibAliases { names: &["CRYPT_TIMESTAMP_RESPONSE", "_CRYPT_TIMESTAMP_RESPONSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_TIME_STAMP_REQUEST_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_TIME_STAMP_REQUEST_INFO", "_CRYPT_TIME_STAMP_REQUEST_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_URL_ARRAY", aliases: &[
        CLibAliases { names: &["CRYPT_URL_ARRAY", "_CRYPT_URL_ARRAY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_URL_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_URL_INFO", "_CRYPT_URL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_VERIFY_CERT_SIGN_STRONG_PROPERTIES_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_VERIFY_CERT_SIGN_STRONG_PROPERTIES_INFO", "_CRYPT_VERIFY_CERT_SIGN_STRONG_PROPERTIES_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_VERIFY_CERT_SIGN_WEAK_HASH_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_VERIFY_CERT_SIGN_WEAK_HASH_INFO", "_CRYPT_VERIFY_CERT_SIGN_WEAK_HASH_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_VERIFY_MESSAGE_PARA", aliases: &[
        CLibAliases { names: &["CRYPT_VERIFY_MESSAGE_PARA", "_CRYPT_VERIFY_MESSAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CRYPT_X942_OTHER_INFO", aliases: &[
        CLibAliases { names: &["CRYPT_X942_OTHER_INFO", "_CRYPT_X942_OTHER_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_ANY_SUBJECT_INFO", aliases: &[
        CLibAliases { names: &["CTL_ANY_SUBJECT_INFO", "_CTL_ANY_SUBJECT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_CONTEXT", aliases: &[
        CLibAliases { names: &["CTL_CONTEXT", "_CTL_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_ENTRY", aliases: &[
        CLibAliases { names: &["CTL_ENTRY", "_CTL_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_FIND_SUBJECT_PARA", aliases: &[
        CLibAliases { names: &["CTL_FIND_SUBJECT_PARA", "_CTL_FIND_SUBJECT_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_FIND_USAGE_PARA", aliases: &[
        CLibAliases { names: &["CTL_FIND_USAGE_PARA", "_CTL_FIND_USAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_INFO", aliases: &[
        CLibAliases { names: &["CTL_INFO", "_CTL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_USAGE", aliases: &[
        CLibAliases { names: &["CTL_USAGE", "_CTL_USAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_USAGE_MATCH", aliases: &[
        CLibAliases { names: &["CTL_USAGE_MATCH", "_CTL_USAGE_MATCH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_VERIFY_USAGE_PARA", aliases: &[
        CLibAliases { names: &["CTL_VERIFY_USAGE_PARA", "_CTL_VERIFY_USAGE_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::CTL_VERIFY_USAGE_STATUS", aliases: &[
        CLibAliases { names: &["CTL_VERIFY_USAGE_STATUS", "_CTL_VERIFY_USAGE_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::DSSSEED", aliases: &[
        CLibAliases { names: &["DSSSEED", "_DSSSEED"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::EV_EXTRA_CERT_CHAIN_POLICY_PARA", aliases: &[
        CLibAliases { names: &["EV_EXTRA_CERT_CHAIN_POLICY_PARA", "_EV_EXTRA_CERT_CHAIN_POLICY_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::EV_EXTRA_CERT_CHAIN_POLICY_STATUS", aliases: &[
        CLibAliases { names: &["EV_EXTRA_CERT_CHAIN_POLICY_STATUS", "_EV_EXTRA_CERT_CHAIN_POLICY_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::HCERTCHAINENGINE", aliases: &[
        CLibAliases { names: &["HCERTCHAINENGINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::HCERTSTORE", aliases: &[
        CLibAliases { names: &["HCERTSTORE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::HCERTSTOREPROV", aliases: &[
        CLibAliases { names: &["HCERTSTOREPROV"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::HCRYPTASYNC", aliases: &[
        CLibAliases { names: &["HCRYPTASYNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::HMAC_INFO", aliases: &[
        CLibAliases { names: &["HMAC_INFO", "_HMAC_Info"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::HTTPSPolicyCallbackData", aliases: &[
        CLibAliases { names: &["HTTPSPolicyCallbackData", "_HTTPSPolicyCallbackData"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::KEY_TYPE_SUBTYPE", aliases: &[
        CLibAliases { names: &["KEY_TYPE_SUBTYPE", "_KEY_TYPE_SUBTYPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_ALLOC_PARA", aliases: &[
        CLibAliases { names: &["NCRYPT_ALLOC_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_CIPHER_PADDING_INFO", aliases: &[
        CLibAliases { names: &["NCRYPT_CIPHER_PADDING_INFO", "_NCRYPT_CIPHER_PADDING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_EXPORTED_ISOLATED_KEY_ENVELOPE", aliases: &[
        CLibAliases { names: &["NCRYPT_EXPORTED_ISOLATED_KEY_ENVELOPE", "_NCRYPT_EXPORTED_ISOLATED_KEY_ENVELOPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_ISOLATED_KEY_ATTESTED_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["NCRYPT_ISOLATED_KEY_ATTESTED_ATTRIBUTES", "_NCRYPT_ISOLATED_KEY_ATTESTED_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_KEY_ACCESS_POLICY_BLOB", aliases: &[
        CLibAliases { names: &["NCRYPT_KEY_ACCESS_POLICY_BLOB", "__NCRYPT_KEY_ACCESS_POLICY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_KEY_ATTEST_PADDING_INFO", aliases: &[
        CLibAliases { names: &["NCRYPT_KEY_ATTEST_PADDING_INFO", "_NCRYPT_KEY_ATTEST_PADDING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_KEY_BLOB_HEADER", aliases: &[
        CLibAliases { names: &["NCRYPT_KEY_BLOB_HEADER", "_NCRYPT_KEY_BLOB_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_PCP_HMAC_AUTH_SIGNATURE_INFO", aliases: &[
        CLibAliases { names: &["NCRYPT_PCP_HMAC_AUTH_SIGNATURE_INFO", "__NCRYPT_PCP_HMAC_AUTH_SIGNATURE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_PCP_RAW_POLICYDIGEST_INFO", aliases: &[
        CLibAliases { names: &["NCRYPT_PCP_RAW_POLICYDIGEST_INFO", "__NCRYPT_PCP_RAW_POLICYDIGEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_PCP_TPM_FW_VERSION_INFO", aliases: &[
        CLibAliases { names: &["NCRYPT_PCP_TPM_FW_VERSION_INFO", "__NCRYPT_PCP_TPM_FW_VERSION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_PCP_TPM_WEB_AUTHN_ATTESTATION_STATEMENT", aliases: &[
        CLibAliases { names: &["NCRYPT_PCP_TPM_WEB_AUTHN_ATTESTATION_STATEMENT", "__NCRYPT_PCP_TPM_WEB_AUTHN_ATTESTATION_STATEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_PLATFORM_ATTEST_PADDING_INFO", aliases: &[
        CLibAliases { names: &["NCRYPT_PLATFORM_ATTEST_PADDING_INFO", "_NCRYPT_PLATFORM_ATTEST_PADDING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_SUPPORTED_LENGTHS", aliases: &[
        CLibAliases { names: &["NCRYPT_SUPPORTED_LENGTHS", "__NCRYPT_SUPPORTED_LENGTHS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_TPM_LOADABLE_KEY_BLOB_HEADER", aliases: &[
        CLibAliases { names: &["NCRYPT_TPM_LOADABLE_KEY_BLOB_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_TPM_PLATFORM_ATTESTATION_STATEMENT", aliases: &[
        CLibAliases { names: &["NCRYPT_TPM_PLATFORM_ATTESTATION_STATEMENT", "_NCRYPT_TPM_PLATFORM_ATTESTATION_STATEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCRYPT_UI_POLICY", aliases: &[
        CLibAliases { names: &["NCRYPT_UI_POLICY", "__NCRYPT_UI_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCryptAlgorithmName", aliases: &[
        CLibAliases { names: &["NCryptAlgorithmName", "_NCryptAlgorithmName"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCryptKeyName", aliases: &[
        CLibAliases { names: &["NCryptKeyName"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::NCryptProviderName", aliases: &[
        CLibAliases { names: &["NCryptProviderName"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_BASIC_RESPONSE_ENTRY", aliases: &[
        CLibAliases { names: &["OCSP_BASIC_RESPONSE_ENTRY", "_OCSP_BASIC_RESPONSE_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_BASIC_RESPONSE_INFO", aliases: &[
        CLibAliases { names: &["OCSP_BASIC_RESPONSE_INFO", "_OCSP_BASIC_RESPONSE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_BASIC_REVOKED_INFO", aliases: &[
        CLibAliases { names: &["OCSP_BASIC_REVOKED_INFO", "_OCSP_BASIC_REVOKED_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_BASIC_SIGNED_RESPONSE_INFO", aliases: &[
        CLibAliases { names: &["OCSP_BASIC_SIGNED_RESPONSE_INFO", "_OCSP_BASIC_SIGNED_RESPONSE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_CERT_ID", aliases: &[
        CLibAliases { names: &["OCSP_CERT_ID", "_OCSP_CERT_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_REQUEST_ENTRY", aliases: &[
        CLibAliases { names: &["OCSP_REQUEST_ENTRY", "_OCSP_REQUEST_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_REQUEST_INFO", aliases: &[
        CLibAliases { names: &["OCSP_REQUEST_INFO", "_OCSP_REQUEST_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_RESPONSE_INFO", aliases: &[
        CLibAliases { names: &["OCSP_RESPONSE_INFO", "_OCSP_RESPONSE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_SIGNATURE_INFO", aliases: &[
        CLibAliases { names: &["OCSP_SIGNATURE_INFO", "_OCSP_SIGNATURE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::OCSP_SIGNED_REQUEST_INFO", aliases: &[
        CLibAliases { names: &["OCSP_SIGNED_REQUEST_INFO", "_OCSP_SIGNED_REQUEST_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PCRYPT_DECRYPT_PRIVATE_KEY_FUNC", aliases: &[
        CLibAliases { names: &["PCRYPT_DECRYPT_PRIVATE_KEY_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PCRYPT_ENCRYPT_PRIVATE_KEY_FUNC", aliases: &[
        CLibAliases { names: &["PCRYPT_ENCRYPT_PRIVATE_KEY_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PCRYPT_RESOLVE_HCRYPTPROV_FUNC", aliases: &[
        CLibAliases { names: &["PCRYPT_RESOLVE_HCRYPTPROV_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CANCEL_ASYNC_RETRIEVAL_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CANCEL_ASYNC_RETRIEVAL_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_CHAIN_FIND_BY_ISSUER_CALLBACK", aliases: &[
        CLibAliases { names: &["PFN_CERT_CHAIN_FIND_BY_ISSUER_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_CREATE_CONTEXT_SORT_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CERT_CREATE_CONTEXT_SORT_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_DLL_OPEN_STORE_PROV_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CERT_DLL_OPEN_STORE_PROV_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_ENUM_PHYSICAL_STORE", aliases: &[
        CLibAliases { names: &["PFN_CERT_ENUM_PHYSICAL_STORE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_ENUM_SYSTEM_STORE", aliases: &[
        CLibAliases { names: &["PFN_CERT_ENUM_SYSTEM_STORE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_ENUM_SYSTEM_STORE_LOCATION", aliases: &[
        CLibAliases { names: &["PFN_CERT_ENUM_SYSTEM_STORE_LOCATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_IS_WEAK_HASH", aliases: &[
        CLibAliases { names: &["PFN_CERT_IS_WEAK_HASH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_SERVER_OCSP_RESPONSE_UPDATE_CALLBACK", aliases: &[
        CLibAliases { names: &["PFN_CERT_SERVER_OCSP_RESPONSE_UPDATE_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_CLOSE", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_CLOSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_CONTROL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_CONTROL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_DELETE_CERT", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_DELETE_CERT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_DELETE_CRL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_DELETE_CRL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_DELETE_CTL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_DELETE_CTL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_FIND_CERT", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_FIND_CERT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_FIND_CRL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_FIND_CRL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_FIND_CTL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_FIND_CTL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_FREE_FIND_CERT", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_FREE_FIND_CERT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_FREE_FIND_CRL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_FREE_FIND_CRL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_FREE_FIND_CTL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_FREE_FIND_CTL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_GET_CERT_PROPERTY", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_GET_CERT_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_GET_CRL_PROPERTY", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_GET_CRL_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_GET_CTL_PROPERTY", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_GET_CTL_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_READ_CERT", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_READ_CERT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_READ_CRL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_READ_CRL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_READ_CTL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_READ_CTL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_SET_CERT_PROPERTY", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_SET_CERT_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_SET_CRL_PROPERTY", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_SET_CRL_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_SET_CTL_PROPERTY", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_SET_CTL_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_WRITE_CERT", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_WRITE_CERT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_WRITE_CRL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_WRITE_CRL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CERT_STORE_PROV_WRITE_CTL", aliases: &[
        CLibAliases { names: &["PFN_CERT_STORE_PROV_WRITE_CTL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_ALLOC", aliases: &[
        CLibAliases { names: &["PFN_CMSG_ALLOC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_CNG_IMPORT_CONTENT_ENCRYPT_KEY", aliases: &[
        CLibAliases { names: &["PFN_CMSG_CNG_IMPORT_CONTENT_ENCRYPT_KEY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_CNG_IMPORT_KEY_AGREE", aliases: &[
        CLibAliases { names: &["PFN_CMSG_CNG_IMPORT_KEY_AGREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_CNG_IMPORT_KEY_TRANS", aliases: &[
        CLibAliases { names: &["PFN_CMSG_CNG_IMPORT_KEY_TRANS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_EXPORT_ENCRYPT_KEY", aliases: &[
        CLibAliases { names: &["PFN_CMSG_EXPORT_ENCRYPT_KEY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_EXPORT_KEY_AGREE", aliases: &[
        CLibAliases { names: &["PFN_CMSG_EXPORT_KEY_AGREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_EXPORT_KEY_TRANS", aliases: &[
        CLibAliases { names: &["PFN_CMSG_EXPORT_KEY_TRANS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_EXPORT_MAIL_LIST", aliases: &[
        CLibAliases { names: &["PFN_CMSG_EXPORT_MAIL_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_FREE", aliases: &[
        CLibAliases { names: &["PFN_CMSG_FREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_GEN_CONTENT_ENCRYPT_KEY", aliases: &[
        CLibAliases { names: &["PFN_CMSG_GEN_CONTENT_ENCRYPT_KEY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_GEN_ENCRYPT_KEY", aliases: &[
        CLibAliases { names: &["PFN_CMSG_GEN_ENCRYPT_KEY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_IMPORT_ENCRYPT_KEY", aliases: &[
        CLibAliases { names: &["PFN_CMSG_IMPORT_ENCRYPT_KEY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_IMPORT_KEY_AGREE", aliases: &[
        CLibAliases { names: &["PFN_CMSG_IMPORT_KEY_AGREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_IMPORT_KEY_TRANS", aliases: &[
        CLibAliases { names: &["PFN_CMSG_IMPORT_KEY_TRANS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_IMPORT_MAIL_LIST", aliases: &[
        CLibAliases { names: &["PFN_CMSG_IMPORT_MAIL_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CMSG_STREAM_OUTPUT", aliases: &[
        CLibAliases { names: &["PFN_CMSG_STREAM_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_ALLOC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_ALLOC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_ASYNC_PARAM_FREE_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_ASYNC_PARAM_FREE_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_ASYNC_RETRIEVAL_COMPLETION_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_ASYNC_RETRIEVAL_COMPLETION_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_CANCEL_RETRIEVAL", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_CANCEL_RETRIEVAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_ENUM_KEYID_PROP", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_ENUM_KEYID_PROP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_ENUM_OID_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_ENUM_OID_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_ENUM_OID_INFO", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_ENUM_OID_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_EXPORT_PUBLIC_KEY_INFO_EX2_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_EXPORT_PUBLIC_KEY_INFO_EX2_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_EXPORT_PUBLIC_KEY_INFO_FROM_BCRYPT_HANDLE_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_EXPORT_PUBLIC_KEY_INFO_FROM_BCRYPT_HANDLE_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_EXTRACT_ENCODED_SIGNATURE_PARAMETERS_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_EXTRACT_ENCODED_SIGNATURE_PARAMETERS_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_FREE", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_FREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_GET_SIGNER_CERTIFICATE", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_GET_SIGNER_CERTIFICATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FLUSH", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FLUSH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FREE", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FREE_IDENTIFIER", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FREE_IDENTIFIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FREE_PASSWORD", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_FREE_PASSWORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_GET", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_GET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_INITIALIZE", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_INITIALIZE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_RELEASE", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_OBJECT_LOCATOR_PROVIDER_RELEASE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_SIGN_AND_ENCODE_HASH_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_SIGN_AND_ENCODE_HASH_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_CRYPT_VERIFY_ENCODED_SIGNATURE_FUNC", aliases: &[
        CLibAliases { names: &["PFN_CRYPT_VERIFY_ENCODED_SIGNATURE_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_EXPORT_PRIV_KEY_FUNC", aliases: &[
        CLibAliases { names: &["PFN_EXPORT_PRIV_KEY_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_FREE_ENCODED_OBJECT_FUNC", aliases: &[
        CLibAliases { names: &["PFN_FREE_ENCODED_OBJECT_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_IMPORT_PRIV_KEY_FUNC", aliases: &[
        CLibAliases { names: &["PFN_IMPORT_PRIV_KEY_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_IMPORT_PUBLIC_KEY_INFO_EX2_FUNC", aliases: &[
        CLibAliases { names: &["PFN_IMPORT_PUBLIC_KEY_INFO_EX2_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_NCRYPT_ALLOC", aliases: &[
        CLibAliases { names: &["PFN_NCRYPT_ALLOC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PFN_NCRYPT_FREE", aliases: &[
        CLibAliases { names: &["PFN_NCRYPT_FREE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PKCS12_PBES2_EXPORT_PARAMS", aliases: &[
        CLibAliases { names: &["PKCS12_PBES2_EXPORT_PARAMS", "_PKCS12_PBES2_EXPORT_PARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PROV_ENUMALGS", aliases: &[
        CLibAliases { names: &["PROV_ENUMALGS", "_PROV_ENUMALGS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PROV_ENUMALGS_EX", aliases: &[
        CLibAliases { names: &["PROV_ENUMALGS_EX", "_PROV_ENUMALGS_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::PUBLICKEYSTRUC", aliases: &[
        CLibAliases { names: &["PUBLICKEYSTRUC", "_PUBLICKEYSTRUC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::ROOT_INFO_LUID", aliases: &[
        CLibAliases { names: &["ROOT_INFO_LUID", "_ROOT_INFO_LUID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::RSAPUBKEY", aliases: &[
        CLibAliases { names: &["RSAPUBKEY", "_RSAPUBKEY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::SCHANNEL_ALG", aliases: &[
        CLibAliases { names: &["SCHANNEL_ALG", "_SCHANNEL_ALG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::SSL_ECCKEY_BLOB", aliases: &[
        CLibAliases { names: &["SSL_ECCKEY_BLOB", "_SSL_ECCKEY_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::SSL_F12_EXTRA_CERT_CHAIN_POLICY_STATUS", aliases: &[
        CLibAliases { names: &["SSL_F12_EXTRA_CERT_CHAIN_POLICY_STATUS", "_SSL_F12_EXTRA_CERT_CHAIN_POLICY_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::SSL_HPKP_HEADER_EXTRA_CERT_CHAIN_POLICY_PARA", aliases: &[
        CLibAliases { names: &["SSL_HPKP_HEADER_EXTRA_CERT_CHAIN_POLICY_PARA", "_SSL_HPKP_HEADER_EXTRA_CERT_CHAIN_POLICY_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::SSL_KEY_PIN_EXTRA_CERT_CHAIN_POLICY_PARA", aliases: &[
        CLibAliases { names: &["SSL_KEY_PIN_EXTRA_CERT_CHAIN_POLICY_PARA", "_SSL_KEY_PIN_EXTRA_CERT_CHAIN_POLICY_PARA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::Cryptography::SSL_KEY_PIN_EXTRA_CERT_CHAIN_POLICY_STATUS", aliases: &[
        CLibAliases { names: &["SSL_KEY_PIN_EXTRA_CERT_CHAIN_POLICY_STATUS", "_SSL_KEY_PIN_EXTRA_CERT_CHAIN_POLICY_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::GENERIC_MAPPING", aliases: &[
        CLibAliases { names: &["GENERIC_MAPPING", "_GENERIC_MAPPING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::LUID_AND_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["LUID_AND_ATTRIBUTES", "_LUID_AND_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::OBJECT_TYPE_LIST", aliases: &[
        CLibAliases { names: &["OBJECT_TYPE_LIST", "_OBJECT_TYPE_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::PRIVILEGE_SET", aliases: &[
        CLibAliases { names: &["PRIVILEGE_SET", "_PRIVILEGE_SET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::PSECURITY_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["PSECURITY_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::PSID", aliases: &[
        CLibAliases { names: &["PSID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::QUOTA_LIMITS", aliases: &[
        CLibAliases { names: &["QUOTA_LIMITS", "_QUOTA_LIMITS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SECURITY_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["SECURITY_ATTRIBUTES", "_SECURITY_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SECURITY_CAPABILITIES", aliases: &[
        CLibAliases { names: &["SECURITY_CAPABILITIES", "_SECURITY_CAPABILITIES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SECURITY_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["SECURITY_DESCRIPTOR", "_SECURITY_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SECURITY_DESCRIPTOR_RELATIVE", aliases: &[
        CLibAliases { names: &["SECURITY_DESCRIPTOR_RELATIVE", "_SECURITY_DESCRIPTOR_RELATIVE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SECURITY_QUALITY_OF_SERVICE", aliases: &[
        CLibAliases { names: &["SECURITY_QUALITY_OF_SERVICE", "_SECURITY_QUALITY_OF_SERVICE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SE_ACCESS_REPLY", aliases: &[
        CLibAliases { names: &["SE_ACCESS_REPLY", "_SE_ACCESS_REPLY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SE_ACCESS_REQUEST", aliases: &[
        CLibAliases { names: &["SE_ACCESS_REQUEST", "_SE_ACCESS_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SE_IMPERSONATION_STATE", aliases: &[
        CLibAliases { names: &["SE_IMPERSONATION_STATE", "_SE_IMPERSONATION_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SE_SECURITY_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["SE_SECURITY_DESCRIPTOR", "_SE_SECURITY_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SE_SID", aliases: &[
        CLibAliases { names: &["SE_SID", "_SE_SID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SID", aliases: &[
        CLibAliases { names: &["SID", "_SID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SID_AND_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["SID_AND_ATTRIBUTES", "_SID_AND_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SID_AND_ATTRIBUTES_HASH", aliases: &[
        CLibAliases { names: &["SID_AND_ATTRIBUTES_HASH", "_SID_AND_ATTRIBUTES_HASH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SID_IDENTIFIER_AUTHORITY", aliases: &[
        CLibAliases { names: &["SID_IDENTIFIER_AUTHORITY", "_SID_IDENTIFIER_AUTHORITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_ACCESS_FILTER_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_ACCESS_FILTER_ACE", "_SYSTEM_ACCESS_FILTER_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_ALARM_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_ALARM_ACE", "_SYSTEM_ALARM_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_ALARM_CALLBACK_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_ALARM_CALLBACK_ACE", "_SYSTEM_ALARM_CALLBACK_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_ALARM_CALLBACK_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_ALARM_CALLBACK_OBJECT_ACE", "_SYSTEM_ALARM_CALLBACK_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_ALARM_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_ALARM_OBJECT_ACE", "_SYSTEM_ALARM_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_AUDIT_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_AUDIT_ACE", "_SYSTEM_AUDIT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_AUDIT_CALLBACK_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_AUDIT_CALLBACK_ACE", "_SYSTEM_AUDIT_CALLBACK_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_AUDIT_CALLBACK_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_AUDIT_CALLBACK_OBJECT_ACE", "_SYSTEM_AUDIT_CALLBACK_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_AUDIT_OBJECT_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_AUDIT_OBJECT_ACE", "_SYSTEM_AUDIT_OBJECT_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_MANDATORY_LABEL_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_MANDATORY_LABEL_ACE", "_SYSTEM_MANDATORY_LABEL_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_PROCESS_TRUST_LABEL_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_PROCESS_TRUST_LABEL_ACE", "_SYSTEM_PROCESS_TRUST_LABEL_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_RESOURCE_ATTRIBUTE_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_RESOURCE_ATTRIBUTE_ACE", "_SYSTEM_RESOURCE_ATTRIBUTE_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::SYSTEM_SCOPED_POLICY_ID_ACE", aliases: &[
        CLibAliases { names: &["SYSTEM_SCOPED_POLICY_ID_ACE", "_SYSTEM_SCOPED_POLICY_ID_ACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_ACCESS_INFORMATION", aliases: &[
        CLibAliases { names: &["TOKEN_ACCESS_INFORMATION", "_TOKEN_ACCESS_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_APPCONTAINER_INFORMATION", aliases: &[
        CLibAliases { names: &["TOKEN_APPCONTAINER_INFORMATION", "_TOKEN_APPCONTAINER_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_CONTROL", aliases: &[
        CLibAliases { names: &["TOKEN_CONTROL", "_TOKEN_CONTROL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_DEFAULT_DACL", aliases: &[
        CLibAliases { names: &["TOKEN_DEFAULT_DACL", "_TOKEN_DEFAULT_DACL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_DEVICE_CLAIMS", aliases: &[
        CLibAliases { names: &["TOKEN_DEVICE_CLAIMS", "_TOKEN_DEVICE_CLAIMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_ELEVATION", aliases: &[
        CLibAliases { names: &["TOKEN_ELEVATION", "_TOKEN_ELEVATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_GROUPS", aliases: &[
        CLibAliases { names: &["TOKEN_GROUPS", "_TOKEN_GROUPS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_GROUPS_AND_PRIVILEGES", aliases: &[
        CLibAliases { names: &["TOKEN_GROUPS_AND_PRIVILEGES", "_TOKEN_GROUPS_AND_PRIVILEGES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_LINKED_TOKEN", aliases: &[
        CLibAliases { names: &["TOKEN_LINKED_TOKEN", "_TOKEN_LINKED_TOKEN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_MANDATORY_LABEL", aliases: &[
        CLibAliases { names: &["TOKEN_MANDATORY_LABEL", "_TOKEN_MANDATORY_LABEL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_MANDATORY_POLICY", aliases: &[
        CLibAliases { names: &["TOKEN_MANDATORY_POLICY", "_TOKEN_MANDATORY_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_ORIGIN", aliases: &[
        CLibAliases { names: &["TOKEN_ORIGIN", "_TOKEN_ORIGIN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_OWNER", aliases: &[
        CLibAliases { names: &["TOKEN_OWNER", "_TOKEN_OWNER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_PRIMARY_GROUP", aliases: &[
        CLibAliases { names: &["TOKEN_PRIMARY_GROUP", "_TOKEN_PRIMARY_GROUP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_PRIVILEGES", aliases: &[
        CLibAliases { names: &["TOKEN_PRIVILEGES", "_TOKEN_PRIVILEGES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_SOURCE", aliases: &[
        CLibAliases { names: &["TOKEN_SOURCE", "_TOKEN_SOURCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_STATISTICS", aliases: &[
        CLibAliases { names: &["TOKEN_STATISTICS", "_TOKEN_STATISTICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_USER", aliases: &[
        CLibAliases { names: &["TOKEN_USER", "_TOKEN_USER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Security::TOKEN_USER_CLAIMS", aliases: &[
        CLibAliases { names: &["TOKEN_USER_CLAIMS", "_TOKEN_USER_CLAIMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::BY_HANDLE_FILE_INFORMATION", aliases: &[
        CLibAliases { names: &["BY_HANDLE_FILE_INFORMATION", "_BY_HANDLE_FILE_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::COPYFILE2_EXTENDED_PARAMETERS", aliases: &[
        CLibAliases { names: &["COPYFILE2_EXTENDED_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::COPYFILE2_MESSAGE", aliases: &[
        CLibAliases { names: &["COPYFILE2_MESSAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::CREATEFILE2_EXTENDED_PARAMETERS", aliases: &[
        CLibAliases { names: &["CREATEFILE2_EXTENDED_PARAMETERS", "_CREATEFILE2_EXTENDED_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::DISK_SPACE_INFORMATION", aliases: &[
        CLibAliases { names: &["DISK_SPACE_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_CERTIFICATE_BLOB", aliases: &[
        CLibAliases { names: &["EFS_CERTIFICATE_BLOB", "_CERTIFICATE_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_COMPATIBILITY_INFO", aliases: &[
        CLibAliases { names: &["EFS_COMPATIBILITY_INFO", "_EFS_COMPATIBILITY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_DECRYPTION_STATUS_INFO", aliases: &[
        CLibAliases { names: &["EFS_DECRYPTION_STATUS_INFO", "_EFS_DECRYPTION_STATUS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_ENCRYPTION_STATUS_INFO", aliases: &[
        CLibAliases { names: &["EFS_ENCRYPTION_STATUS_INFO", "_EFS_ENCRYPTION_STATUS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_HASH_BLOB", aliases: &[
        CLibAliases { names: &["EFS_HASH_BLOB", "_EFS_HASH_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_KEY_INFO", aliases: &[
        CLibAliases { names: &["EFS_KEY_INFO", "_EFS_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_PIN_BLOB", aliases: &[
        CLibAliases { names: &["EFS_PIN_BLOB", "_EFS_PIN_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_RPC_BLOB", aliases: &[
        CLibAliases { names: &["EFS_RPC_BLOB", "_EFS_RPC_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::EFS_VERSION_INFO", aliases: &[
        CLibAliases { names: &["EFS_VERSION_INFO", "_EFS_VERSION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::ENCRYPTED_FILE_METADATA_SIGNATURE", aliases: &[
        CLibAliases { names: &["ENCRYPTED_FILE_METADATA_SIGNATURE", "_ENCRYPTED_FILE_METADATA_SIGNATURE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::ENCRYPTION_CERTIFICATE", aliases: &[
        CLibAliases { names: &["ENCRYPTION_CERTIFICATE", "_ENCRYPTION_CERTIFICATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::ENCRYPTION_CERTIFICATE_HASH", aliases: &[
        CLibAliases { names: &["ENCRYPTION_CERTIFICATE_HASH", "_ENCRYPTION_CERTIFICATE_HASH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::ENCRYPTION_CERTIFICATE_HASH_LIST", aliases: &[
        CLibAliases { names: &["ENCRYPTION_CERTIFICATE_HASH_LIST", "_ENCRYPTION_CERTIFICATE_HASH_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::ENCRYPTION_CERTIFICATE_LIST", aliases: &[
        CLibAliases { names: &["ENCRYPTION_CERTIFICATE_LIST", "_ENCRYPTION_CERTIFICATE_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::ENCRYPTION_PROTECTOR", aliases: &[
        CLibAliases { names: &["ENCRYPTION_PROTECTOR", "_ENCRYPTION_PROTECTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::ENCRYPTION_PROTECTOR_LIST", aliases: &[
        CLibAliases { names: &["ENCRYPTION_PROTECTOR_LIST", "_ENCRYPTION_PROTECTOR_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ALIGNMENT_INFO", aliases: &[
        CLibAliases { names: &["FILE_ALIGNMENT_INFO", "_FILE_ALIGNMENT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ALLOCATION_INFO", aliases: &[
        CLibAliases { names: &["FILE_ALLOCATION_INFO", "_FILE_ALLOCATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ATTRIBUTE_TAG_INFO", aliases: &[
        CLibAliases { names: &["FILE_ATTRIBUTE_TAG_INFO", "_FILE_ATTRIBUTE_TAG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_BASIC_INFO", aliases: &[
        CLibAliases { names: &["FILE_BASIC_INFO", "_FILE_BASIC_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_CASE_SENSITIVE_INFO", aliases: &[
        CLibAliases { names: &["FILE_CASE_SENSITIVE_INFO", "_FILE_CASE_SENSITIVE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_COMPRESSION_INFO", aliases: &[
        CLibAliases { names: &["FILE_COMPRESSION_INFO", "_FILE_COMPRESSION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_DISPOSITION_INFO_EX", aliases: &[
        CLibAliases { names: &["FILE_DISPOSITION_INFO_EX", "_FILE_DISPOSITION_INFO_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_END_OF_FILE_INFO", aliases: &[
        CLibAliases { names: &["FILE_END_OF_FILE_INFO", "_FILE_END_OF_FILE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_FULL_DIR_INFO", aliases: &[
        CLibAliases { names: &["FILE_FULL_DIR_INFO", "_FILE_FULL_DIR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ID_128", aliases: &[
        CLibAliases { names: &["FILE_ID_128", "_FILE_ID_128"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ID_BOTH_DIR_INFO", aliases: &[
        CLibAliases { names: &["FILE_ID_BOTH_DIR_INFO", "_FILE_ID_BOTH_DIR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ID_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["FILE_ID_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ID_EXTD_DIR_INFO", aliases: &[
        CLibAliases { names: &["FILE_ID_EXTD_DIR_INFO", "_FILE_ID_EXTD_DIR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_ID_INFO", aliases: &[
        CLibAliases { names: &["FILE_ID_INFO", "_FILE_ID_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_IO_PRIORITY_HINT_INFO", aliases: &[
        CLibAliases { names: &["FILE_IO_PRIORITY_HINT_INFO", "_FILE_IO_PRIORITY_HINT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_NAME_INFO", aliases: &[
        CLibAliases { names: &["FILE_NAME_INFO", "_FILE_NAME_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_NOTIFY_EXTENDED_INFORMATION", aliases: &[
        CLibAliases { names: &["FILE_NOTIFY_EXTENDED_INFORMATION", "_FILE_NOTIFY_EXTENDED_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_NOTIFY_INFORMATION", aliases: &[
        CLibAliases { names: &["FILE_NOTIFY_INFORMATION", "_FILE_NOTIFY_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_REMOTE_PROTOCOL_INFO", aliases: &[
        CLibAliases { names: &["FILE_REMOTE_PROTOCOL_INFO", "_FILE_REMOTE_PROTOCOL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_RENAME_INFO", aliases: &[
        CLibAliases { names: &["FILE_RENAME_INFO", "_FILE_RENAME_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_SEGMENT_ELEMENT", aliases: &[
        CLibAliases { names: &["FILE_SEGMENT_ELEMENT", "_FILE_SEGMENT_ELEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_STANDARD_INFO", aliases: &[
        CLibAliases { names: &["FILE_STANDARD_INFO", "_FILE_STANDARD_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_STORAGE_INFO", aliases: &[
        CLibAliases { names: &["FILE_STORAGE_INFO", "_FILE_STORAGE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::FILE_STREAM_INFO", aliases: &[
        CLibAliases { names: &["FILE_STREAM_INFO", "_FILE_STREAM_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::KCRM_MARSHAL_HEADER", aliases: &[
        CLibAliases { names: &["KCRM_MARSHAL_HEADER", "_KCRM_MARSHAL_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::KCRM_PROTOCOL_BLOB", aliases: &[
        CLibAliases { names: &["KCRM_PROTOCOL_BLOB", "_KCRM_PROTOCOL_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::KCRM_TRANSACTION_BLOB", aliases: &[
        CLibAliases { names: &["KCRM_TRANSACTION_BLOB", "_KCRM_TRANSACTION_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::LPPROGRESS_ROUTINE", aliases: &[
        CLibAliases { names: &["LPPROGRESS_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::OFSTRUCT", aliases: &[
        CLibAliases { names: &["OFSTRUCT", "_OFSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::PCOPYFILE2_PROGRESS_ROUTINE", aliases: &[
        CLibAliases { names: &["PCOPYFILE2_PROGRESS_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::PFE_EXPORT_FUNC", aliases: &[
        CLibAliases { names: &["PFE_EXPORT_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::PFE_IMPORT_FUNC", aliases: &[
        CLibAliases { names: &["PFE_IMPORT_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::REPARSE_GUID_DATA_BUFFER", aliases: &[
        CLibAliases { names: &["REPARSE_GUID_DATA_BUFFER", "_REPARSE_GUID_DATA_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TAPE_ERASE", aliases: &[
        CLibAliases { names: &["TAPE_ERASE", "_TAPE_ERASE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TAPE_GET_POSITION", aliases: &[
        CLibAliases { names: &["TAPE_GET_POSITION", "_TAPE_GET_POSITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TAPE_PREPARE", aliases: &[
        CLibAliases { names: &["TAPE_PREPARE", "_TAPE_PREPARE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TAPE_SET_POSITION", aliases: &[
        CLibAliases { names: &["TAPE_SET_POSITION", "_TAPE_SET_POSITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TAPE_WRITE_MARKS", aliases: &[
        CLibAliases { names: &["TAPE_WRITE_MARKS", "_TAPE_WRITE_MARKS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TRANSACTION_NOTIFICATION", aliases: &[
        CLibAliases { names: &["TRANSACTION_NOTIFICATION", "_TRANSACTION_NOTIFICATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TRANSACTION_NOTIFICATION_MARSHAL_ARGUMENT", aliases: &[
        CLibAliases { names: &["TRANSACTION_NOTIFICATION_MARSHAL_ARGUMENT", "_TRANSACTION_NOTIFICATION_MARSHAL_ARGUMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TRANSACTION_NOTIFICATION_PROPAGATE_ARGUMENT", aliases: &[
        CLibAliases { names: &["TRANSACTION_NOTIFICATION_PROPAGATE_ARGUMENT", "_TRANSACTION_NOTIFICATION_PROPAGATE_ARGUMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TRANSACTION_NOTIFICATION_RECOVERY_ARGUMENT", aliases: &[
        CLibAliases { names: &["TRANSACTION_NOTIFICATION_RECOVERY_ARGUMENT", "_TRANSACTION_NOTIFICATION_RECOVERY_ARGUMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TRANSACTION_NOTIFICATION_SAVEPOINT_ARGUMENT", aliases: &[
        CLibAliases { names: &["TRANSACTION_NOTIFICATION_SAVEPOINT_ARGUMENT", "_TRANSACTION_NOTIFICATION_SAVEPOINT_ARGUMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::TRANSACTION_NOTIFICATION_TM_ONLINE_ARGUMENT", aliases: &[
        CLibAliases { names: &["TRANSACTION_NOTIFICATION_TM_ONLINE_ARGUMENT", "_TRANSACTION_NOTIFICATION_TM_ONLINE_ARGUMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::VS_FIXEDFILEINFO", aliases: &[
        CLibAliases { names: &["VS_FIXEDFILEINFO", "tagVS_FIXEDFILEINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::WIN32_FILE_ATTRIBUTE_DATA", aliases: &[
        CLibAliases { names: &["WIN32_FILE_ATTRIBUTE_DATA", "_WIN32_FILE_ATTRIBUTE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::WIN32_FIND_DATAA", aliases: &[
        CLibAliases { names: &["WIN32_FIND_DATAA", "_WIN32_FIND_DATAA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::WIN32_FIND_DATAW", aliases: &[
        CLibAliases { names: &["WIN32_FIND_DATAW", "_WIN32_FIND_DATAW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::WIN32_FIND_STREAM_DATA", aliases: &[
        CLibAliases { names: &["WIN32_FIND_STREAM_DATA", "_WIN32_FIND_STREAM_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::FileSystem::WIN32_STREAM_ID", aliases: &[
        CLibAliases { names: &["WIN32_STREAM_ID", "_WIN32_STREAM_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::OperationRecorder::OPERATION_END_PARAMETERS", aliases: &[
        CLibAliases { names: &["OPERATION_END_PARAMETERS", "_OPERATION_END_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::OperationRecorder::OPERATION_START_PARAMETERS", aliases: &[
        CLibAliases { names: &["OPERATION_START_PARAMETERS", "_OPERATION_START_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Vhd::VIRTUAL_STORAGE_TYPE", aliases: &[
        CLibAliases { names: &["VIRTUAL_STORAGE_TYPE", "_VIRTUAL_STORAGE_TYPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Xps::ABORTPROC", aliases: &[
        CLibAliases { names: &["ABORTPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Xps::DOCINFOA", aliases: &[
        CLibAliases { names: &["DOCINFOA", "_DOCINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Xps::DOCINFOW", aliases: &[
        CLibAliases { names: &["DOCINFOW", "_DOCINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Xps::DRAWPATRECT", aliases: &[
        CLibAliases { names: &["DRAWPATRECT", "_DRAWPATRECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Xps::PSFEATURE_CUSTPAPER", aliases: &[
        CLibAliases { names: &["PSFEATURE_CUSTPAPER", "_PSFEATURE_CUSTPAPER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Xps::PSFEATURE_OUTPUT", aliases: &[
        CLibAliases { names: &["PSFEATURE_OUTPUT", "_PSFEATURE_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::Storage::Xps::PSINJECTDATA", aliases: &[
        CLibAliases { names: &["PSINJECTDATA", "_PSINJECTDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ACTCTXA", aliases: &[
        CLibAliases { names: &["ACTCTXA", "tagACTCTXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ACTCTXW", aliases: &[
        CLibAliases { names: &["ACTCTXW", "tagACTCTXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ACTCTX_SECTION_KEYED_DATA", aliases: &[
        CLibAliases { names: &["ACTCTX_SECTION_KEYED_DATA", "tagACTCTX_SECTION_KEYED_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION", aliases: &[
        CLibAliases { names: &["ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION", "_ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ACTIVATION_CONTEXT_DETAILED_INFORMATION", aliases: &[
        CLibAliases { names: &["ACTIVATION_CONTEXT_DETAILED_INFORMATION", "_ACTIVATION_CONTEXT_DETAILED_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ACTIVATION_CONTEXT_QUERY_INDEX", aliases: &[
        CLibAliases { names: &["ACTIVATION_CONTEXT_QUERY_INDEX", "_ACTIVATION_CONTEXT_QUERY_INDEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ACTIVATION_CONTEXT_RUN_LEVEL_INFORMATION", aliases: &[
        CLibAliases { names: &["ACTIVATION_CONTEXT_RUN_LEVEL_INFORMATION", "_ACTIVATION_CONTEXT_RUN_LEVEL_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::ASSEMBLY_FILE_DETAILED_INFORMATION", aliases: &[
        CLibAliases { names: &["ASSEMBLY_FILE_DETAILED_INFORMATION", "_ASSEMBLY_FILE_DETAILED_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::ApplicationInstallationAndServicing::COMPATIBILITY_CONTEXT_ELEMENT", aliases: &[
        CLibAliases { names: &["COMPATIBILITY_CONTEXT_ELEMENT", "_COMPATIBILITY_CONTEXT_ELEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::AUTHENTICATEINFO", aliases: &[
        CLibAliases { names: &["AUTHENTICATEINFO", "_tagAUTHENTICATEINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::BINDINFO", aliases: &[
        CLibAliases { names: &["BINDINFO", "_tagBINDINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::BINDPTR", aliases: &[
        CLibAliases { names: &["BINDPTR", "tagBINDPTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::BIND_OPTS", aliases: &[
        CLibAliases { names: &["BIND_OPTS", "tagBIND_OPTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::BLOB", aliases: &[
        CLibAliases { names: &["BLOB", "tagBLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::BYTE_BLOB", aliases: &[
        CLibAliases { names: &["BYTE_BLOB", "_BYTE_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::BYTE_SIZEDARR", aliases: &[
        CLibAliases { names: &["BYTE_SIZEDARR", "_BYTE_SIZEDARR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::COAUTHIDENTITY", aliases: &[
        CLibAliases { names: &["COAUTHIDENTITY", "_COAUTHIDENTITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::COAUTHINFO", aliases: &[
        CLibAliases { names: &["COAUTHINFO", "_COAUTHINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::COSERVERINFO", aliases: &[
        CLibAliases { names: &["COSERVERINFO", "_COSERVERINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::CO_DEVICE_CATALOG_COOKIE", aliases: &[
        CLibAliases { names: &["CO_DEVICE_CATALOG_COOKIE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::CO_MTA_USAGE_COOKIE", aliases: &[
        CLibAliases { names: &["CO_MTA_USAGE_COOKIE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::CSPLATFORM", aliases: &[
        CLibAliases { names: &["CSPLATFORM", "tagCSPLATFORM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::CUSTDATA", aliases: &[
        CLibAliases { names: &["CUSTDATA", "tagCUSTDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::CUSTDATAITEM", aliases: &[
        CLibAliases { names: &["CUSTDATAITEM", "tagCUSTDATAITEM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::CY", aliases: &[
        CLibAliases { names: &["CY", "tagCY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::DISPPARAMS", aliases: &[
        CLibAliases { names: &["DISPPARAMS", "tagDISPPARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::DVTARGETDEVICE", aliases: &[
        CLibAliases { names: &["DVTARGETDEVICE", "tagDVTARGETDEVICE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::DWORD_BLOB", aliases: &[
        CLibAliases { names: &["DWORD_BLOB", "_DWORD_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::DWORD_SIZEDARR", aliases: &[
        CLibAliases { names: &["DWORD_SIZEDARR", "_LONG_SIZEDARR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::ELEMDESC", aliases: &[
        CLibAliases { names: &["ELEMDESC", "tagELEMDESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::EXCEPINFO", aliases: &[
        CLibAliases { names: &["EXCEPINFO", "tagEXCEPINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::FLAGGED_BYTE_BLOB", aliases: &[
        CLibAliases { names: &["FLAGGED_BYTE_BLOB", "_FLAGGED_BYTE_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::FLAGGED_WORD_BLOB", aliases: &[
        CLibAliases { names: &["FLAGGED_WORD_BLOB", "_FLAGGED_WORD_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::FLAG_STGMEDIUM", aliases: &[
        CLibAliases { names: &["FLAG_STGMEDIUM", "_FLAG_STGMEDIUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::FORMATETC", aliases: &[
        CLibAliases { names: &["FORMATETC", "tagFORMATETC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::FUNCDESC", aliases: &[
        CLibAliases { names: &["FUNCDESC", "tagFUNCDESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::GDI_OBJECT", aliases: &[
        CLibAliases { names: &["GDI_OBJECT", "_GDI_OBJECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::HYPER_SIZEDARR", aliases: &[
        CLibAliases { names: &["HYPER_SIZEDARR", "_HYPER_SIZEDARR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::IDLDESC", aliases: &[
        CLibAliases { names: &["IDLDESC", "tagIDLDESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::INTERFACEINFO", aliases: &[
        CLibAliases { names: &["INTERFACEINFO", "tagINTERFACEINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::LPFNCANUNLOADNOW", aliases: &[
        CLibAliases { names: &["LPFNCANUNLOADNOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::LPFNGETCLASSOBJECT", aliases: &[
        CLibAliases { names: &["LPFNGETCLASSOBJECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::MULTI_QI", aliases: &[
        CLibAliases { names: &["MULTI_QI", "tagMULTI_QI"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::MachineGlobalObjectTableRegistrationToken", aliases: &[
        CLibAliases { names: &["MachineGlobalObjectTableRegistrationToken"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::QUERYCONTEXT", aliases: &[
        CLibAliases { names: &["QUERYCONTEXT", "tagQUERYCONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::RPCOLEMESSAGE", aliases: &[
        CLibAliases { names: &["RPCOLEMESSAGE", "tagRPCOLEMESSAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::RemSTGMEDIUM", aliases: &[
        CLibAliases { names: &["RemSTGMEDIUM", "tagRemSTGMEDIUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::SAFEARRAY", aliases: &[
        CLibAliases { names: &["SAFEARRAY", "tagSAFEARRAY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::SAFEARRAYBOUND", aliases: &[
        CLibAliases { names: &["SAFEARRAYBOUND", "tagSAFEARRAYBOUND"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::SChannelHookCallInfo", aliases: &[
        CLibAliases { names: &["SChannelHookCallInfo"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::SOLE_AUTHENTICATION_INFO", aliases: &[
        CLibAliases { names: &["SOLE_AUTHENTICATION_INFO", "tagSOLE_AUTHENTICATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::SOLE_AUTHENTICATION_LIST", aliases: &[
        CLibAliases { names: &["SOLE_AUTHENTICATION_LIST", "tagSOLE_AUTHENTICATION_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::SOLE_AUTHENTICATION_SERVICE", aliases: &[
        CLibAliases { names: &["SOLE_AUTHENTICATION_SERVICE", "tagSOLE_AUTHENTICATION_SERVICE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::STATDATA", aliases: &[
        CLibAliases { names: &["STATDATA", "tagSTATDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::STATSTG", aliases: &[
        CLibAliases { names: &["STATSTG", "tagSTATSTG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::STGMEDIUM", aliases: &[
        CLibAliases { names: &["STGMEDIUM", "tagSTGMEDIUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StorageLayout", aliases: &[
        CLibAliases { names: &["StorageLayout", "tagStorageLayout"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::BSTRBLOB", aliases: &[
        CLibAliases { names: &["BSTRBLOB", "tagBSTRBLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CABOOL", aliases: &[
        CLibAliases { names: &["CABOOL", "tagCABOOL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CABSTR", aliases: &[
        CLibAliases { names: &["CABSTR", "tagCABSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CABSTRBLOB", aliases: &[
        CLibAliases { names: &["CABSTRBLOB", "tagCABSTRBLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAC", aliases: &[
        CLibAliases { names: &["CAC", "tagCAC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CACLIPDATA", aliases: &[
        CLibAliases { names: &["CACLIPDATA", "tagCACLIPDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CACLSID", aliases: &[
        CLibAliases { names: &["CACLSID", "tagCACLSID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CACY", aliases: &[
        CLibAliases { names: &["CACY", "tagCACY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CADATE", aliases: &[
        CLibAliases { names: &["CADATE", "tagCADATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CADBL", aliases: &[
        CLibAliases { names: &["CADBL", "tagCADBL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAFILETIME", aliases: &[
        CLibAliases { names: &["CAFILETIME", "tagCAFILETIME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAFLT", aliases: &[
        CLibAliases { names: &["CAFLT", "tagCAFLT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAH", aliases: &[
        CLibAliases { names: &["CAH", "tagCAH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAI", aliases: &[
        CLibAliases { names: &["CAI", "tagCAI"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAL", aliases: &[
        CLibAliases { names: &["CAL", "tagCAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CALPSTR", aliases: &[
        CLibAliases { names: &["CALPSTR", "tagCALPSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CALPWSTR", aliases: &[
        CLibAliases { names: &["CALPWSTR", "tagCALPWSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAPROPVARIANT", aliases: &[
        CLibAliases { names: &["CAPROPVARIANT", "tagCAPROPVARIANT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CASCODE", aliases: &[
        CLibAliases { names: &["CASCODE", "tagCASCODE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAUB", aliases: &[
        CLibAliases { names: &["CAUB", "tagCAUB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAUH", aliases: &[
        CLibAliases { names: &["CAUH", "tagCAUH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAUI", aliases: &[
        CLibAliases { names: &["CAUI", "tagCAUI"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CAUL", aliases: &[
        CLibAliases { names: &["CAUL", "tagCAUL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::CLIPDATA", aliases: &[
        CLibAliases { names: &["CLIPDATA", "tagCLIPDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::OLESTREAM", aliases: &[
        CLibAliases { names: &["OLESTREAM", "_OLESTREAM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::OLESTREAMVTBL", aliases: &[
        CLibAliases { names: &["OLESTREAMVTBL", "_OLESTREAMVTBL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::PROPSPEC", aliases: &[
        CLibAliases { names: &["PROPSPEC", "tagPROPSPEC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::PROPVARIANT", aliases: &[
        CLibAliases { names: &["PROPVARIANT", "tagPROPVARIANT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::RemSNB", aliases: &[
        CLibAliases { names: &["RemSNB", "tagRemSNB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::SERIALIZEDPROPERTYVALUE", aliases: &[
        CLibAliases { names: &["SERIALIZEDPROPERTYVALUE", "tagSERIALIZEDPROPERTYVALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::STATPROPSETSTG", aliases: &[
        CLibAliases { names: &["STATPROPSETSTG", "tagSTATPROPSETSTG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::STATPROPSTG", aliases: &[
        CLibAliases { names: &["STATPROPSTG", "tagSTATPROPSTG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::STGOPTIONS", aliases: &[
        CLibAliases { names: &["STGOPTIONS", "tagSTGOPTIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::StructuredStorage::VERSIONEDSTREAM", aliases: &[
        CLibAliases { names: &["VERSIONEDSTREAM", "tagVersionedStream"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::TLIBATTR", aliases: &[
        CLibAliases { names: &["TLIBATTR", "tagTLIBATTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::TYPEATTR", aliases: &[
        CLibAliases { names: &["TYPEATTR", "tagTYPEATTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::TYPEDESC", aliases: &[
        CLibAliases { names: &["TYPEDESC", "tagTYPEDESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::CODEBASEHOLD", aliases: &[
        CLibAliases { names: &["CODEBASEHOLD", "_tagCODEBASEHOLD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::CONFIRMSAFETY", aliases: &[
        CLibAliases { names: &["CONFIRMSAFETY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::DATAINFO", aliases: &[
        CLibAliases { names: &["DATAINFO", "_tagDATAINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::HIT_LOGGING_INFO", aliases: &[
        CLibAliases { names: &["HIT_LOGGING_INFO", "_tagHIT_LOGGING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::PROTOCOLDATA", aliases: &[
        CLibAliases { names: &["PROTOCOLDATA", "_tagPROTOCOLDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::PROTOCOLFILTERDATA", aliases: &[
        CLibAliases { names: &["PROTOCOLFILTERDATA", "_tagPROTOCOLFILTERDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::PROTOCOL_ARGUMENT", aliases: &[
        CLibAliases { names: &["PROTOCOL_ARGUMENT", "_tagPROTOCOL_ARGUMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::REMSECURITY_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["REMSECURITY_ATTRIBUTES", "_REMSECURITY_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::RemBINDINFO", aliases: &[
        CLibAliases { names: &["RemBINDINFO", "_tagRemBINDINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::RemFORMATETC", aliases: &[
        CLibAliases { names: &["RemFORMATETC", "tagRemFORMATETC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::SOFTDISTINFO", aliases: &[
        CLibAliases { names: &["SOFTDISTINFO", "_tagSOFTDISTINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::StartParam", aliases: &[
        CLibAliases { names: &["StartParam", "_tagStartParam"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::Urlmon::ZONEATTRIBUTES", aliases: &[
        CLibAliases { names: &["ZONEATTRIBUTES", "_ZONEATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::VARDESC", aliases: &[
        CLibAliases { names: &["VARDESC", "tagVARDESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::WORD_BLOB", aliases: &[
        CLibAliases { names: &["WORD_BLOB", "_WORD_BLOB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::WORD_SIZEDARR", aliases: &[
        CLibAliases { names: &["WORD_SIZEDARR", "_SHORT_SIZEDARR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::uCLSSPEC", aliases: &[
        CLibAliases { names: &["__MIDL___MIDL_itf_wtypes_0000_0001_0001", "uCLSSPEC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::userFLAG_STGMEDIUM", aliases: &[
        CLibAliases { names: &["_userFLAG_STGMEDIUM", "userFLAG_STGMEDIUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Com::userSTGMEDIUM", aliases: &[
        CLibAliases { names: &["_userSTGMEDIUM", "userSTGMEDIUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CHAR_INFO", aliases: &[
        CLibAliases { names: &["CHAR_INFO", "_CHAR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_CURSOR_INFO", aliases: &[
        CLibAliases { names: &["CONSOLE_CURSOR_INFO", "_CONSOLE_CURSOR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_FONT_INFO", aliases: &[
        CLibAliases { names: &["CONSOLE_FONT_INFO", "_CONSOLE_FONT_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_FONT_INFOEX", aliases: &[
        CLibAliases { names: &["CONSOLE_FONT_INFOEX", "_CONSOLE_FONT_INFOEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_HISTORY_INFO", aliases: &[
        CLibAliases { names: &["CONSOLE_HISTORY_INFO", "_CONSOLE_HISTORY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_READCONSOLE_CONTROL", aliases: &[
        CLibAliases { names: &["CONSOLE_READCONSOLE_CONTROL", "_CONSOLE_READCONSOLE_CONTROL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_SCREEN_BUFFER_INFO", aliases: &[
        CLibAliases { names: &["CONSOLE_SCREEN_BUFFER_INFO", "_CONSOLE_SCREEN_BUFFER_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_SCREEN_BUFFER_INFOEX", aliases: &[
        CLibAliases { names: &["CONSOLE_SCREEN_BUFFER_INFOEX", "_CONSOLE_SCREEN_BUFFER_INFOEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::CONSOLE_SELECTION_INFO", aliases: &[
        CLibAliases { names: &["CONSOLE_SELECTION_INFO", "_CONSOLE_SELECTION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::COORD", aliases: &[
        CLibAliases { names: &["COORD", "_COORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::FOCUS_EVENT_RECORD", aliases: &[
        CLibAliases { names: &["FOCUS_EVENT_RECORD", "_FOCUS_EVENT_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::HPCON", aliases: &[
        CLibAliases { names: &["HPCON"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::INPUT_RECORD", aliases: &[
        CLibAliases { names: &["INPUT_RECORD", "_INPUT_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::KEY_EVENT_RECORD", aliases: &[
        CLibAliases { names: &["KEY_EVENT_RECORD", "_KEY_EVENT_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::MENU_EVENT_RECORD", aliases: &[
        CLibAliases { names: &["MENU_EVENT_RECORD", "_MENU_EVENT_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::MOUSE_EVENT_RECORD", aliases: &[
        CLibAliases { names: &["MOUSE_EVENT_RECORD", "_MOUSE_EVENT_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::PHANDLER_ROUTINE", aliases: &[
        CLibAliases { names: &["PHANDLER_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::SMALL_RECT", aliases: &[
        CLibAliases { names: &["SMALL_RECT", "_SMALL_RECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Console::WINDOW_BUFFER_SIZE_RECORD", aliases: &[
        CLibAliases { names: &["WINDOW_BUFFER_SIZE_RECORD", "_WINDOW_BUFFER_SIZE_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::CorrelationVector::CORRELATION_VECTOR", aliases: &[
        CLibAliases { names: &["CORRELATION_VECTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::CONVCONTEXT", aliases: &[
        CLibAliases { names: &["CONVCONTEXT", "tagCONVCONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::CONVINFO", aliases: &[
        CLibAliases { names: &["CONVINFO", "tagCONVINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::COPYDATASTRUCT", aliases: &[
        CLibAliases { names: &["COPYDATASTRUCT", "tagCOPYDATASTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::DDEML_MSG_HOOK_DATA", aliases: &[
        CLibAliases { names: &["DDEML_MSG_HOOK_DATA", "tagDDEML_MSG_HOOK_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::HCONV", aliases: &[
        CLibAliases { names: &["HCONV"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::HCONVLIST", aliases: &[
        CLibAliases { names: &["HCONVLIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::HDDEDATA", aliases: &[
        CLibAliases { names: &["HDDEDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::HSZ", aliases: &[
        CLibAliases { names: &["HSZ"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::HSZPAIR", aliases: &[
        CLibAliases { names: &["HSZPAIR", "tagHSZPAIR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::METAFILEPICT", aliases: &[
        CLibAliases { names: &["METAFILEPICT", "tagMETAFILEPICT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::MONCBSTRUCT", aliases: &[
        CLibAliases { names: &["MONCBSTRUCT", "tagMONCBSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::MONCONVSTRUCT", aliases: &[
        CLibAliases { names: &["MONCONVSTRUCT", "tagMONCONVSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::MONERRSTRUCT", aliases: &[
        CLibAliases { names: &["MONERRSTRUCT", "tagMONERRSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::MONHSZSTRUCTA", aliases: &[
        CLibAliases { names: &["MONHSZSTRUCTA", "tagMONHSZSTRUCTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::MONHSZSTRUCTW", aliases: &[
        CLibAliases { names: &["MONHSZSTRUCTW", "tagMONHSZSTRUCTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::MONLINKSTRUCT", aliases: &[
        CLibAliases { names: &["MONLINKSTRUCT", "tagMONLINKSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::MONMSGSTRUCT", aliases: &[
        CLibAliases { names: &["MONMSGSTRUCT", "tagMONMSGSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::DataExchange::PFNCALLBACK", aliases: &[
        CLibAliases { names: &["PFNCALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::APC_CALLBACK_DATA", aliases: &[
        CLibAliases { names: &["APC_CALLBACK_DATA", "_APC_CALLBACK_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::ARM64_NT_NEON128", aliases: &[
        CLibAliases { names: &["ARM64_NT_NEON128", "_ARM64_NT_NEON128"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::CONTEXT", aliases: &[
        CLibAliases { names: &["CONTEXT", "_CONTEXT"], targets: &["i686-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::CREATE_PROCESS_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["CREATE_PROCESS_DEBUG_INFO", "_CREATE_PROCESS_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::CREATE_THREAD_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["CREATE_THREAD_DEBUG_INFO", "_CREATE_THREAD_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::DEBUG_EVENT", aliases: &[
        CLibAliases { names: &["DEBUG_EVENT", "_DEBUG_EVENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::DISPATCHER_CONTEXT", aliases: &[
        CLibAliases { names: &["DISPATCHER_CONTEXT", "_DISPATCHER_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::EXCEPTION_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["EXCEPTION_DEBUG_INFO", "_EXCEPTION_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::EXCEPTION_POINTERS", aliases: &[
        CLibAliases { names: &["EXCEPTION_POINTERS", "_EXCEPTION_POINTERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::EXCEPTION_RECORD", aliases: &[
        CLibAliases { names: &["EXCEPTION_RECORD", "_EXCEPTION_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::EXCEPTION_RECORD32", aliases: &[
        CLibAliases { names: &["EXCEPTION_RECORD32", "_EXCEPTION_RECORD32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::EXCEPTION_RECORD64", aliases: &[
        CLibAliases { names: &["EXCEPTION_RECORD64", "_EXCEPTION_RECORD64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::EXIT_PROCESS_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["EXIT_PROCESS_DEBUG_INFO", "_EXIT_PROCESS_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::EXIT_THREAD_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["EXIT_THREAD_DEBUG_INFO", "_EXIT_THREAD_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_ARM64_RUNTIME_FUNCTION_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_ARM64_RUNTIME_FUNCTION_ENTRY", "_IMAGE_ARM64_RUNTIME_FUNCTION_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_COFF_SYMBOLS_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_COFF_SYMBOLS_HEADER", "_IMAGE_COFF_SYMBOLS_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_COR20_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_COR20_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_DATA_DIRECTORY", aliases: &[
        CLibAliases { names: &["IMAGE_DATA_DIRECTORY", "_IMAGE_DATA_DIRECTORY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_DEBUG_DIRECTORY", aliases: &[
        CLibAliases { names: &["IMAGE_DEBUG_DIRECTORY", "_IMAGE_DEBUG_DIRECTORY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_FILE_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_FILE_HEADER", "_IMAGE_FILE_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_FUNCTION_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_FUNCTION_ENTRY", "_IMAGE_FUNCTION_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_FUNCTION_ENTRY64", aliases: &[
        CLibAliases { names: &["IMAGE_FUNCTION_ENTRY64", "_IMAGE_FUNCTION_ENTRY64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_LOAD_CONFIG_CODE_INTEGRITY", aliases: &[
        CLibAliases { names: &["IMAGE_LOAD_CONFIG_CODE_INTEGRITY", "_IMAGE_LOAD_CONFIG_CODE_INTEGRITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_NT_HEADERS32", aliases: &[
        CLibAliases { names: &["IMAGE_NT_HEADERS32", "_IMAGE_NT_HEADERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_NT_HEADERS64", aliases: &[
        CLibAliases { names: &["IMAGE_NT_HEADERS64", "_IMAGE_NT_HEADERS64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_OPTIONAL_HEADER32", aliases: &[
        CLibAliases { names: &["IMAGE_OPTIONAL_HEADER32", "_IMAGE_OPTIONAL_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_OPTIONAL_HEADER64", aliases: &[
        CLibAliases { names: &["IMAGE_OPTIONAL_HEADER64", "_IMAGE_OPTIONAL_HEADER64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_ROM_HEADERS", aliases: &[
        CLibAliases { names: &["IMAGE_ROM_HEADERS", "_IMAGE_ROM_HEADERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_ROM_OPTIONAL_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_ROM_OPTIONAL_HEADER", "_IMAGE_ROM_OPTIONAL_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_RUNTIME_FUNCTION_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_RUNTIME_FUNCTION_ENTRY", "_IMAGE_RUNTIME_FUNCTION_ENTRY"], targets: &["i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::IMAGE_SECTION_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_SECTION_HEADER", "_IMAGE_SECTION_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::KNONVOLATILE_CONTEXT_POINTERS", aliases: &[
        CLibAliases { names: &["_KNONVOLATILE_CONTEXT_POINTERS_ARM64"], targets: &["aarch64-pc-windows-msvc"] },
        CLibAliases { names: &["KNONVOLATILE_CONTEXT_POINTERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
        CLibAliases { names: &["_KNONVOLATILE_CONTEXT_POINTERS"], targets: &["i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::LDT_ENTRY", aliases: &[
        CLibAliases { names: &["LDT_ENTRY", "_LDT_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::LOAD_DLL_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["LOAD_DLL_DEBUG_INFO", "_LOAD_DLL_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::LPTOP_LEVEL_EXCEPTION_FILTER", aliases: &[
        CLibAliases { names: &["LPTOP_LEVEL_EXCEPTION_FILTER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::OUTPUT_DEBUG_STRING_INFO", aliases: &[
        CLibAliases { names: &["OUTPUT_DEBUG_STRING_INFO", "_OUTPUT_DEBUG_STRING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::PGET_RUNTIME_FUNCTION_CALLBACK", aliases: &[
        CLibAliases { names: &["PGET_RUNTIME_FUNCTION_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::PVECTORED_EXCEPTION_HANDLER", aliases: &[
        CLibAliases { names: &["PVECTORED_EXCEPTION_HANDLER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::RIP_INFO", aliases: &[
        CLibAliases { names: &["RIP_INFO", "_RIP_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::UNLOAD_DLL_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["UNLOAD_DLL_DEBUG_INFO", "_UNLOAD_DLL_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::UNWIND_HISTORY_TABLE", aliases: &[
        CLibAliases { names: &["UNWIND_HISTORY_TABLE", "_UNWIND_HISTORY_TABLE"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::UNWIND_HISTORY_TABLE_ENTRY", aliases: &[
        CLibAliases { names: &["UNWIND_HISTORY_TABLE_ENTRY", "_UNWIND_HISTORY_TABLE_ENTRY"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::WOW64_CONTEXT", aliases: &[
        CLibAliases { names: &["WOW64_CONTEXT", "_WOW64_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::WOW64_DESCRIPTOR_TABLE_ENTRY", aliases: &[
        CLibAliases { names: &["WOW64_DESCRIPTOR_TABLE_ENTRY", "_WOW64_DESCRIPTOR_TABLE_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::WOW64_FLOATING_SAVE_AREA", aliases: &[
        CLibAliases { names: &["WOW64_FLOATING_SAVE_AREA", "_WOW64_FLOATING_SAVE_AREA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::WOW64_LDT_ENTRY", aliases: &[
        CLibAliases { names: &["WOW64_LDT_ENTRY", "_WOW64_LDT_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::XSAVE_AREA_HEADER", aliases: &[
        CLibAliases { names: &["XSAVE_AREA_HEADER", "_XSAVE_AREA_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Diagnostics::Debug::XSTATE_FEATURE", aliases: &[
        CLibAliases { names: &["XSTATE_FEATURE", "_XSTATE_FEATURE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::EventLog::EVENTLOGRECORD", aliases: &[
        CLibAliases { names: &["EVENTLOGRECORD", "_EVENTLOGRECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::EventLog::EVENTLOG_FULL_INFORMATION", aliases: &[
        CLibAliases { names: &["EVENTLOG_FULL_INFORMATION", "_EVENTLOG_FULL_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::IO::LPOVERLAPPED_COMPLETION_ROUTINE", aliases: &[
        CLibAliases { names: &["LPOVERLAPPED_COMPLETION_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::IO::OVERLAPPED", aliases: &[
        CLibAliases { names: &["OVERLAPPED", "_OVERLAPPED"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::IO::OVERLAPPED_ENTRY", aliases: &[
        CLibAliases { names: &["OVERLAPPED_ENTRY", "_OVERLAPPED_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::ASYNC_DUPLICATE_EXTENTS_STATUS", aliases: &[
        CLibAliases { names: &["ASYNC_DUPLICATE_EXTENTS_STATUS", "_ASYNC_DUPLICATE_EXTENTS_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::BIN_COUNT", aliases: &[
        CLibAliases { names: &["BIN_COUNT", "_BIN_COUNT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::BIN_RANGE", aliases: &[
        CLibAliases { names: &["BIN_RANGE", "_BIN_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::BIN_RESULTS", aliases: &[
        CLibAliases { names: &["BIN_RESULTS", "_BIN_RESULTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::BOOT_AREA_INFO", aliases: &[
        CLibAliases { names: &["BOOT_AREA_INFO", "_BOOT_AREA_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::BULK_SECURITY_TEST_DATA", aliases: &[
        CLibAliases { names: &["BULK_SECURITY_TEST_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_ELEMENT", aliases: &[
        CLibAliases { names: &["CHANGER_ELEMENT", "_CHANGER_ELEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_ELEMENT_LIST", aliases: &[
        CLibAliases { names: &["CHANGER_ELEMENT_LIST", "_CHANGER_ELEMENT_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_ELEMENT_STATUS", aliases: &[
        CLibAliases { names: &["CHANGER_ELEMENT_STATUS", "_CHANGER_ELEMENT_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_ELEMENT_STATUS_EX", aliases: &[
        CLibAliases { names: &["CHANGER_ELEMENT_STATUS_EX", "_CHANGER_ELEMENT_STATUS_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_EXCHANGE_MEDIUM", aliases: &[
        CLibAliases { names: &["CHANGER_EXCHANGE_MEDIUM", "_CHANGER_EXCHANGE_MEDIUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_INITIALIZE_ELEMENT_STATUS", aliases: &[
        CLibAliases { names: &["CHANGER_INITIALIZE_ELEMENT_STATUS", "_CHANGER_INITIALIZE_ELEMENT_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_MOVE_MEDIUM", aliases: &[
        CLibAliases { names: &["CHANGER_MOVE_MEDIUM", "_CHANGER_MOVE_MEDIUM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_PRODUCT_DATA", aliases: &[
        CLibAliases { names: &["CHANGER_PRODUCT_DATA", "_CHANGER_PRODUCT_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_READ_ELEMENT_STATUS", aliases: &[
        CLibAliases { names: &["CHANGER_READ_ELEMENT_STATUS", "_CHANGER_READ_ELEMENT_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_SEND_VOLUME_TAG_INFORMATION", aliases: &[
        CLibAliases { names: &["CHANGER_SEND_VOLUME_TAG_INFORMATION", "_CHANGER_SEND_VOLUME_TAG_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_SET_ACCESS", aliases: &[
        CLibAliases { names: &["CHANGER_SET_ACCESS", "_CHANGER_SET_ACCESS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CHANGER_SET_POSITION", aliases: &[
        CLibAliases { names: &["CHANGER_SET_POSITION", "_CHANGER_SET_POSITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CLASS_MEDIA_CHANGE_CONTEXT", aliases: &[
        CLibAliases { names: &["CLASS_MEDIA_CHANGE_CONTEXT", "_CLASS_MEDIA_CHANGE_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CLUSTER_RANGE", aliases: &[
        CLibAliases { names: &["CLUSTER_RANGE", "_CLUSTER_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CONTAINER_ROOT_INFO_INPUT", aliases: &[
        CLibAliases { names: &["CONTAINER_ROOT_INFO_INPUT", "_CONTAINER_ROOT_INFO_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CONTAINER_ROOT_INFO_OUTPUT", aliases: &[
        CLibAliases { names: &["CONTAINER_ROOT_INFO_OUTPUT", "_CONTAINER_ROOT_INFO_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CONTAINER_VOLUME_STATE", aliases: &[
        CLibAliases { names: &["CONTAINER_VOLUME_STATE", "_CONTAINER_VOLUME_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CREATE_DISK", aliases: &[
        CLibAliases { names: &["CREATE_DISK", "_CREATE_DISK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CREATE_DISK_GPT", aliases: &[
        CLibAliases { names: &["CREATE_DISK_GPT", "_CREATE_DISK_GPT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CREATE_DISK_MBR", aliases: &[
        CLibAliases { names: &["CREATE_DISK_MBR", "_CREATE_DISK_MBR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CREATE_USN_JOURNAL_DATA", aliases: &[
        CLibAliases { names: &["CREATE_USN_JOURNAL_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_CONTROL_PARAM", aliases: &[
        CLibAliases { names: &["CSV_CONTROL_PARAM", "_CSV_CONTROL_PARAM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_IS_OWNED_BY_CSVFS", aliases: &[
        CLibAliases { names: &["CSV_IS_OWNED_BY_CSVFS", "_CSV_IS_OWNED_BY_CSVFS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_MGMT_LOCK", aliases: &[
        CLibAliases { names: &["CSV_MGMT_LOCK", "_CSV_MGMT_LOCK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_NAMESPACE_INFO", aliases: &[
        CLibAliases { names: &["CSV_NAMESPACE_INFO", "_CSV_NAMESPACE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_FILE_REVISION", aliases: &[
        CLibAliases { names: &["CSV_QUERY_FILE_REVISION", "_CSV_QUERY_FILE_REVISION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_FILE_REVISION_FILE_ID_128", aliases: &[
        CLibAliases { names: &["CSV_QUERY_FILE_REVISION_FILE_ID_128", "_CSV_QUERY_FILE_REVISION_FILE_ID_128"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_MDS_PATH", aliases: &[
        CLibAliases { names: &["CSV_QUERY_MDS_PATH", "_CSV_QUERY_MDS_PATH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_MDS_PATH_V2", aliases: &[
        CLibAliases { names: &["CSV_QUERY_MDS_PATH_V2", "_CSV_QUERY_MDS_PATH_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_REDIRECT_STATE", aliases: &[
        CLibAliases { names: &["CSV_QUERY_REDIRECT_STATE", "_CSV_QUERY_REDIRECT_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_VETO_FILE_DIRECT_IO_OUTPUT", aliases: &[
        CLibAliases { names: &["CSV_QUERY_VETO_FILE_DIRECT_IO_OUTPUT", "_CSV_QUERY_VETO_FILE_DIRECT_IO_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_VOLUME_ID", aliases: &[
        CLibAliases { names: &["CSV_QUERY_VOLUME_ID", "_CSV_QUERY_VOLUME_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_QUERY_VOLUME_REDIRECT_STATE", aliases: &[
        CLibAliases { names: &["CSV_QUERY_VOLUME_REDIRECT_STATE", "_CSV_QUERY_VOLUME_REDIRECT_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::CSV_SET_VOLUME_ID", aliases: &[
        CLibAliases { names: &["CSV_SET_VOLUME_ID", "_CSV_SET_VOLUME_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DECRYPTION_STATUS_BUFFER", aliases: &[
        CLibAliases { names: &["DECRYPTION_STATUS_BUFFER", "_DECRYPTION_STATUS_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DELETE_USN_JOURNAL_DATA", aliases: &[
        CLibAliases { names: &["DELETE_USN_JOURNAL_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_PRIVATE_SUBSECTION", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_PRIVATE_SUBSECTION", "_DEVICEDUMP_PRIVATE_SUBSECTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_PUBLIC_SUBSECTION", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_PUBLIC_SUBSECTION", "_DEVICEDUMP_PUBLIC_SUBSECTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_RESTRICTED_SUBSECTION", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_RESTRICTED_SUBSECTION", "_DEVICEDUMP_RESTRICTED_SUBSECTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_SECTION_HEADER", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_SECTION_HEADER", "_DEVICEDUMP_SECTION_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_STORAGEDEVICE_DATA", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_STORAGEDEVICE_DATA", "_DEVICEDUMP_STORAGEDEVICE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_STORAGESTACK_PUBLIC_DUMP", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_STORAGESTACK_PUBLIC_DUMP", "_DEVICEDUMP_STORAGESTACK_PUBLIC_DUMP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_STORAGESTACK_PUBLIC_STATE_RECORD", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_STORAGESTACK_PUBLIC_STATE_RECORD", "_DEVICEDUMP_STORAGESTACK_PUBLIC_STATE_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_STRUCTURE_VERSION", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_STRUCTURE_VERSION", "_DEVICEDUMP_STRUCTURE_VERSION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICEDUMP_SUBSECTION_POINTER", aliases: &[
        CLibAliases { names: &["DEVICEDUMP_SUBSECTION_POINTER", "_DEVICEDUMP_SUBSECTION_POINTER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_COPY_OFFLOAD_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["DEVICE_COPY_OFFLOAD_DESCRIPTOR", "_DEVICE_COPY_OFFLOAD_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_LBP_STATE_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_LBP_STATE_PARAMETERS", "_DEVICE_DATA_SET_LBP_STATE_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_LB_PROVISIONING_STATE", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_LB_PROVISIONING_STATE", "_DEVICE_DATA_SET_LB_PROVISIONING_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_LB_PROVISIONING_STATE_V2", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_LB_PROVISIONING_STATE_V2", "_DEVICE_DATA_SET_LB_PROVISIONING_STATE_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_RANGE", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_RANGE", "_DEVICE_DATA_SET_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_REPAIR_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_REPAIR_OUTPUT", "_DEVICE_DATA_SET_REPAIR_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_REPAIR_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_REPAIR_PARAMETERS", "_DEVICE_DATA_SET_REPAIR_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_SCRUB_EX_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_SCRUB_EX_OUTPUT", "_DEVICE_DATA_SET_SCRUB_EX_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_SCRUB_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_SCRUB_OUTPUT", "_DEVICE_DATA_SET_SCRUB_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DATA_SET_TOPOLOGY_ID_QUERY_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DATA_SET_TOPOLOGY_ID_QUERY_OUTPUT", "_DEVICE_DATA_SET_TOPOLOGY_ID_QUERY_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_CONVERSION_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_CONVERSION_OUTPUT", "_DEVICE_DSM_CONVERSION_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_DEFINITION", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_DEFINITION", "_DEVICE_DSM_DEFINITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_FREE_SPACE_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_FREE_SPACE_OUTPUT", "_DEVICE_DSM_FREE_SPACE_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_LOST_QUERY_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_LOST_QUERY_OUTPUT", "_DEVICE_DSM_LOST_QUERY_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_LOST_QUERY_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_LOST_QUERY_PARAMETERS", "_DEVICE_DSM_LOST_QUERY_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_NOTIFICATION_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_NOTIFICATION_PARAMETERS", "_DEVICE_DSM_NOTIFICATION_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_NVCACHE_CHANGE_PRIORITY_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_NVCACHE_CHANGE_PRIORITY_PARAMETERS", "_DEVICE_DSM_NVCACHE_CHANGE_PRIORITY_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_OFFLOAD_READ_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_OFFLOAD_READ_PARAMETERS", "_DEVICE_DSM_OFFLOAD_READ_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_OFFLOAD_WRITE_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_OFFLOAD_WRITE_PARAMETERS", "_DEVICE_DSM_OFFLOAD_WRITE_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_PHYSICAL_ADDRESSES_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_PHYSICAL_ADDRESSES_OUTPUT", "_DEVICE_DSM_PHYSICAL_ADDRESSES_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_RANGE_ERROR_INFO", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_RANGE_ERROR_INFO", "_DEVICE_DSM_RANGE_ERROR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_REPORT_ZONES_DATA", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_REPORT_ZONES_DATA", "_DEVICE_DSM_REPORT_ZONES_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_REPORT_ZONES_PARAMETERS", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_REPORT_ZONES_PARAMETERS", "_DEVICE_DSM_REPORT_ZONES_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_TIERING_QUERY_INPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_TIERING_QUERY_INPUT", "_DEVICE_DSM_TIERING_QUERY_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_DSM_TIERING_QUERY_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_DSM_TIERING_QUERY_OUTPUT", "_DEVICE_DSM_TIERING_QUERY_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_INTERNAL_STATUS_DATA", aliases: &[
        CLibAliases { names: &["DEVICE_INTERNAL_STATUS_DATA", "_DEVICE_INTERNAL_STATUS_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_LOCATION", aliases: &[
        CLibAliases { names: &["DEVICE_LOCATION", "_DEVICE_LOCATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_MANAGE_DATA_SET_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["DEVICE_MANAGE_DATA_SET_ATTRIBUTES", "_DEVICE_MANAGE_DATA_SET_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_MANAGE_DATA_SET_ATTRIBUTES_OUTPUT", aliases: &[
        CLibAliases { names: &["DEVICE_MANAGE_DATA_SET_ATTRIBUTES_OUTPUT", "_DEVICE_MANAGE_DATA_SET_ATTRIBUTES_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_MEDIA_INFO", aliases: &[
        CLibAliases { names: &["DEVICE_MEDIA_INFO", "_DEVICE_MEDIA_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_POWER_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["DEVICE_POWER_DESCRIPTOR", "_DEVICE_POWER_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_SEEK_PENALTY_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["DEVICE_SEEK_PENALTY_DESCRIPTOR", "_DEVICE_SEEK_PENALTY_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_STORAGE_ADDRESS_RANGE", aliases: &[
        CLibAliases { names: &["DEVICE_STORAGE_ADDRESS_RANGE", "_DEVICE_STORAGE_ADDRESS_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_STORAGE_RANGE_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["DEVICE_STORAGE_RANGE_ATTRIBUTES", "_DEVICE_STORAGE_RANGE_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_TRIM_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["DEVICE_TRIM_DESCRIPTOR", "_DEVICE_TRIM_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DEVICE_WRITE_AGGREGATION_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["DEVICE_WRITE_AGGREGATION_DESCRIPTOR", "_DEVICE_WRITE_AGGREGATION_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_CACHE_INFORMATION", aliases: &[
        CLibAliases { names: &["DISK_CACHE_INFORMATION", "_DISK_CACHE_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_CONTROLLER_NUMBER", aliases: &[
        CLibAliases { names: &["DISK_CONTROLLER_NUMBER", "_DISK_CONTROLLER_NUMBER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_DETECTION_INFO", aliases: &[
        CLibAliases { names: &["DISK_DETECTION_INFO", "_DISK_DETECTION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_EXTENT", aliases: &[
        CLibAliases { names: &["DISK_EXTENT", "_DISK_EXTENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_EX_INT13_INFO", aliases: &[
        CLibAliases { names: &["DISK_EX_INT13_INFO", "_DISK_EX_INT13_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_GEOMETRY", aliases: &[
        CLibAliases { names: &["DISK_GEOMETRY", "_DISK_GEOMETRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_GEOMETRY_EX", aliases: &[
        CLibAliases { names: &["DISK_GEOMETRY_EX", "_DISK_GEOMETRY_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_GROW_PARTITION", aliases: &[
        CLibAliases { names: &["DISK_GROW_PARTITION", "_DISK_GROW_PARTITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_HISTOGRAM", aliases: &[
        CLibAliases { names: &["DISK_HISTOGRAM", "_DISK_HISTOGRAM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_INT13_INFO", aliases: &[
        CLibAliases { names: &["DISK_INT13_INFO", "_DISK_INT13_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_LOGGING", aliases: &[
        CLibAliases { names: &["DISK_LOGGING", "_DISK_LOGGING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_PARTITION_INFO", aliases: &[
        CLibAliases { names: &["DISK_PARTITION_INFO", "_DISK_PARTITION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_PERFORMANCE", aliases: &[
        CLibAliases { names: &["DISK_PERFORMANCE", "_DISK_PERFORMANCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DISK_RECORD", aliases: &[
        CLibAliases { names: &["DISK_RECORD", "_DISK_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DRIVERSTATUS", aliases: &[
        CLibAliases { names: &["DRIVERSTATUS", "_DRIVERSTATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DRIVE_LAYOUT_INFORMATION", aliases: &[
        CLibAliases { names: &["DRIVE_LAYOUT_INFORMATION", "_DRIVE_LAYOUT_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DRIVE_LAYOUT_INFORMATION_EX", aliases: &[
        CLibAliases { names: &["DRIVE_LAYOUT_INFORMATION_EX", "_DRIVE_LAYOUT_INFORMATION_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DRIVE_LAYOUT_INFORMATION_GPT", aliases: &[
        CLibAliases { names: &["DRIVE_LAYOUT_INFORMATION_GPT", "_DRIVE_LAYOUT_INFORMATION_GPT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DRIVE_LAYOUT_INFORMATION_MBR", aliases: &[
        CLibAliases { names: &["DRIVE_LAYOUT_INFORMATION_MBR", "_DRIVE_LAYOUT_INFORMATION_MBR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DUPLICATE_EXTENTS_DATA", aliases: &[
        CLibAliases { names: &["DUPLICATE_EXTENTS_DATA", "_DUPLICATE_EXTENTS_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DUPLICATE_EXTENTS_DATA32", aliases: &[
        CLibAliases { names: &["DUPLICATE_EXTENTS_DATA32", "_DUPLICATE_EXTENTS_DATA32"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DUPLICATE_EXTENTS_DATA_EX", aliases: &[
        CLibAliases { names: &["DUPLICATE_EXTENTS_DATA_EX", "_DUPLICATE_EXTENTS_DATA_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::DUPLICATE_EXTENTS_DATA_EX32", aliases: &[
        CLibAliases { names: &["DUPLICATE_EXTENTS_DATA_EX32", "_DUPLICATE_EXTENTS_DATA_EX32"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::ENCRYPTED_DATA_INFO", aliases: &[
        CLibAliases { names: &["ENCRYPTED_DATA_INFO", "_ENCRYPTED_DATA_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::ENCRYPTION_BUFFER", aliases: &[
        CLibAliases { names: &["ENCRYPTION_BUFFER", "_ENCRYPTION_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::ENCRYPTION_KEY_CTRL_INPUT", aliases: &[
        CLibAliases { names: &["ENCRYPTION_KEY_CTRL_INPUT", "_ENCRYPTION_KEY_CTRL_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::EXFAT_STATISTICS", aliases: &[
        CLibAliases { names: &["EXFAT_STATISTICS", "_EXFAT_STATISTICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::EXTENDED_ENCRYPTED_DATA_INFO", aliases: &[
        CLibAliases { names: &["EXTENDED_ENCRYPTED_DATA_INFO", "_EXTENDED_ENCRYPTED_DATA_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FAT_STATISTICS", aliases: &[
        CLibAliases { names: &["FAT_STATISTICS", "_FAT_STATISTICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILESYSTEM_STATISTICS", aliases: &[
        CLibAliases { names: &["FILESYSTEM_STATISTICS", "_FILESYSTEM_STATISTICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILESYSTEM_STATISTICS_EX", aliases: &[
        CLibAliases { names: &["FILESYSTEM_STATISTICS_EX", "_FILESYSTEM_STATISTICS_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_ALLOCATED_RANGE_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_ALLOCATED_RANGE_BUFFER", "_FILE_ALLOCATED_RANGE_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_DESIRED_STORAGE_CLASS_INFORMATION", aliases: &[
        CLibAliases { names: &["FILE_DESIRED_STORAGE_CLASS_INFORMATION", "_FILE_DESIRED_STORAGE_CLASS_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_FS_PERSISTENT_VOLUME_INFORMATION", aliases: &[
        CLibAliases { names: &["FILE_FS_PERSISTENT_VOLUME_INFORMATION", "_FILE_FS_PERSISTENT_VOLUME_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_INITIATE_REPAIR_OUTPUT_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_INITIATE_REPAIR_OUTPUT_BUFFER", "_FILE_INITIATE_REPAIR_OUTPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_LAYOUT_ENTRY", aliases: &[
        CLibAliases { names: &["FILE_LAYOUT_ENTRY", "_FILE_LAYOUT_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_LAYOUT_INFO_ENTRY", aliases: &[
        CLibAliases { names: &["FILE_LAYOUT_INFO_ENTRY", "_FILE_LAYOUT_INFO_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_LAYOUT_NAME_ENTRY", aliases: &[
        CLibAliases { names: &["FILE_LAYOUT_NAME_ENTRY", "_FILE_LAYOUT_NAME_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_LEVEL_TRIM", aliases: &[
        CLibAliases { names: &["FILE_LEVEL_TRIM", "_FILE_LEVEL_TRIM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_LEVEL_TRIM_OUTPUT", aliases: &[
        CLibAliases { names: &["FILE_LEVEL_TRIM_OUTPUT", "_FILE_LEVEL_TRIM_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_LEVEL_TRIM_RANGE", aliases: &[
        CLibAliases { names: &["FILE_LEVEL_TRIM_RANGE", "_FILE_LEVEL_TRIM_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_MAKE_COMPATIBLE_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_MAKE_COMPATIBLE_BUFFER", "_FILE_MAKE_COMPATIBLE_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_OBJECTID_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_OBJECTID_BUFFER", "_FILE_OBJECTID_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_PREFETCH", aliases: &[
        CLibAliases { names: &["FILE_PREFETCH", "_FILE_PREFETCH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_PREFETCH_EX", aliases: &[
        CLibAliases { names: &["FILE_PREFETCH_EX", "_FILE_PREFETCH_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_PROVIDER_EXTERNAL_INFO_V0", aliases: &[
        CLibAliases { names: &["FILE_PROVIDER_EXTERNAL_INFO_V0", "_FILE_PROVIDER_EXTERNAL_INFO_V0"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_PROVIDER_EXTERNAL_INFO_V1", aliases: &[
        CLibAliases { names: &["FILE_PROVIDER_EXTERNAL_INFO_V1", "_FILE_PROVIDER_EXTERNAL_INFO_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_QUERY_ON_DISK_VOL_INFO_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_QUERY_ON_DISK_VOL_INFO_BUFFER", "_FILE_QUERY_ON_DISK_VOL_INFO_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_QUERY_SPARING_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_QUERY_SPARING_BUFFER", "_FILE_QUERY_SPARING_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_REFERENCE_RANGE", aliases: &[
        CLibAliases { names: &["FILE_REFERENCE_RANGE", "_FILE_REFERENCE_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_REGION_INFO", aliases: &[
        CLibAliases { names: &["FILE_REGION_INFO", "_FILE_REGION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_REGION_INPUT", aliases: &[
        CLibAliases { names: &["FILE_REGION_INPUT", "_FILE_REGION_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_REGION_OUTPUT", aliases: &[
        CLibAliases { names: &["FILE_REGION_OUTPUT", "_FILE_REGION_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_SET_DEFECT_MGMT_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_SET_DEFECT_MGMT_BUFFER", "_FILE_SET_DEFECT_MGMT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_SET_SPARSE_BUFFER", aliases: &[
        CLibAliases { names: &["FILE_SET_SPARSE_BUFFER", "_FILE_SET_SPARSE_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_STORAGE_TIER", aliases: &[
        CLibAliases { names: &["FILE_STORAGE_TIER", "_FILE_STORAGE_TIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_STORAGE_TIER_REGION", aliases: &[
        CLibAliases { names: &["FILE_STORAGE_TIER_REGION", "_FILE_STORAGE_TIER_REGION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_SYSTEM_RECOGNITION_INFORMATION", aliases: &[
        CLibAliases { names: &["FILE_SYSTEM_RECOGNITION_INFORMATION", "_FILE_SYSTEM_RECOGNITION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_TYPE_NOTIFICATION_INPUT", aliases: &[
        CLibAliases { names: &["FILE_TYPE_NOTIFICATION_INPUT", "_FILE_TYPE_NOTIFICATION_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_ZERO_DATA_INFORMATION", aliases: &[
        CLibAliases { names: &["FILE_ZERO_DATA_INFORMATION", "_FILE_ZERO_DATA_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FILE_ZERO_DATA_INFORMATION_EX", aliases: &[
        CLibAliases { names: &["FILE_ZERO_DATA_INFORMATION_EX", "_FILE_ZERO_DATA_INFORMATION_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FIND_BY_SID_DATA", aliases: &[
        CLibAliases { names: &["FIND_BY_SID_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FIND_BY_SID_OUTPUT", aliases: &[
        CLibAliases { names: &["FIND_BY_SID_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FORMAT_EX_PARAMETERS", aliases: &[
        CLibAliases { names: &["FORMAT_EX_PARAMETERS", "_FORMAT_EX_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FORMAT_PARAMETERS", aliases: &[
        CLibAliases { names: &["FORMAT_PARAMETERS", "_FORMAT_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_GET_INTEGRITY_INFORMATION_BUFFER", aliases: &[
        CLibAliases { names: &["FSCTL_GET_INTEGRITY_INFORMATION_BUFFER", "_FSCTL_GET_INTEGRITY_INFORMATION_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_OFFLOAD_READ_INPUT", aliases: &[
        CLibAliases { names: &["FSCTL_OFFLOAD_READ_INPUT", "_FSCTL_OFFLOAD_READ_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_OFFLOAD_READ_OUTPUT", aliases: &[
        CLibAliases { names: &["FSCTL_OFFLOAD_READ_OUTPUT", "_FSCTL_OFFLOAD_READ_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_OFFLOAD_WRITE_INPUT", aliases: &[
        CLibAliases { names: &["FSCTL_OFFLOAD_WRITE_INPUT", "_FSCTL_OFFLOAD_WRITE_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_OFFLOAD_WRITE_OUTPUT", aliases: &[
        CLibAliases { names: &["FSCTL_OFFLOAD_WRITE_OUTPUT", "_FSCTL_OFFLOAD_WRITE_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_QUERY_FAT_BPB_BUFFER", aliases: &[
        CLibAliases { names: &["FSCTL_QUERY_FAT_BPB_BUFFER", "_FSCTL_QUERY_FAT_BPB_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_QUERY_REGION_INFO_INPUT", aliases: &[
        CLibAliases { names: &["FSCTL_QUERY_REGION_INFO_INPUT", "_FSCTL_QUERY_REGION_INFO_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_QUERY_REGION_INFO_OUTPUT", aliases: &[
        CLibAliases { names: &["FSCTL_QUERY_REGION_INFO_OUTPUT", "_FSCTL_QUERY_REGION_INFO_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_QUERY_STORAGE_CLASSES_OUTPUT", aliases: &[
        CLibAliases { names: &["FSCTL_QUERY_STORAGE_CLASSES_OUTPUT", "_FSCTL_QUERY_STORAGE_CLASSES_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_SET_INTEGRITY_INFORMATION_BUFFER", aliases: &[
        CLibAliases { names: &["FSCTL_SET_INTEGRITY_INFORMATION_BUFFER", "_FSCTL_SET_INTEGRITY_INFORMATION_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FSCTL_SET_INTEGRITY_INFORMATION_BUFFER_EX", aliases: &[
        CLibAliases { names: &["FSCTL_SET_INTEGRITY_INFORMATION_BUFFER_EX", "_FSCTL_SET_INTEGRITY_INFORMATION_BUFFER_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FS_BPIO_INFO", aliases: &[
        CLibAliases { names: &["FS_BPIO_INFO", "_FS_BPIO_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FS_BPIO_INPUT", aliases: &[
        CLibAliases { names: &["FS_BPIO_INPUT", "_FS_BPIO_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FS_BPIO_OUTPUT", aliases: &[
        CLibAliases { names: &["FS_BPIO_OUTPUT", "_FS_BPIO_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::FS_BPIO_RESULTS", aliases: &[
        CLibAliases { names: &["FS_BPIO_RESULTS", "_FS_BPIO_RESULTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GETVERSIONINPARAMS", aliases: &[
        CLibAliases { names: &["GETVERSIONINPARAMS", "_GETVERSIONINPARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GET_CHANGER_PARAMETERS", aliases: &[
        CLibAliases { names: &["GET_CHANGER_PARAMETERS", "_GET_CHANGER_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GET_DEVICE_INTERNAL_STATUS_DATA_REQUEST", aliases: &[
        CLibAliases { names: &["GET_DEVICE_INTERNAL_STATUS_DATA_REQUEST", "_GET_DEVICE_INTERNAL_STATUS_DATA_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GET_DISK_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["GET_DISK_ATTRIBUTES", "_GET_DISK_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GET_FILTER_FILE_IDENTIFIER_INPUT", aliases: &[
        CLibAliases { names: &["GET_FILTER_FILE_IDENTIFIER_INPUT", "_GET_FILTER_FILE_IDENTIFIER_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GET_FILTER_FILE_IDENTIFIER_OUTPUT", aliases: &[
        CLibAliases { names: &["GET_FILTER_FILE_IDENTIFIER_OUTPUT", "_GET_FILTER_FILE_IDENTIFIER_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GET_LENGTH_INFORMATION", aliases: &[
        CLibAliases { names: &["GET_LENGTH_INFORMATION", "_GET_LENGTH_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GET_MEDIA_TYPES", aliases: &[
        CLibAliases { names: &["GET_MEDIA_TYPES", "_GET_MEDIA_TYPES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::GP_LOG_PAGE_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["GP_LOG_PAGE_DESCRIPTOR", "_GP_LOG_PAGE_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::HISTOGRAM_BUCKET", aliases: &[
        CLibAliases { names: &["HISTOGRAM_BUCKET", "_HISTOGRAM_BUCKET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::IDEREGS", aliases: &[
        CLibAliases { names: &["IDEREGS", "_IDEREGS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::IO_IRP_EXT_TRACK_OFFSET_HEADER", aliases: &[
        CLibAliases { names: &["IO_IRP_EXT_TRACK_OFFSET_HEADER", "_IO_IRP_EXT_TRACK_OFFSET_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::LMR_QUERY_INFO_PARAM", aliases: &[
        CLibAliases { names: &["LMR_QUERY_INFO_PARAM", "_LMR_QUERY_INFO_PARAM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::LMR_QUERY_SESSION_INFO", aliases: &[
        CLibAliases { names: &["LMR_QUERY_SESSION_INFO", "_LMR_QUERY_SESSION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::LOOKUP_STREAM_FROM_CLUSTER_ENTRY", aliases: &[
        CLibAliases { names: &["LOOKUP_STREAM_FROM_CLUSTER_ENTRY", "_LOOKUP_STREAM_FROM_CLUSTER_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::LOOKUP_STREAM_FROM_CLUSTER_INPUT", aliases: &[
        CLibAliases { names: &["LOOKUP_STREAM_FROM_CLUSTER_INPUT", "_LOOKUP_STREAM_FROM_CLUSTER_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::LOOKUP_STREAM_FROM_CLUSTER_OUTPUT", aliases: &[
        CLibAliases { names: &["LOOKUP_STREAM_FROM_CLUSTER_OUTPUT", "_LOOKUP_STREAM_FROM_CLUSTER_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::MARK_HANDLE_INFO", aliases: &[
        CLibAliases { names: &["MARK_HANDLE_INFO", "_MARK_HANDLE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::MARK_HANDLE_INFO32", aliases: &[
        CLibAliases { names: &["MARK_HANDLE_INFO32", "_MARK_HANDLE_INFO32"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::MFT_ENUM_DATA_V0", aliases: &[
        CLibAliases { names: &["MFT_ENUM_DATA_V0"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::MFT_ENUM_DATA_V1", aliases: &[
        CLibAliases { names: &["MFT_ENUM_DATA_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::MOVE_FILE_DATA", aliases: &[
        CLibAliases { names: &["MOVE_FILE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::MOVE_FILE_DATA32", aliases: &[
        CLibAliases { names: &["MOVE_FILE_DATA32", "_MOVE_FILE_DATA32"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::MOVE_FILE_RECORD_DATA", aliases: &[
        CLibAliases { names: &["MOVE_FILE_RECORD_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::NTFS_EXTENDED_VOLUME_DATA", aliases: &[
        CLibAliases { names: &["NTFS_EXTENDED_VOLUME_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::NTFS_FILE_RECORD_INPUT_BUFFER", aliases: &[
        CLibAliases { names: &["NTFS_FILE_RECORD_INPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::NTFS_FILE_RECORD_OUTPUT_BUFFER", aliases: &[
        CLibAliases { names: &["NTFS_FILE_RECORD_OUTPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::NTFS_STATISTICS", aliases: &[
        CLibAliases { names: &["NTFS_STATISTICS", "_NTFS_STATISTICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::NTFS_STATISTICS_EX", aliases: &[
        CLibAliases { names: &["NTFS_STATISTICS_EX", "_NTFS_STATISTICS_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::NTFS_VOLUME_DATA_BUFFER", aliases: &[
        CLibAliases { names: &["NTFS_VOLUME_DATA_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PARTITION_INFORMATION", aliases: &[
        CLibAliases { names: &["PARTITION_INFORMATION", "_PARTITION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PARTITION_INFORMATION_EX", aliases: &[
        CLibAliases { names: &["PARTITION_INFORMATION_EX", "_PARTITION_INFORMATION_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PARTITION_INFORMATION_GPT", aliases: &[
        CLibAliases { names: &["PARTITION_INFORMATION_GPT", "_PARTITION_INFORMATION_GPT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PARTITION_INFORMATION_MBR", aliases: &[
        CLibAliases { names: &["PARTITION_INFORMATION_MBR", "_PARTITION_INFORMATION_MBR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PATHNAME_BUFFER", aliases: &[
        CLibAliases { names: &["PATHNAME_BUFFER", "_PATHNAME_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PERF_BIN", aliases: &[
        CLibAliases { names: &["PERF_BIN", "_PERF_BIN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PERSISTENT_RESERVE_COMMAND", aliases: &[
        CLibAliases { names: &["PERSISTENT_RESERVE_COMMAND", "_PERSISTENT_RESERVE_COMMAND"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PHYSICAL_ELEMENT_STATUS", aliases: &[
        CLibAliases { names: &["PHYSICAL_ELEMENT_STATUS", "_PHYSICAL_ELEMENT_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PHYSICAL_ELEMENT_STATUS_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["PHYSICAL_ELEMENT_STATUS_DESCRIPTOR", "_PHYSICAL_ELEMENT_STATUS_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PHYSICAL_ELEMENT_STATUS_REQUEST", aliases: &[
        CLibAliases { names: &["PHYSICAL_ELEMENT_STATUS_REQUEST", "_PHYSICAL_ELEMENT_STATUS_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PIO_IRP_EXT_PROCESS_TRACKED_OFFSET_CALLBACK", aliases: &[
        CLibAliases { names: &["PIO_IRP_EXT_PROCESS_TRACKED_OFFSET_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PLEX_READ_DATA_REQUEST", aliases: &[
        CLibAliases { names: &["PLEX_READ_DATA_REQUEST", "_PLEX_READ_DATA_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::PREVENT_MEDIA_REMOVAL", aliases: &[
        CLibAliases { names: &["PREVENT_MEDIA_REMOVAL", "_PREVENT_MEDIA_REMOVAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::QUERY_BAD_RANGES_INPUT", aliases: &[
        CLibAliases { names: &["QUERY_BAD_RANGES_INPUT", "_QUERY_BAD_RANGES_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::QUERY_BAD_RANGES_INPUT_RANGE", aliases: &[
        CLibAliases { names: &["QUERY_BAD_RANGES_INPUT_RANGE", "_QUERY_BAD_RANGES_INPUT_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::QUERY_BAD_RANGES_OUTPUT", aliases: &[
        CLibAliases { names: &["QUERY_BAD_RANGES_OUTPUT", "_QUERY_BAD_RANGES_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::QUERY_BAD_RANGES_OUTPUT_RANGE", aliases: &[
        CLibAliases { names: &["QUERY_BAD_RANGES_OUTPUT_RANGE", "_QUERY_BAD_RANGES_OUTPUT_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::QUERY_FILE_LAYOUT_INPUT", aliases: &[
        CLibAliases { names: &["QUERY_FILE_LAYOUT_INPUT", "_QUERY_FILE_LAYOUT_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::QUERY_FILE_LAYOUT_OUTPUT", aliases: &[
        CLibAliases { names: &["QUERY_FILE_LAYOUT_OUTPUT", "_QUERY_FILE_LAYOUT_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::READ_ELEMENT_ADDRESS_INFO", aliases: &[
        CLibAliases { names: &["READ_ELEMENT_ADDRESS_INFO", "_READ_ELEMENT_ADDRESS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::READ_FILE_USN_DATA", aliases: &[
        CLibAliases { names: &["READ_FILE_USN_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::READ_USN_JOURNAL_DATA_V0", aliases: &[
        CLibAliases { names: &["READ_USN_JOURNAL_DATA_V0"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::READ_USN_JOURNAL_DATA_V1", aliases: &[
        CLibAliases { names: &["READ_USN_JOURNAL_DATA_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REASSIGN_BLOCKS", aliases: &[
        CLibAliases { names: &["REASSIGN_BLOCKS", "_REASSIGN_BLOCKS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REASSIGN_BLOCKS_EX", aliases: &[
        CLibAliases { names: &["REASSIGN_BLOCKS_EX", "_REASSIGN_BLOCKS_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REFS_SMR_VOLUME_GC_PARAMETERS", aliases: &[
        CLibAliases { names: &["REFS_SMR_VOLUME_GC_PARAMETERS", "_REFS_SMR_VOLUME_GC_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REFS_SMR_VOLUME_INFO_OUTPUT", aliases: &[
        CLibAliases { names: &["REFS_SMR_VOLUME_INFO_OUTPUT", "_REFS_SMR_VOLUME_INFO_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REMOVE_ELEMENT_AND_TRUNCATE_REQUEST", aliases: &[
        CLibAliases { names: &["REMOVE_ELEMENT_AND_TRUNCATE_REQUEST", "_REMOVE_ELEMENT_AND_TRUNCATE_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REPAIR_COPIES_INPUT", aliases: &[
        CLibAliases { names: &["REPAIR_COPIES_INPUT", "_REPAIR_COPIES_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REPAIR_COPIES_OUTPUT", aliases: &[
        CLibAliases { names: &["REPAIR_COPIES_OUTPUT", "_REPAIR_COPIES_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REQUEST_OPLOCK_INPUT_BUFFER", aliases: &[
        CLibAliases { names: &["REQUEST_OPLOCK_INPUT_BUFFER", "_REQUEST_OPLOCK_INPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REQUEST_OPLOCK_OUTPUT_BUFFER", aliases: &[
        CLibAliases { names: &["REQUEST_OPLOCK_OUTPUT_BUFFER", "_REQUEST_OPLOCK_OUTPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::REQUEST_RAW_ENCRYPTED_DATA", aliases: &[
        CLibAliases { names: &["REQUEST_RAW_ENCRYPTED_DATA", "_REQUEST_RAW_ENCRYPTED_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::RETRIEVAL_POINTERS_AND_REFCOUNT_BUFFER", aliases: &[
        CLibAliases { names: &["RETRIEVAL_POINTERS_AND_REFCOUNT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::RETRIEVAL_POINTERS_BUFFER", aliases: &[
        CLibAliases { names: &["RETRIEVAL_POINTERS_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::RETRIEVAL_POINTER_BASE", aliases: &[
        CLibAliases { names: &["RETRIEVAL_POINTER_BASE", "_RETRIEVAL_POINTER_BASE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::RETRIEVAL_POINTER_COUNT", aliases: &[
        CLibAliases { names: &["RETRIEVAL_POINTER_COUNT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_BUS_DEDICATED_MEMORY_DEVICES_INFO", aliases: &[
        CLibAliases { names: &["SCM_BUS_DEDICATED_MEMORY_DEVICES_INFO", "_SCM_BUS_DEDICATED_MEMORY_DEVICES_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_BUS_DEDICATED_MEMORY_DEVICE_INFO", aliases: &[
        CLibAliases { names: &["SCM_BUS_DEDICATED_MEMORY_DEVICE_INFO", "_SCM_BUS_DEDICATED_MEMORY_DEVICE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_BUS_DEDICATED_MEMORY_STATE", aliases: &[
        CLibAliases { names: &["SCM_BUS_DEDICATED_MEMORY_STATE", "_SCM_BUS_DEDICATED_MEMORY_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_BUS_PROPERTY_QUERY", aliases: &[
        CLibAliases { names: &["SCM_BUS_PROPERTY_QUERY", "_SCM_BUS_PROPERTY_QUERY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_BUS_PROPERTY_SET", aliases: &[
        CLibAliases { names: &["SCM_BUS_PROPERTY_SET", "_SCM_BUS_PROPERTY_SET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_BUS_RUNTIME_FW_ACTIVATION_INFO", aliases: &[
        CLibAliases { names: &["SCM_BUS_RUNTIME_FW_ACTIVATION_INFO", "_SCM_BUS_RUNTIME_FW_ACTIVATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_INTERLEAVED_PD_INFO", aliases: &[
        CLibAliases { names: &["SCM_INTERLEAVED_PD_INFO", "_SCM_INTERLEAVED_PD_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_LD_INTERLEAVE_SET_INFO", aliases: &[
        CLibAliases { names: &["SCM_LD_INTERLEAVE_SET_INFO", "_SCM_LD_INTERLEAVE_SET_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_LOGICAL_DEVICES", aliases: &[
        CLibAliases { names: &["SCM_LOGICAL_DEVICES", "_SCM_LOGICAL_DEVICES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_LOGICAL_DEVICE_INSTANCE", aliases: &[
        CLibAliases { names: &["SCM_LOGICAL_DEVICE_INSTANCE", "_SCM_LOGICAL_DEVICE_INSTANCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_DESCRIPTOR_HEADER", aliases: &[
        CLibAliases { names: &["SCM_PD_DESCRIPTOR_HEADER", "_SCM_PD_DESCRIPTOR_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_DEVICE_HANDLE", aliases: &[
        CLibAliases { names: &["SCM_PD_DEVICE_HANDLE", "_SCM_PD_DEVICE_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_DEVICE_INFO", aliases: &[
        CLibAliases { names: &["SCM_PD_DEVICE_INFO", "_SCM_PD_DEVICE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_DEVICE_SPECIFIC_INFO", aliases: &[
        CLibAliases { names: &["SCM_PD_DEVICE_SPECIFIC_INFO", "_SCM_PD_DEVICE_SPECIFIC_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_DEVICE_SPECIFIC_PROPERTY", aliases: &[
        CLibAliases { names: &["SCM_PD_DEVICE_SPECIFIC_PROPERTY", "_SCM_PD_DEVICE_SPECIFIC_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_FIRMWARE_ACTIVATE", aliases: &[
        CLibAliases { names: &["SCM_PD_FIRMWARE_ACTIVATE", "_SCM_PD_FIRMWARE_ACTIVATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_FIRMWARE_DOWNLOAD", aliases: &[
        CLibAliases { names: &["SCM_PD_FIRMWARE_DOWNLOAD", "_SCM_PD_FIRMWARE_DOWNLOAD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_FIRMWARE_INFO", aliases: &[
        CLibAliases { names: &["SCM_PD_FIRMWARE_INFO", "_SCM_PD_FIRMWARE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_FRU_ID_STRING", aliases: &[
        CLibAliases { names: &["SCM_PD_FRU_ID_STRING", "_SCM_PD_FRU_ID_STRING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_HEALTH_NOTIFICATION_DATA", aliases: &[
        CLibAliases { names: &["SCM_PD_HEALTH_NOTIFICATION_DATA", "_SCM_PD_HEALTH_NOTIFICATION_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_LOCATION_STRING", aliases: &[
        CLibAliases { names: &["SCM_PD_LOCATION_STRING", "_SCM_PD_LOCATION_STRING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_MANAGEMENT_STATUS", aliases: &[
        CLibAliases { names: &["SCM_PD_MANAGEMENT_STATUS", "_SCM_PD_MANAGEMENT_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_PASSTHROUGH_INPUT", aliases: &[
        CLibAliases { names: &["SCM_PD_PASSTHROUGH_INPUT", "_SCM_PD_PASSTHROUGH_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_PASSTHROUGH_INVDIMM_INPUT", aliases: &[
        CLibAliases { names: &["SCM_PD_PASSTHROUGH_INVDIMM_INPUT", "_SCM_PD_PASSTHROUGH_INVDIMM_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_PASSTHROUGH_INVDIMM_OUTPUT", aliases: &[
        CLibAliases { names: &["SCM_PD_PASSTHROUGH_INVDIMM_OUTPUT", "_SCM_PD_PASSTHROUGH_INVDIMM_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_PASSTHROUGH_OUTPUT", aliases: &[
        CLibAliases { names: &["SCM_PD_PASSTHROUGH_OUTPUT", "_SCM_PD_PASSTHROUGH_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_PROPERTY_QUERY", aliases: &[
        CLibAliases { names: &["SCM_PD_PROPERTY_QUERY", "_SCM_PD_PROPERTY_QUERY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_PROPERTY_SET", aliases: &[
        CLibAliases { names: &["SCM_PD_PROPERTY_SET", "_SCM_PD_PROPERTY_SET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_REINITIALIZE_MEDIA_INPUT", aliases: &[
        CLibAliases { names: &["SCM_PD_REINITIALIZE_MEDIA_INPUT", "_SCM_PD_REINITIALIZE_MEDIA_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_REINITIALIZE_MEDIA_OUTPUT", aliases: &[
        CLibAliases { names: &["SCM_PD_REINITIALIZE_MEDIA_OUTPUT", "_SCM_PD_REINITIALIZE_MEDIA_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_RUNTIME_FW_ACTIVATION_ARM_STATE", aliases: &[
        CLibAliases { names: &["SCM_PD_RUNTIME_FW_ACTIVATION_ARM_STATE", "_SCM_PD_RUNTIME_FW_ACTIVATION_ARM_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PD_RUNTIME_FW_ACTIVATION_INFO", aliases: &[
        CLibAliases { names: &["SCM_PD_RUNTIME_FW_ACTIVATION_INFO", "_SCM_PD_RUNTIME_FW_ACTIVATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PHYSICAL_DEVICES", aliases: &[
        CLibAliases { names: &["SCM_PHYSICAL_DEVICES", "_SCM_PHYSICAL_DEVICES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_PHYSICAL_DEVICE_INSTANCE", aliases: &[
        CLibAliases { names: &["SCM_PHYSICAL_DEVICE_INSTANCE", "_SCM_PHYSICAL_DEVICE_INSTANCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_REGION", aliases: &[
        CLibAliases { names: &["SCM_REGION", "_SCM_REGION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SCM_REGIONS", aliases: &[
        CLibAliases { names: &["SCM_REGIONS", "_SCM_REGIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_CHANGE_MACHINE_SID_INPUT", aliases: &[
        CLibAliases { names: &["SD_CHANGE_MACHINE_SID_INPUT", "_SD_CHANGE_MACHINE_SID_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_CHANGE_MACHINE_SID_OUTPUT", aliases: &[
        CLibAliases { names: &["SD_CHANGE_MACHINE_SID_OUTPUT", "_SD_CHANGE_MACHINE_SID_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_ENUM_SDS_ENTRY", aliases: &[
        CLibAliases { names: &["SD_ENUM_SDS_ENTRY", "_SD_ENUM_SDS_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_ENUM_SDS_INPUT", aliases: &[
        CLibAliases { names: &["SD_ENUM_SDS_INPUT", "_SD_ENUM_SDS_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_ENUM_SDS_OUTPUT", aliases: &[
        CLibAliases { names: &["SD_ENUM_SDS_OUTPUT", "_SD_ENUM_SDS_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_GLOBAL_CHANGE_INPUT", aliases: &[
        CLibAliases { names: &["SD_GLOBAL_CHANGE_INPUT", "_SD_GLOBAL_CHANGE_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_GLOBAL_CHANGE_OUTPUT", aliases: &[
        CLibAliases { names: &["SD_GLOBAL_CHANGE_OUTPUT", "_SD_GLOBAL_CHANGE_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_QUERY_STATS_INPUT", aliases: &[
        CLibAliases { names: &["SD_QUERY_STATS_INPUT", "_SD_QUERY_STATS_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SD_QUERY_STATS_OUTPUT", aliases: &[
        CLibAliases { names: &["SD_QUERY_STATS_OUTPUT", "_SD_QUERY_STATS_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SENDCMDINPARAMS", aliases: &[
        CLibAliases { names: &["SENDCMDINPARAMS", "_SENDCMDINPARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SENDCMDOUTPARAMS", aliases: &[
        CLibAliases { names: &["SENDCMDOUTPARAMS", "_SENDCMDOUTPARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SET_DAX_ALLOC_ALIGNMENT_HINT_INPUT", aliases: &[
        CLibAliases { names: &["SET_DAX_ALLOC_ALIGNMENT_HINT_INPUT", "_SET_DAX_ALLOC_ALIGNMENT_HINT_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SET_DISK_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["SET_DISK_ATTRIBUTES", "_SET_DISK_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SET_PARTITION_INFORMATION", aliases: &[
        CLibAliases { names: &["SET_PARTITION_INFORMATION", "_SET_PARTITION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SET_PARTITION_INFORMATION_EX", aliases: &[
        CLibAliases { names: &["SET_PARTITION_INFORMATION_EX", "_SET_PARTITION_INFORMATION_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SET_PURGE_FAILURE_MODE_INPUT", aliases: &[
        CLibAliases { names: &["SET_PURGE_FAILURE_MODE_INPUT", "_SET_PURGE_FAILURE_MODE_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SHRINK_VOLUME_INFORMATION", aliases: &[
        CLibAliases { names: &["SHRINK_VOLUME_INFORMATION", "_SHRINK_VOLUME_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SI_COPYFILE", aliases: &[
        CLibAliases { names: &["SI_COPYFILE", "_SI_COPYFILE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SMB_SHARE_FLUSH_AND_PURGE_INPUT", aliases: &[
        CLibAliases { names: &["SMB_SHARE_FLUSH_AND_PURGE_INPUT", "_SMB_SHARE_FLUSH_AND_PURGE_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::SMB_SHARE_FLUSH_AND_PURGE_OUTPUT", aliases: &[
        CLibAliases { names: &["SMB_SHARE_FLUSH_AND_PURGE_OUTPUT", "_SMB_SHARE_FLUSH_AND_PURGE_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STARTING_LCN_INPUT_BUFFER", aliases: &[
        CLibAliases { names: &["STARTING_LCN_INPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STARTING_LCN_INPUT_BUFFER_EX", aliases: &[
        CLibAliases { names: &["STARTING_LCN_INPUT_BUFFER_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STARTING_VCN_INPUT_BUFFER", aliases: &[
        CLibAliases { names: &["STARTING_VCN_INPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR", "_STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ADAPTER_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_ADAPTER_DESCRIPTOR", "_STORAGE_ADAPTER_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ADAPTER_SERIAL_NUMBER", aliases: &[
        CLibAliases { names: &["STORAGE_ADAPTER_SERIAL_NUMBER", "_STORAGE_ADAPTER_SERIAL_NUMBER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ALLOCATE_BC_STREAM_INPUT", aliases: &[
        CLibAliases { names: &["STORAGE_ALLOCATE_BC_STREAM_INPUT", "_STORAGE_ALLOCATE_BC_STREAM_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ALLOCATE_BC_STREAM_OUTPUT", aliases: &[
        CLibAliases { names: &["STORAGE_ALLOCATE_BC_STREAM_OUTPUT", "_STORAGE_ALLOCATE_BC_STREAM_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ATTRIBUTE_MGMT", aliases: &[
        CLibAliases { names: &["STORAGE_ATTRIBUTE_MGMT", "_STORAGE_ATTRIBUTE_MGMT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_BREAK_RESERVATION_REQUEST", aliases: &[
        CLibAliases { names: &["STORAGE_BREAK_RESERVATION_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_BUS_RESET_REQUEST", aliases: &[
        CLibAliases { names: &["STORAGE_BUS_RESET_REQUEST", "_STORAGE_BUS_RESET_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_COUNTER", aliases: &[
        CLibAliases { names: &["STORAGE_COUNTER", "_STORAGE_COUNTER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_COUNTERS", aliases: &[
        CLibAliases { names: &["STORAGE_COUNTERS", "_STORAGE_COUNTERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_CRYPTO_CAPABILITY", aliases: &[
        CLibAliases { names: &["STORAGE_CRYPTO_CAPABILITY", "_STORAGE_CRYPTO_CAPABILITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_CRYPTO_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_CRYPTO_DESCRIPTOR", "_STORAGE_CRYPTO_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DESCRIPTOR_HEADER", aliases: &[
        CLibAliases { names: &["STORAGE_DESCRIPTOR_HEADER", "_STORAGE_DESCRIPTOR_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_ATTRIBUTES_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_ATTRIBUTES_DESCRIPTOR", "_STORAGE_DEVICE_ATTRIBUTES_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_DESCRIPTOR", "_STORAGE_DEVICE_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_FAULT_DOMAIN_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_FAULT_DOMAIN_DESCRIPTOR", "_STORAGE_DEVICE_FAULT_DOMAIN_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_ID_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_ID_DESCRIPTOR", "_STORAGE_DEVICE_ID_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_IO_CAPABILITY_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_IO_CAPABILITY_DESCRIPTOR", "_STORAGE_DEVICE_IO_CAPABILITY_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_LED_STATE_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_LED_STATE_DESCRIPTOR", "_STORAGE_DEVICE_LED_STATE_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_LOCATION_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_LOCATION_DESCRIPTOR", "_STORAGE_DEVICE_LOCATION_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_MANAGEMENT_STATUS", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_MANAGEMENT_STATUS", "_STORAGE_DEVICE_MANAGEMENT_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_NUMA_PROPERTY", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_NUMA_PROPERTY", "_STORAGE_DEVICE_NUMA_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_NUMBER", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_NUMBER", "_STORAGE_DEVICE_NUMBER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_NUMBERS", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_NUMBERS", "_STORAGE_DEVICE_NUMBERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_NUMBER_EX", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_NUMBER_EX", "_STORAGE_DEVICE_NUMBER_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_POWER_CAP", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_POWER_CAP", "_STORAGE_DEVICE_POWER_CAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_RESILIENCY_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_RESILIENCY_DESCRIPTOR", "_STORAGE_DEVICE_RESILIENCY_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_SELF_ENCRYPTION_PROPERTY", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_SELF_ENCRYPTION_PROPERTY", "_STORAGE_DEVICE_SELF_ENCRYPTION_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_SELF_ENCRYPTION_PROPERTY_V2", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_SELF_ENCRYPTION_PROPERTY_V2", "_STORAGE_DEVICE_SELF_ENCRYPTION_PROPERTY_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_TIERING_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_TIERING_DESCRIPTOR", "_STORAGE_DEVICE_TIERING_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DEVICE_UNSAFE_SHUTDOWN_COUNT", aliases: &[
        CLibAliases { names: &["STORAGE_DEVICE_UNSAFE_SHUTDOWN_COUNT", "_STORAGE_DEVICE_UNSAFE_SHUTDOWN_COUNT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DIAGNOSTIC_DATA", aliases: &[
        CLibAliases { names: &["STORAGE_DIAGNOSTIC_DATA", "_STORAGE_DIAGNOSTIC_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_DIAGNOSTIC_REQUEST", aliases: &[
        CLibAliases { names: &["STORAGE_DIAGNOSTIC_REQUEST", "_STORAGE_DIAGNOSTIC_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_EVENT_NOTIFICATION", aliases: &[
        CLibAliases { names: &["STORAGE_EVENT_NOTIFICATION", "_STORAGE_EVENT_NOTIFICATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_FAILURE_PREDICTION_CONFIG", aliases: &[
        CLibAliases { names: &["STORAGE_FAILURE_PREDICTION_CONFIG", "_STORAGE_FAILURE_PREDICTION_CONFIG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_FRU_ID_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_FRU_ID_DESCRIPTOR", "_STORAGE_FRU_ID_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_GET_BC_PROPERTIES_OUTPUT", aliases: &[
        CLibAliases { names: &["STORAGE_GET_BC_PROPERTIES_OUTPUT", "_STORAGE_GET_BC_PROPERTIES_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_HOTPLUG_INFO", aliases: &[
        CLibAliases { names: &["STORAGE_HOTPLUG_INFO", "_STORAGE_HOTPLUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_HW_ENDURANCE_DATA_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_HW_ENDURANCE_DATA_DESCRIPTOR", "_STORAGE_HW_ENDURANCE_DATA_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_HW_ENDURANCE_INFO", aliases: &[
        CLibAliases { names: &["STORAGE_HW_ENDURANCE_INFO", "_STORAGE_HW_ENDURANCE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_HW_FIRMWARE_ACTIVATE", aliases: &[
        CLibAliases { names: &["STORAGE_HW_FIRMWARE_ACTIVATE", "_STORAGE_HW_FIRMWARE_ACTIVATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_HW_FIRMWARE_DOWNLOAD", aliases: &[
        CLibAliases { names: &["STORAGE_HW_FIRMWARE_DOWNLOAD", "_STORAGE_HW_FIRMWARE_DOWNLOAD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_HW_FIRMWARE_DOWNLOAD_V2", aliases: &[
        CLibAliases { names: &["STORAGE_HW_FIRMWARE_DOWNLOAD_V2", "_STORAGE_HW_FIRMWARE_DOWNLOAD_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_HW_FIRMWARE_INFO_QUERY", aliases: &[
        CLibAliases { names: &["STORAGE_HW_FIRMWARE_INFO_QUERY", "_STORAGE_HW_FIRMWARE_INFO_QUERY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_IDENTIFIER", aliases: &[
        CLibAliases { names: &["STORAGE_IDENTIFIER", "_STORAGE_IDENTIFIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_IDLE_POWERUP_REASON", aliases: &[
        CLibAliases { names: &["STORAGE_IDLE_POWERUP_REASON", "_STORAGE_IDLE_POWERUP_REASON"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_MEDIUM_PRODUCT_TYPE_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_MEDIUM_PRODUCT_TYPE_DESCRIPTOR", "_STORAGE_MEDIUM_PRODUCT_TYPE_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_MINIPORT_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_MINIPORT_DESCRIPTOR", "_STORAGE_MINIPORT_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_OFFLOAD_READ_OUTPUT", aliases: &[
        CLibAliases { names: &["STORAGE_OFFLOAD_READ_OUTPUT", "_STORAGE_OFFLOAD_READ_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_OFFLOAD_TOKEN", aliases: &[
        CLibAliases { names: &["STORAGE_OFFLOAD_TOKEN", "_STORAGE_OFFLOAD_TOKEN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_OFFLOAD_WRITE_OUTPUT", aliases: &[
        CLibAliases { names: &["STORAGE_OFFLOAD_WRITE_OUTPUT", "_STORAGE_OFFLOAD_WRITE_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_OPERATIONAL_REASON", aliases: &[
        CLibAliases { names: &["STORAGE_OPERATIONAL_REASON", "_STORAGE_OPERATIONAL_REASON"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PHYSICAL_ADAPTER_DATA", aliases: &[
        CLibAliases { names: &["STORAGE_PHYSICAL_ADAPTER_DATA", "_STORAGE_PHYSICAL_ADAPTER_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PHYSICAL_DEVICE_DATA", aliases: &[
        CLibAliases { names: &["STORAGE_PHYSICAL_DEVICE_DATA", "_STORAGE_PHYSICAL_DEVICE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PHYSICAL_NODE_DATA", aliases: &[
        CLibAliases { names: &["STORAGE_PHYSICAL_NODE_DATA", "_STORAGE_PHYSICAL_NODE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PHYSICAL_TOPOLOGY_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_PHYSICAL_TOPOLOGY_DESCRIPTOR", "_STORAGE_PHYSICAL_TOPOLOGY_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PREDICT_FAILURE", aliases: &[
        CLibAliases { names: &["STORAGE_PREDICT_FAILURE", "_STORAGE_PREDICT_FAILURE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PRIORITY_HINT_SUPPORT", aliases: &[
        CLibAliases { names: &["STORAGE_PRIORITY_HINT_SUPPORT", "_STORAGE_PRIORITY_HINT_SUPPORT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PROPERTY_QUERY", aliases: &[
        CLibAliases { names: &["STORAGE_PROPERTY_QUERY", "_STORAGE_PROPERTY_QUERY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PROPERTY_SET", aliases: &[
        CLibAliases { names: &["STORAGE_PROPERTY_SET", "_STORAGE_PROPERTY_SET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PROTOCOL_DATA_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_PROTOCOL_DATA_DESCRIPTOR", "_STORAGE_PROTOCOL_DATA_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PROTOCOL_DATA_DESCRIPTOR_EXT", aliases: &[
        CLibAliases { names: &["STORAGE_PROTOCOL_DATA_DESCRIPTOR_EXT", "_STORAGE_PROTOCOL_DATA_DESCRIPTOR_EXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PROTOCOL_DATA_SUBVALUE_GET_LOG_PAGE", aliases: &[
        CLibAliases { names: &["STORAGE_PROTOCOL_DATA_SUBVALUE_GET_LOG_PAGE", "_STORAGE_PROTOCOL_DATA_SUBVALUE_GET_LOG_PAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_PROTOCOL_SPECIFIC_DATA", aliases: &[
        CLibAliases { names: &["STORAGE_PROTOCOL_SPECIFIC_DATA", "_STORAGE_PROTOCOL_SPECIFIC_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_QUERY_DEPENDENT_VOLUME_LEV1_ENTRY", aliases: &[
        CLibAliases { names: &["STORAGE_QUERY_DEPENDENT_VOLUME_LEV1_ENTRY", "_STORAGE_QUERY_DEPENDENT_VOLUME_LEV1_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_QUERY_DEPENDENT_VOLUME_LEV2_ENTRY", aliases: &[
        CLibAliases { names: &["STORAGE_QUERY_DEPENDENT_VOLUME_LEV2_ENTRY", "_STORAGE_QUERY_DEPENDENT_VOLUME_LEV2_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_QUERY_DEPENDENT_VOLUME_REQUEST", aliases: &[
        CLibAliases { names: &["STORAGE_QUERY_DEPENDENT_VOLUME_REQUEST", "_STORAGE_QUERY_DEPENDENT_VOLUME_REQUEST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_READ_CAPACITY", aliases: &[
        CLibAliases { names: &["STORAGE_READ_CAPACITY", "_STORAGE_READ_CAPACITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_REINITIALIZE_MEDIA", aliases: &[
        CLibAliases { names: &["STORAGE_REINITIALIZE_MEDIA", "_STORAGE_REINITIALIZE_MEDIA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_RPMB_DATA_FRAME", aliases: &[
        CLibAliases { names: &["STORAGE_RPMB_DATA_FRAME", "_STORAGE_RPMB_DATA_FRAME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_RPMB_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_RPMB_DESCRIPTOR", "_STORAGE_RPMB_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_SPEC_VERSION", aliases: &[
        CLibAliases { names: &["STORAGE_SPEC_VERSION", "_STORAGE_SPEC_VERSION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_TEMPERATURE_DATA_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_TEMPERATURE_DATA_DESCRIPTOR", "_STORAGE_TEMPERATURE_DATA_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_TEMPERATURE_INFO", aliases: &[
        CLibAliases { names: &["STORAGE_TEMPERATURE_INFO", "_STORAGE_TEMPERATURE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_TEMPERATURE_THRESHOLD", aliases: &[
        CLibAliases { names: &["STORAGE_TEMPERATURE_THRESHOLD", "_STORAGE_TEMPERATURE_THRESHOLD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_TIER", aliases: &[
        CLibAliases { names: &["STORAGE_TIER", "_STORAGE_TIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_TIER_REGION", aliases: &[
        CLibAliases { names: &["STORAGE_TIER_REGION", "_STORAGE_TIER_REGION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_WRITE_CACHE_PROPERTY", aliases: &[
        CLibAliases { names: &["STORAGE_WRITE_CACHE_PROPERTY", "_STORAGE_WRITE_CACHE_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ZONED_DEVICE_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_ZONED_DEVICE_DESCRIPTOR", "_STORAGE_ZONED_DEVICE_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ZONE_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["STORAGE_ZONE_DESCRIPTOR", "_STORAGE_ZONE_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STORAGE_ZONE_GROUP", aliases: &[
        CLibAliases { names: &["STORAGE_ZONE_GROUP", "_STORAGE_ZONE_GROUP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STREAMS_ASSOCIATE_ID_INPUT_BUFFER", aliases: &[
        CLibAliases { names: &["STREAMS_ASSOCIATE_ID_INPUT_BUFFER", "_STREAMS_ASSOCIATE_ID_INPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STREAMS_QUERY_ID_OUTPUT_BUFFER", aliases: &[
        CLibAliases { names: &["STREAMS_QUERY_ID_OUTPUT_BUFFER", "_STREAMS_QUERY_ID_OUTPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STREAMS_QUERY_PARAMETERS_OUTPUT_BUFFER", aliases: &[
        CLibAliases { names: &["STREAMS_QUERY_PARAMETERS_OUTPUT_BUFFER", "_STREAMS_QUERY_PARAMETERS_OUTPUT_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STREAM_EXTENT_ENTRY", aliases: &[
        CLibAliases { names: &["STREAM_EXTENT_ENTRY", "_STREAM_EXTENT_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STREAM_INFORMATION_ENTRY", aliases: &[
        CLibAliases { names: &["STREAM_INFORMATION_ENTRY", "_STREAM_INFORMATION_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::STREAM_LAYOUT_ENTRY", aliases: &[
        CLibAliases { names: &["STREAM_LAYOUT_ENTRY", "_STREAM_LAYOUT_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TAPE_GET_STATISTICS", aliases: &[
        CLibAliases { names: &["TAPE_GET_STATISTICS", "_TAPE_GET_STATISTICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TAPE_STATISTICS", aliases: &[
        CLibAliases { names: &["TAPE_STATISTICS", "_TAPE_STATISTICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_CREATE_MINIVERSION_INFO", aliases: &[
        CLibAliases { names: &["TXFS_CREATE_MINIVERSION_INFO", "_TXFS_CREATE_MINIVERSION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_GET_METADATA_INFO_OUT", aliases: &[
        CLibAliases { names: &["TXFS_GET_METADATA_INFO_OUT", "_TXFS_GET_METADATA_INFO_OUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_GET_TRANSACTED_VERSION", aliases: &[
        CLibAliases { names: &["TXFS_GET_TRANSACTED_VERSION", "_TXFS_GET_TRANSACTED_VERSION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_LIST_TRANSACTIONS", aliases: &[
        CLibAliases { names: &["TXFS_LIST_TRANSACTIONS", "_TXFS_LIST_TRANSACTIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_LIST_TRANSACTIONS_ENTRY", aliases: &[
        CLibAliases { names: &["TXFS_LIST_TRANSACTIONS_ENTRY", "_TXFS_LIST_TRANSACTIONS_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_LIST_TRANSACTION_LOCKED_FILES", aliases: &[
        CLibAliases { names: &["TXFS_LIST_TRANSACTION_LOCKED_FILES", "_TXFS_LIST_TRANSACTION_LOCKED_FILES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_LIST_TRANSACTION_LOCKED_FILES_ENTRY", aliases: &[
        CLibAliases { names: &["TXFS_LIST_TRANSACTION_LOCKED_FILES_ENTRY", "_TXFS_LIST_TRANSACTION_LOCKED_FILES_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_MODIFY_RM", aliases: &[
        CLibAliases { names: &["TXFS_MODIFY_RM", "_TXFS_MODIFY_RM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_QUERY_RM_INFORMATION", aliases: &[
        CLibAliases { names: &["TXFS_QUERY_RM_INFORMATION", "_TXFS_QUERY_RM_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_READ_BACKUP_INFORMATION_OUT", aliases: &[
        CLibAliases { names: &["TXFS_READ_BACKUP_INFORMATION_OUT", "_TXFS_READ_BACKUP_INFORMATION_OUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_ROLLFORWARD_REDO_INFORMATION", aliases: &[
        CLibAliases { names: &["TXFS_ROLLFORWARD_REDO_INFORMATION", "_TXFS_ROLLFORWARD_REDO_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_SAVEPOINT_INFORMATION", aliases: &[
        CLibAliases { names: &["TXFS_SAVEPOINT_INFORMATION", "_TXFS_SAVEPOINT_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_START_RM_INFORMATION", aliases: &[
        CLibAliases { names: &["TXFS_START_RM_INFORMATION", "_TXFS_START_RM_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_TRANSACTION_ACTIVE_INFO", aliases: &[
        CLibAliases { names: &["TXFS_TRANSACTION_ACTIVE_INFO", "_TXFS_TRANSACTION_ACTIVE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::TXFS_WRITE_BACKUP_INFORMATION", aliases: &[
        CLibAliases { names: &["TXFS_WRITE_BACKUP_INFORMATION", "_TXFS_WRITE_BACKUP_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_JOURNAL_DATA_V0", aliases: &[
        CLibAliases { names: &["USN_JOURNAL_DATA_V0"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_JOURNAL_DATA_V1", aliases: &[
        CLibAliases { names: &["USN_JOURNAL_DATA_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_JOURNAL_DATA_V2", aliases: &[
        CLibAliases { names: &["USN_JOURNAL_DATA_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_RANGE_TRACK_OUTPUT", aliases: &[
        CLibAliases { names: &["USN_RANGE_TRACK_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_RECORD_COMMON_HEADER", aliases: &[
        CLibAliases { names: &["USN_RECORD_COMMON_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_RECORD_EXTENT", aliases: &[
        CLibAliases { names: &["USN_RECORD_EXTENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_RECORD_UNION", aliases: &[
        CLibAliases { names: &["USN_RECORD_UNION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_RECORD_V2", aliases: &[
        CLibAliases { names: &["USN_RECORD_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_RECORD_V3", aliases: &[
        CLibAliases { names: &["USN_RECORD_V3"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_RECORD_V4", aliases: &[
        CLibAliases { names: &["USN_RECORD_V4"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::USN_TRACK_MODIFIED_RANGES", aliases: &[
        CLibAliases { names: &["USN_TRACK_MODIFIED_RANGES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VERIFY_INFORMATION", aliases: &[
        CLibAliases { names: &["VERIFY_INFORMATION", "_VERIFY_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VIRTUALIZATION_INSTANCE_INFO_INPUT", aliases: &[
        CLibAliases { names: &["VIRTUALIZATION_INSTANCE_INFO_INPUT", "_VIRTUALIZATION_INSTANCE_INFO_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VIRTUALIZATION_INSTANCE_INFO_INPUT_EX", aliases: &[
        CLibAliases { names: &["VIRTUALIZATION_INSTANCE_INFO_INPUT_EX", "_VIRTUALIZATION_INSTANCE_INFO_INPUT_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VIRTUALIZATION_INSTANCE_INFO_OUTPUT", aliases: &[
        CLibAliases { names: &["VIRTUALIZATION_INSTANCE_INFO_OUTPUT", "_VIRTUALIZATION_INSTANCE_INFO_OUTPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VIRTUAL_STORAGE_SET_BEHAVIOR_INPUT", aliases: &[
        CLibAliases { names: &["VIRTUAL_STORAGE_SET_BEHAVIOR_INPUT", "_VIRTUAL_STORAGE_SET_BEHAVIOR_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VOLUME_BITMAP_BUFFER", aliases: &[
        CLibAliases { names: &["VOLUME_BITMAP_BUFFER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VOLUME_DISK_EXTENTS", aliases: &[
        CLibAliases { names: &["VOLUME_DISK_EXTENTS", "_VOLUME_DISK_EXTENTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::VOLUME_GET_GPT_ATTRIBUTES_INFORMATION", aliases: &[
        CLibAliases { names: &["VOLUME_GET_GPT_ATTRIBUTES_INFORMATION", "_VOLUME_GET_GPT_ATTRIBUTES_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WIM_PROVIDER_ADD_OVERLAY_INPUT", aliases: &[
        CLibAliases { names: &["WIM_PROVIDER_ADD_OVERLAY_INPUT", "_WIM_PROVIDER_ADD_OVERLAY_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WIM_PROVIDER_EXTERNAL_INFO", aliases: &[
        CLibAliases { names: &["WIM_PROVIDER_EXTERNAL_INFO", "_WIM_PROVIDER_EXTERNAL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WIM_PROVIDER_OVERLAY_ENTRY", aliases: &[
        CLibAliases { names: &["WIM_PROVIDER_OVERLAY_ENTRY", "_WIM_PROVIDER_OVERLAY_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WIM_PROVIDER_REMOVE_OVERLAY_INPUT", aliases: &[
        CLibAliases { names: &["WIM_PROVIDER_REMOVE_OVERLAY_INPUT", "_WIM_PROVIDER_REMOVE_OVERLAY_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WIM_PROVIDER_SUSPEND_OVERLAY_INPUT", aliases: &[
        CLibAliases { names: &["WIM_PROVIDER_SUSPEND_OVERLAY_INPUT", "_WIM_PROVIDER_SUSPEND_OVERLAY_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WIM_PROVIDER_UPDATE_OVERLAY_INPUT", aliases: &[
        CLibAliases { names: &["WIM_PROVIDER_UPDATE_OVERLAY_INPUT", "_WIM_PROVIDER_UPDATE_OVERLAY_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WOF_EXTERNAL_FILE_ID", aliases: &[
        CLibAliases { names: &["WOF_EXTERNAL_FILE_ID", "_WOF_EXTERNAL_FILE_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WOF_EXTERNAL_INFO", aliases: &[
        CLibAliases { names: &["WOF_EXTERNAL_INFO", "_WOF_EXTERNAL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WOF_VERSION_INFO", aliases: &[
        CLibAliases { names: &["WOF_VERSION_INFO", "_WOF_VERSION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ioctl::WRITE_USN_REASON_INPUT", aliases: &[
        CLibAliases { names: &["WRITE_USN_REASON_INPUT", "_WRITE_USN_REASON_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_ASSOCIATE_COMPLETION_PORT", aliases: &[
        CLibAliases { names: &["JOBOBJECT_ASSOCIATE_COMPLETION_PORT", "_JOBOBJECT_ASSOCIATE_COMPLETION_PORT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_BASIC_ACCOUNTING_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_BASIC_ACCOUNTING_INFORMATION", "_JOBOBJECT_BASIC_ACCOUNTING_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION", "_JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_BASIC_LIMIT_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_BASIC_LIMIT_INFORMATION", "_JOBOBJECT_BASIC_LIMIT_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_BASIC_PROCESS_ID_LIST", aliases: &[
        CLibAliases { names: &["JOBOBJECT_BASIC_PROCESS_ID_LIST", "_JOBOBJECT_BASIC_PROCESS_ID_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_BASIC_UI_RESTRICTIONS", aliases: &[
        CLibAliases { names: &["JOBOBJECT_BASIC_UI_RESTRICTIONS", "_JOBOBJECT_BASIC_UI_RESTRICTIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_CPU_RATE_CONTROL_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_CPU_RATE_CONTROL_INFORMATION", "_JOBOBJECT_CPU_RATE_CONTROL_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_END_OF_JOB_TIME_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_END_OF_JOB_TIME_INFORMATION", "_JOBOBJECT_END_OF_JOB_TIME_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_EXTENDED_LIMIT_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_EXTENDED_LIMIT_INFORMATION", "_JOBOBJECT_EXTENDED_LIMIT_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_IO_ATTRIBUTION_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_IO_ATTRIBUTION_INFORMATION", "_JOBOBJECT_IO_ATTRIBUTION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_IO_ATTRIBUTION_STATS", aliases: &[
        CLibAliases { names: &["JOBOBJECT_IO_ATTRIBUTION_STATS", "_JOBOBJECT_IO_ATTRIBUTION_STATS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_IO_RATE_CONTROL_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_IO_RATE_CONTROL_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_IO_RATE_CONTROL_INFORMATION_NATIVE_V1", aliases: &[
        CLibAliases { names: &["JOBOBJECT_IO_RATE_CONTROL_INFORMATION_NATIVE", "JOBOBJECT_IO_RATE_CONTROL_INFORMATION_NATIVE_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_IO_RATE_CONTROL_INFORMATION_NATIVE_V2", aliases: &[
        CLibAliases { names: &["JOBOBJECT_IO_RATE_CONTROL_INFORMATION_NATIVE_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_IO_RATE_CONTROL_INFORMATION_NATIVE_V3", aliases: &[
        CLibAliases { names: &["JOBOBJECT_IO_RATE_CONTROL_INFORMATION_NATIVE_V3"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_JOBSET_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_JOBSET_INFORMATION", "_JOBOBJECT_JOBSET_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_LIMIT_VIOLATION_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_LIMIT_VIOLATION_INFORMATION", "_JOBOBJECT_LIMIT_VIOLATION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_LIMIT_VIOLATION_INFORMATION_2", aliases: &[
        CLibAliases { names: &["JOBOBJECT_LIMIT_VIOLATION_INFORMATION_2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_NET_RATE_CONTROL_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_NET_RATE_CONTROL_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_NOTIFICATION_LIMIT_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_NOTIFICATION_LIMIT_INFORMATION", "_JOBOBJECT_NOTIFICATION_LIMIT_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_NOTIFICATION_LIMIT_INFORMATION_2", aliases: &[
        CLibAliases { names: &["JOBOBJECT_NOTIFICATION_LIMIT_INFORMATION_2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOBOBJECT_SECURITY_LIMIT_INFORMATION", aliases: &[
        CLibAliases { names: &["JOBOBJECT_SECURITY_LIMIT_INFORMATION", "_JOBOBJECT_SECURITY_LIMIT_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::JobObjects::JOB_SET_ARRAY", aliases: &[
        CLibAliases { names: &["JOB_SET_ARRAY", "_JOB_SET_ARRAY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::EXCEPTION_REGISTRATION_RECORD", aliases: &[
        CLibAliases { names: &["EXCEPTION_REGISTRATION_RECORD", "_EXCEPTION_REGISTRATION_RECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::FLOATING_SAVE_AREA", aliases: &[
        CLibAliases { names: &["FLOATING_SAVE_AREA", "_FLOATING_SAVE_AREA"], targets: &["i686-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::LIST_ENTRY", aliases: &[
        CLibAliases { names: &["LIST_ENTRY", "_LIST_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::LIST_ENTRY32", aliases: &[
        CLibAliases { names: &["LIST_ENTRY32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::LIST_ENTRY64", aliases: &[
        CLibAliases { names: &["LIST_ENTRY64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::OBJECTID", aliases: &[
        CLibAliases { names: &["OBJECTID", "_OBJECTID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::PROCESSOR_NUMBER", aliases: &[
        CLibAliases { names: &["PROCESSOR_NUMBER", "_PROCESSOR_NUMBER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::SINGLE_LIST_ENTRY", aliases: &[
        CLibAliases { names: &["SINGLE_LIST_ENTRY", "_SINGLE_LIST_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::SLIST_ENTRY", aliases: &[
        CLibAliases { names: &["SLIST_ENTRY"], targets: &["i686-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Kernel::SLIST_HEADER", aliases: &[
        CLibAliases { names: &["SLIST_HEADER", "_SLIST_HEADER"], targets: &["i686-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::ENUMRESLANGPROCA", aliases: &[
        CLibAliases { names: &["ENUMRESLANGPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::ENUMRESLANGPROCW", aliases: &[
        CLibAliases { names: &["ENUMRESLANGPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::ENUMRESNAMEPROCA", aliases: &[
        CLibAliases { names: &["ENUMRESNAMEPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::ENUMRESNAMEPROCW", aliases: &[
        CLibAliases { names: &["ENUMRESNAMEPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::ENUMRESTYPEPROCA", aliases: &[
        CLibAliases { names: &["ENUMRESTYPEPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::ENUMRESTYPEPROCW", aliases: &[
        CLibAliases { names: &["ENUMRESTYPEPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::ENUMUILANG", aliases: &[
        CLibAliases { names: &["ENUMUILANG", "tagENUMUILANG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::PGET_MODULE_HANDLE_EXA", aliases: &[
        CLibAliases { names: &["PGET_MODULE_HANDLE_EXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::PGET_MODULE_HANDLE_EXW", aliases: &[
        CLibAliases { names: &["PGET_MODULE_HANDLE_EXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::REDIRECTION_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["REDIRECTION_DESCRIPTOR", "_REDIRECTION_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::LibraryLoader::REDIRECTION_FUNCTION_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["REDIRECTION_FUNCTION_DESCRIPTOR", "_REDIRECTION_FUNCTION_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::CFG_CALL_TARGET_INFO", aliases: &[
        CLibAliases { names: &["CFG_CALL_TARGET_INFO", "_CFG_CALL_TARGET_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::HEAP_SUMMARY", aliases: &[
        CLibAliases { names: &["HEAP_SUMMARY", "_HEAP_SUMMARY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::MEMORY_BASIC_INFORMATION", aliases: &[
        CLibAliases { names: &["MEMORY_BASIC_INFORMATION", "_MEMORY_BASIC_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::MEMORY_BASIC_INFORMATION32", aliases: &[
        CLibAliases { names: &["MEMORY_BASIC_INFORMATION32", "_MEMORY_BASIC_INFORMATION32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::MEMORY_PARTITION_DEDICATED_MEMORY_ATTRIBUTE", aliases: &[
        CLibAliases { names: &["MEMORY_PARTITION_DEDICATED_MEMORY_ATTRIBUTE", "_MEMORY_PARTITION_DEDICATED_MEMORY_ATTRIBUTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::MEMORY_PARTITION_DEDICATED_MEMORY_INFORMATION", aliases: &[
        CLibAliases { names: &["MEMORY_PARTITION_DEDICATED_MEMORY_INFORMATION", "_MEMORY_PARTITION_DEDICATED_MEMORY_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::MEM_ADDRESS_REQUIREMENTS", aliases: &[
        CLibAliases { names: &["MEM_ADDRESS_REQUIREMENTS", "_MEM_ADDRESS_REQUIREMENTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::MEM_EXTENDED_PARAMETER", aliases: &[
        CLibAliases { names: &["MEM_EXTENDED_PARAMETER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::NonVolatile::NV_MEMORY_RANGE", aliases: &[
        CLibAliases { names: &["NV_MEMORY_RANGE", "_NV_MEMORY_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::PBAD_MEMORY_CALLBACK_ROUTINE", aliases: &[
        CLibAliases { names: &["PBAD_MEMORY_CALLBACK_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::PROCESS_HEAP_ENTRY", aliases: &[
        CLibAliases { names: &["PROCESS_HEAP_ENTRY", "_PROCESS_HEAP_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::PSECURE_MEMORY_CACHE_CALLBACK", aliases: &[
        CLibAliases { names: &["PSECURE_MEMORY_CACHE_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::WIN32_MEMORY_PARTITION_INFORMATION", aliases: &[
        CLibAliases { names: &["WIN32_MEMORY_PARTITION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::WIN32_MEMORY_RANGE_ENTRY", aliases: &[
        CLibAliases { names: &["WIN32_MEMORY_RANGE_ENTRY", "_WIN32_MEMORY_RANGE_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Memory::WIN32_MEMORY_REGION_INFORMATION", aliases: &[
        CLibAliases { names: &["WIN32_MEMORY_REGION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::ARRAYDESC", aliases: &[
        CLibAliases { names: &["ARRAYDESC", "tagARRAYDESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::CLEANLOCALSTORAGE", aliases: &[
        CLibAliases { names: &["CLEANLOCALSTORAGE", "tagCLEANLOCALSTORAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::INTERFACEDATA", aliases: &[
        CLibAliases { names: &["INTERFACEDATA", "tagINTERFACEDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::METHODDATA", aliases: &[
        CLibAliases { names: &["METHODDATA", "tagMETHODDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::NUMPARSE", aliases: &[
        CLibAliases { names: &["NUMPARSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::OBJECTDESCRIPTOR", aliases: &[
        CLibAliases { names: &["OBJECTDESCRIPTOR", "tagOBJECTDESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::OLEINPLACEFRAMEINFO", aliases: &[
        CLibAliases { names: &["OLEINPLACEFRAMEINFO", "tagOIFI"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::OLEMENUGROUPWIDTHS", aliases: &[
        CLibAliases { names: &["OLEMENUGROUPWIDTHS", "tagOleMenuGroupWidths"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::OLESTREAMQUERYCONVERTOLELINKCALLBACK", aliases: &[
        CLibAliases { names: &["OLESTREAMQUERYCONVERTOLELINKCALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::OLEVERB", aliases: &[
        CLibAliases { names: &["OLEVERB", "tagOLEVERB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::PARAMDATA", aliases: &[
        CLibAliases { names: &["PARAMDATA", "tagPARAMDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::PARAMDESC", aliases: &[
        CLibAliases { names: &["PARAMDESC", "tagPARAMDESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::PARAMDESCEX", aliases: &[
        CLibAliases { names: &["PARAMDESCEX", "tagPARAMDESCEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::SAFEARRAYUNION", aliases: &[
        CLibAliases { names: &["SAFEARRAYUNION", "_wireSAFEARRAY_UNION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::SAFEARR_BRECORD", aliases: &[
        CLibAliases { names: &["SAFEARR_BRECORD", "_wireSAFEARR_BRECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::SAFEARR_BSTR", aliases: &[
        CLibAliases { names: &["SAFEARR_BSTR", "_wireSAFEARR_BSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::SAFEARR_DISPATCH", aliases: &[
        CLibAliases { names: &["SAFEARR_DISPATCH", "_wireSAFEARR_DISPATCH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::SAFEARR_HAVEIID", aliases: &[
        CLibAliases { names: &["SAFEARR_HAVEIID", "_wireSAFEARR_HAVEIID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::SAFEARR_UNKNOWN", aliases: &[
        CLibAliases { names: &["SAFEARR_UNKNOWN", "_wireSAFEARR_UNKNOWN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::SAFEARR_VARIANT", aliases: &[
        CLibAliases { names: &["SAFEARR_VARIANT", "_wireSAFEARR_VARIANT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::UDATE", aliases: &[
        CLibAliases { names: &["UDATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::_wireBRECORD", aliases: &[
        CLibAliases { names: &["_wireBRECORD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::_wireSAFEARRAY", aliases: &[
        CLibAliases { names: &["_wireSAFEARRAY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Ole::_wireVARIANT", aliases: &[
        CLibAliases { names: &["_wireVARIANT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Performance::HardwareCounterProfiling::HARDWARE_COUNTER_DATA", aliases: &[
        CLibAliases { names: &["HARDWARE_COUNTER_DATA", "_HARDWARE_COUNTER_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Performance::HardwareCounterProfiling::PERFORMANCE_DATA", aliases: &[
        CLibAliases { names: &["PERFORMANCE_DATA", "_PERFORMANCE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Performance::PERF_COUNTER_BLOCK", aliases: &[
        CLibAliases { names: &["PERF_COUNTER_BLOCK", "_PERF_COUNTER_BLOCK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Performance::PERF_COUNTER_DEFINITION", aliases: &[
        CLibAliases { names: &["PERF_COUNTER_DEFINITION", "_PERF_COUNTER_DEFINITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Performance::PERF_DATA_BLOCK", aliases: &[
        CLibAliases { names: &["PERF_DATA_BLOCK", "_PERF_DATA_BLOCK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Performance::PERF_INSTANCE_DEFINITION", aliases: &[
        CLibAliases { names: &["PERF_INSTANCE_DEFINITION", "_PERF_INSTANCE_DEFINITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Performance::PERF_OBJECT_TYPE", aliases: &[
        CLibAliases { names: &["PERF_OBJECT_TYPE", "_PERF_OBJECT_TYPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::ADMINISTRATOR_POWER_POLICY", aliases: &[
        CLibAliases { names: &["ADMINISTRATOR_POWER_POLICY", "_ADMINISTRATOR_POWER_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::BATTERY_REPORTING_SCALE", aliases: &[
        CLibAliases { names: &["BATTERY_REPORTING_SCALE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::CM_POWER_DATA", aliases: &[
        CLibAliases { names: &["CM_POWER_DATA", "CM_Power_Data_s"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::HPOWERNOTIFY", aliases: &[
        CLibAliases { names: &["HPOWERNOTIFY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWERBROADCAST_SETTING", aliases: &[
        CLibAliases { names: &["POWERBROADCAST_SETTING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_ACTION_POLICY", aliases: &[
        CLibAliases { names: &["POWER_ACTION_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_IDLE_RESILIENCY", aliases: &[
        CLibAliases { names: &["POWER_IDLE_RESILIENCY", "_POWER_IDLE_RESILIENCY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_MONITOR_INVOCATION", aliases: &[
        CLibAliases { names: &["POWER_MONITOR_INVOCATION", "_POWER_MONITOR_INVOCATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_PLATFORM_INFORMATION", aliases: &[
        CLibAliases { names: &["POWER_PLATFORM_INFORMATION", "_POWER_PLATFORM_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_SESSION_ALLOW_EXTERNAL_DMA_DEVICES", aliases: &[
        CLibAliases { names: &["POWER_SESSION_ALLOW_EXTERNAL_DMA_DEVICES", "_POWER_SESSION_ALLOW_EXTERNAL_DMA_DEVICES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_SESSION_CONNECT", aliases: &[
        CLibAliases { names: &["POWER_SESSION_CONNECT", "_POWER_SESSION_CONNECT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_SESSION_RIT_STATE", aliases: &[
        CLibAliases { names: &["POWER_SESSION_RIT_STATE", "_POWER_SESSION_RIT_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_SESSION_TIMEOUTS", aliases: &[
        CLibAliases { names: &["POWER_SESSION_TIMEOUTS", "_POWER_SESSION_TIMEOUTS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_SESSION_WINLOGON", aliases: &[
        CLibAliases { names: &["POWER_SESSION_WINLOGON", "_POWER_SESSION_WINLOGON"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::POWER_USER_PRESENCE", aliases: &[
        CLibAliases { names: &["POWER_USER_PRESENCE", "_POWER_USER_PRESENCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_IDLESTATE_EVENT", aliases: &[
        CLibAliases { names: &["PPM_IDLESTATE_EVENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_IDLE_ACCOUNTING", aliases: &[
        CLibAliases { names: &["PPM_IDLE_ACCOUNTING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_IDLE_ACCOUNTING_EX", aliases: &[
        CLibAliases { names: &["PPM_IDLE_ACCOUNTING_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_IDLE_STATE_ACCOUNTING", aliases: &[
        CLibAliases { names: &["PPM_IDLE_STATE_ACCOUNTING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_IDLE_STATE_ACCOUNTING_EX", aliases: &[
        CLibAliases { names: &["PPM_IDLE_STATE_ACCOUNTING_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_IDLE_STATE_BUCKET_EX", aliases: &[
        CLibAliases { names: &["PPM_IDLE_STATE_BUCKET_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_PERFSTATE_DOMAIN_EVENT", aliases: &[
        CLibAliases { names: &["PPM_PERFSTATE_DOMAIN_EVENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_PERFSTATE_EVENT", aliases: &[
        CLibAliases { names: &["PPM_PERFSTATE_EVENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_THERMALCHANGE_EVENT", aliases: &[
        CLibAliases { names: &["PPM_THERMALCHANGE_EVENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_THERMAL_POLICY_EVENT", aliases: &[
        CLibAliases { names: &["PPM_THERMAL_POLICY_EVENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_WMI_IDLE_STATE", aliases: &[
        CLibAliases { names: &["PPM_WMI_IDLE_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_WMI_IDLE_STATES", aliases: &[
        CLibAliases { names: &["PPM_WMI_IDLE_STATES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_WMI_IDLE_STATES_EX", aliases: &[
        CLibAliases { names: &["PPM_WMI_IDLE_STATES_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_WMI_LEGACY_PERFSTATE", aliases: &[
        CLibAliases { names: &["PPM_WMI_LEGACY_PERFSTATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_WMI_PERF_STATE", aliases: &[
        CLibAliases { names: &["PPM_WMI_PERF_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_WMI_PERF_STATES", aliases: &[
        CLibAliases { names: &["PPM_WMI_PERF_STATES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::PPM_WMI_PERF_STATES_EX", aliases: &[
        CLibAliases { names: &["PPM_WMI_PERF_STATES_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::RESUME_PERFORMANCE", aliases: &[
        CLibAliases { names: &["RESUME_PERFORMANCE", "_RESUME_PERFORMANCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::SET_POWER_SETTING_VALUE", aliases: &[
        CLibAliases { names: &["SET_POWER_SETTING_VALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::SYSTEM_BATTERY_STATE", aliases: &[
        CLibAliases { names: &["SYSTEM_BATTERY_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::SYSTEM_POWER_CAPABILITIES", aliases: &[
        CLibAliases { names: &["SYSTEM_POWER_CAPABILITIES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::SYSTEM_POWER_LEVEL", aliases: &[
        CLibAliases { names: &["SYSTEM_POWER_LEVEL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::SYSTEM_POWER_POLICY", aliases: &[
        CLibAliases { names: &["SYSTEM_POWER_POLICY", "_SYSTEM_POWER_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Power::SYSTEM_POWER_STATUS", aliases: &[
        CLibAliases { names: &["SYSTEM_POWER_STATUS", "_SYSTEM_POWER_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::HKEY", aliases: &[
        CLibAliases { names: &["HKEY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::PQUERYHANDLER", aliases: &[
        CLibAliases { names: &["PQUERYHANDLER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::PVALUEA", aliases: &[
        CLibAliases { names: &["PVALUEA", "pvalueA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::PVALUEW", aliases: &[
        CLibAliases { names: &["PVALUEW", "pvalueW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::REG_PROVIDER", aliases: &[
        CLibAliases { names: &["REG_PROVIDER", "provider_info"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::VALENTA", aliases: &[
        CLibAliases { names: &["VALENTA", "value_entA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::VALENTW", aliases: &[
        CLibAliases { names: &["VALENTW", "value_entW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Registry::val_context", aliases: &[
        CLibAliases { names: &["val_context"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::RemoteDesktop::WTSSESSION_NOTIFICATION", aliases: &[
        CLibAliases { names: &["WTSSESSION_NOTIFICATION", "tagWTSSESSION_NOTIFICATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::ARRAY_INFO", aliases: &[
        CLibAliases { names: &["ARRAY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::BinaryParam", aliases: &[
        CLibAliases { names: &["BinaryParam", "tagBinaryParam"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::CLIENT_CALL_RETURN", aliases: &[
        CLibAliases { names: &["CLIENT_CALL_RETURN", "_CLIENT_CALL_RETURN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::COMM_FAULT_OFFSETS", aliases: &[
        CLibAliases { names: &["COMM_FAULT_OFFSETS", "_COMM_FAULT_OFFSETS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::CS_TAG_GETTING_ROUTINE", aliases: &[
        CLibAliases { names: &["CS_TAG_GETTING_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::CS_TYPE_FROM_NETCS_ROUTINE", aliases: &[
        CLibAliases { names: &["CS_TYPE_FROM_NETCS_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::CS_TYPE_LOCAL_SIZE_ROUTINE", aliases: &[
        CLibAliases { names: &["CS_TYPE_LOCAL_SIZE_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::CS_TYPE_NET_SIZE_ROUTINE", aliases: &[
        CLibAliases { names: &["CS_TYPE_NET_SIZE_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::CS_TYPE_TO_NETCS_ROUTINE", aliases: &[
        CLibAliases { names: &["CS_TYPE_TO_NETCS_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::EXPR_EVAL", aliases: &[
        CLibAliases { names: &["EXPR_EVAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::FULL_PTR_XLAT_TABLES", aliases: &[
        CLibAliases { names: &["FULL_PTR_XLAT_TABLES", "_FULL_PTR_XLAT_TABLES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::GENERIC_BINDING_INFO", aliases: &[
        CLibAliases { names: &["GENERIC_BINDING_INFO", "__GENERIC_BINDING_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::GENERIC_BINDING_ROUTINE", aliases: &[
        CLibAliases { names: &["GENERIC_BINDING_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::GENERIC_BINDING_ROUTINE_PAIR", aliases: &[
        CLibAliases { names: &["GENERIC_BINDING_ROUTINE_PAIR", "_GENERIC_BINDING_ROUTINE_PAIR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::GENERIC_UNBIND_ROUTINE", aliases: &[
        CLibAliases { names: &["GENERIC_UNBIND_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcFreeCalloutStateFn", aliases: &[
        CLibAliases { names: &["I_RpcFreeCalloutStateFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcPerformCalloutFn", aliases: &[
        CLibAliases { names: &["I_RpcPerformCalloutFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyCallbackInterface", aliases: &[
        CLibAliases { names: &["I_RpcProxyCallbackInterface", "tagI_RpcProxyCallbackInterface"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyFilterIfFn", aliases: &[
        CLibAliases { names: &["I_RpcProxyFilterIfFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyGetClientAddressFn", aliases: &[
        CLibAliases { names: &["I_RpcProxyGetClientAddressFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyGetClientSessionAndResourceUUID", aliases: &[
        CLibAliases { names: &["I_RpcProxyGetClientSessionAndResourceUUID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyGetConnectionTimeoutFn", aliases: &[
        CLibAliases { names: &["I_RpcProxyGetConnectionTimeoutFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyIsValidMachineFn", aliases: &[
        CLibAliases { names: &["I_RpcProxyIsValidMachineFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyUpdatePerfCounterBackendServerFn", aliases: &[
        CLibAliases { names: &["I_RpcProxyUpdatePerfCounterBackendServerFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::I_RpcProxyUpdatePerfCounterFn", aliases: &[
        CLibAliases { names: &["I_RpcProxyUpdatePerfCounterFn"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MALLOC_FREE_STRUCT", aliases: &[
        CLibAliases { names: &["MALLOC_FREE_STRUCT", "_MALLOC_FREE_STRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_INTERCEPTION_INFO", aliases: &[
        CLibAliases { names: &["MIDL_INTERCEPTION_INFO", "_MIDL_INTERCEPTION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_INTERFACE_METHOD_PROPERTIES", aliases: &[
        CLibAliases { names: &["MIDL_INTERFACE_METHOD_PROPERTIES", "_MIDL_INTERFACE_METHOD_PROPERTIES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_METHOD_PROPERTY", aliases: &[
        CLibAliases { names: &["MIDL_METHOD_PROPERTY", "_MIDL_METHOD_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_METHOD_PROPERTY_MAP", aliases: &[
        CLibAliases { names: &["MIDL_METHOD_PROPERTY_MAP", "_MIDL_METHOD_PROPERTY_MAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_SERVER_INFO", aliases: &[
        CLibAliases { names: &["MIDL_SERVER_INFO", "_MIDL_SERVER_INFO_"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_STUBLESS_PROXY_INFO", aliases: &[
        CLibAliases { names: &["MIDL_STUBLESS_PROXY_INFO", "_MIDL_STUBLESS_PROXY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_STUB_DESC", aliases: &[
        CLibAliases { names: &["MIDL_STUB_DESC", "_MIDL_STUB_DESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_SYNTAX_INFO", aliases: &[
        CLibAliases { names: &["MIDL_SYNTAX_INFO", "_MIDL_SYNTAX_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::MIDL_WINRT_TYPE_SERIALIZATION_INFO", aliases: &[
        CLibAliases { names: &["MIDL_WINRT_TYPE_SERIALIZATION_INFO", "_MIDL_WINRT_TYPE_SERIALIZATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_ALLOC_ALL_NODES_CONTEXT", aliases: &[
        CLibAliases { names: &["NDR_ALLOC_ALL_NODES_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_CS_ROUTINES", aliases: &[
        CLibAliases { names: &["NDR_CS_ROUTINES", "_NDR_CS_ROUTINES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_CS_SIZE_CONVERT_ROUTINES", aliases: &[
        CLibAliases { names: &["NDR_CS_SIZE_CONVERT_ROUTINES", "_NDR_CS_SIZE_CONVERT_ROUTINES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_EXPR_DESC", aliases: &[
        CLibAliases { names: &["NDR_EXPR_DESC", "_NDR_EXPR_DESC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_NOTIFY2_ROUTINE", aliases: &[
        CLibAliases { names: &["NDR_NOTIFY2_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_NOTIFY_ROUTINE", aliases: &[
        CLibAliases { names: &["NDR_NOTIFY_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_POINTER_QUEUE_STATE", aliases: &[
        CLibAliases { names: &["NDR_POINTER_QUEUE_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_RUNDOWN", aliases: &[
        CLibAliases { names: &["NDR_RUNDOWN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_USER_MARSHAL_INFO", aliases: &[
        CLibAliases { names: &["NDR_USER_MARSHAL_INFO", "_NDR_USER_MARSHAL_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::NDR_USER_MARSHAL_INFO_LEVEL1", aliases: &[
        CLibAliases { names: &["NDR_USER_MARSHAL_INFO_LEVEL1", "_NDR_USER_MARSHAL_INFO_LEVEL1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::PFN_RPCNOTIFICATION_ROUTINE", aliases: &[
        CLibAliases { names: &["PFN_RPCNOTIFICATION_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::PNDR_ASYNC_MESSAGE", aliases: &[
        CLibAliases { names: &["PNDR_ASYNC_MESSAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::PNDR_CORRELATION_INFO", aliases: &[
        CLibAliases { names: &["PNDR_CORRELATION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::PRPC_RUNDOWN", aliases: &[
        CLibAliases { names: &["PRPC_RUNDOWN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RDR_CALLOUT_STATE", aliases: &[
        CLibAliases { names: &["RDR_CALLOUT_STATE", "_RDR_CALLOUT_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPCLT_PDU_FILTER_FUNC", aliases: &[
        CLibAliases { names: &["RPCLT_PDU_FILTER_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_ASYNC_STATE", aliases: &[
        CLibAliases { names: &["RPC_ASYNC_STATE", "_RPC_ASYNC_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_AUTH_KEY_RETRIEVAL_FN", aliases: &[
        CLibAliases { names: &["RPC_AUTH_KEY_RETRIEVAL_FN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_BINDING_HANDLE_OPTIONS_V1", aliases: &[
        CLibAliases { names: &["RPC_BINDING_HANDLE_OPTIONS_V1", "_RPC_BINDING_HANDLE_OPTIONS_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_BINDING_HANDLE_SECURITY_V1_A", aliases: &[
        CLibAliases { names: &["RPC_BINDING_HANDLE_SECURITY_V1_A", "_RPC_BINDING_HANDLE_SECURITY_V1_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_BINDING_HANDLE_SECURITY_V1_W", aliases: &[
        CLibAliases { names: &["RPC_BINDING_HANDLE_SECURITY_V1_W", "_RPC_BINDING_HANDLE_SECURITY_V1_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_BINDING_HANDLE_TEMPLATE_V1_A", aliases: &[
        CLibAliases { names: &["RPC_BINDING_HANDLE_TEMPLATE_V1_A", "_RPC_BINDING_HANDLE_TEMPLATE_V1_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_BINDING_HANDLE_TEMPLATE_V1_W", aliases: &[
        CLibAliases { names: &["RPC_BINDING_HANDLE_TEMPLATE_V1_W", "_RPC_BINDING_HANDLE_TEMPLATE_V1_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_BINDING_VECTOR", aliases: &[
        CLibAliases { names: &["RPC_BINDING_VECTOR", "_RPC_BINDING_VECTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_BLOCKING_FN", aliases: &[
        CLibAliases { names: &["RPC_BLOCKING_FN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CALL_ATTRIBUTES_V1_A", aliases: &[
        CLibAliases { names: &["RPC_CALL_ATTRIBUTES_V1_A", "tagRPC_CALL_ATTRIBUTES_V1_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CALL_ATTRIBUTES_V1_W", aliases: &[
        CLibAliases { names: &["RPC_CALL_ATTRIBUTES_V1_W", "tagRPC_CALL_ATTRIBUTES_V1_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CALL_ATTRIBUTES_V2_A", aliases: &[
        CLibAliases { names: &["RPC_CALL_ATTRIBUTES_V2_A", "tagRPC_CALL_ATTRIBUTES_V2_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CALL_ATTRIBUTES_V2_W", aliases: &[
        CLibAliases { names: &["RPC_CALL_ATTRIBUTES_V2_W", "tagRPC_CALL_ATTRIBUTES_V2_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CALL_ATTRIBUTES_V3_A", aliases: &[
        CLibAliases { names: &["RPC_CALL_ATTRIBUTES_V3_A", "tagRPC_CALL_ATTRIBUTES_V3_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CALL_ATTRIBUTES_V3_W", aliases: &[
        CLibAliases { names: &["RPC_CALL_ATTRIBUTES_V3_W", "tagRPC_CALL_ATTRIBUTES_V3_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CALL_LOCAL_ADDRESS_V1", aliases: &[
        CLibAliases { names: &["RPC_CALL_LOCAL_ADDRESS_V1", "_RPC_CALL_LOCAL_ADDRESS_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CLIENT_INFORMATION1", aliases: &[
        CLibAliases { names: &["RPC_CLIENT_INFORMATION1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_CLIENT_INTERFACE", aliases: &[
        CLibAliases { names: &["RPC_CLIENT_INTERFACE", "_RPC_CLIENT_INTERFACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_C_OPT_COOKIE_AUTH_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["RPC_C_OPT_COOKIE_AUTH_DESCRIPTOR", "_RPC_C_OPT_COOKIE_AUTH_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_DISPATCH_FUNCTION", aliases: &[
        CLibAliases { names: &["RPC_DISPATCH_FUNCTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_DISPATCH_TABLE", aliases: &[
        CLibAliases { names: &["RPC_DISPATCH_TABLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_EE_INFO_PARAM", aliases: &[
        CLibAliases { names: &["RPC_EE_INFO_PARAM", "tagRPC_EE_INFO_PARAM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_ENDPOINT_TEMPLATEA", aliases: &[
        CLibAliases { names: &["RPC_ENDPOINT_TEMPLATEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_ENDPOINT_TEMPLATEW", aliases: &[
        CLibAliases { names: &["RPC_ENDPOINT_TEMPLATEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_ERROR_ENUM_HANDLE", aliases: &[
        CLibAliases { names: &["RPC_ERROR_ENUM_HANDLE", "tagRPC_ERROR_ENUM_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_EXTENDED_ERROR_INFO", aliases: &[
        CLibAliases { names: &["RPC_EXTENDED_ERROR_INFO", "tagRPC_EXTENDED_ERROR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_HTTP_PROXY_FREE_STRING", aliases: &[
        CLibAliases { names: &["RPC_HTTP_PROXY_FREE_STRING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_HTTP_TRANSPORT_CREDENTIALS_A", aliases: &[
        CLibAliases { names: &["RPC_HTTP_TRANSPORT_CREDENTIALS_A", "_RPC_HTTP_TRANSPORT_CREDENTIALS_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_HTTP_TRANSPORT_CREDENTIALS_V2_A", aliases: &[
        CLibAliases { names: &["RPC_HTTP_TRANSPORT_CREDENTIALS_V2_A", "_RPC_HTTP_TRANSPORT_CREDENTIALS_V2_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_HTTP_TRANSPORT_CREDENTIALS_V2_W", aliases: &[
        CLibAliases { names: &["RPC_HTTP_TRANSPORT_CREDENTIALS_V2_W", "_RPC_HTTP_TRANSPORT_CREDENTIALS_V2_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_HTTP_TRANSPORT_CREDENTIALS_V3_A", aliases: &[
        CLibAliases { names: &["RPC_HTTP_TRANSPORT_CREDENTIALS_V3_A", "_RPC_HTTP_TRANSPORT_CREDENTIALS_V3_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_HTTP_TRANSPORT_CREDENTIALS_V3_W", aliases: &[
        CLibAliases { names: &["RPC_HTTP_TRANSPORT_CREDENTIALS_V3_W", "_RPC_HTTP_TRANSPORT_CREDENTIALS_V3_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_HTTP_TRANSPORT_CREDENTIALS_W", aliases: &[
        CLibAliases { names: &["RPC_HTTP_TRANSPORT_CREDENTIALS_W", "_RPC_HTTP_TRANSPORT_CREDENTIALS_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_IF_ID", aliases: &[
        CLibAliases { names: &["RPC_IF_ID", "_RPC_IF_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_IF_ID_VECTOR", aliases: &[
        CLibAliases { names: &["RPC_IF_ID_VECTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_IMPORT_CONTEXT_P", aliases: &[
        CLibAliases { names: &["RPC_IMPORT_CONTEXT_P"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_INTERFACE_TEMPLATEA", aliases: &[
        CLibAliases { names: &["RPC_INTERFACE_TEMPLATEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_INTERFACE_TEMPLATEW", aliases: &[
        CLibAliases { names: &["RPC_INTERFACE_TEMPLATEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_MESSAGE", aliases: &[
        CLibAliases { names: &["RPC_MESSAGE", "_RPC_MESSAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_MGMT_AUTHORIZATION_FN", aliases: &[
        CLibAliases { names: &["RPC_MGMT_AUTHORIZATION_FN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_NEW_HTTP_PROXY_CHANNEL", aliases: &[
        CLibAliases { names: &["RPC_NEW_HTTP_PROXY_CHANNEL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_POLICY", aliases: &[
        CLibAliases { names: &["RPC_POLICY", "_RPC_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_PROTSEQ_ENDPOINT", aliases: &[
        CLibAliases { names: &["RPC_PROTSEQ_ENDPOINT", "_RPC_PROTSEQ_ENDPOINT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_PROTSEQ_VECTORA", aliases: &[
        CLibAliases { names: &["RPC_PROTSEQ_VECTORA", "_RPC_PROTSEQ_VECTORA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_PROTSEQ_VECTORW", aliases: &[
        CLibAliases { names: &["RPC_PROTSEQ_VECTORW", "_RPC_PROTSEQ_VECTORW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS", "_RPC_SECURITY_QOS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V2_A", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V2_A", "_RPC_SECURITY_QOS_V2_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V2_W", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V2_W", "_RPC_SECURITY_QOS_V2_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V3_A", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V3_A", "_RPC_SECURITY_QOS_V3_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V3_W", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V3_W", "_RPC_SECURITY_QOS_V3_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V4_A", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V4_A", "_RPC_SECURITY_QOS_V4_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V4_W", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V4_W", "_RPC_SECURITY_QOS_V4_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V5_A", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V5_A", "_RPC_SECURITY_QOS_V5_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SECURITY_QOS_V5_W", aliases: &[
        CLibAliases { names: &["RPC_SECURITY_QOS_V5_W", "_RPC_SECURITY_QOS_V5_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SEC_CONTEXT_KEY_INFO", aliases: &[
        CLibAliases { names: &["RPC_SEC_CONTEXT_KEY_INFO", "_RPC_SEC_CONTEXT_KEY_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SERVER_INTERFACE", aliases: &[
        CLibAliases { names: &["RPC_SERVER_INTERFACE", "_RPC_SERVER_INTERFACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SETFILTER_FUNC", aliases: &[
        CLibAliases { names: &["RPC_SETFILTER_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_STATS_VECTOR", aliases: &[
        CLibAliases { names: &["RPC_STATS_VECTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_SYNTAX_IDENTIFIER", aliases: &[
        CLibAliases { names: &["RPC_SYNTAX_IDENTIFIER", "_RPC_SYNTAX_IDENTIFIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_TRANSFER_SYNTAX", aliases: &[
        CLibAliases { names: &["RPC_TRANSFER_SYNTAX", "_RPC_TRANSFER_SYNTAX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::RPC_VERSION", aliases: &[
        CLibAliases { names: &["RPC_VERSION", "_RPC_VERSION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::SCONTEXT_QUEUE", aliases: &[
        CLibAliases { names: &["SCONTEXT_QUEUE", "_SCONTEXT_QUEUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::SEC_WINNT_AUTH_IDENTITY_A", aliases: &[
        CLibAliases { names: &["SEC_WINNT_AUTH_IDENTITY_A", "_SEC_WINNT_AUTH_IDENTITY_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::SEC_WINNT_AUTH_IDENTITY_W", aliases: &[
        CLibAliases { names: &["SEC_WINNT_AUTH_IDENTITY_W", "_SEC_WINNT_AUTH_IDENTITY_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::SERVER_ROUTINE", aliases: &[
        CLibAliases { names: &["SERVER_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::STUB_THUNK", aliases: &[
        CLibAliases { names: &["STUB_THUNK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::USER_MARSHAL_CB", aliases: &[
        CLibAliases { names: &["USER_MARSHAL_CB", "_USER_MARSHAL_CB"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::USER_MARSHAL_FREEING_ROUTINE", aliases: &[
        CLibAliases { names: &["USER_MARSHAL_FREEING_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::USER_MARSHAL_MARSHALLING_ROUTINE", aliases: &[
        CLibAliases { names: &["USER_MARSHAL_MARSHALLING_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::USER_MARSHAL_ROUTINE_QUADRUPLE", aliases: &[
        CLibAliases { names: &["USER_MARSHAL_ROUTINE_QUADRUPLE", "_USER_MARSHAL_ROUTINE_QUADRUPLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::USER_MARSHAL_SIZING_ROUTINE", aliases: &[
        CLibAliases { names: &["USER_MARSHAL_SIZING_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::USER_MARSHAL_UNMARSHALLING_ROUTINE", aliases: &[
        CLibAliases { names: &["USER_MARSHAL_UNMARSHALLING_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::UUID_VECTOR", aliases: &[
        CLibAliases { names: &["UUID_VECTOR", "_UUID_VECTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::XMIT_HELPER_ROUTINE", aliases: &[
        CLibAliases { names: &["XMIT_HELPER_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::XMIT_ROUTINE_QUINTUPLE", aliases: &[
        CLibAliases { names: &["XMIT_ROUTINE_QUINTUPLE", "_XMIT_ROUTINE_QUINTUPLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Rpc::_NDR_PROC_CONTEXT", aliases: &[
        CLibAliases { names: &["_NDR_PROC_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::ENUM_SERVICE_STATUSA", aliases: &[
        CLibAliases { names: &["ENUM_SERVICE_STATUSA", "_ENUM_SERVICE_STATUSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::ENUM_SERVICE_STATUSW", aliases: &[
        CLibAliases { names: &["ENUM_SERVICE_STATUSW", "_ENUM_SERVICE_STATUSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::ENUM_SERVICE_STATUS_PROCESSA", aliases: &[
        CLibAliases { names: &["ENUM_SERVICE_STATUS_PROCESSA", "_ENUM_SERVICE_STATUS_PROCESSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::ENUM_SERVICE_STATUS_PROCESSW", aliases: &[
        CLibAliases { names: &["ENUM_SERVICE_STATUS_PROCESSW", "_ENUM_SERVICE_STATUS_PROCESSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::LPHANDLER_FUNCTION", aliases: &[
        CLibAliases { names: &["LPHANDLER_FUNCTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::LPHANDLER_FUNCTION_EX", aliases: &[
        CLibAliases { names: &["LPHANDLER_FUNCTION_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::LPSERVICE_MAIN_FUNCTIONA", aliases: &[
        CLibAliases { names: &["LPSERVICE_MAIN_FUNCTIONA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::LPSERVICE_MAIN_FUNCTIONW", aliases: &[
        CLibAliases { names: &["LPSERVICE_MAIN_FUNCTIONW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::PFN_SC_NOTIFY_CALLBACK", aliases: &[
        CLibAliases { names: &["PFN_SC_NOTIFY_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::PSC_NOTIFICATION_CALLBACK", aliases: &[
        CLibAliases { names: &["PSC_NOTIFICATION_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::PSC_NOTIFICATION_REGISTRATION", aliases: &[
        CLibAliases { names: &["PSC_NOTIFICATION_REGISTRATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::QUERY_SERVICE_CONFIGA", aliases: &[
        CLibAliases { names: &["QUERY_SERVICE_CONFIGA", "_QUERY_SERVICE_CONFIGA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::QUERY_SERVICE_CONFIGW", aliases: &[
        CLibAliases { names: &["QUERY_SERVICE_CONFIGW", "_QUERY_SERVICE_CONFIGW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::QUERY_SERVICE_LOCK_STATUSA", aliases: &[
        CLibAliases { names: &["QUERY_SERVICE_LOCK_STATUSA", "_QUERY_SERVICE_LOCK_STATUSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::QUERY_SERVICE_LOCK_STATUSW", aliases: &[
        CLibAliases { names: &["QUERY_SERVICE_LOCK_STATUSW", "_QUERY_SERVICE_LOCK_STATUSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SC_ACTION", aliases: &[
        CLibAliases { names: &["SC_ACTION", "_SC_ACTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SC_HANDLE", aliases: &[
        CLibAliases { names: &["SC_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_CONTROL_STATUS_REASON_PARAMSA", aliases: &[
        CLibAliases { names: &["SERVICE_CONTROL_STATUS_REASON_PARAMSA", "_SERVICE_CONTROL_STATUS_REASON_PARAMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_CONTROL_STATUS_REASON_PARAMSW", aliases: &[
        CLibAliases { names: &["SERVICE_CONTROL_STATUS_REASON_PARAMSW", "_SERVICE_CONTROL_STATUS_REASON_PARAMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_CUSTOM_SYSTEM_STATE_CHANGE_DATA_ITEM", aliases: &[
        CLibAliases { names: &["SERVICE_CUSTOM_SYSTEM_STATE_CHANGE_DATA_ITEM", "_SERVICE_CUSTOM_SYSTEM_STATE_CHANGE_DATA_ITEM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_DELAYED_AUTO_START_INFO", aliases: &[
        CLibAliases { names: &["SERVICE_DELAYED_AUTO_START_INFO", "_SERVICE_DELAYED_AUTO_START_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_DESCRIPTIONA", aliases: &[
        CLibAliases { names: &["SERVICE_DESCRIPTIONA", "_SERVICE_DESCRIPTIONA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_DESCRIPTIONW", aliases: &[
        CLibAliases { names: &["SERVICE_DESCRIPTIONW", "_SERVICE_DESCRIPTIONW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_FAILURE_ACTIONSA", aliases: &[
        CLibAliases { names: &["SERVICE_FAILURE_ACTIONSA", "_SERVICE_FAILURE_ACTIONSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_FAILURE_ACTIONSW", aliases: &[
        CLibAliases { names: &["SERVICE_FAILURE_ACTIONSW", "_SERVICE_FAILURE_ACTIONSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_FAILURE_ACTIONS_FLAG", aliases: &[
        CLibAliases { names: &["SERVICE_FAILURE_ACTIONS_FLAG", "_SERVICE_FAILURE_ACTIONS_FLAG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_LAUNCH_PROTECTED_INFO", aliases: &[
        CLibAliases { names: &["SERVICE_LAUNCH_PROTECTED_INFO", "_SERVICE_LAUNCH_PROTECTED_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_NOTIFY_1", aliases: &[
        CLibAliases { names: &["SERVICE_NOTIFY_1", "_SERVICE_NOTIFY_1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_NOTIFY_2A", aliases: &[
        CLibAliases { names: &["SERVICE_NOTIFY_2A", "_SERVICE_NOTIFY_2A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_NOTIFY_2W", aliases: &[
        CLibAliases { names: &["SERVICE_NOTIFY_2W", "_SERVICE_NOTIFY_2W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_PREFERRED_NODE_INFO", aliases: &[
        CLibAliases { names: &["SERVICE_PREFERRED_NODE_INFO", "_SERVICE_PREFERRED_NODE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_PRESHUTDOWN_INFO", aliases: &[
        CLibAliases { names: &["SERVICE_PRESHUTDOWN_INFO", "_SERVICE_PRESHUTDOWN_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_REQUIRED_PRIVILEGES_INFOA", aliases: &[
        CLibAliases { names: &["SERVICE_REQUIRED_PRIVILEGES_INFOA", "_SERVICE_REQUIRED_PRIVILEGES_INFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_REQUIRED_PRIVILEGES_INFOW", aliases: &[
        CLibAliases { names: &["SERVICE_REQUIRED_PRIVILEGES_INFOW", "_SERVICE_REQUIRED_PRIVILEGES_INFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_SID_INFO", aliases: &[
        CLibAliases { names: &["SERVICE_SID_INFO", "_SERVICE_SID_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_START_REASON", aliases: &[
        CLibAliases { names: &["SERVICE_START_REASON", "_SERVICE_START_REASON"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_STATUS", aliases: &[
        CLibAliases { names: &["SERVICE_STATUS", "_SERVICE_STATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_STATUS_HANDLE", aliases: &[
        CLibAliases { names: &["SERVICE_STATUS_HANDLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_STATUS_PROCESS", aliases: &[
        CLibAliases { names: &["SERVICE_STATUS_PROCESS", "_SERVICE_STATUS_PROCESS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_TABLE_ENTRYA", aliases: &[
        CLibAliases { names: &["SERVICE_TABLE_ENTRYA", "_SERVICE_TABLE_ENTRYA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_TABLE_ENTRYW", aliases: &[
        CLibAliases { names: &["SERVICE_TABLE_ENTRYW", "_SERVICE_TABLE_ENTRYW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_TIMECHANGE_INFO", aliases: &[
        CLibAliases { names: &["SERVICE_TIMECHANGE_INFO", "_SERVICE_TIMECHANGE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_TRIGGER", aliases: &[
        CLibAliases { names: &["SERVICE_TRIGGER", "_SERVICE_TRIGGER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_TRIGGER_CUSTOM_STATE_ID", aliases: &[
        CLibAliases { names: &["SERVICE_TRIGGER_CUSTOM_STATE_ID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_TRIGGER_INFO", aliases: &[
        CLibAliases { names: &["SERVICE_TRIGGER_INFO", "_SERVICE_TRIGGER_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Services::SERVICE_TRIGGER_SPECIFIC_DATA_ITEM", aliases: &[
        CLibAliases { names: &["SERVICE_TRIGGER_SPECIFIC_DATA_ITEM", "_SERVICE_TRIGGER_SPECIFIC_DATA_ITEM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::BSMINFO", aliases: &[
        CLibAliases { names: &["BSMINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::DESKTOPENUMPROCA", aliases: &[
        CLibAliases { names: &["DESKTOPENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::DESKTOPENUMPROCW", aliases: &[
        CLibAliases { names: &["DESKTOPENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::HDESK", aliases: &[
        CLibAliases { names: &["HDESK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::HWINSTA", aliases: &[
        CLibAliases { names: &["HWINSTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::USEROBJECTFLAGS", aliases: &[
        CLibAliases { names: &["USEROBJECTFLAGS", "tagUSEROBJECTFLAGS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::WINSTAENUMPROCA", aliases: &[
        CLibAliases { names: &["WINSTAENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::StationsAndDesktops::WINSTAENUMPROCW", aliases: &[
        CLibAliases { names: &["WINSTAENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::CACHE_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["CACHE_DESCRIPTOR", "_CACHE_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::CACHE_RELATIONSHIP", aliases: &[
        CLibAliases { names: &["CACHE_RELATIONSHIP", "_CACHE_RELATIONSHIP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::GROUP_AFFINITY", aliases: &[
        CLibAliases { names: &["GROUP_AFFINITY", "_GROUP_AFFINITY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::GROUP_RELATIONSHIP", aliases: &[
        CLibAliases { names: &["GROUP_RELATIONSHIP", "_GROUP_RELATIONSHIP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::MEMORYSTATUS", aliases: &[
        CLibAliases { names: &["MEMORYSTATUS", "_MEMORYSTATUS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::MEMORYSTATUSEX", aliases: &[
        CLibAliases { names: &["MEMORYSTATUSEX", "_MEMORYSTATUSEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::NUMA_NODE_RELATIONSHIP", aliases: &[
        CLibAliases { names: &["NUMA_NODE_RELATIONSHIP", "_NUMA_NODE_RELATIONSHIP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::OSVERSIONINFOA", aliases: &[
        CLibAliases { names: &["OSVERSIONINFOA", "_OSVERSIONINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::OSVERSIONINFOEXA", aliases: &[
        CLibAliases { names: &["OSVERSIONINFOEXA", "_OSVERSIONINFOEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::OSVERSIONINFOEXW", aliases: &[
        CLibAliases { names: &["OSVERSIONINFOEXW", "_OSVERSIONINFOEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::OSVERSIONINFOW", aliases: &[
        CLibAliases { names: &["OSVERSIONINFOW", "_OSVERSIONINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::PGET_SYSTEM_WOW64_DIRECTORY_A", aliases: &[
        CLibAliases { names: &["PGET_SYSTEM_WOW64_DIRECTORY_A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::PGET_SYSTEM_WOW64_DIRECTORY_W", aliases: &[
        CLibAliases { names: &["PGET_SYSTEM_WOW64_DIRECTORY_W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::PROCESSOR_GROUP_INFO", aliases: &[
        CLibAliases { names: &["PROCESSOR_GROUP_INFO", "_PROCESSOR_GROUP_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::PROCESSOR_RELATIONSHIP", aliases: &[
        CLibAliases { names: &["PROCESSOR_RELATIONSHIP", "_PROCESSOR_RELATIONSHIP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::SYSTEM_CPU_SET_INFORMATION", aliases: &[
        CLibAliases { names: &["SYSTEM_CPU_SET_INFORMATION", "_SYSTEM_CPU_SET_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::SYSTEM_INFO", aliases: &[
        CLibAliases { names: &["SYSTEM_INFO", "_SYSTEM_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::SYSTEM_LOGICAL_PROCESSOR_INFORMATION", aliases: &[
        CLibAliases { names: &["SYSTEM_LOGICAL_PROCESSOR_INFORMATION", "_SYSTEM_LOGICAL_PROCESSOR_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX", aliases: &[
        CLibAliases { names: &["SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX", "_SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::SYSTEM_POOL_ZEROING_INFORMATION", aliases: &[
        CLibAliases { names: &["SYSTEM_POOL_ZEROING_INFORMATION", "_SYSTEM_POOL_ZEROING_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemInformation::SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION", aliases: &[
        CLibAliases { names: &["SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION", "_SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::ANON_OBJECT_HEADER", aliases: &[
        CLibAliases { names: &["ANON_OBJECT_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::ANON_OBJECT_HEADER_BIGOBJ", aliases: &[
        CLibAliases { names: &["ANON_OBJECT_HEADER_BIGOBJ"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::ANON_OBJECT_HEADER_V2", aliases: &[
        CLibAliases { names: &["ANON_OBJECT_HEADER_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::APPLICATIONLAUNCH_SETTING_VALUE", aliases: &[
        CLibAliases { names: &["APPLICATIONLAUNCH_SETTING_VALUE", "_APPLICATIONLAUNCH_SETTING_VALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::COMPONENT_FILTER", aliases: &[
        CLibAliases { names: &["COMPONENT_FILTER", "_COMPONENT_FILTER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::DISPATCHER_CONTEXT_NONVOLREG_ARM64", aliases: &[
        CLibAliases { names: &["DISPATCHER_CONTEXT_NONVOLREG_ARM64", "_DISPATCHER_CONTEXT_NONVOLREG_ARM64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::ENLISTMENT_BASIC_INFORMATION", aliases: &[
        CLibAliases { names: &["ENLISTMENT_BASIC_INFORMATION", "_ENLISTMENT_BASIC_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::ENLISTMENT_CRM_INFORMATION", aliases: &[
        CLibAliases { names: &["ENLISTMENT_CRM_INFORMATION", "_ENLISTMENT_CRM_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::FILE_NOTIFY_FULL_INFORMATION", aliases: &[
        CLibAliases { names: &["FILE_NOTIFY_FULL_INFORMATION", "_FILE_NOTIFY_FULL_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::GDI_NONREMOTE", aliases: &[
        CLibAliases { names: &["GDI_NONREMOTE", "_GDI_NONREMOTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::HEAP_OPTIMIZE_RESOURCES_INFORMATION", aliases: &[
        CLibAliases { names: &["HEAP_OPTIMIZE_RESOURCES_INFORMATION", "_HEAP_OPTIMIZE_RESOURCES_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::HIBERFILE_BUCKET", aliases: &[
        CLibAliases { names: &["HIBERFILE_BUCKET", "_HIBERFILE_BUCKET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY", "_IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY", "_IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_ARCHITECTURE_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_ARCHITECTURE_ENTRY", "_ImageArchitectureEntry"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_ARCHIVE_MEMBER_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_ARCHIVE_MEMBER_HEADER", "_IMAGE_ARCHIVE_MEMBER_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_ARM64_RUNTIME_FUNCTION_ENTRY_XDATA", aliases: &[
        CLibAliases { names: &["IMAGE_ARM64_RUNTIME_FUNCTION_ENTRY_XDATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_ARM_RUNTIME_FUNCTION_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_ARM_RUNTIME_FUNCTION_ENTRY", "_IMAGE_ARM_RUNTIME_FUNCTION_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_AUX_SYMBOL", aliases: &[
        CLibAliases { names: &["IMAGE_AUX_SYMBOL", "_IMAGE_AUX_SYMBOL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_AUX_SYMBOL_EX", aliases: &[
        CLibAliases { names: &["IMAGE_AUX_SYMBOL_EX", "_IMAGE_AUX_SYMBOL_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_AUX_SYMBOL_TOKEN_DEF", aliases: &[
        CLibAliases { names: &["IMAGE_AUX_SYMBOL_TOKEN_DEF"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_BASE_RELOCATION", aliases: &[
        CLibAliases { names: &["IMAGE_BASE_RELOCATION", "_IMAGE_BASE_RELOCATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_BDD_DYNAMIC_RELOCATION", aliases: &[
        CLibAliases { names: &["IMAGE_BDD_DYNAMIC_RELOCATION", "_IMAGE_BDD_DYNAMIC_RELOCATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_BDD_INFO", aliases: &[
        CLibAliases { names: &["IMAGE_BDD_INFO", "_IMAGE_BDD_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_BOUND_FORWARDER_REF", aliases: &[
        CLibAliases { names: &["IMAGE_BOUND_FORWARDER_REF", "_IMAGE_BOUND_FORWARDER_REF"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_BOUND_IMPORT_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["IMAGE_BOUND_IMPORT_DESCRIPTOR", "_IMAGE_BOUND_IMPORT_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_DEBUG_MISC", aliases: &[
        CLibAliases { names: &["IMAGE_DEBUG_MISC", "_IMAGE_DEBUG_MISC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_DOS_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_DOS_HEADER", "_IMAGE_DOS_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_DYNAMIC_RELOCATION32", aliases: &[
        CLibAliases { names: &["IMAGE_DYNAMIC_RELOCATION32", "_IMAGE_DYNAMIC_RELOCATION32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_DYNAMIC_RELOCATION32_V2", aliases: &[
        CLibAliases { names: &["IMAGE_DYNAMIC_RELOCATION32_V2", "_IMAGE_DYNAMIC_RELOCATION32_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_DYNAMIC_RELOCATION64", aliases: &[
        CLibAliases { names: &["IMAGE_DYNAMIC_RELOCATION64", "_IMAGE_DYNAMIC_RELOCATION64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_DYNAMIC_RELOCATION64_V2", aliases: &[
        CLibAliases { names: &["IMAGE_DYNAMIC_RELOCATION64_V2", "_IMAGE_DYNAMIC_RELOCATION64_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_DYNAMIC_RELOCATION_TABLE", aliases: &[
        CLibAliases { names: &["IMAGE_DYNAMIC_RELOCATION_TABLE", "_IMAGE_DYNAMIC_RELOCATION_TABLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_EPILOGUE_DYNAMIC_RELOCATION_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_EPILOGUE_DYNAMIC_RELOCATION_HEADER", "_IMAGE_EPILOGUE_DYNAMIC_RELOCATION_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_EXPORT_DIRECTORY", aliases: &[
        CLibAliases { names: &["IMAGE_EXPORT_DIRECTORY", "_IMAGE_EXPORT_DIRECTORY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_FUNCTION_OVERRIDE_DYNAMIC_RELOCATION", aliases: &[
        CLibAliases { names: &["IMAGE_FUNCTION_OVERRIDE_DYNAMIC_RELOCATION", "_IMAGE_FUNCTION_OVERRIDE_DYNAMIC_RELOCATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_FUNCTION_OVERRIDE_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_FUNCTION_OVERRIDE_HEADER", "_IMAGE_FUNCTION_OVERRIDE_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_HOT_PATCH_BASE", aliases: &[
        CLibAliases { names: &["IMAGE_HOT_PATCH_BASE", "_IMAGE_HOT_PATCH_BASE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_HOT_PATCH_HASHES", aliases: &[
        CLibAliases { names: &["IMAGE_HOT_PATCH_HASHES", "_IMAGE_HOT_PATCH_HASHES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_IMPORT_BY_NAME", aliases: &[
        CLibAliases { names: &["IMAGE_IMPORT_BY_NAME", "_IMAGE_IMPORT_BY_NAME"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_IMPORT_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["IMAGE_IMPORT_DESCRIPTOR", "_IMAGE_IMPORT_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_LINENUMBER", aliases: &[
        CLibAliases { names: &["IMAGE_LINENUMBER", "_IMAGE_LINENUMBER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_OS2_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_OS2_HEADER", "_IMAGE_OS2_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_POLICY_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_POLICY_ENTRY", "_IMAGE_POLICY_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_PROLOGUE_DYNAMIC_RELOCATION_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_PROLOGUE_DYNAMIC_RELOCATION_HEADER", "_IMAGE_PROLOGUE_DYNAMIC_RELOCATION_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_RELOCATION", aliases: &[
        CLibAliases { names: &["IMAGE_RELOCATION", "_IMAGE_RELOCATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_RESOURCE_DATA_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_RESOURCE_DATA_ENTRY", "_IMAGE_RESOURCE_DATA_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_RESOURCE_DIRECTORY", aliases: &[
        CLibAliases { names: &["IMAGE_RESOURCE_DIRECTORY", "_IMAGE_RESOURCE_DIRECTORY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_RESOURCE_DIRECTORY_ENTRY", aliases: &[
        CLibAliases { names: &["IMAGE_RESOURCE_DIRECTORY_ENTRY", "_IMAGE_RESOURCE_DIRECTORY_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_RESOURCE_DIRECTORY_STRING", aliases: &[
        CLibAliases { names: &["IMAGE_RESOURCE_DIRECTORY_STRING", "_IMAGE_RESOURCE_DIRECTORY_STRING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_RESOURCE_DIR_STRING_U", aliases: &[
        CLibAliases { names: &["IMAGE_RESOURCE_DIR_STRING_U", "_IMAGE_RESOURCE_DIR_STRING_U"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_SEPARATE_DEBUG_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_SEPARATE_DEBUG_HEADER", "_IMAGE_SEPARATE_DEBUG_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_SYMBOL", aliases: &[
        CLibAliases { names: &["IMAGE_SYMBOL", "_IMAGE_SYMBOL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_SYMBOL_EX", aliases: &[
        CLibAliases { names: &["IMAGE_SYMBOL_EX", "_IMAGE_SYMBOL_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_TLS_DIRECTORY32", aliases: &[
        CLibAliases { names: &["IMAGE_TLS_DIRECTORY32", "_IMAGE_TLS_DIRECTORY32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_TLS_DIRECTORY64", aliases: &[
        CLibAliases { names: &["IMAGE_TLS_DIRECTORY64", "_IMAGE_TLS_DIRECTORY64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::IMAGE_VXD_HEADER", aliases: &[
        CLibAliases { names: &["IMAGE_VXD_HEADER", "_IMAGE_VXD_HEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::KERNEL_CET_CONTEXT", aliases: &[
        CLibAliases { names: &["KERNEL_CET_CONTEXT", "_KERNEL_CET_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::KTMOBJECT_CURSOR", aliases: &[
        CLibAliases { names: &["KTMOBJECT_CURSOR", "_KTMOBJECT_CURSOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::MAXVERSIONTESTED_INFO", aliases: &[
        CLibAliases { names: &["MAXVERSIONTESTED_INFO", "_MAXVERSIONTESTED_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::NETWORK_APP_INSTANCE_EA", aliases: &[
        CLibAliases { names: &["NETWORK_APP_INSTANCE_EA", "_NETWORK_APP_INSTANCE_EA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::NON_PAGED_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["NON_PAGED_DEBUG_INFO", "_NON_PAGED_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::NOTIFY_USER_POWER_SETTING", aliases: &[
        CLibAliases { names: &["NOTIFY_USER_POWER_SETTING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PEXCEPTION_FILTER", aliases: &[
        CLibAliases { names: &["PEXCEPTION_FILTER"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PIMAGE_TLS_CALLBACK", aliases: &[
        CLibAliases { names: &["PIMAGE_TLS_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::POUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK", aliases: &[
        CLibAliases { names: &["POUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESSOR_IDLESTATE_INFO", aliases: &[
        CLibAliases { names: &["PROCESSOR_IDLESTATE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESSOR_IDLESTATE_POLICY", aliases: &[
        CLibAliases { names: &["PROCESSOR_IDLESTATE_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESSOR_PERFSTATE_POLICY", aliases: &[
        CLibAliases { names: &["PROCESSOR_PERFSTATE_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_ASLR_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_ASLR_POLICY", "_PROCESS_MITIGATION_ASLR_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY", "_PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_CHILD_PROCESS_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_CHILD_PROCESS_POLICY", "_PROCESS_MITIGATION_CHILD_PROCESS_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_CONTROL_FLOW_GUARD_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_CONTROL_FLOW_GUARD_POLICY", "_PROCESS_MITIGATION_CONTROL_FLOW_GUARD_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_DEP_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_DEP_POLICY", "_PROCESS_MITIGATION_DEP_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_DYNAMIC_CODE_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_DYNAMIC_CODE_POLICY", "_PROCESS_MITIGATION_DYNAMIC_CODE_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_EXTENSION_POINT_DISABLE_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_EXTENSION_POINT_DISABLE_POLICY", "_PROCESS_MITIGATION_EXTENSION_POINT_DISABLE_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_FONT_DISABLE_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_FONT_DISABLE_POLICY", "_PROCESS_MITIGATION_FONT_DISABLE_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_IMAGE_LOAD_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_IMAGE_LOAD_POLICY", "_PROCESS_MITIGATION_IMAGE_LOAD_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_PAYLOAD_RESTRICTION_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_PAYLOAD_RESTRICTION_POLICY", "_PROCESS_MITIGATION_PAYLOAD_RESTRICTION_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_REDIRECTION_TRUST_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_REDIRECTION_TRUST_POLICY", "_PROCESS_MITIGATION_REDIRECTION_TRUST_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_SEHOP_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_SEHOP_POLICY", "_PROCESS_MITIGATION_SEHOP_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_SIDE_CHANNEL_ISOLATION_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_SIDE_CHANNEL_ISOLATION_POLICY", "_PROCESS_MITIGATION_SIDE_CHANNEL_ISOLATION_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_STRICT_HANDLE_CHECK_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_STRICT_HANDLE_CHECK_POLICY", "_PROCESS_MITIGATION_STRICT_HANDLE_CHECK_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY", "_PROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_SYSTEM_CALL_FILTER_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_SYSTEM_CALL_FILTER_POLICY", "_PROCESS_MITIGATION_SYSTEM_CALL_FILTER_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_USER_POINTER_AUTH_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_USER_POINTER_AUTH_POLICY", "_PROCESS_MITIGATION_USER_POINTER_AUTH_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY", aliases: &[
        CLibAliases { names: &["PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY", "_PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PTERMINATION_HANDLER", aliases: &[
        CLibAliases { names: &["PTERMINATION_HANDLER"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::PUMS_SCHEDULER_ENTRY_POINT", aliases: &[
        CLibAliases { names: &["PUMS_SCHEDULER_ENTRY_POINT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::QUOTA_LIMITS_EX", aliases: &[
        CLibAliases { names: &["QUOTA_LIMITS_EX", "_QUOTA_LIMITS_EX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RATE_QUOTA_LIMIT", aliases: &[
        CLibAliases { names: &["RATE_QUOTA_LIMIT", "_RATE_QUOTA_LIMIT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::REARRANGE_FILE_DATA", aliases: &[
        CLibAliases { names: &["REARRANGE_FILE_DATA", "_REARRANGE_FILE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::REARRANGE_FILE_DATA32", aliases: &[
        CLibAliases { names: &["REARRANGE_FILE_DATA32", "_REARRANGE_FILE_DATA32"], targets: &["aarch64-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RESOURCEMANAGER_BASIC_INFORMATION", aliases: &[
        CLibAliases { names: &["RESOURCEMANAGER_BASIC_INFORMATION", "_RESOURCEMANAGER_BASIC_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RESOURCEMANAGER_COMPLETION_INFORMATION", aliases: &[
        CLibAliases { names: &["RESOURCEMANAGER_COMPLETION_INFORMATION", "_RESOURCEMANAGER_COMPLETION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RemHBITMAP", aliases: &[
        CLibAliases { names: &["RemHBITMAP", "tagRemHBITMAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RemHBRUSH", aliases: &[
        CLibAliases { names: &["RemHBRUSH", "tagRemBRUSH"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RemHENHMETAFILE", aliases: &[
        CLibAliases { names: &["RemHENHMETAFILE", "tagRemHENHMETAFILE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RemHGLOBAL", aliases: &[
        CLibAliases { names: &["RemHGLOBAL", "tagRemHGLOBAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RemHMETAFILEPICT", aliases: &[
        CLibAliases { names: &["RemHMETAFILEPICT", "tagRemHMETAFILEPICT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RemHPALETTE", aliases: &[
        CLibAliases { names: &["RemHPALETTE", "tagRemHPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::RemotableHandle", aliases: &[
        CLibAliases { names: &["RemotableHandle", "_RemotableHandle"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SCOPE_TABLE_AMD64", aliases: &[
        CLibAliases { names: &["SCOPE_TABLE_AMD64", "_SCOPE_TABLE_AMD64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SCOPE_TABLE_ARM", aliases: &[
        CLibAliases { names: &["SCOPE_TABLE_ARM", "_SCOPE_TABLE_ARM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SCOPE_TABLE_ARM64", aliases: &[
        CLibAliases { names: &["SCOPE_TABLE_ARM64", "_SCOPE_TABLE_ARM64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SCRUB_PARITY_EXTENT", aliases: &[
        CLibAliases { names: &["SCRUB_PARITY_EXTENT", "_SCRUB_PARITY_EXTENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SCRUB_PARITY_EXTENT_DATA", aliases: &[
        CLibAliases { names: &["SCRUB_PARITY_EXTENT_DATA", "_SCRUB_PARITY_EXTENT_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SECURITY_OBJECT_AI_PARAMS", aliases: &[
        CLibAliases { names: &["SECURITY_OBJECT_AI_PARAMS", "_SECURITY_OBJECT_AI_PARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SE_TOKEN_USER", aliases: &[
        CLibAliases { names: &["SE_TOKEN_USER", "_SE_TOKEN_USER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SHARED_VIRTUAL_DISK_SUPPORT", aliases: &[
        CLibAliases { names: &["SHARED_VIRTUAL_DISK_SUPPORT", "_SHARED_VIRTUAL_DISK_SUPPORT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SHUFFLE_FILE_DATA", aliases: &[
        CLibAliases { names: &["SHUFFLE_FILE_DATA", "_SHUFFLE_FILE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SILOOBJECT_BASIC_INFORMATION", aliases: &[
        CLibAliases { names: &["SILOOBJECT_BASIC_INFORMATION", "_SILOOBJECT_BASIC_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::SUPPORTED_OS_INFO", aliases: &[
        CLibAliases { names: &["SUPPORTED_OS_INFO", "_SUPPORTED_OS_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TAPE_CREATE_PARTITION", aliases: &[
        CLibAliases { names: &["TAPE_CREATE_PARTITION", "_TAPE_CREATE_PARTITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TAPE_GET_DRIVE_PARAMETERS", aliases: &[
        CLibAliases { names: &["TAPE_GET_DRIVE_PARAMETERS", "_TAPE_GET_DRIVE_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TAPE_GET_MEDIA_PARAMETERS", aliases: &[
        CLibAliases { names: &["TAPE_GET_MEDIA_PARAMETERS", "_TAPE_GET_MEDIA_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TAPE_SET_DRIVE_PARAMETERS", aliases: &[
        CLibAliases { names: &["TAPE_SET_DRIVE_PARAMETERS", "_TAPE_SET_DRIVE_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TAPE_SET_MEDIA_PARAMETERS", aliases: &[
        CLibAliases { names: &["TAPE_SET_MEDIA_PARAMETERS", "_TAPE_SET_MEDIA_PARAMETERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TAPE_WMI_OPERATIONS", aliases: &[
        CLibAliases { names: &["TAPE_WMI_OPERATIONS", "_TAPE_WMI_OPERATIONS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TOKEN_BNO_ISOLATION_INFORMATION", aliases: &[
        CLibAliases { names: &["TOKEN_BNO_ISOLATION_INFORMATION", "_TOKEN_BNO_ISOLATION_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TOKEN_SID_INFORMATION", aliases: &[
        CLibAliases { names: &["TOKEN_SID_INFORMATION", "_TOKEN_SID_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTIONMANAGER_BASIC_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTIONMANAGER_BASIC_INFORMATION", "_TRANSACTIONMANAGER_BASIC_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTIONMANAGER_LOGPATH_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTIONMANAGER_LOGPATH_INFORMATION", "_TRANSACTIONMANAGER_LOGPATH_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTIONMANAGER_LOG_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTIONMANAGER_LOG_INFORMATION", "_TRANSACTIONMANAGER_LOG_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTIONMANAGER_OLDEST_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTIONMANAGER_OLDEST_INFORMATION", "_TRANSACTIONMANAGER_OLDEST_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTIONMANAGER_RECOVERY_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTIONMANAGER_RECOVERY_INFORMATION", "_TRANSACTIONMANAGER_RECOVERY_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_BASIC_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTION_BASIC_INFORMATION", "_TRANSACTION_BASIC_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_BIND_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTION_BIND_INFORMATION", "_TRANSACTION_BIND_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_ENLISTMENTS_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTION_ENLISTMENTS_INFORMATION", "_TRANSACTION_ENLISTMENTS_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_ENLISTMENT_PAIR", aliases: &[
        CLibAliases { names: &["TRANSACTION_ENLISTMENT_PAIR", "_TRANSACTION_ENLISTMENT_PAIR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_LIST_ENTRY", aliases: &[
        CLibAliases { names: &["TRANSACTION_LIST_ENTRY", "_TRANSACTION_LIST_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_LIST_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTION_LIST_INFORMATION", "_TRANSACTION_LIST_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_PROPERTIES_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTION_PROPERTIES_INFORMATION", "_TRANSACTION_PROPERTIES_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::TRANSACTION_SUPERIOR_ENLISTMENT_INFORMATION", aliases: &[
        CLibAliases { names: &["TRANSACTION_SUPERIOR_ENLISTMENT_INFORMATION", "_TRANSACTION_SUPERIOR_ENLISTMENT_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::UMS_CREATE_THREAD_ATTRIBUTES", aliases: &[
        CLibAliases { names: &["UMS_CREATE_THREAD_ATTRIBUTES", "_UMS_CREATE_THREAD_ATTRIBUTES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::XSAVE_CET_U_FORMAT", aliases: &[
        CLibAliases { names: &["XSAVE_CET_U_FORMAT", "_XSAVE_CET_U_FORMAT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::remoteMETAFILEPICT", aliases: &[
        CLibAliases { names: &["_remoteMETAFILEPICT", "remoteMETAFILEPICT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userBITMAP", aliases: &[
        CLibAliases { names: &["_userBITMAP", "userBITMAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userCLIPFORMAT", aliases: &[
        CLibAliases { names: &["_userCLIPFORMAT", "userCLIPFORMAT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userHBITMAP", aliases: &[
        CLibAliases { names: &["_userHBITMAP", "userHBITMAP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userHENHMETAFILE", aliases: &[
        CLibAliases { names: &["_userHENHMETAFILE", "userHENHMETAFILE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userHGLOBAL", aliases: &[
        CLibAliases { names: &["_userHGLOBAL", "userHGLOBAL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userHMETAFILE", aliases: &[
        CLibAliases { names: &["_userHMETAFILE", "userHMETAFILE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userHMETAFILEPICT", aliases: &[
        CLibAliases { names: &["_userHMETAFILEPICT", "userHMETAFILEPICT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::SystemServices::userHPALETTE", aliases: &[
        CLibAliases { names: &["_userHPALETTE", "userHPALETTE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::APC_CALLBACK_FUNCTION", aliases: &[
        CLibAliases { names: &["APC_CALLBACK_FUNCTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::APP_MEMORY_INFORMATION", aliases: &[
        CLibAliases { names: &["APP_MEMORY_INFORMATION", "_APP_MEMORY_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::CONDITION_VARIABLE", aliases: &[
        CLibAliases { names: &["CONDITION_VARIABLE", "_RTL_CONDITION_VARIABLE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::CRITICAL_SECTION", aliases: &[
        CLibAliases { names: &["CRITICAL_SECTION", "_RTL_CRITICAL_SECTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::CRITICAL_SECTION_DEBUG", aliases: &[
        CLibAliases { names: &["CRITICAL_SECTION_DEBUG", "_RTL_CRITICAL_SECTION_DEBUG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::INIT_ONCE", aliases: &[
        CLibAliases { names: &["INIT_ONCE", "_RTL_RUN_ONCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::IO_COUNTERS", aliases: &[
        CLibAliases { names: &["IO_COUNTERS", "_IO_COUNTERS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::LPFIBER_START_ROUTINE", aliases: &[
        CLibAliases { names: &["LPFIBER_START_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::LPPROC_THREAD_ATTRIBUTE_LIST", aliases: &[
        CLibAliases { names: &["LPPROC_THREAD_ATTRIBUTE_LIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::LPTHREAD_START_ROUTINE", aliases: &[
        CLibAliases { names: &["LPTHREAD_START_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::MEMORY_PRIORITY_INFORMATION", aliases: &[
        CLibAliases { names: &["MEMORY_PRIORITY_INFORMATION", "_MEMORY_PRIORITY_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::OVERRIDE_PREFETCH_PARAMETER", aliases: &[
        CLibAliases { names: &["OVERRIDE_PREFETCH_PARAMETER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PFLS_CALLBACK_FUNCTION", aliases: &[
        CLibAliases { names: &["PFLS_CALLBACK_FUNCTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PINIT_ONCE_FN", aliases: &[
        CLibAliases { names: &["PINIT_ONCE_FN"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_DYNAMIC_EH_CONTINUATION_TARGET", aliases: &[
        CLibAliases { names: &["PROCESS_DYNAMIC_EH_CONTINUATION_TARGET", "_PROCESS_DYNAMIC_EH_CONTINUATION_TARGET"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION", aliases: &[
        CLibAliases { names: &["PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION", "_PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE", aliases: &[
        CLibAliases { names: &["PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE", "_PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGES_INFORMATION", aliases: &[
        CLibAliases { names: &["PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGES_INFORMATION", "_PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGES_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_INFORMATION", aliases: &[
        CLibAliases { names: &["PROCESS_INFORMATION", "_PROCESS_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_LEAP_SECOND_INFO", aliases: &[
        CLibAliases { names: &["PROCESS_LEAP_SECOND_INFO", "_PROCESS_LEAP_SECOND_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_MACHINE_INFORMATION", aliases: &[
        CLibAliases { names: &["PROCESS_MACHINE_INFORMATION", "_PROCESS_MACHINE_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_MEMORY_EXHAUSTION_INFO", aliases: &[
        CLibAliases { names: &["PROCESS_MEMORY_EXHAUSTION_INFO", "_PROCESS_MEMORY_EXHAUSTION_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_POWER_THROTTLING_STATE", aliases: &[
        CLibAliases { names: &["PROCESS_POWER_THROTTLING_STATE", "_PROCESS_POWER_THROTTLING_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PROCESS_PROTECTION_LEVEL_INFORMATION", aliases: &[
        CLibAliases { names: &["PROCESS_PROTECTION_LEVEL_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PRTL_UMS_SCHEDULER_ENTRY_POINT", aliases: &[
        CLibAliases { names: &["PRTL_UMS_SCHEDULER_ENTRY_POINT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTIMERAPCROUTINE", aliases: &[
        CLibAliases { names: &["PTIMERAPCROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_CALLBACK_INSTANCE", aliases: &[
        CLibAliases { names: &["PTP_CALLBACK_INSTANCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_CLEANUP_GROUP", aliases: &[
        CLibAliases { names: &["PTP_CLEANUP_GROUP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_CLEANUP_GROUP_CANCEL_CALLBACK", aliases: &[
        CLibAliases { names: &["PTP_CLEANUP_GROUP_CANCEL_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_IO", aliases: &[
        CLibAliases { names: &["PTP_IO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_POOL", aliases: &[
        CLibAliases { names: &["PTP_POOL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_SIMPLE_CALLBACK", aliases: &[
        CLibAliases { names: &["PTP_SIMPLE_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_TIMER", aliases: &[
        CLibAliases { names: &["PTP_TIMER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_TIMER_CALLBACK", aliases: &[
        CLibAliases { names: &["PTP_TIMER_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_WAIT", aliases: &[
        CLibAliases { names: &["PTP_WAIT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_WAIT_CALLBACK", aliases: &[
        CLibAliases { names: &["PTP_WAIT_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_WIN32_IO_CALLBACK", aliases: &[
        CLibAliases { names: &["PTP_WIN32_IO_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_WORK", aliases: &[
        CLibAliases { names: &["PTP_WORK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::PTP_WORK_CALLBACK", aliases: &[
        CLibAliases { names: &["PTP_WORK_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::REASON_CONTEXT", aliases: &[
        CLibAliases { names: &["REASON_CONTEXT", "_REASON_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::SRWLOCK", aliases: &[
        CLibAliases { names: &["SRWLOCK", "_RTL_SRWLOCK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::STARTUPINFOA", aliases: &[
        CLibAliases { names: &["STARTUPINFOA", "_STARTUPINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::STARTUPINFOEXA", aliases: &[
        CLibAliases { names: &["STARTUPINFOEXA", "_STARTUPINFOEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::STARTUPINFOEXW", aliases: &[
        CLibAliases { names: &["STARTUPINFOEXW", "_STARTUPINFOEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::STARTUPINFOW", aliases: &[
        CLibAliases { names: &["STARTUPINFOW", "_STARTUPINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::SYNCHRONIZATION_BARRIER", aliases: &[
        CLibAliases { names: &["SYNCHRONIZATION_BARRIER", "_RTL_BARRIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::THREAD_POWER_THROTTLING_STATE", aliases: &[
        CLibAliases { names: &["THREAD_POWER_THROTTLING_STATE", "_THREAD_POWER_THROTTLING_STATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::TP_CALLBACK_ENVIRON_V3", aliases: &[
        CLibAliases { names: &["TP_CALLBACK_ENVIRON_V3", "_TP_CALLBACK_ENVIRON_V3"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::TP_POOL_STACK_INFORMATION", aliases: &[
        CLibAliases { names: &["TP_POOL_STACK_INFORMATION", "_TP_POOL_STACK_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::UMS_SCHEDULER_STARTUP_INFO", aliases: &[
        CLibAliases { names: &["UMS_SCHEDULER_STARTUP_INFO", "_UMS_SCHEDULER_STARTUP_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::UMS_SYSTEM_THREAD_INFORMATION", aliases: &[
        CLibAliases { names: &["UMS_SYSTEM_THREAD_INFORMATION", "_UMS_SYSTEM_THREAD_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::WAITORTIMERCALLBACK", aliases: &[
        CLibAliases { names: &["WAITORTIMERCALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Threading::WORKERCALLBACKFUNC", aliases: &[
        CLibAliases { names: &["WORKERCALLBACKFUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Time::DYNAMIC_TIME_ZONE_INFORMATION", aliases: &[
        CLibAliases { names: &["DYNAMIC_TIME_ZONE_INFORMATION", "_TIME_DYNAMIC_ZONE_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Time::TIME_ZONE_INFORMATION", aliases: &[
        CLibAliases { names: &["TIME_ZONE_INFORMATION", "_TIME_ZONE_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::Variant::VARIANT", aliases: &[
        CLibAliases { names: &["VARIANT", "tagVARIANT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::ACTCTX_SECTION_KEYED_DATA_2600", aliases: &[
        CLibAliases { names: &["ACTCTX_SECTION_KEYED_DATA_2600", "tagACTCTX_SECTION_KEYED_DATA_2600"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::ACTCTX_SECTION_KEYED_DATA_ASSEMBLY_METADATA", aliases: &[
        CLibAliases { names: &["ACTCTX_SECTION_KEYED_DATA_ASSEMBLY_METADATA", "tagACTCTX_SECTION_KEYED_DATA_ASSEMBLY_METADATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::ACTIVATION_CONTEXT_BASIC_INFORMATION", aliases: &[
        CLibAliases { names: &["ACTIVATION_CONTEXT_BASIC_INFORMATION", "_ACTIVATION_CONTEXT_BASIC_INFORMATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::APPLICATION_RECOVERY_CALLBACK", aliases: &[
        CLibAliases { names: &["APPLICATION_RECOVERY_CALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::CUSTOM_SYSTEM_EVENT_TRIGGER_CONFIG", aliases: &[
        CLibAliases { names: &["CUSTOM_SYSTEM_EVENT_TRIGGER_CONFIG", "_CUSTOM_SYSTEM_EVENT_TRIGGER_CONFIG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::HW_PROFILE_INFOA", aliases: &[
        CLibAliases { names: &["HW_PROFILE_INFOA", "tagHW_PROFILE_INFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::HW_PROFILE_INFOW", aliases: &[
        CLibAliases { names: &["HW_PROFILE_INFOW", "tagHW_PROFILE_INFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::IMAGE_DELAYLOAD_DESCRIPTOR", aliases: &[
        CLibAliases { names: &["IMAGE_DELAYLOAD_DESCRIPTOR", "_IMAGE_DELAYLOAD_DESCRIPTOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::IMAGE_THUNK_DATA32", aliases: &[
        CLibAliases { names: &["IMAGE_THUNK_DATA32", "_IMAGE_THUNK_DATA32"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::IMAGE_THUNK_DATA64", aliases: &[
        CLibAliases { names: &["IMAGE_THUNK_DATA64", "_IMAGE_THUNK_DATA64"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::JIT_DEBUG_INFO", aliases: &[
        CLibAliases { names: &["JIT_DEBUG_INFO", "_JIT_DEBUG_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::PFIBER_CALLOUT_ROUTINE", aliases: &[
        CLibAliases { names: &["PFIBER_CALLOUT_ROUTINE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::System::WindowsProgramming::PQUERYACTCTXW_FUNC", aliases: &[
        CLibAliases { names: &["PQUERYACTCTXW_FUNC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::ACCESSTIMEOUT", aliases: &[
        CLibAliases { names: &["ACCESSTIMEOUT", "tagACCESSTIMEOUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::FILTERKEYS", aliases: &[
        CLibAliases { names: &["FILTERKEYS", "tagFILTERKEYS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::HIGHCONTRASTA", aliases: &[
        CLibAliases { names: &["HIGHCONTRASTA", "tagHIGHCONTRASTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::HIGHCONTRASTW", aliases: &[
        CLibAliases { names: &["HIGHCONTRASTW", "tagHIGHCONTRASTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::HWINEVENTHOOK", aliases: &[
        CLibAliases { names: &["HWINEVENTHOOK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::MOUSEKEYS", aliases: &[
        CLibAliases { names: &["MOUSEKEYS", "tagMOUSEKEYS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::SERIALKEYSA", aliases: &[
        CLibAliases { names: &["SERIALKEYSA", "tagSERIALKEYSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::SERIALKEYSW", aliases: &[
        CLibAliases { names: &["SERIALKEYSW", "tagSERIALKEYSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::SOUNDSENTRYA", aliases: &[
        CLibAliases { names: &["SOUNDSENTRYA", "tagSOUNDSENTRYA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::SOUNDSENTRYW", aliases: &[
        CLibAliases { names: &["SOUNDSENTRYW", "tagSOUNDSENTRYW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::STICKYKEYS", aliases: &[
        CLibAliases { names: &["STICKYKEYS", "tagSTICKYKEYS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::TOGGLEKEYS", aliases: &[
        CLibAliases { names: &["TOGGLEKEYS", "tagTOGGLEKEYS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Accessibility::WINEVENTPROC", aliases: &[
        CLibAliases { names: &["WINEVENTPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::ColorSystem::EMRCREATECOLORSPACE", aliases: &[
        CLibAliases { names: &["EMRCREATECOLORSPACE", "tagEMRCREATECOLORSPACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::ColorSystem::EMRCREATECOLORSPACEW", aliases: &[
        CLibAliases { names: &["EMRCREATECOLORSPACEW", "tagEMRCREATECOLORSPACEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::ColorSystem::HCOLORSPACE", aliases: &[
        CLibAliases { names: &["HCOLORSPACE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::ColorSystem::ICMENUMPROCA", aliases: &[
        CLibAliases { names: &["ICMENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::ColorSystem::ICMENUMPROCW", aliases: &[
        CLibAliases { names: &["ICMENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::ColorSystem::LOGCOLORSPACEA", aliases: &[
        CLibAliases { names: &["LOGCOLORSPACEA", "tagLOGCOLORSPACEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::ColorSystem::LOGCOLORSPACEW", aliases: &[
        CLibAliases { names: &["LOGCOLORSPACEW", "tagLOGCOLORSPACEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::COMBOBOXINFO", aliases: &[
        CLibAliases { names: &["COMBOBOXINFO", "tagCOMBOBOXINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::COMPAREITEMSTRUCT", aliases: &[
        CLibAliases { names: &["COMPAREITEMSTRUCT", "tagCOMPAREITEMSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::DELETEITEMSTRUCT", aliases: &[
        CLibAliases { names: &["DELETEITEMSTRUCT", "tagDELETEITEMSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::DRAWITEMSTRUCT", aliases: &[
        CLibAliases { names: &["DRAWITEMSTRUCT", "tagDRAWITEMSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::CHOOSECOLORA", aliases: &[
        CLibAliases { names: &["CHOOSECOLORA", "tagCHOOSECOLORA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::CHOOSECOLORW", aliases: &[
        CLibAliases { names: &["CHOOSECOLORW", "tagCHOOSECOLORW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::CHOOSEFONTA", aliases: &[
        CLibAliases { names: &["CHOOSEFONTA", "tagCHOOSEFONTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::CHOOSEFONTW", aliases: &[
        CLibAliases { names: &["CHOOSEFONTW", "tagCHOOSEFONTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::DEVNAMES", aliases: &[
        CLibAliases { names: &["DEVNAMES", "tagDEVNAMES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::FINDREPLACEA", aliases: &[
        CLibAliases { names: &["FINDREPLACEA", "tagFINDREPLACEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::FINDREPLACEW", aliases: &[
        CLibAliases { names: &["FINDREPLACEW", "tagFINDREPLACEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPCCHOOKPROC", aliases: &[
        CLibAliases { names: &["LPCCHOOKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPCFHOOKPROC", aliases: &[
        CLibAliases { names: &["LPCFHOOKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPFRHOOKPROC", aliases: &[
        CLibAliases { names: &["LPFRHOOKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPOFNHOOKPROC", aliases: &[
        CLibAliases { names: &["LPOFNHOOKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPPAGEPAINTHOOK", aliases: &[
        CLibAliases { names: &["LPPAGEPAINTHOOK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPPAGESETUPHOOK", aliases: &[
        CLibAliases { names: &["LPPAGESETUPHOOK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPPRINTHOOKPROC", aliases: &[
        CLibAliases { names: &["LPPRINTHOOKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::LPSETUPHOOKPROC", aliases: &[
        CLibAliases { names: &["LPSETUPHOOKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OFNOTIFYA", aliases: &[
        CLibAliases { names: &["OFNOTIFYA", "_OFNOTIFYA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OFNOTIFYEXA", aliases: &[
        CLibAliases { names: &["OFNOTIFYEXA", "_OFNOTIFYEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OFNOTIFYEXW", aliases: &[
        CLibAliases { names: &["OFNOTIFYEXW", "_OFNOTIFYEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OFNOTIFYW", aliases: &[
        CLibAliases { names: &["OFNOTIFYW", "_OFNOTIFYW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OPENFILENAMEA", aliases: &[
        CLibAliases { names: &["OPENFILENAMEA", "tagOFNA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OPENFILENAMEW", aliases: &[
        CLibAliases { names: &["OPENFILENAMEW", "tagOFNW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OPENFILENAME_NT4A", aliases: &[
        CLibAliases { names: &["OPENFILENAME_NT4A", "tagOFN_NT4A"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::OPENFILENAME_NT4W", aliases: &[
        CLibAliases { names: &["OPENFILENAME_NT4W", "tagOFN_NT4W"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::PAGESETUPDLGA", aliases: &[
        CLibAliases { names: &["PAGESETUPDLGA", "tagPSDA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::PAGESETUPDLGW", aliases: &[
        CLibAliases { names: &["PAGESETUPDLGW", "tagPSDW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::PRINTDLGA", aliases: &[
        CLibAliases { names: &["PRINTDLGA", "tagPDA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::PRINTDLGEXA", aliases: &[
        CLibAliases { names: &["PRINTDLGEXA", "tagPDEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::PRINTDLGEXW", aliases: &[
        CLibAliases { names: &["PRINTDLGEXW", "tagPDEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::PRINTDLGW", aliases: &[
        CLibAliases { names: &["PRINTDLGW", "tagPDW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::Dialogs::PRINTPAGERANGE", aliases: &[
        CLibAliases { names: &["PRINTPAGERANGE", "tagPRINTPAGERANGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::EDITWORDBREAKPROCA", aliases: &[
        CLibAliases { names: &["EDITWORDBREAKPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::EDITWORDBREAKPROCW", aliases: &[
        CLibAliases { names: &["EDITWORDBREAKPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::HPROPSHEETPAGE", aliases: &[
        CLibAliases { names: &["HPROPSHEETPAGE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::HSYNTHETICPOINTERDEVICE", aliases: &[
        CLibAliases { names: &["HSYNTHETICPOINTERDEVICE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::LPFNADDPROPSHEETPAGES", aliases: &[
        CLibAliases { names: &["LPFNADDPROPSHEETPAGES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::LPFNPSPCALLBACKA", aliases: &[
        CLibAliases { names: &["LPFNPSPCALLBACKA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::LPFNPSPCALLBACKW", aliases: &[
        CLibAliases { names: &["LPFNPSPCALLBACKW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::MEASUREITEMSTRUCT", aliases: &[
        CLibAliases { names: &["MEASUREITEMSTRUCT", "tagMEASUREITEMSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::NMHDR", aliases: &[
        CLibAliases { names: &["NMHDR", "tagNMHDR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PFNPROPSHEETCALLBACK", aliases: &[
        CLibAliases { names: &["PFNPROPSHEETCALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::POINTER_DEVICE_CURSOR_INFO", aliases: &[
        CLibAliases { names: &["POINTER_DEVICE_CURSOR_INFO", "tagPOINTER_DEVICE_CURSOR_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::POINTER_DEVICE_INFO", aliases: &[
        CLibAliases { names: &["POINTER_DEVICE_INFO", "tagPOINTER_DEVICE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::POINTER_DEVICE_PROPERTY", aliases: &[
        CLibAliases { names: &["POINTER_DEVICE_PROPERTY", "tagPOINTER_DEVICE_PROPERTY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::POINTER_TYPE_INFO", aliases: &[
        CLibAliases { names: &["POINTER_TYPE_INFO", "tagPOINTER_TYPE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETHEADERA_V1", aliases: &[
        CLibAliases { names: &["PROPSHEETHEADERA_V1", "_PROPSHEETHEADERA_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETHEADERA_V2", aliases: &[
        CLibAliases { names: &["PROPSHEETHEADERA_V2", "_PROPSHEETHEADERA_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETHEADERW_V1", aliases: &[
        CLibAliases { names: &["PROPSHEETHEADERW_V1", "_PROPSHEETHEADERW_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETHEADERW_V2", aliases: &[
        CLibAliases { names: &["PROPSHEETHEADERW_V2", "_PROPSHEETHEADERW_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEA", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEA", "_PROPSHEETPAGEA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEA_V1", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEA_V1", "_PROPSHEETPAGEA_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEA_V2", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEA_V2", "_PROPSHEETPAGEA_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEA_V3", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEA_V3", "_PROPSHEETPAGEA_V3"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEW", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEW", "_PROPSHEETPAGEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEW_V1", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEW_V1", "_PROPSHEETPAGEW_V1"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEW_V2", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEW_V2", "_PROPSHEETPAGEW_V2"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PROPSHEETPAGEW_V3", aliases: &[
        CLibAliases { names: &["PROPSHEETPAGEW_V3", "_PROPSHEETPAGEW_V3"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::PSHNOTIFY", aliases: &[
        CLibAliases { names: &["PSHNOTIFY", "_PSHNOTIFY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::TOUCH_HIT_TESTING_INPUT", aliases: &[
        CLibAliases { names: &["TOUCH_HIT_TESTING_INPUT", "tagTOUCH_HIT_TESTING_INPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::TOUCH_HIT_TESTING_PROXIMITY_EVALUATION", aliases: &[
        CLibAliases { names: &["TOUCH_HIT_TESTING_PROXIMITY_EVALUATION", "tagTOUCH_HIT_TESTING_PROXIMITY_EVALUATION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Controls::USAGE_PROPERTIES", aliases: &[
        CLibAliases { names: &["USAGE_PROPERTIES", "tagUSAGE_PROPERTIES"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::HiDpi::DPI_AWARENESS_CONTEXT", aliases: &[
        CLibAliases { names: &["DPI_AWARENESS_CONTEXT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::HRAWINPUT", aliases: &[
        CLibAliases { names: &["HRAWINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::INPUT_MESSAGE_SOURCE", aliases: &[
        CLibAliases { names: &["INPUT_MESSAGE_SOURCE", "tagINPUT_MESSAGE_SOURCE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::CANDIDATEFORM", aliases: &[
        CLibAliases { names: &["CANDIDATEFORM", "tagCANDIDATEFORM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::CANDIDATELIST", aliases: &[
        CLibAliases { names: &["CANDIDATELIST", "tagCANDIDATELIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::COMPOSITIONFORM", aliases: &[
        CLibAliases { names: &["COMPOSITIONFORM", "tagCOMPOSITIONFORM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::HIMC", aliases: &[
        CLibAliases { names: &["HIMC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::HIMCC", aliases: &[
        CLibAliases { names: &["HIMCC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::IMCENUMPROC", aliases: &[
        CLibAliases { names: &["IMCENUMPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::IMECHARPOSITION", aliases: &[
        CLibAliases { names: &["IMECHARPOSITION", "tagIMECHARPOSITION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::IMEMENUITEMINFOA", aliases: &[
        CLibAliases { names: &["IMEMENUITEMINFOA", "tagIMEMENUITEMINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::IMEMENUITEMINFOW", aliases: &[
        CLibAliases { names: &["IMEMENUITEMINFOW", "tagIMEMENUITEMINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::RECONVERTSTRING", aliases: &[
        CLibAliases { names: &["RECONVERTSTRING", "tagRECONVERTSTRING"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::REGISTERWORDA", aliases: &[
        CLibAliases { names: &["REGISTERWORDA", "tagREGISTERWORDA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::REGISTERWORDENUMPROCA", aliases: &[
        CLibAliases { names: &["REGISTERWORDENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::REGISTERWORDENUMPROCW", aliases: &[
        CLibAliases { names: &["REGISTERWORDENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::REGISTERWORDW", aliases: &[
        CLibAliases { names: &["REGISTERWORDW", "tagREGISTERWORDW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::STYLEBUFA", aliases: &[
        CLibAliases { names: &["STYLEBUFA", "tagSTYLEBUFA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Ime::STYLEBUFW", aliases: &[
        CLibAliases { names: &["STYLEBUFW", "tagSTYLEBUFW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::HARDWAREINPUT", aliases: &[
        CLibAliases { names: &["HARDWAREINPUT", "tagHARDWAREINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::HKL", aliases: &[
        CLibAliases { names: &["HKL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::INPUT", aliases: &[
        CLibAliases { names: &["INPUT", "tagINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::KEYBDINPUT", aliases: &[
        CLibAliases { names: &["KEYBDINPUT", "tagKEYBDINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::LASTINPUTINFO", aliases: &[
        CLibAliases { names: &["LASTINPUTINFO", "tagLASTINPUTINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::MOUSEINPUT", aliases: &[
        CLibAliases { names: &["MOUSEINPUT", "tagMOUSEINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::MOUSEMOVEPOINT", aliases: &[
        CLibAliases { names: &["MOUSEMOVEPOINT", "tagMOUSEMOVEPOINT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::KeyboardAndMouse::TRACKMOUSEEVENT", aliases: &[
        CLibAliases { names: &["TRACKMOUSEEVENT", "tagTRACKMOUSEEVENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Pointer::INPUT_INJECTION_VALUE", aliases: &[
        CLibAliases { names: &["INPUT_INJECTION_VALUE", "tagINPUT_INJECTION_VALUE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Pointer::INPUT_TRANSFORM", aliases: &[
        CLibAliases { names: &["INPUT_TRANSFORM", "tagINPUT_TRANSFORM"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Pointer::POINTER_INFO", aliases: &[
        CLibAliases { names: &["POINTER_INFO", "tagPOINTER_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Pointer::POINTER_PEN_INFO", aliases: &[
        CLibAliases { names: &["POINTER_PEN_INFO", "tagPOINTER_PEN_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Pointer::POINTER_TOUCH_INFO", aliases: &[
        CLibAliases { names: &["POINTER_TOUCH_INFO", "tagPOINTER_TOUCH_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RAWHID", aliases: &[
        CLibAliases { names: &["RAWHID", "tagRAWHID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RAWINPUT", aliases: &[
        CLibAliases { names: &["RAWINPUT", "tagRAWINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RAWINPUTDEVICE", aliases: &[
        CLibAliases { names: &["RAWINPUTDEVICE", "tagRAWINPUTDEVICE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RAWINPUTDEVICELIST", aliases: &[
        CLibAliases { names: &["RAWINPUTDEVICELIST", "tagRAWINPUTDEVICELIST"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RAWINPUTHEADER", aliases: &[
        CLibAliases { names: &["RAWINPUTHEADER", "tagRAWINPUTHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RAWKEYBOARD", aliases: &[
        CLibAliases { names: &["RAWKEYBOARD", "tagRAWKEYBOARD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RAWMOUSE", aliases: &[
        CLibAliases { names: &["RAWMOUSE", "tagRAWMOUSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RID_DEVICE_INFO", aliases: &[
        CLibAliases { names: &["RID_DEVICE_INFO", "tagRID_DEVICE_INFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RID_DEVICE_INFO_HID", aliases: &[
        CLibAliases { names: &["RID_DEVICE_INFO_HID", "tagRID_DEVICE_INFO_HID"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RID_DEVICE_INFO_KEYBOARD", aliases: &[
        CLibAliases { names: &["RID_DEVICE_INFO_KEYBOARD", "tagRID_DEVICE_INFO_KEYBOARD"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::RID_DEVICE_INFO_MOUSE", aliases: &[
        CLibAliases { names: &["RID_DEVICE_INFO_MOUSE", "tagRID_DEVICE_INFO_MOUSE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Touch::GESTURECONFIG", aliases: &[
        CLibAliases { names: &["GESTURECONFIG", "tagGESTURECONFIG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Touch::GESTUREINFO", aliases: &[
        CLibAliases { names: &["GESTUREINFO", "tagGESTUREINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Touch::GESTURENOTIFYSTRUCT", aliases: &[
        CLibAliases { names: &["GESTURENOTIFYSTRUCT", "tagGESTURENOTIFYSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Touch::HGESTUREINFO", aliases: &[
        CLibAliases { names: &["HGESTUREINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Touch::HTOUCHINPUT", aliases: &[
        CLibAliases { names: &["HTOUCHINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Input::Touch::TOUCHINPUT", aliases: &[
        CLibAliases { names: &["TOUCHINPUT", "tagTOUCHINPUT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::APPBARDATA", aliases: &[
        CLibAliases { names: &["APPBARDATA", "_AppBarData"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::ASSOCIATIONELEMENT", aliases: &[
        CLibAliases { names: &["ASSOCIATIONELEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::DRAGINFOA", aliases: &[
        CLibAliases { names: &["DRAGINFOA", "_DRAGINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::DRAGINFOW", aliases: &[
        CLibAliases { names: &["DRAGINFOW", "_DRAGINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::HDROP", aliases: &[
        CLibAliases { names: &["HDROP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::HELPINFO", aliases: &[
        CLibAliases { names: &["HELPINFO", "tagHELPINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::HELPWININFOA", aliases: &[
        CLibAliases { names: &["HELPWININFOA", "tagHELPWININFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::HELPWININFOW", aliases: &[
        CLibAliases { names: &["HELPWININFOW", "tagHELPWININFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::MULTIKEYHELPA", aliases: &[
        CLibAliases { names: &["MULTIKEYHELPA", "tagMULTIKEYHELPA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::MULTIKEYHELPW", aliases: &[
        CLibAliases { names: &["MULTIKEYHELPW", "tagMULTIKEYHELPW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::NC_ADDRESS", aliases: &[
        CLibAliases { names: &["NC_ADDRESS", "tagNC_ADDRESS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::NOTIFYICONDATAA", aliases: &[
        CLibAliases { names: &["NOTIFYICONDATAA", "_NOTIFYICONDATAA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::NOTIFYICONDATAW", aliases: &[
        CLibAliases { names: &["NOTIFYICONDATAW", "_NOTIFYICONDATAW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::NOTIFYICONIDENTIFIER", aliases: &[
        CLibAliases { names: &["NOTIFYICONIDENTIFIER", "_NOTIFYICONIDENTIFIER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::OPEN_PRINTER_PROPS_INFOA", aliases: &[
        CLibAliases { names: &["OPEN_PRINTER_PROPS_INFOA", "_OPEN_PRINTER_PROPS_INFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::OPEN_PRINTER_PROPS_INFOW", aliases: &[
        CLibAliases { names: &["OPEN_PRINTER_PROPS_INFOW", "_OPEN_PRINTER_PROPS_INFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::PFNCANSHAREFOLDERW", aliases: &[
        CLibAliases { names: &["PFNCANSHAREFOLDERW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::PFNSHOWSHAREFOLDERUIW", aliases: &[
        CLibAliases { names: &["PFNSHOWSHAREFOLDERUIW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHCREATEPROCESSINFOW", aliases: &[
        CLibAliases { names: &["SHCREATEPROCESSINFOW", "_SHCREATEPROCESSINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHELLEXECUTEINFOA", aliases: &[
        CLibAliases { names: &["SHELLEXECUTEINFOA", "_SHELLEXECUTEINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHELLEXECUTEINFOW", aliases: &[
        CLibAliases { names: &["SHELLEXECUTEINFOW", "_SHELLEXECUTEINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHFILEINFOA", aliases: &[
        CLibAliases { names: &["SHFILEINFOA", "_SHFILEINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHFILEINFOW", aliases: &[
        CLibAliases { names: &["SHFILEINFOW", "_SHFILEINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHFILEOPSTRUCTA", aliases: &[
        CLibAliases { names: &["SHFILEOPSTRUCTA", "_SHFILEOPSTRUCTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHFILEOPSTRUCTW", aliases: &[
        CLibAliases { names: &["SHFILEOPSTRUCTW", "_SHFILEOPSTRUCTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHNAMEMAPPINGA", aliases: &[
        CLibAliases { names: &["SHNAMEMAPPINGA", "_SHNAMEMAPPINGA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHNAMEMAPPINGW", aliases: &[
        CLibAliases { names: &["SHNAMEMAPPINGW", "_SHNAMEMAPPINGW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHQUERYRBINFO", aliases: &[
        CLibAliases { names: &["SHQUERYRBINFO", "_SHQUERYRBINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::Shell::SHSTOCKICONINFO", aliases: &[
        CLibAliases { names: &["SHSTOCKICONINFO", "_SHSTOCKICONINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ACCEL", aliases: &[
        CLibAliases { names: &["ACCEL", "tagACCEL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ALTTABINFO", aliases: &[
        CLibAliases { names: &["ALTTABINFO", "tagALTTABINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ANIMATIONINFO", aliases: &[
        CLibAliases { names: &["ANIMATIONINFO", "tagANIMATIONINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::AUDIODESCRIPTION", aliases: &[
        CLibAliases { names: &["AUDIODESCRIPTION", "tagAUDIODESCRIPTION"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CBTACTIVATESTRUCT", aliases: &[
        CLibAliases { names: &["CBTACTIVATESTRUCT", "tagCBTACTIVATESTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CBT_CREATEWNDA", aliases: &[
        CLibAliases { names: &["CBT_CREATEWNDA", "tagCBT_CREATEWNDA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CBT_CREATEWNDW", aliases: &[
        CLibAliases { names: &["CBT_CREATEWNDW", "tagCBT_CREATEWNDW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CHANGEFILTERSTRUCT", aliases: &[
        CLibAliases { names: &["CHANGEFILTERSTRUCT", "tagCHANGEFILTERSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CLIENTCREATESTRUCT", aliases: &[
        CLibAliases { names: &["CLIENTCREATESTRUCT", "tagCLIENTCREATESTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CREATESTRUCTA", aliases: &[
        CLibAliases { names: &["CREATESTRUCTA", "tagCREATESTRUCTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CREATESTRUCTW", aliases: &[
        CLibAliases { names: &["CREATESTRUCTW", "tagCREATESTRUCTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CURSORINFO", aliases: &[
        CLibAliases { names: &["CURSORINFO", "tagCURSORINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CURSORSHAPE", aliases: &[
        CLibAliases { names: &["CURSORSHAPE", "tagCURSORSHAPE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CWPRETSTRUCT", aliases: &[
        CLibAliases { names: &["CWPRETSTRUCT", "tagCWPRETSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::CWPSTRUCT", aliases: &[
        CLibAliases { names: &["CWPSTRUCT", "tagCWPSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::DEBUGHOOKINFO", aliases: &[
        CLibAliases { names: &["DEBUGHOOKINFO", "tagDEBUGHOOKINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::DLGITEMTEMPLATE", aliases: &[
        CLibAliases { names: &["DLGITEMTEMPLATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::DLGPROC", aliases: &[
        CLibAliases { names: &["DLGPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::DLGTEMPLATE", aliases: &[
        CLibAliases { names: &["DLGTEMPLATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::DROPSTRUCT", aliases: &[
        CLibAliases { names: &["DROPSTRUCT", "tagDROPSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::EVENTMSG", aliases: &[
        CLibAliases { names: &["EVENTMSG", "tagEVENTMSG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::FLASHWINFO", aliases: &[
        CLibAliases { names: &["FLASHWINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::GETCLIPBMETADATA", aliases: &[
        CLibAliases { names: &["GETCLIPBMETADATA", "tagGETCLIPBMETADATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::GUITHREADINFO", aliases: &[
        CLibAliases { names: &["GUITHREADINFO", "tagGUITHREADINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HACCEL", aliases: &[
        CLibAliases { names: &["HACCEL"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HARDWAREHOOKSTRUCT", aliases: &[
        CLibAliases { names: &["HARDWAREHOOKSTRUCT", "tagHARDWAREHOOKSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HCURSOR", aliases: &[
        CLibAliases { names: &["HCURSOR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HDEVNOTIFY", aliases: &[
        CLibAliases { names: &["HDEVNOTIFY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HDWP", aliases: &[
        CLibAliases { names: &["HDWP"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HHOOK", aliases: &[
        CLibAliases { names: &["HHOOK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HICON", aliases: &[
        CLibAliases { names: &["HICON"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HMENU", aliases: &[
        CLibAliases { names: &["HMENU"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::HOOKPROC", aliases: &[
        CLibAliases { names: &["HOOKPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ICONINFO", aliases: &[
        CLibAliases { names: &["ICONINFO", "_ICONINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ICONINFOEXA", aliases: &[
        CLibAliases { names: &["ICONINFOEXA", "_ICONINFOEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ICONINFOEXW", aliases: &[
        CLibAliases { names: &["ICONINFOEXW", "_ICONINFOEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ICONMETRICSA", aliases: &[
        CLibAliases { names: &["ICONMETRICSA", "tagICONMETRICSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::ICONMETRICSW", aliases: &[
        CLibAliases { names: &["ICONMETRICSW", "tagICONMETRICSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::KBDLLHOOKSTRUCT", aliases: &[
        CLibAliases { names: &["KBDLLHOOKSTRUCT", "tagKBDLLHOOKSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MDICREATESTRUCTA", aliases: &[
        CLibAliases { names: &["MDICREATESTRUCTA", "tagMDICREATESTRUCTA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MDICREATESTRUCTW", aliases: &[
        CLibAliases { names: &["MDICREATESTRUCTW", "tagMDICREATESTRUCTW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MDINEXTMENU", aliases: &[
        CLibAliases { names: &["MDINEXTMENU", "tagMDINEXTMENU"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MENUGETOBJECTINFO", aliases: &[
        CLibAliases { names: &["MENUGETOBJECTINFO", "tagMENUGETOBJECTINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MENUINFO", aliases: &[
        CLibAliases { names: &["MENUINFO", "tagMENUINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MENUITEMINFOA", aliases: &[
        CLibAliases { names: &["MENUITEMINFOA", "tagMENUITEMINFOA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MENUITEMINFOW", aliases: &[
        CLibAliases { names: &["MENUITEMINFOW", "tagMENUITEMINFOW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MENUITEMTEMPLATE", aliases: &[
        CLibAliases { names: &["MENUITEMTEMPLATE"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MENUITEMTEMPLATEHEADER", aliases: &[
        CLibAliases { names: &["MENUITEMTEMPLATEHEADER"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MESSAGE_RESOURCE_BLOCK", aliases: &[
        CLibAliases { names: &["MESSAGE_RESOURCE_BLOCK", "_MESSAGE_RESOURCE_BLOCK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MESSAGE_RESOURCE_DATA", aliases: &[
        CLibAliases { names: &["MESSAGE_RESOURCE_DATA", "_MESSAGE_RESOURCE_DATA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MESSAGE_RESOURCE_ENTRY", aliases: &[
        CLibAliases { names: &["MESSAGE_RESOURCE_ENTRY", "_MESSAGE_RESOURCE_ENTRY"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MINIMIZEDMETRICS", aliases: &[
        CLibAliases { names: &["MINIMIZEDMETRICS", "tagMINIMIZEDMETRICS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MINMAXINFO", aliases: &[
        CLibAliases { names: &["MINMAXINFO", "tagMINMAXINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MOUSEHOOKSTRUCT", aliases: &[
        CLibAliases { names: &["MOUSEHOOKSTRUCT", "tagMOUSEHOOKSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MOUSEHOOKSTRUCTEX", aliases: &[
        CLibAliases { names: &["MOUSEHOOKSTRUCTEX", "tagMOUSEHOOKSTRUCTEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MSG", aliases: &[
        CLibAliases { names: &["MSG", "tagMSG"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MSGBOXCALLBACK", aliases: &[
        CLibAliases { names: &["MSGBOXCALLBACK"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MSGBOXPARAMSA", aliases: &[
        CLibAliases { names: &["MSGBOXPARAMSA", "tagMSGBOXPARAMSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MSGBOXPARAMSW", aliases: &[
        CLibAliases { names: &["MSGBOXPARAMSW", "tagMSGBOXPARAMSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::MSLLHOOKSTRUCT", aliases: &[
        CLibAliases { names: &["MSLLHOOKSTRUCT", "tagMSLLHOOKSTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::NAMEENUMPROCA", aliases: &[
        CLibAliases { names: &["NAMEENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::NAMEENUMPROCW", aliases: &[
        CLibAliases { names: &["NAMEENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::NCCALCSIZE_PARAMS", aliases: &[
        CLibAliases { names: &["NCCALCSIZE_PARAMS", "tagNCCALCSIZE_PARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::NONCLIENTMETRICSA", aliases: &[
        CLibAliases { names: &["NONCLIENTMETRICSA", "tagNONCLIENTMETRICSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::NONCLIENTMETRICSW", aliases: &[
        CLibAliases { names: &["NONCLIENTMETRICSW", "tagNONCLIENTMETRICSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::PREGISTERCLASSNAMEW", aliases: &[
        CLibAliases { names: &["PREGISTERCLASSNAMEW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::PROPENUMPROCA", aliases: &[
        CLibAliases { names: &["PROPENUMPROCA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::PROPENUMPROCEXA", aliases: &[
        CLibAliases { names: &["PROPENUMPROCEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::PROPENUMPROCEXW", aliases: &[
        CLibAliases { names: &["PROPENUMPROCEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::PROPENUMPROCW", aliases: &[
        CLibAliases { names: &["PROPENUMPROCW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::SCROLLBARINFO", aliases: &[
        CLibAliases { names: &["SCROLLBARINFO", "tagSCROLLBARINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::SCROLLINFO", aliases: &[
        CLibAliases { names: &["SCROLLINFO", "tagSCROLLINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::SENDASYNCPROC", aliases: &[
        CLibAliases { names: &["SENDASYNCPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::SHELLHOOKINFO", aliases: &[
        CLibAliases { names: &["SHELLHOOKINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::STYLESTRUCT", aliases: &[
        CLibAliases { names: &["STYLESTRUCT", "tagSTYLESTRUCT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::TIMERPROC", aliases: &[
        CLibAliases { names: &["TIMERPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::TITLEBARINFO", aliases: &[
        CLibAliases { names: &["TITLEBARINFO", "tagTITLEBARINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::TITLEBARINFOEX", aliases: &[
        CLibAliases { names: &["TITLEBARINFOEX", "tagTITLEBARINFOEX"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::TOUCHPREDICTIONPARAMETERS", aliases: &[
        CLibAliases { names: &["TOUCHPREDICTIONPARAMETERS", "tagTouchPredictionParameters"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::TPMPARAMS", aliases: &[
        CLibAliases { names: &["TPMPARAMS", "tagTPMPARAMS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::UPDATELAYEREDWINDOWINFO", aliases: &[
        CLibAliases { names: &["UPDATELAYEREDWINDOWINFO", "tagUPDATELAYEREDWINDOWINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WINDOWINFO", aliases: &[
        CLibAliases { names: &["WINDOWINFO", "tagWINDOWINFO"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WINDOWPLACEMENT", aliases: &[
        CLibAliases { names: &["WINDOWPLACEMENT", "tagWINDOWPLACEMENT"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WINDOWPOS", aliases: &[
        CLibAliases { names: &["WINDOWPOS", "tagWINDOWPOS"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WNDCLASSA", aliases: &[
        CLibAliases { names: &["WNDCLASSA", "tagWNDCLASSA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WNDCLASSEXA", aliases: &[
        CLibAliases { names: &["WNDCLASSEXA", "tagWNDCLASSEXA"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WNDCLASSEXW", aliases: &[
        CLibAliases { names: &["WNDCLASSEXW", "tagWNDCLASSEXW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WNDCLASSW", aliases: &[
        CLibAliases { names: &["WNDCLASSW", "tagWNDCLASSW"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WNDENUMPROC", aliases: &[
        CLibAliases { names: &["WNDENUMPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::Win32::UI::WindowsAndMessaging::WNDPROC", aliases: &[
        CLibAliases { names: &["WNDPROC"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::core::BSTR", aliases: &[
        CLibAliases { names: &["BSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::core::PCSTR", aliases: &[
        CLibAliases { names: &["PCSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::core::PCWSTR", aliases: &[
        CLibAliases { names: &["PCWSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::core::PSTR", aliases: &[
        CLibAliases { names: &["PSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::windows::core::PWSTR", aliases: &[
        CLibAliases { names: &["PWSTR"], targets: &["aarch64-pc-windows-msvc", "i686-pc-windows-msvc", "x86_64-pc-windows-msvc"] },
    ] },
    CLibRecord { rust_path: "slate_support::winsize", aliases: &[
        CLibAliases { names: &["winsize"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
];
