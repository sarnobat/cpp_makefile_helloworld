build:
	# 2) Compile (clang++ or g++)
	clang++ -std=gnu++20 -O2 hello_pipeline.cpp -o hello_pipeline

init:
	# 1) Fetch the header
	curl -L -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

run:

	# 3) Run
	echo "Now run:		make test_client"
	./hello_pipeline &
	open 'http://localhost:8080/'
	sleep 5
	
test:
	./hello_pipeline &
	sleep 1
	curl -i http://localhost:8080/
	sleep 1
	# HTTP/1.1 200
	# Content-Type: application/json; charset=utf-8
	# ...
	# {"message":"Hello, World!"}
	
	
	curl -i http://localhost:8080/health
	sleep 5
	# HTTP/1.1 200
	# OK
	
test_client_error:

	curl -i -X POST http://localhost:8080/
	# HTTP/1.1 405
	# {"error":"Method Not Allowed"}
	