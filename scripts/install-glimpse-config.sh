#!/bin/sh
sed -e "s:PYTHON_INTERPRETER_PATH:${1}:g"\
	-e "s:GLIMPSE_INCLUDE_PATH:${2}/include/glimpse:g"\
	-e "s:GLIMPSE_LIBRARY_PATH:${2}/lib/glimpse:g"\
	scripts/glimpse-config.txt > ${2}/bin/glimpse-config
chmod 755 ${2}/bin/glimpse-config

