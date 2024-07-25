#compile grades.c to grades.o
CC= gcc
CCFLAGS= -g -Wall -std=c99
CCLINK=$(CC)

#object and libary linkage
OBJS= grades.o
SO= libgrades.so

#delete the files created throughout compiling
RM= rm -rf grades.o libgrades.so

#shared objects libaries
$(SO): $(OBJS)
	$(CCLINK) $(CCFLAGS) -shared $(OBJS) -o $(SO) -llinked-list -L.

#compiling grades
grades.o: grades.c grades.h linked-list.h
	$(CC) $(CCFLAGS) -c -fpic grades.c

#delete files
clean:
	$(RM)
