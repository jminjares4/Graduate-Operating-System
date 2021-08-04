#Author:	Jesus Minjares BSEE
CC = gcc
Lib = 
file = minjares_jesus_assignment_1
Exe = demo 
$(Exe):	$(file).o
	$(CC) $(file).o -o $(Exe) $(Lib)
$(file).o:	$(file).c 
	$(CC) -c $(file).c 
clean:
	rm *.o $(Exe)