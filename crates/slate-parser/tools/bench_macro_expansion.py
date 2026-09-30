import argparse
import subprocess
import tempfile
import time
from pathlib import Path


def source(uses, depth, literal_only):
    lines = [] if literal_only else [
        f"#define M{index} " + (f"M{index + 1}" if index + 1 < depth else "1")
        for index in range(depth)
    ]
    term = "1 == 1" if literal_only else "M0 == 1"
    expression = " && ".join([term] * uses)
    lines.extend((f"#if {expression}", "int reached;", "#endif"))
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", type=Path, default=Path("target/release/slate-parser"))
    parser.add_argument("--uses", type=int, default=50000)
    parser.add_argument("--depth", type=int, default=59)
    parser.add_argument("--runs", type=int, default=3)
    parser.add_argument("--literal-only", action="store_true")
    args = parser.parse_args()
    with tempfile.TemporaryDirectory() as directory:
        input_path = Path(directory) / "macro-expansion.c"
        input_path.write_text(
            source(args.uses, args.depth, args.literal_only)
        )
        elapsed = []
        for _ in range(args.runs):
            start = time.perf_counter()
            subprocess.run(
                [str(args.binary), "parse", str(input_path)],
                stdout=subprocess.DEVNULL,
                check=True,
            )
            elapsed.append(time.perf_counter() - start)
    print(" ".join(f"{duration:.3f}s" for duration in elapsed))
    shape_name = "literal-only #if" if args.literal_only else f"macro depth {args.depth}"
    print(f"best: {min(elapsed):.3f}s ({args.uses} terms, {shape_name})")


if __name__ == "__main__":
    main()
