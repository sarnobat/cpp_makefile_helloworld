all:	build

build:
	/opt/homebrew/Cellar/gcc/15.1.0/bin/g++-15  -I/opt/homebrew/Cellar/boost/1.88.0/include -L /opt/homebrew/Cellar/boost/1.88.0/lib -l boost_filesystem helloworld.cpp -o helloworld -g
init:
	# for a new machine	
	
run:
	find /tmp/ | ./helloworld