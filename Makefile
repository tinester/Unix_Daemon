# Zach Eagan
# zse150030@utdallas.edu
# CS 3376.002

CXX = g++
CXXFLAGS = -Wall -I /home/012/z/zs/zse150030/include
LDFLAGS =
LDLIBS =

OBJS = prog5.o runNotDaemon.o sigHandler.o runAsDaemon.o

all : prog5

.PHONY: clean
clean:
	rm $(OBJS) *~ *log *pid prog5

prog5 : $(OBJS)
	$(CXX) $(CXXFLAGS) -o prog5 $(OBJS) /home/012/z/zs/zse150030/lib/librudeconfig.a


-include $(OBJS:%.o=%.P)
