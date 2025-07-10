win-args = -Wl,-Bstatic -Iinclude/win/ -Linclude/win/ -lraylib -lpthread -Wl,-Bdynamic -lopengl32 -lgdi32 -lwinmm

ubuntu-args = -Wl,-Bstatic -Iinclude/linux/ -Linclude/linux/ -lraylib -Wl,-Bdynamic -lGL -lm -lpthread -lrt -lX11

screens = src/screens/game.c

etc = src/uielements.c src/settings.c

layouts = src/layouts/settingslayout.c src/layouts/settingslayout.h

build-all : ubuntu windows

ubuntu : build/plinko

windows : build/plinko.exe

build/plinko : src/plinko.c src/global.c $(screens) $(etc) $(layouts)
	mkdir -p build
	gcc src/plinko.c src/global.c $(screens) $(etc) $(layouts) $(ubuntu-args) -o build/plinko

build/plinko.exe : src/plinko.c src/global.c $(screens) $(etc) $(layouts)
	mkdir -p build
	x86_64-w64-mingw32-gcc src/plinko.c src/global.c $(screens) $(etc) $(layouts) $(win-args) -o build/plinko.exe

build-release : build/release/plinko.exe

build/release/plinko.exe : src/plinko.c src/global.c $(screens) $(etc) $(layouts)
	mkdir -p build/release
	x86_64-w64-mingw32-gcc src/plinko.c src/global.c $(screens) $(etc) $(layouts) $(win-args) -Wl,--subsystem,windows -o build/release/plinko.exe
	cp -r src/resources build/release/resources