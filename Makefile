FLAGS = -lws2_32 -liphlpapi -static

build_ac:
	@ zig cc .\examples\ac.c $(FLAGS) -o .\builds\ac.exe

build_acc:
	@ zig cc .\examples\acc.c $(FLAGS) -o .\builds\acc.exe

build_ace:
	@ zig cc .\examples\ace.c $(FLAGS) -o .\builds\ace.exe

build_iracing:
	@ zig cc .\examples\iracing.c $(FLAGS) -o .\builds\iracing.exe

build_le_mans:
	@ zig cc .\examples\lmu.c $(FLAGS) -o .\builds\lmu.exe

clean:
	rm -r .\builds\...

