dependencyDiscovererSingleThread: dependencyDiscovererSingleThread.cpp
	clang++ -Wall -Werror -std=c++17 -o dependencyDiscovererSingleThread dependencyDiscovererSingleThread.cpp -lpthread

dependencyDiscoverer: dependencyDiscoverer.cpp
	clang++ -Wall -Werror -std=c++17 -o dependencyDiscoverer dependencyDiscoverer.cpp -lpthread

sequential: sequential.cpp
	clang++ -Wall -Werror -std=c++17 -o sequential sequential.cpp -lpthread

clean:
	rm -f *.o dependencyDiscoverer *~
	rm -f *.o dependencyDiscovererSingleThread *~
	rm -f *.o sequential*~