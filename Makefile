main: main.c
	gcc main.c -o main
	./main
.PHONY: clean
clean:
	rm -rf ./main

