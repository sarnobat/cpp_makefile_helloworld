build:
	# 2) Compile (clang++ or g++)
	clang++ -std=gnu++20 -O2 hello_pipeline.cpp -o hello_pipeline

init:
	# 1) Fetch the header
	curl -L -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

test:

	# 3) Run
	echo "Now run:		make test_client"
	./hello_pipeline &
	
test_client:
	curl -i http://localhost:8080/
	# HTTP/1.1 200
	# Content-Type: application/json; charset=utf-8
	# ...
	# {"message":"Hello, World!"}
	
	curl -i -X POST http://localhost:8080/
	# HTTP/1.1 405
	# {"error":"Method Not Allowed"}
	
	curl -i http://localhost:8080/health
	# HTTP/1.1 200
	# OK
	
