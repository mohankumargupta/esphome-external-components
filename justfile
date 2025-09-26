set shell := ["sh", "-c"]
set windows-shell := ["powershell", "-c"]

# _main:
#     @just --list

compile:
    uv run --with=pip python -m esphome compile examples/epaper-bruteforce.yaml


