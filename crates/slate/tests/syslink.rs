mod support;

use std::path::{Path, PathBuf};

fn syslink_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.syslink")
}

fn fixture_libs(dir: &Path) -> Vec<String> {
    std::fs::read_to_string(dir.join("libs.txt"))
        .map(|contents| {
            contents
                .lines()
                .map(str::trim)
                .filter(|line| !line.is_empty())
                .map(str::to_string)
                .collect()
        })
        .unwrap_or_default()
}

#[test]
fn syslink_fixture_runs_match_c() {
    let root = syslink_fixtures_dir();
    if !root.exists() {
        return;
    }
    for entry in std::fs::read_dir(&root).expect("read syslink fixtures dir") {
        let dir = entry.expect("read entry").path();
        if !dir.is_dir() {
            continue;
        }
        let name = dir.file_name().unwrap().to_string_lossy().into_owned();
        let main_c = dir.join("main.c");
        if !main_c.is_file() {
            eprintln!("no main.c in {}: skipping", dir.display());
            continue;
        }
        let libs = fixture_libs(&dir);
        let link_args: Vec<String> = libs.iter().map(|lib| format!("-l{lib}")).collect();

        let work = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/syslink")
            .join(&name);
        let _ = std::fs::remove_dir_all(&work);
        std::fs::create_dir_all(&work).expect("create work dir");

        let c_bin = work.join("c_bin");
        support::compile_c_multi_with_std_include_and_args(
            std::slice::from_ref(&main_c),
            &c_bin,
            "c23",
            None,
            &link_args,
        )
        .expect("compile C");

        let rs_out = work.join("generated_main.rs");
        let (_, program) = slate::api::lowered_program(&main_c).expect("lower C to Rust");
        let shim_source = slate::frontend::c_shim::render_shim_c_source_for_program(&program);
        std::fs::write(&rs_out, slate::backend::apply(program).emit())
            .expect("write generated Rust");

        let rs_bin = support::compile_rs_cargo_with_syslibs(
            &rs_out,
            &work,
            &name,
            &libs,
            Some(&shim_source),
        )
        .expect("compile rust with syslibs");

        let cfg = support::RunConfig::default();
        let run_dir = work.join("run");
        let _ = std::fs::remove_dir_all(&run_dir);
        std::fs::create_dir_all(&run_dir).expect("create run dir");

        let c_run = support::run_with_config(&c_bin, &cfg, &run_dir).expect("run C");
        let r_run = support::run_with_config(&rs_bin, &cfg, &run_dir).expect("run Rust");
        support::compare_runs(&c_run, &r_run, false).expect("C and Rust outputs differ");
    }
}
