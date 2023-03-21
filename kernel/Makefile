export TOKEN := anZP9zD6
export MODULE  := L1
NAME           := kernel
SRCS           := framework/main.c $(shell find -L ./src/ -name "*.c")
INC_PATH       := include/ framework/

>>>>>>> 4d14fa2a3536e0df8361b8ee0f3beaa6943f4aff
export AM_HOME := $(PWD)/../abstract-machine
ifeq ($(ARCH),)
export ARCH := x86_64-qemu
endif

include $(AM_HOME)/Makefile
include ../Makefile.lab
image: git
