all:
	bash -c "g++ Method1.cpp -o method1 -std=c++11 `pkg-config --cflags --libs opencv`"
	bash -c "g++ Method2.cpp -o method2 -std=c++11 `pkg-config --cflags --libs opencv`"
	bash -c "g++ Method3.cpp -o method3 -lpthread `pkg-config --cflags --libs opencv`"
	bash -c "g++ Method3.cpp -o method4 -lpthread `pkg-config --cflags --libs opencv`"
