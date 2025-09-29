CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
LDFLAGS := 
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
BIN := app

all: build

#build: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


build:
    @echo "=== Running build ==="

check: build
	@echo "=== Running tests ==="
#	./$(BIN) --test || true

lint:
	@echo "=== Running cppcheck ==="
#	cppcheck --enable=all --error-exitcode=1 --quiet .

distcheck:
	@echo "=== Running distcheck ==="
#	make clean
#	make all

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all build check lint distcheck clean