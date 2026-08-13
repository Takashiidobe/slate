mod support;

use std::path::{Path, PathBuf};

fn link_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.link")
}

#[test]
fn link_fixture_runs_match_c() {
    let root = link_fixtures_dir();
    if !root.exists() {
        return;
    }
    for entry in std::fs::read_dir(&root).expect("read link fixtures dir") {
        let dir = entry.expect("read entry").path();
        if !dir.is_dir() {
            continue;
        }
        let name = dir.file_name().unwrap().to_string_lossy().into_owned();
        eprintln!("running link fixture: {name}");

        let main_c = dir.join("main.c");
        if !main_c.is_file() {
            eprintln!("no main.c in {}: skipping", dir.display());
            continue;
        }

        let work = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/link")
            .join(&name);
        let _ = std::fs::remove_dir_all(&work);
        std::fs::create_dir_all(&work).expect("create work dir");

        // Every .c file other than main.c provides native definitions; compile
        // each into an object file that both binaries link against.
        let mut aux_srcs: Vec<PathBuf> = Vec::new();
        for e in std::fs::read_dir(&dir).expect("read fixture dir") {
            let p = e.expect("entry").path();
            if p.file_name().and_then(|f| f.to_str()) == Some("main.c") {
                continue;
            }
            if p.extension().and_then(|s| s.to_str()) == Some("c") {
                aux_srcs.push(p);
            }
        }
        aux_srcs.sort();

        let mut objects: Vec<PathBuf> = Vec::new();
        for src in &aux_srcs {
            let stem = src.file_stem().unwrap().to_string_lossy();
            let obj = work.join(format!("{stem}.o"));
            support::compile_c_object(src, &obj).expect("compile link object");
            objects.push(obj);
        }

        let c_bin = work.join("c_bin");
        let extra_args: Vec<String> = objects.iter().map(|p| p.display().to_string()).collect();
        support::compile_c_with_args(&main_c, &c_bin, &extra_args).expect("compile C");

        let rs_out = work.join("generated_main.rs");
        let (_, program) = slate::api::lowered_program(&main_c).expect("lower C to Rust");
        let shim_source = slate::frontend::c_shim::render_shim_c_source_for_program(&program);
        std::fs::write(&rs_out, slate::backend::apply(program).emit())
            .expect("write generated Rust");

        let link_dir = work.join("linksrc");
        std::fs::create_dir_all(&link_dir).expect("create link dir");
        for obj in &objects {
            let fname = obj.file_name().unwrap();
            std::fs::copy(obj, link_dir.join(fname)).expect("copy link object");
        }

        let rs_bin = support::compile_rs_cargo_with_link_and_shims(
            &rs_out,
            &work,
            &name,
            &link_dir,
            Some(&shim_source),
        )
        .expect("compile rust with link");

        let cfg = support::RunConfig::default();
        let run_dir = work.join("run");
        if run_dir.exists() {
            std::fs::remove_dir_all(&run_dir).expect("remove old run dir");
        }
        std::fs::create_dir_all(&run_dir).expect("create run dir");

        let c_run = support::run_with_config(&c_bin, &cfg, &run_dir).expect("run C");
        let r_run = support::run_with_config(&rs_bin, &cfg, &run_dir).expect("run Rust");
        support::compare_runs(&c_run, &r_run, false).expect("C and Rust outputs differ");
    }
}
