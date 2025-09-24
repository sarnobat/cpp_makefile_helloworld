all:	build	test	run

build:
	g++-15	helloworld.cpp -o helloworld -g

init:
	# for a new machine	

fmt:
	clang-fmt helloworld.cpp

test:
	ls -lh ../../c/1_hello_world/helloworld ../../cpp/01_helloworld/helloworld

run:
	./helloworld