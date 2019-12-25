
INC_PATH =                                  \
        -Icore/config/                      \
        -Icore/engine/                      \
        -Icore/system/                      \
        -Icore/tracer/                      \
        -Idata/materials/                   \
        -Idata/objects/                     \
        -Idata/scenes/                      \
        -Idata/textures/

SRC_LIST =                                  \
        core/engine/engine.cpp              \
        core/engine/object.cpp              \
        core/engine/rtgeom.cpp              \
        core/engine/rtimag.cpp              \
        core/system/system.cpp              \
        core/tracer/tracer.cpp              \
        core/tracer/tracer_128v1.cpp        \
        core/tracer/tracer_256v1.cpp        \
        RooT_linux.cpp

LIB_PATH =

LIB_LIST =                                  \
        -lm                                 \
        -lstdc++                            \
        -lX11                               \
        -lXext                              \
        -lpthread


build: RooT_m32

strip:
	mipsel-linux-gnu-strip RooT.m32

clean:
	rm RooT.m32


RooT_m32:
	mipsel-linux-gnu-g++ -O3 -g -pthread -mips32r5 -mmsa -mnan=2008 \
        -DRT_LINUX -DRT_M32 -DRT_128=1 -DRT_256=1 \
        -DRT_POINTER=32 -DRT_ADDRESS=32 -DRT_ELEMENT=32 -DRT_ENDIAN=0 \
        -DRT_DEBUG=0 -DRT_PATH="./" -DRT_FULLSCREEN=0 \
        -DRT_EMBED_STDOUT=0 -DRT_EMBED_FILEIO=0 -DRT_EMBED_TEX=1 \
        ${INC_PATH} ${SRC_LIST} ${LIB_PATH} ${LIB_LIST} -o RooT.m32


# Prerequisites for the build:
# native-compiler for MIPS+MSA is installed and in the PATH variable.
# sudo apt-get install g++ libxext-dev (on P5600 host or QEMU system mode)
# (recent upstream g++-5-mipsel series may not fully support MSA)
#
# Building/running RooT demo:
# make -f RooT_make_m32.mk
# ./RooT.m32 (on P5600 host or QEMU system mode)
# (hasn't been verified due to lack of target host system)
# (SIMD and CORE tests pass in QEMU linux-user mode, check test subfolder)

# RooT demo uses runtime SIMD target selection, multiple can be specified above
# on RISC targets top value above is chosen by default, use -n/-k/-s to override
# 256-bit SIMD is achieved by combining pairs of 128-bit registers/instructions

# For interpretation of SIMD build flags check compatibility layer in rtzero.h
