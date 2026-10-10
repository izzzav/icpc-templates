#!/usr/bin/env bash
# Склеивает решение и все его #include "..." (KACTL, свои файлы) в ОДИН файл для отправки.
#   ./scripts/bundle.sh sol.cpp > send.cpp
#   ./scripts/bundle.sh sol.cpp | pbcopy      (macOS: сразу в буфер обмена, потом Cmd+V в систему)
#
# Что делает:
#   - #include <...> (системные) оставляет как есть;
#   - #include "..." заменяет содержимым файла, рекурсивно (LCA.h сам подтянет RMQ.h);
#   - каждый файл вставляет один раз (Point.h не продублируется);
#   - выкидывает #pragma once.
# Где ищет файл: рядом с текущим файлом -> ext/kactl/content -> корень репо.
# Если KACTL-файл переименован в стиле "Точка_Point.h", найдёт его и по имени "Point.h".
REPO="$(cd "$(dirname "$0")/.." && pwd -P)"
KACTL="$REPO/ext/kactl/content"
NL=$'\n'
SEEN="$NL"
inc_re='^[[:space:]]*#[[:space:]]*include[[:space:]]*"([^"]+)"'
once_re='^[[:space:]]*#[[:space:]]*pragma[[:space:]]+once'
missing=0

canon() { echo "$(cd "$(dirname "$1")" && pwd -P)/$(basename "$1")"; }

resolve() {  # resolve INC DIR -> печатает полный путь или ничего
  local inc="$1" dir="$2" cand base
  for cand in "$dir/$inc" "$KACTL/$inc" "$REPO/$inc"; do
    if [ -f "$cand" ]; then canon "$cand"; return; fi
  done
  base="$(basename "$inc")"
  cand="$(find "$KACTL" "$REPO/hw" \( -name "$base" -o -name "*_$base" \) -type f 2>/dev/null | head -1)"
  if [ -n "$cand" ]; then canon "$cand"; fi
}

emit() {  # emit FILE
  local f="$1" dir line inc p
  dir="$(dirname "$f")"
  while IFS= read -r line || [ -n "$line" ]; do
    if [[ $line =~ $once_re ]]; then continue; fi
    if [[ $line =~ $inc_re ]]; then
      inc="${BASH_REMATCH[1]}"
      p="$(resolve "$inc" "$dir")"
      if [ -z "$p" ]; then
        printf '%s\n' "$line"
        echo "bundle: НЕ НАЙДЕН \"$inc\" (подключается из $f)" >&2
        missing=1
        continue
      fi
      case "$SEEN" in *"$NL$p$NL"*) continue ;; esac
      SEEN="$SEEN$p$NL"
      printf '// ===== %s =====\n' "$(basename "$p")"
      emit "$p"
      continue
    fi
    printf '%s\n' "$line"
  done < "$f"
}

if [ -z "$1" ] || [ ! -f "$1" ]; then echo "использование: $0 sol.cpp > send.cpp" >&2; exit 1; fi
main="$(canon "$1")"
SEEN="$SEEN$main$NL"
emit "$main"
exit $missing
