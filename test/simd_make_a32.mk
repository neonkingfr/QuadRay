
INC_PATH =                          \
        -I../core/config/

SRC_LIST =                          \
        simd_test.cpp

simd_test:
	aarch64-linux-gnu-g++ -O3 -g -mabi=ilp32 \
        -DRT_LINUX -DRT_A32 -DRT_128=1 -DRT_DEBUG=1 \
        ${INC_PATH} ${SRC_LIST} -o simd_test.a32