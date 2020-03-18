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
			 -Wno-unused-result
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
			 -Wno-global-constructors -Wno-exit-time-destructors
INSTRUMENTFLAGS=-O0 -g -fsanitize=undefined  \
				-fsanitize=address

# -fsanitize=safe-stack
# -fsanitize=memory

endif

# set targets here
RELEASE_TARGETS=
DEBUG_TARGETS=bigint_basic_test
BENCHMARK_TARGETS=


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

bigint_basic_test: compile/bigint_basic_test.o compile/bigint.o
	$(CXX) $(CXXFLAGS) compile/bigint_basic_test.o compile/bigint.o \
		-o bigint_basic_test


compile/bigint_basic_test.o: compile/bigint.o src/bigint.hpp
	$(CXX) $(CXXFLAGS) -c tests/bigint_basic_test.cpp \
		-o compile/bigint_basic_test.o

compile/bigint.o: src/bigint.cpp src/bigint.hpp src/bigint_bit_arith.cpp
	$(CXX) $(CXXFLAGS) -c src/bigint.cpp -o compile/bigint.o

.PHONY: all clean clean-all
clean:
	-rm compile/*

clean-all: clean
	-rm $(TARGETS)
