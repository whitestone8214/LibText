#!/bin/bash

if (test "$1" = "build"); then
	rm -rf libtext.so || exit 1
	gcc libtext.c -lm -fPIC -shared -o libtext.so || exit 1
	
elif (test "$1" = "install"); then
	cp -f libtext.so /lib || exit 1
	cp -f libtext.h /usr/include || exit 1
	cp -f libtext.pc /lib/pkgconfig || exit 1
	
	rm -f libtext.so
	
elif (test "$1" = "remove"); then
	rm -rf /lib/libtext.so /usr/include/libtext.h /lib/pkgconfig/libtext.pc || exit 1
	
fi
