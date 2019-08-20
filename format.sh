#!/bin/bash
if [ "$1" = "-d" ];
then
  find ./src ./test -iname '*.h' -o -iname '*.cpp' | xargs clang-format-7 -i -style=Chromium
else
  command=`find ./src ./test -iname '*.h' -o -iname '*.cpp' | xargs clang-format-7 -output-replacements-xml | grep -c "<replacement "`
  #echo ${command[@]}
  if [ "${command}" -gt 0 ];
  then
    echo "${command} unsresolved issue(s). Call format.sh -d to resolve them."
  else
    echo "ok"
  fi
fi
