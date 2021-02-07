*** Tested on Ubuntu 20.10 ***

1. Install prerequisites

sudo apt install -y git build-essential cmake libboost-all-dev libgtest-dev && \
cd /usr/src/googletest && sudo mkdir build && cd build && \
sudo cmake .. && sudo make install

2. Download and build hash_server

cd ~ && \
git clone https://github.com/w5346c/hash_server.git && \
cd ./hash_server && \
./build.sh

3. Run unit tests

dbavykin@VirtualBox02:~/hash_server$ ./build/tests/hash_server_tests 

[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 5 tests from RequestResponderTest
[ RUN      ] RequestResponderTest.CompleteRequest
[       OK ] RequestResponderTest.CompleteRequest (0 ms)
[ RUN      ] RequestResponderTest.IncompleteRequest
[       OK ] RequestResponderTest.IncompleteRequest (0 ms)
[ RUN      ] RequestResponderTest.PartialRequests
[       OK ] RequestResponderTest.PartialRequests (0 ms)
[ RUN      ] RequestResponderTest.AllStringsEmpty
[       OK ] RequestResponderTest.AllStringsEmpty (0 ms)
[ RUN      ] RequestResponderTest.SomeStringsEmpty
[       OK ] RequestResponderTest.SomeStringsEmpty (0 ms)
[----------] 5 tests from RequestResponderTest (0 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 5 tests.


4. Run the server

dbavykin@VirtualBox02:~/hash_server$ ./build/app/hash_server -p 6666
Hash server started on port 6666

5. Open another terminal and send some requests to the server

dbavykin@VirtualBox02:~$ printf "String01\nString02\nString03\n" | nc -N localhost 6666
4a4e5d4e10815b53
b77ad2355ec30835
cd246a43141830b7