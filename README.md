# Trabalho01CG

1. Karla Aparecida Justen
1. Evandro Coan

Trabalho de Computação Gráfica - UFSC - 2018.2


### Download

To download it, you need to use the `git clone` with the `--recursive` option:
```
git clone --recursive repository_url
```

If you had already cloned it without the `--recursive` option, you need to run the command:
```
git submodule init
git submodule update
```


### Required dependencies for MSYS2 on Windows

To run `GTK+` and `GTKmm` on Windows you will need `MSYS2`,
with a Unix-Shell complaint terminal.
You can install all of them on the following links respectively:
1. http://www.msys2.org/
1. https://www.gtk.org/download/windows.php
1. https://wiki.gnome.org/Projects/gtkmm/MSWindows

These are the commands you need to run,
to install everything on Windows MSYS2 Unit-Shell terminal:
```
pacman -Syu   # After complete, it will ask you to kill the terminal window with your task manager
pacman -Su    # Now run it again without y
pacman -S pkg-config
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-boost
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gtkmm3
pacman -S mingw-w64-x86_64-toolchain base-devel
```


Para configurar a IDE Eclipse 3.18.3 ou Neon.3 Release (4.6.3) para incluir GTK:

#### Para C:

1. Clica com o botão direito no projeto, e clica em Propriedades.
Expande as opções do C/C++ General e vai em "Paths and Symbols".
clica no GNU C e adiciona: "/usr/include/gtk-3.0" (sem as aspas)

2. Ainda nas propriedades do projeto, expande as opções do C/C++ Build e vai em "Build Variables", clica em "Add".
Agora, coloca "Variable Name": gtkc (ou qualquer outro nome que queiras, mas vais usar depois), e em "Value" coloca o comando: `pkg-config --cflags --libs gtk+-3.0` (copia e cola o comando aqui, inclusive com os acentos).

3. Ainda nas opções do C/C++ Build, vai em "Settings", e em "GCC C Compiler" e em "GCC C Linker" adiciona em "Expert settings - Command line pattern o "${gtkc} por último.


#### Para C++:

1. Clica com o botão direito no projeto, e clica em Propriedades.
Expande as opções do C/C++ General e vai em "Paths and Symbols".
clica no GNU C++ e adiciona: "/usr/include/gtk-3.0" (sem as aspas) e adiciona "/usr/include/gtkmm-3.0".

1. Ainda nas propriedades do projeto, expande as opções do C/C++ Build e vai em "Build Variables", clica em "Add".
Agora, coloca "Variable Name": gtkcmm (ou qualquer outro nome que queiras, mas vais usar depois), e em "Value" coloca o comando:  `pkg-config --cflags --libs gtk+-3.0 gtkmm-3.0` (copia e cola o comando aqui, inclusive com os acentos).

1. Ainda nas opções do C/C++ Build, vai em "Settings", e em "GCC C++ Compiler" e em "GCC C++ Linker" adiciona em "Expert settings - Command line pattern"  o "${gtkcmm}" (sem aspas) por último.


### Build from Command Line

To build it on `Windows` or `Linux`, just run:
```
make
```

Build output example:
```
$ make all run
if [ -d resources ]; then cp resources/* binaries/; fi;
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/addobjectwindow.o src/addobjectwindow.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/displayfile.o src/displayfile.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/drawoptionsbox.o src/drawoptionsbox.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/line.o src/line.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/main.o src/main.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/mainwindow.o src/mainwindow.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/point.o src/point.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/polygon.o src/polygon.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/viewport.o src/viewport.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -Iincludes -I/usr/local/include -c -o objects/viewwindow.o src/viewwindow.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g  -o binaries/main objects/addobjectwindow.o objects/displayfile.o objects/drawoptionsbox.o objects/line.o objects/main.o objects/mainwindow.o objects/point.o objects/polygon.o objects/viewport.o objects/viewwindow.o `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
./binaries/main
```


Links de Docmentação do GTKmm:
- https://developer.gnome.org/gtkmm/stable/group__gtkmmEnums.html#gga83727a1b6fed51566dfd5c8e58890dbaa7b88e2329efd6367522358da92a3a736
- https://developer.gnome.org/gtkmm/stable/group__Widgets.html

