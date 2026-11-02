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
    CLibRecord { rust_path: "slate_support::locale_t", aliases: &[
        CLibAliases { names: &["locale_t"], targets: &["aarch64-linux-android", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
        CLibAliases { names: &["sockaddr"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
    CLibRecord { rust_path: "slate_support::tcp_fastopen", aliases: &[
        CLibAliases { names: &["tcp_fastopen"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::tcp_function_set", aliases: &[
        CLibAliases { names: &["tcp_function_set"], targets: &["aarch64-unknown-freebsd", "x86_64-unknown-freebsd"] },
    ] },
    CLibRecord { rust_path: "slate_support::termios", aliases: &[
        CLibAliases { names: &["termios"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timer_t", aliases: &[
        CLibAliases { names: &["timer_t"], targets: &["aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timespec", aliases: &[
        CLibAliases { names: &["timespec"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-pc-windows-msvc", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-pc-windows-msvc", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-pc-windows-msvc", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
    ] },
    CLibRecord { rust_path: "slate_support::timeval", aliases: &[
        CLibAliases { names: &["timeval"], targets: &["aarch64-apple-darwin", "aarch64-linux-android", "aarch64-unknown-freebsd", "aarch64-unknown-linux-gnu", "aarch64-unknown-linux-musl", "armv7-unknown-linux-gnueabihf", "i686-unknown-linux-gnu", "x86_64-apple-darwin", "x86_64-linux-android", "x86_64-unknown-freebsd", "x86_64-unknown-linux-gnu", "x86_64-unknown-linux-musl"] },
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
    CLibRecord { rust_path: "slate_support::winsize", aliases: &[
        CLibAliases { names: &["winsize"], targets: &["aarch64-unknown-freebsd", "aarch64-unknown-linux-musl", "x86_64-unknown-freebsd", "x86_64-unknown-linux-musl"] },
    ] },
];
