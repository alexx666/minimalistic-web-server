.DEFAULT_GOAL := compile

compile:
	@rm -rf build
	@mkdir build
	@gcc src/server.c -o build/server.out

run:
	@./build/server.out