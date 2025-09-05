# api/main.py
import subprocess
import threading
import os
import json
from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse

app = FastAPI()

cpp_process = None
proc_lock = threading.Lock()

def read_stderr(proc):
    for line in proc.stderr:
        print("[C++ stderr]", line.rstrip())

@app.on_event("startup")
def on_startup():
    """
    Start game.exe exactly once when FastAPI starts.
    """
    global cpp_process
    bin_path = os.path.join(os.path.dirname(__file__), "..", "game.exe")

    if not os.path.exists(bin_path):
        raise RuntimeError(f"game.exe not found at: {bin_path}")

    cpp_process = subprocess.Popen(
        [bin_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,          # text mode
        bufsize=1,          # line buffered
        encoding="utf-8",   # make Windows decode UTF-8
        errors="replace"    # avoid crashes on odd chars
    )
    threading.Thread(target=read_stderr, args=(cpp_process,), daemon=True).start()
    print("[FASTAPI] Started game.exe:", bin_path)

@app.post("/game")
async def game_endpoint(request: Request):
    """
    Forward JSON to C++ (one line), read one JSON line back.
    """
    if cpp_process is None or cpp_process.poll() is not None:
        return JSONResponse(status_code=500, content={"error": "game.exe is not running"})

    req_json = await request.json()

    # UI may send class_ — C++ expects class
    if "class_" in req_json:
        req_json["class"] = req_json.pop("class_")

    line = json.dumps(req_json, ensure_ascii=False)

    with proc_lock:
        # write one line
        cpp_process.stdin.write(line + "\n")
        cpp_process.stdin.flush()

        # read one non-empty line
        resp_line = cpp_process.stdout.readline()
        while resp_line is not None and resp_line.strip() == "":
            resp_line = cpp_process.stdout.readline()

        print("[C++]", (resp_line or "").strip())

    # parse JSON from C++
    try:
        return json.loads(resp_line)
    except Exception as e:
        return JSONResponse(
            status_code=500,
            content={"error": f"Bad JSON from C++: {e}", "raw": resp_line},
        )

@app.on_event("shutdown")
def on_shutdown():
    """
    Cleanly stop the C++ process when FastAPI stops.
    """
    global cpp_process
    try:
        if cpp_process and cpp_process.poll() is None:
            cpp_process.terminate()
            cpp_process.wait(timeout=2)
    except Exception:
        pass
