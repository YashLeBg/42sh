import os
import subprocess

class ShellResult:
    def __init__(self, returncode, stdout, stderr, timeout):
        self.returncode = returncode
        self.stdout = stdout
        self.stderr = stderr
        self.timeout = timeout

def run_shell(script: str, timeout_s: float = 1.0) -> ShellResult:
    bin_path = os.environ.get("BIN_PATH")
    if not bin_path:
        raise RuntimeError("BIN_PATH is required")

    try:
        cp = subprocess.run(
            [bin_path],
            input=script,
            text=True,
            capture_output=True,
            timeout=timeout_s,
        )
        return ShellResult(cp.returncode, cp.stdout, cp.stderr, False)
    except subprocess.TimeoutExpired as e:
        return ShellResult(124, e.stdout or "", e.stderr or "", True)
