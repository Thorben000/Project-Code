build: clean 
	g++ -g $(wildcard *.cpp) -o FluidSim -O0

run: build
	./FluidSim

clean:
	rm -f ./FluidSim