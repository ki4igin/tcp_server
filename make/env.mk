# BARDY ENV
BARDYDIR := /home/al/bardy/
BARDYLIB := ${BARDYDIR}/bin
GIPCYDIR := ${BARDYDIR}/gipcy
GIPCYLIB := ${GIPCYDIR}/lib
GIPCYINC := ${GIPCYDIR}/include
LD_LIBRARY_PATH := ${LD_LIBRARY_PATH} ${GIPCYLIB} ${BARDYLIB}

# ARAGO ENV
TOOLS_DIR := ${HOME}/project/fm403c
TOOLCHAIN_PATH := ${TOOLS_DIR}/host/opt/ext-toolchain/bin
TARGET_SYS := $(TOOLCHAIN_PATH)/arm-arago-linux-gnueabi
CC := ${TARGET_SYS}-gcc
CPP := "${TARGET_SYS}-gcc"
CXX := "${TARGET_SYS}-gcc"
NM := ${TARGET_SYS}-nm
RANLIB := ${TARGET_SYS}-ranlib
OBJCOPY := ${TARGET_SYS}-objcopy
STRIP := ${TARGET_SYS}-strip
AS := ${TARGET_SYS}-as
AR := ${TARGET_SYS}-ar
OBJDUMP := ${TARGET_SYS}-objdump
CROSS_COMPILE := ${TARGET_SYS}-

