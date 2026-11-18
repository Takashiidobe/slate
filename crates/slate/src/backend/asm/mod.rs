pub mod aarch64;
pub mod arm;
pub mod directive;
pub mod x86;

use slate_parser::target_info::TargetFamily;
use std::fmt;

pub use directive::Directive;

#[derive(Debug, Clone, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct AsmTemplate {
    lines: Vec<AsmLine>,
}

impl AsmTemplate {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn push(&mut self, line: impl Into<AsmLine>) -> &mut Self {
        self.lines.push(line.into());
        self
    }

    pub fn with(mut self, line: impl Into<AsmLine>) -> Self {
        self.push(line);
        self
    }

    pub fn prepend(&mut self, line: impl Into<AsmLine>) -> &mut Self {
        self.lines.insert(0, line.into());
        self
    }

    pub fn append(&mut self, other: AsmTemplate) -> &mut Self {
        self.lines.extend(other.lines);
        self
    }

    pub fn wrap(
        &mut self,
        before: impl IntoIterator<Item = AsmLine>,
        after: impl IntoIterator<Item = AsmLine>,
    ) -> &mut Self {
        let body = std::mem::take(&mut self.lines);
        self.lines = before.into_iter().chain(body).chain(after).collect();
        self
    }
}

impl FromIterator<AsmLine> for AsmTemplate {
    fn from_iter<I: IntoIterator<Item = AsmLine>>(lines: I) -> Self {
        Self {
            lines: lines.into_iter().collect(),
        }
    }
}

impl fmt::Display for AsmTemplate {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for (index, line) in self.lines.iter().enumerate() {
            if index > 0 {
                f.write_str("\n")?;
            }
            write!(f, "{line}")?;
        }
        Ok(())
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AsmLine {
    Passthrough(Passthrough),
    X86 {
        syntax: x86::Syntax,
        inst: x86::Inst,
    },
    AArch64(aarch64::Inst),
    Arm(arm::Inst),
    Directive(Directive),
    Mention {
        arch: Arch,
        operand: Operand,
    },
}

impl fmt::Display for AsmLine {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Passthrough(passthrough) => write!(f, "{passthrough}"),
            Self::X86 { syntax, inst } => inst.print(*syntax, f),
            Self::AArch64(inst) => write!(f, "{inst}"),
            Self::Arm(inst) => write!(f, "{inst}"),
            Self::Directive(directive) => write!(f, "{directive}"),
            Self::Mention { arch, operand } => write!(f, "{} {operand}", arch.comment_marker()),
        }
    }
}

impl From<Passthrough> for AsmLine {
    fn from(passthrough: Passthrough) -> Self {
        Self::Passthrough(passthrough)
    }
}

impl From<Directive> for AsmLine {
    fn from(directive: Directive) -> Self {
        Self::Directive(directive)
    }
}

impl From<aarch64::Inst> for AsmLine {
    fn from(inst: aarch64::Inst) -> Self {
        Self::AArch64(inst)
    }
}

impl From<arm::Inst> for AsmLine {
    fn from(inst: arm::Inst) -> Self {
        Self::Arm(inst)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Arch {
    X86,
    AArch64,
    Arm,
}

impl Arch {
    pub fn of(family: TargetFamily) -> Self {
        match family {
            TargetFamily::X86 | TargetFamily::X86_64 => Self::X86,
            TargetFamily::AArch64 => Self::AArch64,
            TargetFamily::Arm32 => Self::Arm,
        }
    }

    fn comment_marker(self) -> &'static str {
        match self {
            Self::X86 => "#",
            Self::AArch64 => "//",
            Self::Arm => "@",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Operand {
    pub slot: usize,
    pub modifier: Option<Modifier>,
}

impl Operand {
    pub fn new(slot: usize) -> Self {
        Self {
            slot,
            modifier: None,
        }
    }

    pub fn x86(slot: usize, modifier: x86::Modifier) -> Self {
        Self::with_modifier(slot, Modifier::X86(modifier))
    }

    pub fn aarch64(slot: usize, modifier: aarch64::Modifier) -> Self {
        Self::with_modifier(slot, Modifier::AArch64(modifier))
    }

    pub fn arm(slot: usize, modifier: arm::Modifier) -> Self {
        Self::with_modifier(slot, Modifier::Arm(modifier))
    }

    fn with_modifier(slot: usize, modifier: Modifier) -> Self {
        Self {
            slot,
            modifier: Some(modifier),
        }
    }
}

impl fmt::Display for Operand {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.modifier {
            Some(modifier) => write!(f, "{{{}:{}}}", self.slot, modifier.letter()),
            None => write!(f, "{{{}}}", self.slot),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Modifier {
    X86(x86::Modifier),
    AArch64(aarch64::Modifier),
    Arm(arm::Modifier),
}

impl Modifier {
    fn letter(self) -> char {
        match self {
            Self::X86(modifier) => modifier.letter(),
            Self::AArch64(modifier) => modifier.letter(),
            Self::Arm(modifier) => modifier.letter(),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Condition(pub &'static str);

impl fmt::Display for Condition {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(self.0)
    }
}

#[derive(Debug, Clone, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Passthrough {
    fragments: Vec<Fragment>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Fragment {
    Text(String),
    Operand(Operand),
}

impl Passthrough {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn text(&mut self, text: &str) -> &mut Self {
        if text.is_empty() {
            return self;
        }
        match self.fragments.last_mut() {
            Some(Fragment::Text(last)) => last.push_str(text),
            _ => self.fragments.push(Fragment::Text(text.to_owned())),
        }
        self
    }

    pub fn char(&mut self, c: char) -> &mut Self {
        self.text(c.encode_utf8(&mut [0; 4]))
    }

    pub fn operand(&mut self, operand: Operand) -> &mut Self {
        self.fragments.push(Fragment::Operand(operand));
        self
    }

    pub fn fragment(&mut self, fragment: Fragment) -> &mut Self {
        match fragment {
            Fragment::Text(text) => self.text(&text),
            Fragment::Operand(operand) => self.operand(operand),
        }
    }

    pub fn append(&mut self, other: Passthrough) -> &mut Self {
        for fragment in other.fragments {
            self.fragment(fragment);
        }
        self
    }
}

impl fmt::Display for Passthrough {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for fragment in &self.fragments {
            match fragment {
                Fragment::Text(text) => write!(f, "{}", Escaped(text))?,
                Fragment::Operand(operand) => write!(f, "{operand}")?,
            }
        }
        Ok(())
    }
}

pub(crate) struct Escaped<'t>(pub &'t str);

impl fmt::Display for Escaped<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for piece in self.0.split_inclusive(['{', '}']) {
            match piece.chars().last() {
                Some(brace @ ('{' | '}')) => {
                    f.write_str(piece)?;
                    write!(f, "{brace}")?;
                }
                _ => f.write_str(piece)?,
            }
        }
        Ok(())
    }
}
