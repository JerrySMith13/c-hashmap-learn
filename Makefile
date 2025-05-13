main: main.c
	gcc -Wall -Werror -Wno-missing-braces main.c -o main

	./main
.PHONY: clean
clean:
	rm -rf ./main

