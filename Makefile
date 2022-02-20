.DEFAULT_GOAL := compile

compile:
	@rm -rf bin
	@mkdir bin
	@gcc src/*.c -o bin/main.out

run:
	@./bin/main.out