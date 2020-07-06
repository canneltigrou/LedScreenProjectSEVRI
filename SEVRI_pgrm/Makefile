CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS)
OBJECTS=code_leds.o ABE_IoPi.o
BINARIES=code_leds

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=../submodule_rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all : $(BINARIES)


$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

ABE_IoPi.o : ABE_IoPi.cc $(RGB_LIBRARY)
	     $(CXX) -c ABE_IoPi.cc

code_leds.o : ABE_IoPi.cc code_leds.cpp $(RGB_LIBRARY)
	$(CXX) -c code_leds.cpp $(LDFLAGS) 

code_leds : ABE_IoPi.o code_leds.o $(RGB_LIBRARY)
	$(CXX) ABE_IoPi.o code_leds.o -o $@ $(LDFLAGS)


# All the binaries that have the same name as the object file.q
% : %.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

# Since the C example uses the C++ library underneath, which depends on C++
# runtime stuff, you still have to also link -lstdc++
#c-example : c-example.o $(RGB_LIBRARY)
#	$(CC) $< -o $@ $(LDFLAGS) -lstdc++

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
