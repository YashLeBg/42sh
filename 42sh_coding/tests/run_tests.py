#!/usr/bin/env python3
import os
import sys
from pathlib import Path

import pytest


class ResultsCounter:
    def __init__(self) -> None:
        self.passed = 0
        self.failed = 0

    def pytest_runtest_logreport(self, report):
        if report.when != "call":
            return
        #ici on va juste test les call qui passent, donc les codes qui sont executes 

        if report.passed:
            self.passed += 1
        elif report.failed:
            self.failed += 1
        else:
            pass


def write_percentage(output_file: Path, passed: int, failed: int) -> None:
    total = passed + failed
    if total == 0:
        pct = 0
    else:
        pct = (passed * 100) // total #rappel que le // en python c'est une division qui donne des entiers 

    output_file.parent.mkdir(parents=True, exist_ok=True)
    output_file.write_text(str(pct))
    # write_text() ouvre le path du output file si il existe pas il le cree puis ecris le pct et finalement le ferme


def main() -> int:
    bin_path = os.environ.get("BIN_PATH")
    output_file = os.environ.get("OUTPUT_FILE")
    coverage = os.environ.get("COVERAGE") == "yes"

    if not bin_path:
        print("BIN_PATH is required", file=sys.stderr)
        return 0

    if not output_file:
        print("OUTPUT_FILE is required", file=sys.stderr)
        return 0
    #pour check des erreurs et return 0

    os.environ["BIN_PATH"] = bin_path

    marker_expr = "functional or unit" if coverage else "functional"

    counter = ResultsCounter()

    pytest_args = [
        "-q",
        "-m",
        marker_expr,
        str(Path(__file__).resolve().parent), #pour garantir que il collecte que depuis tests
    ]

    _pytest_rc = pytest.main(pytest_args, plugins=[counter])

    write_percentage(Path(output_file), counter.passed, counter.failed)
    #rappel que le output_file est un pathlib.Path du genre Path("machin/truc/42sh")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
