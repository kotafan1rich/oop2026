#!/bin/sh
set -eu

script_dir=$(CDPATH= cd "$(dirname "$0")" && pwd)
source_dir="$script_dir/src"
render_dir="$script_dir/rendered"

if ! command -v curl >/dev/null 2>&1; then
    echo 'Ошибка: нужен curl.' >&2
    exit 1
fi

mkdir -p "$render_dir"

found=false
tmp_file=
trap 'if [ -n "$tmp_file" ]; then rm -f "$tmp_file"; fi' 0

for diagram in "$source_dir"/*.puml; do
    [ -f "$diagram" ] || continue
    found=true

    name=${diagram##*/}
    name=${name%.puml}
    tmp_file=$(mktemp "$render_dir/.${name}.XXXXXX")

    curl -sS --fail -X POST -H 'Content-Type: text/plain' \
        --data-binary "@$diagram" \
        https://kroki.io/plantuml/svg \
        -o "$tmp_file"

    mv "$tmp_file" "$render_dir/$name.svg"
    tmp_file=
    printf 'Готово: %s.svg\n' "$name"
done

if [ "$found" = false ]; then
    echo "В $source_dir нет файлов .puml." >&2
    exit 1
fi
