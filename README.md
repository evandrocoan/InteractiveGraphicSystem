# Graphics Computing - Interactive Graphic System

An Interactive Graphic System with basic modeling forms using C++ GTKmm API.


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

### Running it

```
make run
```
![](https://user-images.githubusercontent.com/5332158/49179916-803e0100-f33a-11e8-9fc7-34f53b565206.png)


### Segmentation fault

In case of segmentation fault, you need to run the program from the command line with:
```
$ gdb binaries/main.exe
gdb $ run
gdb $ bt
```

See:

1. https://stackoverflow.com/questions/7848771/how-can-one-see-content-of-stack-with-gdb


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
pacman -Su    # Now, after opening a new terminal, run it again without y
pacman -S gcc
pacman -S base-devel
pacman -S pkg-config
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-boost
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gtkmm3
pacman -S mingw-w64-x86_64-toolchain
```
One liner:
`
pacman -S gcc base-devel pkg-config mingw-w64-x86_64-gtk3 mingw-w64-x86_64-boost mingw-w64-x86_64-gcc mingw-w64-x86_64-gtkmm3 mingw-w64-x86_64-toolchain
`

### IMPORTANT!

For Msys2, add to your `~/.bashrc`:
```
PKG_CONFIG_PATH="$PKG_CONFIG_PATH:/usr/lib/pkgconfig:/usr/share/pkgconfig:/lib/pkgconfig:/mingw64/lib/pkgconfig"
```
1. https://stackoverflow.com/questions/47713297/gtkmm-3-0-program-build-failed-msys2-netbeans-using-pkg-config

And fully install the https://github.com/tschoonj/GTK-for-Windows-Runtime-Environment-Installer/releases and add these value to path:
```
C:\Program Files\GTK3-Runtime Win64\bin
D:\msys64\usr\bin
D:\msys64\mingw64\bin
```


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



## License

See the file [LICENSE.txt](LICENSE.txt)

