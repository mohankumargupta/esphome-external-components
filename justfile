set shell := ["sh", "-c"]
set windows-shell := ["powershell", "-c"]

_main:
    @just --list

compile:
    uv run --with=pip python -m esphome compile examples/example.yaml


