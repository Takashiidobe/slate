mod c_side;
mod decide;
mod providers;
mod rust_side;
mod symbols;

use anyhow::Result;
use clap::Parser;
use decide::TargetData;
use rayon::prelude::*;
use std::collections::BTreeMap;
use std::path::PathBuf;

const DEFAULT_TARGETS: &[&str] = &[
    "x86_64-unknown-linux-gnu",
    "i686-unknown-linux-gnu",
    "aarch64-unknown-linux-gnu",
    "armv7-unknown-linux-gnueabihf",
    "x86_64-unknown-linux-musl",
    "aarch64-unknown-linux-musl",
    "x86_64-apple-darwin",
    "aarch64-apple-darwin",
    "x86_64-unknown-freebsd",
    "aarch64-unknown-freebsd",
    "x86_64-linux-android",
    "aarch64-linux-android",
    "x86_64-pc-windows-msvc",
    "i686-pc-windows-msvc",
    "aarch64-pc-windows-msvc",
];

#[derive(Parser)]
struct Args {
    #[arg(long, default_value = "target/test-cache/release/slate-parser")]
    parser: PathBuf,
    #[arg(long, default_value = "crates/slate/vendor/libc")]
    libc: PathBuf,
    #[arg(long, default_value = "crates/slate/src/backend/clib_records_table.rs")]
    out: PathBuf,
    #[arg(long, default_value = "target/libc-gen-report.md")]
    report: PathBuf,
    #[arg(long, default_value = "crates/slate/src/libc_symbols_table.rs")]
    symbols_out: PathBuf,
    #[arg(long)]
    work: Option<PathBuf>,
    #[arg(long = "target")]
    targets: Vec<String>,
}

fn main() -> Result<()> {
    let args = Args::parse();
    let targets: Vec<String> = if args.targets.is_empty() {
        DEFAULT_TARGETS.iter().map(|t| t.to_string()).collect()
    } else {
        args.targets.clone()
    };
    let temp;
    let work = match &args.work {
        Some(dir) => {
            std::fs::create_dir_all(dir)?;
            dir.clone()
        }
        None => {
            temp = tempfile::tempdir()?;
            temp.path().to_path_buf()
        }
    };
    let work = std::fs::canonicalize(work)?;
    let libc_dir = rust_side::prepare_libc(&args.libc, &work)?;

    let c_maps: Vec<(String, c_side::CMap, providers::Inventory)> = targets
        .par_iter()
        .map(|target| {
            let scratch = &work.join(format!("c-{target}"));
            std::fs::create_dir_all(scratch)?;
            eprintln!("c side: {target}");
            let providers = providers::Inventory::collect(target, &work)?;
            let c = c_side::collect(&args.parser, target, scratch, &providers)?;
            Ok((target.clone(), c, providers))
        })
        .collect::<Result<_>>()?;

    let data: BTreeMap<String, TargetData> = c_maps
        .into_par_iter()
        .map(|(target, c, providers)| {
            let candidates: Vec<(String, Vec<String>)> = c
                .iter()
                .map(|(name, ty)| {
                    let fields = ty
                        .fields
                        .iter()
                        .flatten()
                        .filter(|field| field.name != "<anonymous>")
                        .map(|field| field.name.clone())
                        .collect();
                    (name.clone(), fields)
                })
                .collect();
            eprintln!("rust side: {target} ({} candidates)", candidates.len());
            let rust = rust_side::collect(&libc_dir, &work, &target, &candidates, &providers)?;
            Ok((target, TargetData { c, rust, providers }))
        })
        .collect::<Result<_>>()?;

    let entries = decide::decide(&data);
    let (table, conflicts) = decide::table(&entries);
    if let Some(parent) = args.report.parent() {
        std::fs::create_dir_all(parent)?;
    }
    let symbol_data: BTreeMap<String, symbols::TargetSymbols> = targets
        .par_iter()
        .map(|target| {
            eprintln!("symbols: {target}");
            let work = work.clone();
            let libc_dir = libc_dir.clone();
            let entries = &entries;
            let target = target.clone();
            let result = std::thread::scope(|scope| {
                std::thread::Builder::new()
                    .stack_size(64 << 20)
                    .spawn_scoped(scope, || {
                        symbols::collect(
                            &target,
                            &work,
                            &libc_dir,
                            entries,
                            &data[&target].providers,
                        )
                    })
                    .expect("symbol worker")
                    .join()
                    .expect("symbol worker panicked")
            })?;
            Ok((target, result))
        })
        .collect::<Result<_>>()?;
    std::fs::write(&args.out, table)?;
    std::fs::write(&args.symbols_out, symbols::table(&symbol_data))?;
    let mut report = decide::report(&entries, &data, &conflicts);
    report.push_str(&symbols::report(&symbol_data));
    std::fs::write(&args.report, report)?;
    eprintln!("wrote {} and {}", args.out.display(), args.report.display());
    Ok(())
}
