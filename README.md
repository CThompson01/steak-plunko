# Plinko

## Description

Plinko utilizes Raylib for C. It is a recreation of those Plinko gambling games in the gambling site ads that are everywhere. 

## Building

Building is a little different depending on your OS. Find your OS below, and follow the instructions.

### Linux

Currently, automated setup will only work on Ubuntu. 

Open terminal and run the following commands.

```
chmod +x setup.sh
./setup.sh
```

This should allow `setup.sh` to be executable, then automatically install all of the required dependencies.

After the setup is finished running, you should be able to build the project using make. If you've never used make, simply navigate to the `steak-plunko/` directory in your terminal and run the following command.

```
make
```

If you only want to build for a specific operating system, the following commands do so for the specified OS.

```
make ubuntu
make windows
```

Once the build is finished, you must copy the `resources/` folder found in the `src/` directory into the `build/` directory.

The finished build can be found within the `build/` directory. To run the program, you can execute the application from the command line with `./plinko` when in the build directory. You may need to mess with permissions in order to allow it to be executed using chmod. The following command should work when run within the build directory.

```
chmod +x plinko
```

### Windows

TODO: Add Windows build instructions.

## TODO

- [ ] Have makefile move necessary files for Windows builds into the build directory and split them based on OS.
- [ ] Improve makefile as it is a bit of a mess currently.
- [ ] Automated build setup for other operating systems.
- [ ] Improve memory usage
- [ ] Enforce one naming convention