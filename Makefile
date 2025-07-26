build_ac:
	@ zig cc examples/ac.c -o builds/ac/example.exe

build_acc:
	@ zig cc examples/acc.c -o builds/acc/example.exe

build_ace:
	@ zig cc examples/ace.c -o builds/ace/example.exe

build_iracing:
	@ zig cc examples/iracing.c -o builds/iracing/example.exe

build_le_mans:
	@ zig cc examples/lemans_ultimate.c -o builds/lemans_ultimate/example.exe

clean:
	rm -r builds/...

