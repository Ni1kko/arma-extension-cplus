#
	# Name: compile_linux.sh
    # Author: Nikko
    # Description: compiles the extension for linux
#


# (32 bit)
#g++ -shared -fPIC -std=c++11 ../dllmain.cpp -o Example.so
#g++ -shared -fPIC -std=c++17 ../dllmain.cpp -o Example.so
g++ -shared -fPIC -std=c++2a ../dllmain.cpp -o Example.so


# (64 bit)
#g++ -shared -fPIC -m64 -std=c++11 ../dllmain.cpp -o Example_x64.so
#g++ -shared -fPIC -m64 -std=c++17 ../dllmain.cpp -o Example_x64.so
g++ -shared -fPIC -m64 -std=c++2a ../dllmain.cpp -o Example_x64.so