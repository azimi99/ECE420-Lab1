all: matrixgen main serialtester

matrixgen:
	gcc matrixgen.c -o matrixgen

main:
	gcc main.c lab1_IO.c -o main -lpthread -Wall -lm

serialtester:
	gcc serialtester.c lab1_IO.c -o serialtester

clean:
	rm -f *.o main serialtester matrixgen data_input data_output