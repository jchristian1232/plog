plog: assemble.cpp
	g++ assemble.cpp -o assemble
run: assemble
	./assemble
	./plog.sh