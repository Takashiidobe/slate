use std::sync::OnceLock;
use std::time::Instant;

fn enabled() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("SLATE_INTERPROC_TIMING").is_some())
}

pub(in crate::backend) struct PhaseTimer {
    pass: &'static str,
    last: Option<Instant>,
}

impl PhaseTimer {
    pub(in crate::backend) fn new(pass: &'static str) -> Self {
        Self {
            pass,
            last: enabled().then(Instant::now),
        }
    }

    pub(in crate::backend) fn lap(&mut self, phase: &str) {
        if let Some(last) = &mut self.last {
            eprintln!(
                "interproc-timing pass={} phase={phase} ms={:.3}",
                self.pass,
                last.elapsed().as_secs_f64() * 1e3
            );
            *last = Instant::now();
        }
    }

    pub(in crate::backend) fn count(&self, name: &str, value: usize) {
        if self.last.is_some() {
            eprintln!(
                "interproc-count pass={} name={name} value={value}",
                self.pass
            );
        }
    }
}
