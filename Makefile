build: clean 
	g++ $(wildcard *.cpp) -o FluidSim -O3

build_debug: clean 
	g++ -g $(wildcard *.cpp) -o FluidSim -O0

run: build
	./FluidSim

clean:
	rm -f ./FluidSim