main: src/networking.c src/http.c src/main.c
	gcc src/networking.c src/http.c src/main.c src/structs.c -o bin/main -g
