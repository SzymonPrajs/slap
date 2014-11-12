CXX = clang++
C = clang
CPPFLAGS = -c -O3 -std=c++11
CFLAGS = -c -O3
LDFLAGS = 
EXECUTABLE = slap


CXXSOURCES = src/SLAP.cpp
MPFIT = src/utils/mpfit.c
MODELS = src/models/BB4.cpp src/models/BB6.cpp
CORE = src/core/Cosmology.cpp src/core/Filters.cpp src/core/SNModel.cpp src/core/SNEvent.cpp
UTILS = src/utils/utils.cpp
FUNC = src/func/fit.cpp

OBJECTS = ${CXXSOURCES:.cpp=.o} ${MPFIT:.c=.o} ${MODELS:.cpp=.o} ${CORE:.cpp=.o} ${UTILS:.cpp=.o} ${FUNC:.cpp=.o}


all: $(CXXSOURCES) $(MPFIT) $(MODELS) $(CORE) $(EXECUTABLE) $(UTILS) $(FUNC) 
	
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CPPFLAGS) $< -o $@

.c.o:
	$(C) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)