use std::io;

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Target {
    X86_64PcWindowsMsvc,
    Aarch64PcWindowsMsvc,
    X86_64UnknownLinuxGnu,
    Aarch64UnknownLinuxGnu,
    X86_64UnknownLinuxMusl,
    Aarch64UnknownLinuxMusl,
}

impl Target {
    pub fn triple(self) -> &'static str {
        match self {
            Self::X86_64PcWindowsMsvc => "x86_64-pc-windows-msvc",
            Self::Aarch64PcWindowsMsvc => "aarch64-pc-windows-msvc",
            Self::X86_64UnknownLinuxGnu => "x86_64-unknown-linux-gnu",
            Self::Aarch64UnknownLinuxGnu => "aarch64-unknown-linux-gnu",
            Self::X86_64UnknownLinuxMusl => "x86_64-unknown-linux-musl",
            Self::Aarch64UnknownLinuxMusl => "aarch64-unknown-linux-musl",
        }
    }
}

impl std::str::FromStr for Target {
    type Err = io::Error;

    fn from_str(value: &str) -> io::Result<Self> {
        match value {
            "x86_64-pc-windows-msvc" => Ok(Self::X86_64PcWindowsMsvc),
            "aarch64-pc-windows-msvc" => Ok(Self::Aarch64PcWindowsMsvc),
            "x86_64-unknown-linux-gnu" => Ok(Self::X86_64UnknownLinuxGnu),
            "aarch64-unknown-linux-gnu" => Ok(Self::Aarch64UnknownLinuxGnu),
            "x86_64-unknown-linux-musl" => Ok(Self::X86_64UnknownLinuxMusl),
            "aarch64-unknown-linux-musl" => Ok(Self::Aarch64UnknownLinuxMusl),
            _ => Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                format!("unsupported Rust target: {value}"),
            )),
        }
    }
}
