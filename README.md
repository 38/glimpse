
Glimpse
=======
A generic,high-performance key-value log processing framework written in C.

#How to build?

	1) run `cmake .' to generate Makefile

	2) Edit File <include/future.h> to enable/disable features (optional)

	3) make

After you build Glimpse, you can type 'make install' to install glimpse onto your computer.

If you want to remove Glimpse, run the script 'scripts/uninstall.sh ${INSTALL_PREFIX}'

#Build Glimpse

prequisite:

	cmake

	libreadline
	
#Try Glimpse

You can try the framework with the shell, glimpse-cli.

#How to build a Plugin
	
Steps to compiling examples/plugin.c into a loadable Plugin

1) Compile plugin.c 

	gcc `glimpse-config --plugin --cflags TypeAPI` -c -o plugin.o plugin.c
	
2) Link libplugin.so

	gcc `glimpse-config --plugin --libs TypeAPI` -o ligplugin.so plugin.o

3) Test plugin with glimpse-cli
	
	$glimpse-cli 
	Glimpse CLI Shell (libglimpse 0.0.1)
	with multi-char-seperator=no,parser-stack=no,multi-threading=no,lazy-instance=yes,char-table=no
	Type `help' for avaiable commands
	
	Glimpse> import plugin
	F[myinit@plugin.c:  4] Hello, Glimpse!
	0.299409 ms
	
	
