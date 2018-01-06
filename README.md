### Hierarchical State Machine

### Description

> Portable cross-platform hierarchical state machine that uses a trick to 
> implement inheritance in C.

### Build tool requirements

Required:

Under GNU/Linux environment( Makefile is not tested on Windows or Mac OS )

1. GNU make
2. gcc
3. g++
4. perl
5. tput
6. git
7. doxygen
8. graphviz
9. cppcheck
10. cppUtest

```sh
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install make
sudo apt-get install build-essential
sudo apt-get install perl
sudo apt-get install libncurses5-dev libncursesw5-dev
sudo apt-get install git
sudo apt-get install doxygen
sudo apt-get install graphviz
sudo apt-get install cppcheck
#sudo apt-get install cpputest( build from Github )
```

Optional:

1. Eclipse (Neon 3) with CDT
2. Segger JLink drivers
3. Saleae logic analyser
4. picocom
5. STM32CubeMx
6. Segger Ozone

```sh
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install picocom
```

### How to build

To see available targets and help information:

```sh
make help
```

Build with ARM as target:

```sh
make all
```

Build with GNU/Linux as target:

```sh
make TARGET_SELECT=host all
```

Eclipse:

1. Open Eclipse with **proj/eclipse_workspace** as the workspace.
2. Import existing project in the solution.
3. Start developing...

### Maintainer

[Kanelis Ilias](mailto:hkanelhs@yahoo.gr)

### License

[GNU3](LICENSE) © Kanelis Ilias