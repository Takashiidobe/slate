use std::collections::HashSet;
use std::fs;
use std::io::{BufRead, BufReader, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};
use std::time::{Duration, Instant};

const GLIBC_MINOR_VERSION: u32 = 43;

fn default_clang_args() -> Vec<String> {
    vec![
        "-std=gnu23".into(),
        format!("-D__SLATE_GLIBC_MINOR__={GLIBC_MINOR_VERSION}"),
    ]
}

#[derive(Debug, Clone)]
struct FuzzConfig {
    yarpgen_bin: PathBuf,
    clang_bin: PathBuf,
    slate_bin: Option<PathBuf>,
    max_array_dims: usize,
    timeout: Duration,
    work_dir: PathBuf,
    failures_dir: PathBuf,
    seeds_file: Option<PathBuf>,
    export_fixture: Option<PathBuf>,
    clang_args: Vec<String>,
    single_seed: Option<u64>,
    start_seed: u64,
    count: Option<usize>,
    continuous: bool,
    jobs: usize,
    stop_on_failure: bool,
}

#[derive(Debug)]
struct RunOutput {
    exit_code: Option<i32>,
    stdout: Vec<u8>,
    stderr: Vec<u8>,
    timed_out: bool,
}

#[derive(Debug)]
enum CaseFailure {
    YarpgenGen(String),
    CCompile(String),
    CRuntime(String),
    SlateTranslate(String),
    RustCargo(String),
    RustRuntime(String),
    OutputMismatch {
        c_stdout: String,
        c_exit: Option<i32>,
        rs_stdout: String,
        rs_exit: Option<i32>,
    },
}

impl std::fmt::Display for CaseFailure {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            CaseFailure::YarpgenGen(e) => write!(f, "YARPGen generation failed: {e}"),
            CaseFailure::CCompile(e) => write!(f, "C compile failed: {e}"),
            CaseFailure::CRuntime(e) => write!(f, "C execution failed/timed out: {e}"),
            CaseFailure::SlateTranslate(e) => write!(f, "Slate translation failed: {e}"),
            CaseFailure::RustCargo(e) => write!(f, "Rust cargo build failed: {e}"),
            CaseFailure::RustRuntime(e) => write!(f, "Rust execution failed/timed out: {e}"),
            CaseFailure::OutputMismatch {
                c_stdout,
                c_exit,
                rs_stdout,
                rs_exit,
            } => {
                write!(
                    f,
                    "Differential mismatch:\n  C exit: {c_exit:?}, stdout: {}\n  Rust exit: {rs_exit:?}, stdout: {}",
                    c_stdout.trim(),
                    rs_stdout.trim()
                )
            }
        }
    }
}

struct SeedTracker {
    seen: Mutex<HashSet<u64>>,
    file: Option<Mutex<fs::File>>,
}

impl SeedTracker {
    fn new(seeds_file: Option<&Path>) -> (Self, usize) {
        let mut seen = HashSet::new();
        let file = if let Some(path) = seeds_file {
            if let Some(parent) = path.parent() {
                let _ = fs::create_dir_all(parent);
            }
            if path.is_file()
                && let Ok(f) = fs::File::open(path)
            {
                for line in BufReader::new(f).lines().map_while(Result::ok) {
                    let trimmed = line.trim();
                    if let Ok(seed) = trimmed.parse::<u64>() {
                        seen.insert(seed);
                    }
                }
            }
            match fs::OpenOptions::new().create(true).append(true).open(path) {
                Ok(f) => Some(Mutex::new(f)),
                Err(e) => {
                    eprintln!("warning: could not open seeds file {}: {e}", path.display());
                    None
                }
            }
        } else {
            None
        };
        let initial_count = seen.len();
        (
            Self {
                seen: Mutex::new(seen),
                file,
            },
            initial_count,
        )
    }

    fn contains(&self, seed: u64) -> bool {
        self.seen.lock().unwrap().contains(&seed)
    }

    fn record_pass(&self, seed: u64) {
        let mut seen = self.seen.lock().unwrap();
        if seen.insert(seed)
            && let Some(ref file_mutex) = self.file
            && let Ok(mut file) = file_mutex.lock()
        {
            let _ = writeln!(file, "{seed}");
            let _ = file.flush();
        }
    }

    fn count(&self) -> usize {
        self.seen.lock().unwrap().len()
    }
}

fn find_default_yarpgen() -> PathBuf {
    if let Ok(env_path) = std::env::var("YARPGEN_BIN") {
        return PathBuf::from(env_path);
    }
    if let Ok(home) = std::env::var("HOME") {
        let candidate = PathBuf::from(home).join("yarpgen/build/yarpgen");
        if candidate.is_file() {
            return candidate;
        }
    }
    PathBuf::from("yarpgen")
}

fn find_default_clang() -> PathBuf {
    if let Ok(env_path) = std::env::var("SLATE_CLANG") {
        return PathBuf::from(env_path);
    }
    if let Ok(home) = std::env::var("HOME") {
        let candidate = PathBuf::from(home).join("llvm-project/build-cir/bin/clang");
        if candidate.is_file() {
            return candidate;
        }
    }
    PathBuf::from("clang")
}

fn find_slate_bin() -> Result<PathBuf, String> {
    if let Ok(env_path) = std::env::var("SLATE_BIN") {
        let p = PathBuf::from(env_path);
        if p.is_file() {
            return Ok(p);
        }
    }

    if let Ok(current_exe) = std::env::current_exe()
        && let Some(parent) = current_exe.parent()
    {
        let sibling = parent.join("slate");
        if sibling.is_file() {
            return Ok(sibling);
        }
    }

    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    let candidates = [
        manifest_dir.join("target/release/slate"),
        manifest_dir.join("target/debug/slate"),
        manifest_dir.join("target/test-cache/debug/slate"),
    ];
    for c in &candidates {
        if c.is_file() {
            return Ok(c.clone());
        }
    }

    // Attempt to build slate binary
    eprintln!("Slate binary not found; building slate...");
    let status = Command::new("cargo")
        .args(["build", "--bin", "slate"])
        .current_dir(&manifest_dir)
        .status()
        .map_err(|e| format!("failed to build slate: {e}"))?;
    if !status.success() {
        return Err("cargo build --bin slate failed".into());
    }

    for c in &candidates {
        if c.is_file() {
            return Ok(c.clone());
        }
    }

    Err("Could not locate slate binary. Set SLATE_BIN or build slate first.".into())
}

fn print_usage() {
    eprintln!(
        r#"yarpgen-fuzz: differential fuzzer for YARPGen C -> Slate Rust

USAGE:
    yarpgen_fuzz [OPTIONS]

MODES:
    -s, --seed <SEED>           Run a single specific seed
    -c, --continuous            Run continuously with random seeds until stopped
    -n, --count <N>             Run N seeds sequentially starting from --start-seed
    --start-seed <SEED>         Starting seed for --count (default: 1)

OPTIONS:
    --yarpgen <PATH>            Path to yarpgen binary (default: $YARPGEN_BIN or ~/yarpgen/build/yarpgen)
    --clang <PATH>              Path to Clang binary (default: $SLATE_CLANG or ~/llvm-project/build-cir/bin/clang)
    --slate <PATH>              Path to Slate binary (default: auto-detected)
    --max-array-dims <N>        Upper bound on array dimensions (default: 3)
    --timeout <SECS>            Execution timeout per binary in seconds (default: 10)
    --work-dir <PATH>           Working directory (default: target/yarpgen-fuzz/work)
    --failures-dir <PATH>       Directory to save failing test cases (default: target/yarpgen-fuzz/failures)
    --seeds-file <PATH>         Path to file recording explored seeds (default: target/yarpgen-fuzz/explored_seeds.txt)
    --no-history                Disable loading and saving explored seeds history
    --export-fixture <PATH>     Save the generated C program to PATH (for creating regression fixtures)
    -j, --jobs <N>              Number of concurrent workers for continuous/batch (default: 1)
    --no-stop                   Do not stop immediately on failure (keep going)
    -h, --help                  Print this help message
"#
    );
}

fn parse_args() -> Result<FuzzConfig, String> {
    let args = std::env::var("YARPGEN_FUZZ_ARGS")
        .map_err(|error| format!("read YARPGEN_FUZZ_ARGS: {error}"))
        .and_then(|args| {
            shlex::split(&args).ok_or_else(|| "invalid YARPGEN_FUZZ_ARGS quoting".into())
        })
        .unwrap_or_default();
    let mut yarpgen_bin = find_default_yarpgen();
    let mut clang_bin = find_default_clang();
    let mut slate_bin = None;
    let mut max_array_dims = 3;
    let mut timeout = Duration::from_secs(10);
    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    let mut work_dir = manifest_dir.join("target/yarpgen-fuzz/work");
    let mut failures_dir = manifest_dir.join("target/yarpgen-fuzz/failures");
    let mut seeds_file = Some(manifest_dir.join("tests/yarpgen-seeds.txt"));
    let mut export_fixture = None;
    let mut single_seed = None;
    let mut start_seed = 1;
    let mut count = None;
    let mut continuous = false;
    let mut jobs = 1;
    let mut stop_on_failure = true;

    let mut i = 0;
    while i < args.len() {
        match args[i].as_str() {
            "-s" | "--seed" => {
                i += 1;
                single_seed = Some(
                    args.get(i)
                        .ok_or_else(|| "--seed requires an integer".to_string())?
                        .parse()
                        .map_err(|e| format!("invalid seed: {e}"))?,
                );
            }
            "-c" | "--continuous" | "--fuzz" => {
                continuous = true;
            }
            "-n" | "--count" => {
                i += 1;
                count = Some(
                    args.get(i)
                        .ok_or_else(|| "--count requires an integer".to_string())?
                        .parse()
                        .map_err(|e| format!("invalid count: {e}"))?,
                );
            }
            "--start-seed" => {
                i += 1;
                start_seed = args
                    .get(i)
                    .ok_or_else(|| "--start-seed requires an integer".to_string())?
                    .parse()
                    .map_err(|e| format!("invalid start-seed: {e}"))?;
            }
            "--yarpgen" => {
                i += 1;
                yarpgen_bin = PathBuf::from(
                    args.get(i)
                        .ok_or_else(|| "--yarpgen requires a path".to_string())?,
                );
            }
            "--clang" => {
                i += 1;
                clang_bin = PathBuf::from(
                    args.get(i)
                        .ok_or_else(|| "--clang requires a path".to_string())?,
                );
            }
            "--slate" => {
                i += 1;
                slate_bin = Some(PathBuf::from(
                    args.get(i)
                        .ok_or_else(|| "--slate requires a path".to_string())?,
                ));
            }
            "--max-array-dims" => {
                i += 1;
                max_array_dims = args
                    .get(i)
                    .ok_or_else(|| "--max-array-dims requires an integer".to_string())?
                    .parse()
                    .map_err(|e| format!("invalid max-array-dims: {e}"))?;
            }
            "--timeout" => {
                i += 1;
                let secs: u64 = args
                    .get(i)
                    .ok_or_else(|| "--timeout requires seconds".to_string())?
                    .parse()
                    .map_err(|e| format!("invalid timeout: {e}"))?;
                timeout = Duration::from_secs(secs);
            }
            "--work-dir" => {
                i += 1;
                work_dir = PathBuf::from(
                    args.get(i)
                        .ok_or_else(|| "--work-dir requires a path".to_string())?,
                );
            }
            "--failures-dir" => {
                i += 1;
                failures_dir = PathBuf::from(
                    args.get(i)
                        .ok_or_else(|| "--failures-dir requires a path".to_string())?,
                );
            }
            "--seeds-file" => {
                i += 1;
                seeds_file = Some(PathBuf::from(
                    args.get(i)
                        .ok_or_else(|| "--seeds-file requires a path".to_string())?,
                ));
            }
            "--no-history" => {
                seeds_file = None;
            }
            "--export-fixture" => {
                i += 1;
                export_fixture =
                    Some(PathBuf::from(args.get(i).ok_or_else(|| {
                        "--export-fixture requires a path".to_string()
                    })?));
            }
            "-j" | "--jobs" => {
                i += 1;
                jobs = args
                    .get(i)
                    .ok_or_else(|| "--jobs requires an integer".to_string())?
                    .parse()
                    .map_err(|e| format!("invalid jobs: {e}"))?;
            }
            "--no-stop" => {
                stop_on_failure = false;
            }
            "-h" | "--help" => {
                print_usage();
                return Err("help requested".into());
            }
            unknown => return Err(format!("unknown argument: {unknown}")),
        }
        i += 1;
    }

    if single_seed.is_none() && count.is_none() && !continuous {
        single_seed = Some(1);
    }

    let slate_bin = match slate_bin {
        Some(b) => b,
        None => find_slate_bin()?,
    };

    Ok(FuzzConfig {
        yarpgen_bin,
        clang_bin,
        slate_bin: Some(slate_bin),
        max_array_dims,
        timeout,
        work_dir,
        failures_dir,
        seeds_file,
        export_fixture,
        clang_args: default_clang_args(),
        single_seed,
        start_seed,
        count,
        continuous,
        jobs: jobs.max(1),
        stop_on_failure,
    })
}

fn run_command_with_timeout(
    mut cmd: Command,
    timeout: Duration,
) -> Result<RunOutput, std::io::Error> {
    let start = Instant::now();
    cmd.stdout(Stdio::piped()).stderr(Stdio::piped());
    let child = cmd.spawn()?;

    let (tx, rx) = std::sync::mpsc::channel();
    let child_id = child.id();

    let timer_thread = std::thread::spawn(move || {
        if rx.recv_timeout(timeout).is_err() {
            unsafe {
                libc::kill(child_id as i32, libc::SIGKILL);
            }
        }
    });

    let output = child.wait_with_output()?;
    let _ = tx.send(());
    let _ = timer_thread.join();

    let duration = start.elapsed();
    let timed_out = duration >= timeout || matches!(output.status.code(), Some(124 | 137));

    Ok(RunOutput {
        exit_code: output.status.code(),
        stdout: output.stdout,
        stderr: output.stderr,
        timed_out,
    })
}

fn run_single_case(
    seed: u64,
    worker_id: usize,
    config: &FuzzConfig,
) -> Result<(String, Duration), CaseFailure> {
    let total_start = Instant::now();
    let case_dir = config
        .work_dir
        .join(format!("worker_{worker_id}_seed_{seed}"));
    let c_dir = case_dir.join("c");
    let crate_dir = case_dir.join("rs_crate");
    let _ = fs::remove_dir_all(&case_dir);
    fs::create_dir_all(&c_dir)
        .map_err(|e| CaseFailure::YarpgenGen(format!("create c_dir: {e}")))?;

    // 1. Generate C program using YARPGen
    let mut yarpgen_cmd = Command::new(&config.yarpgen_bin);
    yarpgen_cmd
        .arg("--std=c")
        .arg(format!("--seed={seed}"))
        .arg(format!("--max-array-dims={}", config.max_array_dims))
        .arg(format!("--out-dir={}", c_dir.display()));

    let yarpgen_run = run_command_with_timeout(yarpgen_cmd, config.timeout)
        .map_err(|e| CaseFailure::YarpgenGen(format!("spawn yarpgen: {e}")))?;
    if yarpgen_run.exit_code != Some(0) || yarpgen_run.timed_out {
        return Err(CaseFailure::YarpgenGen(
            String::from_utf8_lossy(&yarpgen_run.stderr).into_owned(),
        ));
    }

    if !c_dir.join("driver.c").is_file() || !c_dir.join("func.c").is_file() {
        return Err(CaseFailure::YarpgenGen(
            "YARPGen did not emit driver.c and func.c".into(),
        ));
    }

    // 2. Compile C program with Clang
    let c_bin = case_dir.join("c_bin");
    let mut clang_cmd = Command::new(&config.clang_bin);
    clang_cmd
        .arg("-O0")
        .args(&config.clang_args)
        .arg(c_dir.join("driver.c"))
        .arg(c_dir.join("func.c"))
        .arg("-o")
        .arg(&c_bin)
        .arg("-lm");

    let clang_run = run_command_with_timeout(clang_cmd, config.timeout)
        .map_err(|e| CaseFailure::CCompile(format!("spawn clang: {e}")))?;
    if clang_run.exit_code != Some(0) || clang_run.timed_out {
        return Err(CaseFailure::CCompile(
            String::from_utf8_lossy(&clang_run.stderr).into_owned(),
        ));
    }

    // 3. Run C binary
    let mut c_run_cmd = Command::new(&c_bin);
    c_run_cmd.current_dir(&case_dir);
    let c_run = run_command_with_timeout(c_run_cmd, config.timeout)
        .map_err(|e| CaseFailure::CRuntime(format!("spawn C binary: {e}")))?;
    if c_run.timed_out {
        return Err(CaseFailure::CRuntime(format!(
            "C binary timed out after {:?}",
            config.timeout
        )));
    }
    if c_run.exit_code != Some(0) {
        return Err(CaseFailure::CRuntime(format!(
            "C binary exited with {:?}\nstderr: {}",
            c_run.exit_code,
            String::from_utf8_lossy(&c_run.stderr)
        )));
    }
    let c_hash = String::from_utf8_lossy(&c_run.stdout).trim().to_string();

    // 4. Translate with Slate translate-project
    let slate_bin = config.slate_bin.as_ref().unwrap();
    let mut slate_cmd = Command::new(slate_bin);
    slate_cmd
        .arg("translate-project")
        .arg(&c_dir)
        .arg(&crate_dir)
        .env("SLATE_CLANG", &config.clang_bin)
        .env("SLATE_CLANG_ARGS", config.clang_args.join(" "));

    let translate_timeout = config.timeout.max(Duration::from_secs(30)) * 2;
    let slate_run = run_command_with_timeout(slate_cmd, translate_timeout)
        .map_err(|e| CaseFailure::SlateTranslate(format!("spawn slate: {e}")))?;
    if slate_run.timed_out {
        return Err(CaseFailure::SlateTranslate(format!(
            "timed out after {translate_timeout:?}"
        )));
    }
    if slate_run.exit_code != Some(0) {
        return Err(CaseFailure::SlateTranslate(
            String::from_utf8_lossy(&slate_run.stderr).into_owned(),
        ));
    }

    // 5. Build and Run Rust crate with Cargo
    let shared_target_dir = config.work_dir.join("shared_target");
    let mut cargo_cmd = Command::new("cargo");
    cargo_cmd
        .args(["run", "--quiet", "--manifest-path"])
        .arg(crate_dir.join("Cargo.toml"))
        .arg("--target-dir")
        .arg(&shared_target_dir);

    let rust_run = run_command_with_timeout(cargo_cmd, config.timeout * 3)
        .map_err(|e| CaseFailure::RustCargo(format!("spawn cargo run: {e}")))?;
    if rust_run.timed_out {
        return Err(CaseFailure::RustRuntime(format!(
            "Rust binary timed out after {:?}",
            config.timeout
        )));
    }
    if rust_run.exit_code != Some(0) {
        let stderr = String::from_utf8_lossy(&rust_run.stderr);
        if stderr.contains("error[E") || stderr.contains("could not compile") {
            return Err(CaseFailure::RustCargo(stderr.into_owned()));
        } else {
            return Err(CaseFailure::RustRuntime(format!(
                "Rust exited with {:?}\nstderr: {stderr}",
                rust_run.exit_code
            )));
        }
    }
    let rust_hash = String::from_utf8_lossy(&rust_run.stdout).trim().to_string();

    // 6. Compare output
    if c_hash != rust_hash || c_run.exit_code != rust_run.exit_code {
        return Err(CaseFailure::OutputMismatch {
            c_stdout: c_hash,
            c_exit: c_run.exit_code,
            rs_stdout: rust_hash,
            rs_exit: rust_run.exit_code,
        });
    }

    // Export fixture if requested for this seed
    if let Some(export_path) = &config.export_fixture {
        let _ = fs::create_dir_all(export_path);
        for file in ["driver.c", "func.c", "init.h"] {
            if c_dir.join(file).is_file() {
                let _ = fs::copy(c_dir.join(file), export_path.join(file));
            }
        }
    }

    // Clean up temporary run dir on success to save disk space
    let _ = fs::remove_dir_all(&case_dir);

    Ok((c_hash, total_start.elapsed()))
}

fn save_failure_bundle(
    seed: u64,
    worker_id: usize,
    failure: &CaseFailure,
    config: &FuzzConfig,
) -> Result<PathBuf, String> {
    let failure_dir = config.failures_dir.join(format!("seed_{seed}"));
    let _ = fs::remove_dir_all(&failure_dir);
    fs::create_dir_all(&failure_dir)
        .map_err(|e| format!("create failure dir {}: {e}", failure_dir.display()))?;

    let work_case_dir = config
        .work_dir
        .join(format!("worker_{worker_id}_seed_{seed}"));
    let work_c_dir = work_case_dir.join("c");
    let work_crate_dir = work_case_dir.join("rs_crate");

    // Copy C sources
    if work_c_dir.is_dir() {
        for file in ["driver.c", "func.c", "init.h"] {
            let src = work_c_dir.join(file);
            if src.is_file() {
                let _ = fs::copy(&src, failure_dir.join(file));
            }
        }
    }

    // Copy generated Rust crate if it exists
    if work_crate_dir.is_dir() {
        let dest_crate = failure_dir.join("crate");
        copy_dir_recursive(&work_crate_dir, &dest_crate)
            .map_err(|e| format!("copy crate to failure dir: {e}"))?;
    }

    // Write reproduction shell script
    let repro_sh = format!(
        r#"#!/usr/bin/env bash
set -euo pipefail
DIR="$(cd "$(dirname "${{BASH_SOURCE[0]}}")" && pwd)"
ROOT="{}"
CLANG="${{SLATE_CLANG:-{}}}"
SLATE="${{SLATE_BIN:-{}}}"

echo "=== 1. Compiling and running C oracle ==="
"$CLANG" -O0 -std=c23 "$DIR/driver.c" "$DIR/func.c" -o "$DIR/c_oracle" -lm
"$DIR/c_oracle" > "$DIR/c_output.actual" 2>&1 || true
echo "C output: $(cat "$DIR/c_output.actual")"

echo "=== 2. Translating with Slate ==="
"$SLATE" translate-project "$DIR" "$DIR/repro_crate"

echo "=== 3. Compiling and running Rust crate ==="
cargo run --manifest-path "$DIR/repro_crate/Cargo.toml" > "$DIR/rust_output.actual" 2>&1 || true
echo "Rust output: $(cat "$DIR/rust_output.actual")"

echo "=== 4. Diffing output ==="
if diff -u "$DIR/c_output.actual" "$DIR/rust_output.actual"; then
    echo "SUCCESS: Output matches!"
else
    echo "FAILURE: Output differs!"
fi
"#,
        PathBuf::from(env!("CARGO_MANIFEST_DIR")).display(),
        config.clang_bin.display(),
        config.slate_bin.as_ref().unwrap().display(),
    );

    fs::write(failure_dir.join("reproduce.sh"), repro_sh.as_bytes())
        .map_err(|e| format!("write reproduce.sh: {e}"))?;

    #[cfg(unix)]
    {
        use std::os::unix::fs::PermissionsExt;
        let _ = fs::set_permissions(
            failure_dir.join("reproduce.sh"),
            fs::Permissions::from_mode(0o755),
        );
    }

    // Write diagnostic failure report
    let mut report = format!("YARPGen Fuzz Failure Report\nSeed: {seed}\nError: {failure}\n\n");
    if let CaseFailure::OutputMismatch {
        c_stdout,
        c_exit,
        rs_stdout,
        rs_exit,
    } = failure
    {
        report.push_str(&format!(
            "--- C Execution ---\nExit Code: {c_exit:?}\nOutput:\n{c_stdout}\n\n--- Rust Execution ---\nExit Code: {rs_exit:?}\nOutput:\n{rs_stdout}\n"
        ));
    }
    fs::write(failure_dir.join("report.txt"), report.as_bytes())
        .map_err(|e| format!("write report.txt: {e}"))?;

    Ok(failure_dir)
}

fn copy_dir_recursive(src: &Path, dst: &Path) -> std::io::Result<()> {
    fs::create_dir_all(dst)?;
    for entry in fs::read_dir(src)? {
        let entry = entry?;
        let ty = entry.file_type()?;
        let dest_path = dst.join(entry.file_name());
        if ty.is_dir() {
            if entry.file_name() != "target" {
                copy_dir_recursive(&entry.path(), &dest_path)?;
            }
        } else {
            fs::copy(entry.path(), dest_path)?;
        }
    }
    Ok(())
}

fn run_fuzzer() -> Result<(), String> {
    let config = parse_args().map_err(|e| format!("{e}\n"))?;

    let (tracker, initial_seeds_count) = SeedTracker::new(config.seeds_file.as_deref());
    let tracker = Arc::new(tracker);

    println!("=== Slate YARPGen Fuzzer ===");
    println!("YARPGen: {}", config.yarpgen_bin.display());
    println!("Clang:   {}", config.clang_bin.display());
    println!("Slate:   {}", config.slate_bin.as_ref().unwrap().display());
    println!("Max Dim: {}", config.max_array_dims);
    println!("WorkDir: {}", config.work_dir.display());
    println!("FailDir: {}", config.failures_dir.display());
    if let Some(ref path) = config.seeds_file {
        println!(
            "SeedsLog:{} ({} previously explored seeds loaded)",
            path.display(),
            initial_seeds_count
        );
    } else {
        println!("SeedsLog: disabled");
    }
    println!("============================");

    let _ = fs::create_dir_all(&config.work_dir);
    let _ = fs::create_dir_all(&config.failures_dir);

    // Single seed mode
    if let Some(seed) = config.single_seed {
        if tracker.contains(seed) {
            println!("Note: seed {seed} was previously explored and passed.");
        }
        println!("Testing single seed: {seed} ...");
        match run_single_case(seed, 0, &config) {
            Ok((hash, duration)) => {
                tracker.record_pass(seed);
                println!(
                    "PASS: seed {seed} matches (hash: {hash}, elapsed: {:.2}s)",
                    duration.as_secs_f64()
                );
                if let Some(export_path) = &config.export_fixture {
                    println!("Exported fixture to {}", export_path.display());
                }
            }
            Err(failure) => {
                eprintln!("\nFAIL: seed {seed}\n{failure}\n");
                match save_failure_bundle(seed, 0, &failure, &config) {
                    Ok(path) => eprintln!("Saved reproduction bundle to: {}", path.display()),
                    Err(e) => eprintln!("Failed to save reproduction bundle: {e}"),
                }
                return Err(format!("seed {seed} failed: {failure}"));
            }
        }
        return Ok(());
    }

    // Batch or continuous mode
    let stop_signal = Arc::new(AtomicBool::new(false));

    let total_tested = Arc::new(AtomicUsize::new(0));
    let total_passed = Arc::new(AtomicUsize::new(0));
    let total_failed = Arc::new(AtomicUsize::new(0));
    let total_skipped = Arc::new(AtomicUsize::new(0));

    let seed_counter = Arc::new(std::sync::atomic::AtomicU64::new(config.start_seed));
    let max_count = config.count;

    let mut handles = Vec::new();
    for worker_id in 0..config.jobs {
        let config = config.clone();
        let tracker = tracker.clone();
        let stop_signal = stop_signal.clone();
        let total_tested = total_tested.clone();
        let total_passed = total_passed.clone();
        let total_failed = total_failed.clone();
        let total_skipped = total_skipped.clone();
        let seed_counter = seed_counter.clone();

        let handle = std::thread::spawn(move || {
            loop {
                if stop_signal.load(Ordering::Relaxed) {
                    break;
                }

                let seed = if config.continuous {
                    // Pick a random seed that has not been explored yet
                    let mut attempts = 0;
                    loop {
                        if stop_signal.load(Ordering::Relaxed) {
                            return;
                        }
                        let random_bytes: [u8; 8] = rand_seed();
                        let s = u64::from_le_bytes(random_bytes);
                        let s = if s == 0 { 1 } else { s };
                        if !tracker.contains(s) {
                            break s;
                        }
                        attempts += 1;
                        if attempts > 100_000 {
                            break s;
                        }
                    }
                } else {
                    let s = seed_counter.fetch_add(1, Ordering::Relaxed);
                    if let Some(count) = max_count
                        && s >= config.start_seed + count as u64
                    {
                        break;
                    }
                    if tracker.contains(s) {
                        total_skipped.fetch_add(1, Ordering::Relaxed);
                        continue;
                    }
                    s
                };

                let iter = total_tested.fetch_add(1, Ordering::Relaxed) + 1;
                match run_single_case(seed, worker_id, &config) {
                    Ok((hash, duration)) => {
                        tracker.record_pass(seed);
                        total_passed.fetch_add(1, Ordering::Relaxed);
                        println!(
                            "[#{iter}] Seed {seed}: PASS (hash: {hash}, {:.2}s)",
                            duration.as_secs_f64()
                        );
                    }
                    Err(failure) => {
                        total_failed.fetch_add(1, Ordering::Relaxed);
                        eprintln!("\n[#{iter}] Seed {seed}: FAIL\n{failure}\n");
                        match save_failure_bundle(seed, worker_id, &failure, &config) {
                            Ok(path) => {
                                eprintln!("Saved reproduction bundle to: {}\n", path.display())
                            }
                            Err(e) => eprintln!("Failed to save reproduction bundle: {e}\n"),
                        }

                        if config.stop_on_failure {
                            stop_signal.store(true, Ordering::SeqCst);
                            break;
                        }
                    }
                }
            }
        });
        handles.push(handle);
    }

    for handle in handles {
        let _ = handle.join();
    }

    let tested = total_tested.load(Ordering::SeqCst);
    let passed = total_passed.load(Ordering::SeqCst);
    let failed = total_failed.load(Ordering::SeqCst);
    let skipped = total_skipped.load(Ordering::SeqCst);

    println!("\n=== Summary ===");
    println!("Total Tested:  {tested}");
    println!("Passed:        {passed}");
    println!("Failed:        {failed}");
    if skipped > 0 {
        println!("Skipped (Seen):{skipped}");
    }
    println!("Total Unique:  {}", tracker.count());
    println!("===============");

    if failed > 0 {
        return Err(format!("{failed} fuzz cases failed"));
    }

    Ok(())
}

#[test]
#[ignore = "requires an external YARPGen installation"]
fn yarpgen_fuzz() {
    run_fuzzer().unwrap_or_else(|error| panic!("{error}"));
}

fn rand_seed() -> [u8; 8] {
    let mut buf = [0u8; 8];
    if let Ok(mut f) = fs::File::open("/dev/urandom") {
        use std::io::Read;
        let _ = f.read_exact(&mut buf);
    } else {
        let nanos = Instant::now().elapsed().as_nanos();
        buf = (nanos as u64).to_le_bytes();
    }
    buf
}
