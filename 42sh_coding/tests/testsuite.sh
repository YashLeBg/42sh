#!/bin/sh
set -eu

: "${BIN_PATH:?BIN_PATH is required}"

TESTS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$TESTS_DIR"

# SI OUTPUT_FILE est pas la on cree un truc temporaire 
if [ "${OUTPUT_FILE:-}" = "" ]
then
    OUTPUT_FILE="$(mktemp)"
fi

export BIN_PATH
export OUTPUT_FILE

VENV_DIR="$TESTS_DIR/.venv"
PYTHON="$VENV_DIR/bin/python"
PIP="$VENV_DIR/bin/pip"

# If python3/venv ne marche pas on fail pas le testsuite
if ! command -v python3 >/dev/null 2>&1
then
    printf "0" > "$OUTPUT_FILE"
    exit 0
fi

# Cree la venv
if [ ! -d "$VENV_DIR" ]
then
    if ! python3 -m venv "$VENV_DIR"
    then
        printf "0" > "$OUTPUT_FILE"
        exit 0
    fi
fi

# On installe pytests si jamais ca marche pas on ne fail pas le testsuite
if ! "$PYTHON" -c "import pytest" >/dev/null 2>&1
then
    if ! "$PIP" install --quiet pytest >/dev/null 2>&1
    then
        printf "0" > "$OUTPUT_FILE"
        exit 0
    fi
fi

# Run le runner wazaaaaaa
"$PYTHON" "$TESTS_DIR/run_tests.py" || true

exit 0
