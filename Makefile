CXX            = g++
CXXFLAGS       = -O3 -Wall -std=c++17 -static-libgcc -static-libstdc++
BIN_DIR        := bin
CATCH_AMALG    := vector/catch_amalgamated.cpp
VECTOR_MAIN_SRCS := vector/vector.cpp vector/vectorfunk.cpp
DLL_NAME       = mylib
DLL_SRC        = vector/dll_exports.cpp
DLL_OBJ        = vector/dll_exports.o

VECTOR_LIB_SRCS := $(filter-out \
    vector/vector.cpp \
    vector/vectorfunk.cpp \
    vector/catch_amalgamated.cpp \
    vector/catch.cpp, \
    $(VECTOR_SRCS))

.PHONY: all clean dll

all: dll $(BIN_DIR)/catch_tests.exe $(BIN_DIR)/vector.exe

$(BIN_DIR):
	if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

dll: $(DLL_OBJ)
	$(CXX) -shared -o $(DLL_NAME).dll $^

$(DLL_OBJ): $(DLL_SRC)
	$(CXX) $(CXXFLAGS) -fPIC -DSPAUSDINIMO_EXPORTS -c $< -o $@

$(BIN_DIR)/catch_tests.exe: vector/catch.cpp $(VECTOR_LIB_SRCS) $(CATCH_AMALG) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I. -Ivector \
	    -o $@ vector/catch.cpp $(VECTOR_LIB_SRCS) $(CATCH_AMALG)

$(BIN_DIR)/vector.exe: $(VECTOR_MAIN_SRCS) | $(BIN_DIR) dll
	$(CXX) $(CXXFLAGS) -I. -Ivector -o $@ $^ -L. -l$(DLL_NAME)

clean:
	if exist "$(BIN_DIR)" rmdir /S /Q "$(BIN_DIR)"

