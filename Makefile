CXX = clang++
C = clang
CPPFLAGS = -c -O3 -std=c++11
CFLAGS = -c  -O3
LDFLAGS = -lreadline -lgsl -lnest3 -lgfortran -llapack -Wl,-no_compact_unwind
# -lMinuit2
EXECUTABLE = slap


CXXSOURCES = src/SLAP.cpp
MODELS = src/models/BB4.cpp src/models/BB6.cpp src/models/Magnetar.cpp src/models/MagnetarK.cpp src/models/MagnetarR.cpp
CORE = src/core/Cosmology.cpp src/core/Filters.cpp src/core/SNModel.cpp src/core/SNEvent.cpp src/core/Workspace.cpp src/core/Absorption.cpp
UTILS = src/utils/utils.cpp
FUNC = src/func/fit.cpp src/func/plot.cpp src/func/analyse.cpp
# src/func/fit2.cpp

OBJECTS = ${CXXSOURCES:.cpp=.o} ${MODELS:.cpp=.o} ${CORE:.cpp=.o} ${UTILS:.cpp=.o} ${FUNC:.cpp=.o}


all: $(CXXSOURCES) $(MPFIT) $(MODELS) $(CORE) $(EXECUTABLE) $(UTILS) $(FUNC) 
	
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CPPFLAGS) $< -o $@

.c.o:
	$(C) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -f */*.o

install:
	cp slap /usr/local/bin/