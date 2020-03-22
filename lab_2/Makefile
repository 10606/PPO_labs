.PHONY: test
CXX_FLAGS = --std=c++17 -lcurl -lpthread -lgtest
	
run:	build
	java -jar target/lab_2-1.0-SNAPSHOT-jar-with-dependencies.jar
	
build:
	mvn clean compile assembly:single

test: test.cpp.elf
	./test.cpp.elf

test.cpp.elf: test/test.cpp test/curl_wrap.cpp test/curl_wrap.h
	g++ $(CXX_FLAGS) test/test.cpp test/curl_wrap.cpp -o test.cpp.elf

mongo:
	mongod --config /etc/mongodb.conf &
