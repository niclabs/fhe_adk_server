# Copyright (C) 2014,2015 NIC Chile Research Labs.
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
CFLAGS =  -I./cppzmq


main:  
	$(CC) $(CFLAGS) -o $@ querier.cpp -lzmq



clean:
	rm -f *.o *_x *_x.exe *.a core.*
	rm -rf *.dSYM

info:
	: requires zeroQM 
	: Compilation flags are 'CFLAGS=$(CFLAGS)'
	:
