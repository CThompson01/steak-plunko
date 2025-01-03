win-args = -Wl,-Bstatic -Iinclude/win/ -Linclude/win/ -lraylib -lpthread -Wl,-Bdynamic -lopengl32 -lgdi32 -lwinmm

ubuntu-args = -Wl,-Bstatic -Iinclude/linux/ -Linclude/linux/ -lraylib -Wl,-Bdynamic -lGL -lm -lpthread -lrt -lX11

screens = src/screens/game.c src/screens/options.c

build-all : ubuntu windows

ubuntu : build/plinko

windows : build/plinko.exe

build/plinko : src/plinko.c src/global.c $(screens)
	mkdir -p build
	gcc src/plinko.c src/global.c $(screens) $(ubuntu-args) -o build/plinko

build/plinko.exe : src/plinko.c src/global.c $(screens)
	mkdir -p build
	x86_64-w64-mingw32-gcc src/plinko.c src/global.c $(screens) $(win-args) -o build/plinko.exe

build-release : build/release/plinko.exe

build/release/plinko.exe : src/plinko.c src/global.c $(screens)
	mkdir -p build/release
	x86_64-w64-mingw32-gcc src/plinko.c src/global.c $(screens) $(win-args) -Wl,--subsystem,windows -o build/release/plinko.exe
	cp -r src/resources build/release/resources