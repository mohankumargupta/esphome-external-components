set shell := ["sh", "-c"]
set windows-shell := ["powershell", "-c"]

# _main:
#     @just --list

compile:
    uv run --with=pip python -m esphome compile examples/epaper-bruteforce.yaml

pio:
    cd examples/.esphome/build/epaper; uvx --with=pip,platformio pio run -t compiledb

cpp:
    cd examples/.esphome/build/epaper; uvx --with=pip,platformio pio init --ide=vscode
    uv run python -c "import shutil; shutil.copy('examples/.esphome/build/epaper/.vscode/c_cpp_properties.json', '.vscode/c_cpp_properties.json')"



