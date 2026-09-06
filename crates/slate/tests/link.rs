mod support;

use std::path::{Path, PathBuf};

fn link_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.link")
}

const KNOWN_BROKEN_FIXTURES: &[&str] = &["long_double", "long_double_aggregate"];

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
        if KNOWN_BROKEN_FIXTURES.contains(&name.as_str()) {
            eprintln!("skipping known-broken link fixture: {name}");
            continue;
        }
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

fn link_project_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.link.project")
}

#[test]
fn link_project_partial_translation_runs_match_c() {
    let root = link_project_fixtures_dir();
    if !root.exists() {
        return;
    }
    for entry in std::fs::read_dir(&root).expect("read link project fixtures dir") {
        let dir = entry.expect("read entry").path();
        if !dir.is_dir() {
            continue;
        }
        let name = dir.file_name().unwrap().to_string_lossy().into_owned();
        eprintln!("running link project fixture: {name}");

        let database_path = dir.join("compile_commands.json");
        let translated_stems: std::collections::BTreeSet<String> =
            std::fs::read_to_string(dir.join("translate.list"))
                .expect("read translate.list")
                .lines()
                .map(str::trim)
                .filter(|line| !line.is_empty())
                .map(str::to_string)
                .collect();

        let commands: Vec<serde_json::Value> = serde_json::from_str(
            &std::fs::read_to_string(&database_path).expect("read compile_commands.json"),
        )
        .expect("parse compile_commands.json");

        let work = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/link-project")
            .join(&name);
        let _ = std::fs::remove_dir_all(&work);
        std::fs::create_dir_all(&work).expect("create work dir");

        let mut translated_entries = Vec::new();
        let mut native_srcs: Vec<PathBuf> = Vec::new();
        for command in &commands {
            let file = command["file"].as_str().expect("file field");
            let stem = Path::new(file)
                .file_stem()
                .and_then(|s| s.to_str())
                .expect("file stem")
                .to_string();
            if translated_stems.contains(&stem) {
                let mut entry = command.clone();
                entry["directory"] = serde_json::Value::String(dir.display().to_string());
                translated_entries.push(entry);
            } else {
                native_srcs.push(dir.join(file));
            }
        }
        assert!(
            !translated_entries.is_empty(),
            "{name}: translate.list selected no compile_commands entries"
        );
        assert!(
            !native_srcs.is_empty(),
            "{name}: no native-only sources left to link -- not exercising partial translation"
        );

        let filtered_database = work.join("compile_commands.json");
        std::fs::write(
            &filtered_database,
            serde_json::to_vec(&translated_entries).expect("encode filtered compile commands"),
        )
        .expect("write filtered compile_commands.json");

        let mut native_objects: Vec<PathBuf> = Vec::new();
        for src in &native_srcs {
            let stem = src.file_stem().unwrap().to_string_lossy();
            let obj = work.join(format!("{stem}.o"));
            support::compile_c_object(src, &obj).expect("compile native object");
            native_objects.push(obj);
        }

        let crate_dir = work.join("rs");
        let _ = std::fs::remove_dir_all(&crate_dir);
        support::translate_project_from_database(&dir, &crate_dir, &filtered_database)
            .expect("translate project (partial)");

        let link_args: String = native_objects
            .iter()
            .map(|obj| format!("println!(\"cargo:rustc-link-arg={}\");\n", obj.display()))
            .collect();
        std::fs::write(
            crate_dir.join("build.rs"),
            format!("fn main() {{\n{link_args}}}\n"),
        )
        .expect("write build.rs");

        let rs_bin = support::compile_rs_project(&crate_dir).expect("compile Rust project");

        let mut all_srcs: Vec<PathBuf> = commands
            .iter()
            .map(|command| dir.join(command["file"].as_str().expect("file field")))
            .collect();
        all_srcs.sort();
        let c_bin = work.join("c_bin");
        support::compile_c_multi(&all_srcs, &c_bin).expect("compile C reference");

        let cfg = support::RunConfig::default();
        let run_dir = work.join("run");
        let _ = std::fs::remove_dir_all(&run_dir);
        std::fs::create_dir_all(&run_dir).expect("create run dir");
        let c_run = support::run_with_config(&c_bin, &cfg, &run_dir).expect("run C");
        let r_run = support::run_with_config(&rs_bin, &cfg, &run_dir).expect("run Rust");
        support::compare_runs(&c_run, &r_run, false).expect("C and Rust outputs differ");
    }
}
