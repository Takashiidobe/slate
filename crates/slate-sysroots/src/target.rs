use std::io;

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Target {
    I686PcWindowsMsvc,
    X86_64PcWindowsMsvc,
    Aarch64PcWindowsMsvc,
    Thumbv7aPcWindowsMsvc,
    X86_64UnknownLinuxGnu,
    Aarch64UnknownLinuxGnu,
    I686UnknownLinuxGnu,
    Armv7UnknownLinuxGnueabi,
    Armv7UnknownLinuxGnueabihf,
    X86_64UnknownLinuxMusl,
    Aarch64UnknownLinuxMusl,
    X86_64AppleDarwin,
    Aarch64AppleDarwin,
    X86_64UnknownFreebsd,
    Aarch64UnknownFreebsd,
    X86_64LinuxAndroid,
    Aarch64LinuxAndroid,
}

impl Target {
    pub fn triple(self) -> &'static str {
        match self {
            Self::I686PcWindowsMsvc => "i686-pc-windows-msvc",
            Self::X86_64PcWindowsMsvc => "x86_64-pc-windows-msvc",
            Self::Aarch64PcWindowsMsvc => "aarch64-pc-windows-msvc",
            Self::Thumbv7aPcWindowsMsvc => "thumbv7a-pc-windows-msvc",
            Self::X86_64UnknownLinuxGnu => "x86_64-unknown-linux-gnu",
            Self::Aarch64UnknownLinuxGnu => "aarch64-unknown-linux-gnu",
            Self::I686UnknownLinuxGnu => "i686-unknown-linux-gnu",
            Self::Armv7UnknownLinuxGnueabi => "armv7-unknown-linux-gnueabi",
            Self::Armv7UnknownLinuxGnueabihf => "armv7-unknown-linux-gnueabihf",
            Self::X86_64UnknownLinuxMusl => "x86_64-unknown-linux-musl",
            Self::Aarch64UnknownLinuxMusl => "aarch64-unknown-linux-musl",
            Self::X86_64AppleDarwin => "x86_64-apple-darwin",
            Self::Aarch64AppleDarwin => "aarch64-apple-darwin",
            Self::X86_64UnknownFreebsd => "x86_64-unknown-freebsd",
            Self::Aarch64UnknownFreebsd => "aarch64-unknown-freebsd",
            Self::X86_64LinuxAndroid => "x86_64-linux-android",
            Self::Aarch64LinuxAndroid => "aarch64-linux-android",
        }
    }
}

impl std::str::FromStr for Target {
    type Err = io::Error;

    fn from_str(value: &str) -> io::Result<Self> {
        match value {
            "i686-pc-windows-msvc" => Ok(Self::I686PcWindowsMsvc),
            "x86_64-pc-windows-msvc" => Ok(Self::X86_64PcWindowsMsvc),
            "aarch64-pc-windows-msvc" => Ok(Self::Aarch64PcWindowsMsvc),
            "thumbv7a-pc-windows-msvc" => Ok(Self::Thumbv7aPcWindowsMsvc),
            "x86_64-unknown-linux-gnu" => Ok(Self::X86_64UnknownLinuxGnu),
            "aarch64-unknown-linux-gnu" => Ok(Self::Aarch64UnknownLinuxGnu),
            "i686-unknown-linux-gnu" => Ok(Self::I686UnknownLinuxGnu),
            "armv7-unknown-linux-gnueabi" => Ok(Self::Armv7UnknownLinuxGnueabi),
            "armv7-unknown-linux-gnueabihf" => Ok(Self::Armv7UnknownLinuxGnueabihf),
            "x86_64-unknown-linux-musl" => Ok(Self::X86_64UnknownLinuxMusl),
            "aarch64-unknown-linux-musl" => Ok(Self::Aarch64UnknownLinuxMusl),
            "x86_64-apple-darwin" => Ok(Self::X86_64AppleDarwin),
            "aarch64-apple-darwin" => Ok(Self::Aarch64AppleDarwin),
            "x86_64-unknown-freebsd" => Ok(Self::X86_64UnknownFreebsd),
            "aarch64-unknown-freebsd" => Ok(Self::Aarch64UnknownFreebsd),
            "x86_64-linux-android" => Ok(Self::X86_64LinuxAndroid),
            "aarch64-linux-android" => Ok(Self::Aarch64LinuxAndroid),
            _ => Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                format!("unsupported Rust target: {value}"),
            )),
        }
    }
}
