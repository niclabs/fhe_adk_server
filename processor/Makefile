# Copyright (C) 2012,2013 IBM Corp.
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# 
CC = g++
#
CFLAGS = -g -O2 -Wfatal-errors -Wshadow -Wall -I/usr/local/include 

#  If you get compilation errors, you may need to add -std=c++11 or -std=c++0x
#CFLAGS = -g -O2  -Wfatal-errors -Wshadow -Wall -I/usr/local/include -std=c++11 -lstdc++
#CFLAGS = -g -O2  -Wfatal-errors -Wshadow -Wall -I/usr/local/include -std=c++0x

#  To get the old DoubleCRT implementation, add NO_ALT_CRT flag
#CFLAGS= -g -O2 -Wfatal-errors -Wshadow -Wall -I/usr/local/include -DNO_ALT_CRT

$(info HElib requires NTL version 6.1.0 or higher, see http://shoup.net/ntl)
$(info If you get compilation errors, try to add/remove -std=c++11 in Makefile)
$(info )

LD = g++
AR = ar
ARFLAGS=ruv
GMP=-lgmp 
LDLIBS = -L/usr/local/lib -lntl $(GMP) -lm

HEADER = ../libs/HElib/EncryptedArray.h ../libs/HElib/FHE.h ../libs/HElib/Ctxt.h ../libs/HElib/CModulus.h ../libs/HElib/FHEContext.h ../libs/HElib/PAlgebra.h ../libs/HElib/SingleCRT.h ../libs/HElib/DoubleCRT.h ../libs/HElib/NumbTh.h ../libs/HElib/bluestein.h ../libs/HElib/IndexSet.h ../libs/HElib/timing.h ../libs/HElib/IndexMap.h ../libs/HElib/replicate.h ../libs/HElib/hypercube.h ../libs/HElib/matching.h ../libs/HElib/powerful.h ../libs/HElib/permutations.h ../libs/HElib/polyEval.h

SRC = ../libs/HElib/KeySwitching.cpp ../libs/HElib/EncryptedArray.cpp ../libs/HElib/FHE.cpp ../libs/HElib/../libs/HElib/Ctxt.cpp ../libs/HElib/CModulus.cpp ../libs/HElib/FHEContext.cpp ../libs/HElib/PAlgebra.cpp ../libs/HElib/SingleCRT.cpp ../libs/HElib/DoubleCRT.cpp ../libs/HElib/NumbTh.cpp ../libs/HElib/PAlgebraMod.cpp ../libs/HElib/bluestein.cpp ../libs/HElib/IndexSet.cpp ../libs/HElib/timing.cpp ../libs/HElib/replicate.cpp ../libs/HElib/hypercube.cpp ../libs/HElib/matching.cpp ../libs/HElib/powerful.cpp ../libs/HElib/BenesNetwork.cpp ../libs/HElib/permutations.cpp ../libs/HElib/PermNetwork.cpp ../libs/HElib/OptimizePermutations.cpp ../libs/HElib/eqtesting.cpp ../libs/HElib/polyEval.cpp

#OBJ = EncryptedArray.o FHE.o Ctxt.o CModulus.o FHEContext.o PAlgebra.o SingleCRT.o DoubleCRT.o NumbTh.o bluestein.o IndexSet.o timing.o KeySwitching.o PAlgebraMod.o

OBJ = ../libs/HElib/NumbTh.o ../libs/HElib/timing.o ../libs/HElib/bluestein.o ../libs/HElib/PAlgebra.o  ../libs/HElib/CModulus.o ../libs/HElib/FHEContext.o ../libs/HElib/IndexSet.o ../libs/HElib/DoubleCRT.o ../libs/HElib/SingleCRT.o ../libs/HElib/FHE.o ../libs/HElib/KeySwitching.o ../libs/HElib/Ctxt.o ../libs/HElib/EncryptedArray.o ../libs/HElib/replicate.o ../libs/HElib/hypercube.o ../libs/HElib/matching.o ../libs/HElib/powerful.o ../libs/HElib/BenesNetwork.o ../libs/HElib/permutations.o ../libs/HElib/PermNetwork.o ../libs/HElib/OptimizePermutations.o ../libs/HElib/eqtesting.o ../libs/HElib/polyEval.o


MONGOLDLIBS = -L/usr/include/lib -L/usr/local/lib -lntl $(GMP) -lm
MONGOINCDIR = -I/usr/local/include

ZQMLIB = -lzmq
ZQMDIR = -I./cppzmq

all: fhe.a


main: process.cpp fhe.a
	$(CC) $(CFLAGS) -o $@ $(MONGOINCDIR) $(ZQMDIR) $< fhe.a $(MONGOLDLIBS) $(ZQMLIB) -pthread -lmongoclient -lboost_thread -lboost_system -lboost_regex -lboost_filesystem -lboost_program_options

obj: $(OBJ)

%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c $<

fhe.a: $(OBJ)
	$(AR) $(ARFLAGS) fhe.a $(OBJ)

./%_x: %.cpp fhe.a
	$(CC) $(CFLAGS) -o $@ $< fhe.a $(LDLIBS)

clean:
	rm -f *.o *_x *_x.exe *.a core.*
	rm -rf *.dSYM

info:
	: HElib require NTL version 6.1.0 or higher
	: Compilation flags are 'CFLAGS=$(CFLAGS)'
	: If errors occur, try adding/removing '-std=c++11' in Makefile
	:
