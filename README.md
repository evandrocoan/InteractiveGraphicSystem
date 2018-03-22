# Trabalho01CG

1. Karla Aparecida Justen
1. Evandro Coan

Trabalho de Computação Gráfica - UFSC - 2018.2


### Download

To download it, you need to use the `git clone` with the `--recursive` option:
```
git clone --recursive https://github.com/evandrocoan/Trabalho01CG
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

And fully install the https://github.com/tschoonj/GTK-for-Windows-Runtime-Environment-Installer/releases and add these value to path:
```
C:\Program Files\GTK3-Runtime Win64\bin
D:\msys64\usr\bin
D:\msys64\mingw64\bin
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
$ make run
make binaries/main -j4
make[1]: Entering directory '/home/user/graphics_project'
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/addobject.o src/addobject.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/addtransformation.o src/addtransformation.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/coordinate.o src/coordinate.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/debugger.o src/debugger.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/displayfile.o src/displayfile.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/drawableobject.o src/drawableobject.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/line.o src/line.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/main.o src/main.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/mainwindow.o src/mainwindow.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/point.o src/point.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/polygon.o src/polygon.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/transformation.o src/transformation.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/viewport.o src/viewport.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -I. -c -o objects/viewwindow.o src/viewwindow.cpp `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
g++ -O0 -g -Wall -std=c++11 -fstack-protector -o binaries/main.exe objects/addobject.o objects/addtransformation.o objects/coordinate.o objects/debugger.o objects/displayfile.o objects/drawableobject.o objects/line.o objects/main.o objects/mainwindow.o objects/point.o objects/polygon.o objects/transformation.o objects/viewport.o objects/viewwindow.o `pkg-config --cflags --libs glibmm-2.4 giomm-2.4 gtkmm-3.0`
make[1]: Leaving directory '/home/user/graphics_project'
Took '0:00:02:00' seconds to run the script, 00:27:20.
./binaries/main
00:27:21:018:927 0.000e+000 0.000e+000 src/main.cpp|main:17 Starting the main application...
```


Links de Docmentação do GTKmm:
- https://developer.gnome.org/gtkmm-tutorial/stable/
- https://developer.gnome.org/gtkmm/stable/group__Widgets.html
- https://developer.gnome.org/gtkmm/stable/group__gtkmmEnums.html#gga83727a1b6fed51566dfd5c8e58890dbaa7b88e2329efd6367522358da92a3a736


### Download GTK Sources


You can access the page: https://www.gtkmm.org/en/git.html

And run the commands:
```
git clone git://git.gnome.org/gtk+
git clone git://git.gnome.org/gtkmm
git clone git://git.gnome.org/glibmm

cd gtk+; checkout 3.22.28; cd -
cd gtkmm; checkout 3.22.0; cd -
cd glibmm; checkout 2.54.1; cd -
```

To know you installed versions of these libraries, you can run the commands:
```
pkg-config --modversion gtk+-3.0
pkg-config --modversion gtkmm-3.0
pkg-config --modversion glibmm-2.4
```


> ### Installing Glade
>
> 1. http://grbd.github.io/posts/2016/01/29/gtksharp-part-2-setting-up-glade/
>
> Next we're going to install glade, make sure you have a MSYS console open
>
> To search for the glade packages:
> ```
> pacman -Ss glade
> ```
> To install for 64Bit Windows:
> ```
> pacman -S mingw64/mingw-w64-x86_64-glade
> ```
>
> We can now run glade via the exe `C:\msys64\mingw64\bin\glade.exe`


> ### Glade creates XML instead of C code
>
> 1. https://ubuntuforums.org/showthread.php?t=1408536
>
> The older version of glade would write C/C++ code for the GUI. You really don't want that. I use Glade 3.6.7 and GtkBuilder. I mainly write using C++ so the API I am using is Gtkmm. The GtkBuilder class is called Gtk::Builder. You can access your glade XML file and interact with the widgets using the Gtk::Builder classes. I don't think you will find any speed issue with the GUI being in an XML file. The entire file can be loaded and parsed at startup. Also many programs use "outside" files for resources and configuration. I don't think that is a problem. In \*nix these files can be placed in system directories that are not accessible by the normal user.
>
> However if you wish to hardcode the GUI then that is entirely possible. You can't use glade to do that. I don't know of any other program that will allow you to create a Gtk GUI and output the code for you.
>
> If you really want something like that then your best bet is to get an older version of glade and try that. Caveat: Gtk has gone through many changes and you won't be able to take advantage of the newer features. And some features have been deprecated.




