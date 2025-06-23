all:	build

build:
	# cxxopts's entire code is in the header, so no library is neeed
	/opt/homebrew/Cellar/gcc/15.1.0/bin/g++-15	\
		-I/opt/homebrew/Cellar/cxxopts/3.3.1/include/	\
		-I/opt/homebrew/Cellar/boost/1.88.0/include	\
		-L /opt/homebrew/Cellar/boost/1.88.0/lib	\
		-l boost_filesystem \
		helloworld.cpp -o helloworld -g
init:
	# for a new machine	

fmt:
	clang-fmt helloworld.cpp

run:
	find /tmp/ | ./helloworld