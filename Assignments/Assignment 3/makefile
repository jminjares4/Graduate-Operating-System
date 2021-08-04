CC = gcc
lib = -lpthread
file = minjares_jesus_assignment3
exe = demo 
$(exe):	$(file).o
	$(CC) $(file).o $(lib) -o $(exe)
$(file).o:	$(file).c 
	$(CC) -c $(file).c 
clean:
	rm *.o *.dat $(exe) 