use crate::{
    BitnessObservable, Canonicalizable, Parsable, ParseError, arch::ArchPart, env::Env,
    kernel::Kernel, obj::Obj, vendor::Vendor,
};

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
#[cfg_attr(feature = "serde", derive(::serde::Serialize, ::serde::Deserialize))]
pub struct Triple {
    pub arch: ArchPart,
    pub vendor: Option<Vendor>,
    pub kernel: Kernel,
    pub env: Option<Env>,
    pub obj: Option<Obj>,
}

impl core::str::FromStr for Triple {
    type Err = ParseError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Triple::parse(s)
    }
}

impl Triple {
    pub fn is_freestanding(&self) -> bool {
        self.kernel == Kernel::None && self.env.is_none() && self.vendor.is_none()
    }

    pub fn bitness(&self) -> u16 {
        self.arch.bitness()
    }

    pub fn parse(s: &str) -> Result<Self, ParseError> {
        let mut pos = 0;

        let (arch, arch_len) = ArchPart::reduce(s).ok_or(ParseError::InvalidArch)?;
        pos += arch_len;

        if pos >= s.len() {
            return Err(ParseError::UnexpectedEnd);
        }
        if !s[pos..].starts_with('-') {
            return Err(ParseError::ExpectedDash);
        }
        pos += 1;

        let (vendor, vendor_len) = if let Some((v, len)) = Vendor::reduce(&s[pos..]) {
            (Some(v), len)
        } else {
            (None, 0)
        };
        pos += vendor_len;

        if vendor.is_some() {
            if pos >= s.len() {
                return Err(ParseError::UnexpectedEnd);
            }
            if !s[pos..].starts_with('-') {
                return Err(ParseError::ExpectedDash);
            }
            pos += 1;
        }

        if pos >= s.len() {
            return Err(ParseError::UnexpectedEnd);
        }
        let (kernel, kernel_len) = Kernel::reduce(&s[pos..]).ok_or(ParseError::InvalidKernel)?;
        pos += kernel_len;

        let (env, obj) = if pos < s.len() {
            if !s[pos..].starts_with('-') {
                return Err(ParseError::ExpectedDash);
            }
            pos += 1;

            if pos >= s.len() {
                return Err(ParseError::UnexpectedEnd);
            }

            if let Some((env, env_len)) = Env::reduce(&s[pos..]) {
                let new_pos = pos + env_len;
                let obj = if new_pos < s.len() {
                    if !s[new_pos..].starts_with('-') {
                        return Err(ParseError::ExpectedDash);
                    }
                    let obj_pos = new_pos + 1;
                    if obj_pos >= s.len() {
                        return Err(ParseError::UnexpectedEnd);
                    }
                    if let Some((obj, obj_len)) = Obj::reduce(&s[obj_pos..]) {
                        if obj_pos + obj_len != s.len() {
                            return Err(ParseError::TrailingCharacters);
                        }
                        Some(obj)
                    } else {
                        return Err(ParseError::InvalidObj);
                    }
                } else {
                    None
                };
                (Some(env), obj)
            } else {
                if let Some((obj, obj_len)) = Obj::reduce(&s[pos..]) {
                    if pos + obj_len != s.len() {
                        return Err(ParseError::TrailingCharacters);
                    }
                    (None, Some(obj))
                } else {
                    return Err(ParseError::InvalidObj);
                }
            }
        } else {
            (None, None)
        };

        Ok(Triple {
            arch,
            vendor,
            kernel,
            env,
            obj,
        })
    }

    // This beast fails Clippy pipeline task, so commented so far
    //
    // #[cfg(feature = "std")]
    // pub fn to_string(&self) -> String {
    //     let mut s = String::new();
    //     s.push_str(self.arch.canonicalize());
    //     if let Some(vendor) = &self.vendor {
    //         s.push('-');
    //         s.push_str(vendor.canonicalize());
    //     }
    //     s.push('-');
    //     s.push_str(self.kernel.canonicalize());
    //     if let Some(env) = &self.env {
    //         s.push('-');
    //         s.push_str(env.canonicalize());
    //     }
    //     if let Some(obj) = &self.obj {
    //         s.push('-');
    //         s.push_str(obj.canonicalize());
    //     }
    //     s
    // }
}

impl core::fmt::Display for Triple {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        f.write_str(self.arch.canonicalize())?;
        if let Some(vendor) = &self.vendor {
            f.write_str("-")?;
            f.write_str(vendor.canonicalize())?;
        }
        f.write_str("-")?;
        f.write_str(self.kernel.canonicalize())?;
        if let Some(env) = &self.env {
            f.write_str("-")?;
            f.write_str(env.canonicalize())?;
        }
        if let Some(obj) = &self.obj {
            f.write_str("-")?;
            f.write_str(obj.canonicalize())?;
        }
        Ok(())
    }
}
