# MODE: debug/benchmark/release
MODE=debug
# CXX: g++/clang++/clang++-9
CXX=g++

ifeq ($(CXX),g++)

CXXFLAGS=-std=c++17
WARNINGFLAGS=-Wall -Weffc++ -pedantic \
			 -pedantic-errors -Wextra -Waggregate-return -Wcast-align \
			 -Wcast-qual -Wconversion \
			 -Wdisabled-optimization \
			 -Wfloat-equal -Wformat=2 \
			 -Wformat-nonliteral -Wformat-security \
			 -Wformat-y2k \
			 -Wimport -Winit-self -Winline \
			 -Winvalid-pch \
			 -Wlong-long \
			 -Wmissing-field-initializers -Wmissing-format-attribute \
			 -Wmissing-include-dirs -Wmissing-noreturn \
			 -Wpacked -Wpointer-arith \
			 -Wredundant-decls \
			 -Wshadow -Wstack-protector \
			 -Wstrict-aliasing=2 -Wswitch-default \
			 -Wswitch-enum \
			 -Wunreachable-code -Wunused \
			 -Wunused-parameter \
			 -Wvariadic-macros \
			 -Wwrite-strings \
			 -Wtautological-compare \
			 -Wno-unused-result \
			 -Wno-aggregate-return \
			 -Wno-conversion
INSTRUMENTFLAGS=-Og -g -fsanitize=address \
				-fsanitize=leak -fsanitize=undefined \
				-fsanitize-address-use-after-scope \
				-fsanitize=float-divide-by-zero -fsanitize=bounds-strict \
				-fstack-protector-all -fstack-check

else

# assume it's clang++
CXXFLAGS=-std=c++17 -Wno-unused-command-line-argument
WARNINGFLAGS=-Weverything -Wno-c++98-compat -Wno-missing-prototypes \
			 -Wno-c++98-compat-pedantic -Wno-weak-template-vtables \
			 -Wno-global-constructors -Wno-exit-time-destructors \
			 -Wno-c++17-extensions
INSTRUMENTFLAGS=-O0 -g -fsanitize=undefined  \
				-fsanitize=address

# -fsanitize=safe-stack
# -fsanitize=memory

endif

# set targets here
RELEASE_TARGETS=bigint64_test bigint64_benchmark bigint64_test2
DEBUG_TARGETS=bigint_basic_test bigint_bit_arith_test bigint_simple_arith_test \
			  bigint_mul_test bigint_divmod_test bigint_ext_arith_test \
			  bigint_io_test
BENCHMARK_TARGETS=bigint_bit_arith_benchmark bigint_simple_arith_benchmark \
				  bigint_mul_benchmark bigint_divmod_benchmark \
				  bigint_ext_arith_benchmark bigint_io_benchmark


ifeq ($(MODE),debug)

TARGETS=$(RELEASE_TARGETS) $(BENCHMARK_TARGETS) $(DEBUG_TARGETS)
CXXFLAGS+=$(WARNINGFLAGS) $(INSTRUMENTFLAGS)
else ifeq ($(MODE), benchmark)

TARGETS=$(RELEASE_TARGETS) $(BENCHMARK_TARGETS)
CXXFLAGS+=-O2

else

TARGETS=$(RELEASE_TARGETS)
CXXFLAGS+=-O2

endif

# recipes start here
all: $(TARGETS)

compile/bigint.o: src/bigint.cpp src/bigint.hpp src/bigint_bit_arith.cpp \
	src/bigint_io.cpp src/bigint_addsub.cpp src/bigint_mul.cpp \
	src/bigint_divmod.cpp src/bigint_ext.cpp
	$(CXX) $(CXXFLAGS) -c src/bigint.cpp -o compile/bigint.o

bigint_basic_test: compile/bigint_basic_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_basic_test.o compile/bigint.o \
		-o bigint_basic_test
compile/bigint_basic_test.o: src/bigint.hpp tests/bigint_basic_test.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_basic_test.cpp \
		-o compile/bigint_basic_test.o

bigint_bit_arith_test: compile/bigint_bit_arith_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_bit_arith_test.o compile/bigint.o \
		-o bigint_bit_arith_test
compile/bigint_bit_arith_test.o: src/bigint.hpp tests/bigint_bit_arith_test.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_bit_arith_test.cpp \
		-o compile/bigint_bit_arith_test.o
bigint_bit_arith_benchmark: compile/bigint_bit_arith_benchmark.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_bit_arith_benchmark.o compile/bigint.o \
		-o bigint_bit_arith_benchmark
compile/bigint_bit_arith_benchmark.o: src/bigint.hpp tests/bigint_bit_arith_benchmark.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_bit_arith_benchmark.cpp \
		-o compile/bigint_bit_arith_benchmark.o

bigint_simple_arith_test: compile/bigint_simple_arith_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_simple_arith_test.o compile/bigint.o \
		-o bigint_simple_arith_test
compile/bigint_simple_arith_test.o: src/bigint.hpp tests/bigint_simple_arith_test.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_simple_arith_test.cpp \
		-o compile/bigint_simple_arith_test.o
bigint_simple_arith_benchmark: compile/bigint_simple_arith_benchmark.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_simple_arith_benchmark.o compile/bigint.o \
		-o bigint_simple_arith_benchmark
compile/bigint_simple_arith_benchmark.o: src/bigint.hpp tests/bigint_simple_arith_benchmark.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_simple_arith_benchmark.cpp \
		-o compile/bigint_simple_arith_benchmark.o

bigint_mul_test: compile/bigint_mul_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_mul_test.o compile/bigint.o \
		-o bigint_mul_test
compile/bigint_mul_test.o: src/bigint.hpp tests/bigint_mul_test.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_mul_test.cpp \
		-o compile/bigint_mul_test.o
bigint_mul_benchmark: compile/bigint_mul_benchmark.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_mul_benchmark.o compile/bigint.o \
		-o bigint_mul_benchmark
compile/bigint_mul_benchmark.o: src/bigint.hpp tests/bigint_mul_benchmark.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_mul_benchmark.cpp \
		-o compile/bigint_mul_benchmark.o

bigint_divmod_test: compile/bigint_divmod_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_divmod_test.o compile/bigint.o \
		-o bigint_divmod_test
compile/bigint_divmod_test.o: src/bigint.hpp tests/bigint_divmod_test.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_divmod_test.cpp \
		-o compile/bigint_divmod_test.o
bigint_divmod_benchmark: compile/bigint_divmod_benchmark.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_divmod_benchmark.o compile/bigint.o \
		-o bigint_divmod_benchmark
compile/bigint_divmod_benchmark.o: src/bigint.hpp tests/bigint_divmod_benchmark.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_divmod_benchmark.cpp \
		-o compile/bigint_divmod_benchmark.o

bigint_ext_arith_test: compile/bigint_ext_arith_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_ext_arith_test.o compile/bigint.o \
		-o bigint_ext_arith_test
compile/bigint_ext_arith_test.o: src/bigint.hpp tests/bigint_ext_arith_test.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_ext_arith_test.cpp \
		-o compile/bigint_ext_arith_test.o
bigint_ext_arith_benchmark: compile/bigint_ext_arith_benchmark.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_ext_arith_benchmark.o compile/bigint.o \
		-o bigint_ext_arith_benchmark
compile/bigint_ext_arith_benchmark.o: src/bigint.hpp tests/bigint_ext_arith_benchmark.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_ext_arith_benchmark.cpp \
		-o compile/bigint_ext_arith_benchmark.o

bigint_io_test: compile/bigint_io_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_io_test.o compile/bigint.o \
		-o bigint_io_test
compile/bigint_io_test.o: src/bigint.hpp tests/bigint_io_test.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_io_test.cpp \
		-o compile/bigint_io_test.o
bigint_io_benchmark: compile/bigint_io_benchmark.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_io_benchmark.o compile/bigint.o \
		-o bigint_io_benchmark
compile/bigint_io_benchmark.o: src/bigint.hpp tests/bigint_io_benchmark.cpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_io_benchmark.cpp \
		-o compile/bigint_io_benchmark.o

compile/bigint64.o: src/bigint64.cpp src/bigint64.hpp src/bigint64_bit.cpp \
	src/bigint64_io.cpp src/bigint64_add.cpp src/bigint64_basic.cpp \
	src/bigint64_mul.cpp src/bigint64_div.cpp src/bigint64_compare.cpp
	$(CXX) $(CXXFLAGS) -c src/bigint64.cpp -o compile/bigint64.o

bigint64_test: tests/bigint64_test.cpp \
	src/bigint64.hpp compile/bigint64.o
	$(CXX) $(CXXFLAGS) tests/bigint64_test.cpp compile/bigint64.o -o bigint64_test

bigint64_test2: tests/bigint64_test2.cpp \
	src/bigint64.hpp compile/bigint64.o
	$(CXX) $(CXXFLAGS) tests/bigint64_test2.cpp compile/bigint64.o -o bigint64_test2

bigint64_benchmark: tests/bigint64_benchmark.cpp \
	src/bigint64.hpp compile/bigint64.o
	$(CXX) $(CXXFLAGS) tests/bigint64_benchmark.cpp compile/bigint64.o -o bigint64_benchmark

.PHONY: all clean clean-all
clean:
	-rm compile/*

clean-all: clean
	-rm $(TARGETS)
