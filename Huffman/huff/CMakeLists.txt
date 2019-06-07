add_library(huffman
	    huffman_coding.cpp
)

add_executable(tester
			   huffman_coding.h
	           huffman_coding.cpp
               Testing/testing.cpp
               Testing/gtest/gtest-all.cc
               Testing/gtest/gtest.h
               Testing/gtest/gtest_main.cc
)

target_link_libraries(tester -lgmp -lgmpxx -lpthread)