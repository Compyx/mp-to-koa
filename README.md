# mp-to-koa

## Convert Mutipaint to Koala

This is a simple tool to convert Multipaint .bin files to a native-C64 Koala format. The idea is when some creates a new file in Multipaint and saves it as .bin, we can just drop the file into a project dir and run `make` without having to run Multipaint to export to Advanced Art Studio.

### Prerequisites

* A C99-compliant compiler (Don't use MSVC, use msys2 for Windows)
* A fairly recent [64tass](https://sourceforge.net/projects/tass64/) if you want to run the ./run_test.sh script
* VICE if you want to run the ./run_test.sh script

### Building
~~~
$ make
~~~

### Testing

The easiest way to test is just running `./run_test.sh`. This will run the tool on `data/focus-3d.bin`, generate `focus-3d.koa` and generate a .prg which is run via x64sc.
This requires a fairly modern `64tass` and `x64sc-gtk3` in $PATH, which I suppose most people won't have, so the `run_test.sh` script accepts an argument to set the proper VICE binary.


Another way is using the Makefile:

```sh
$ make KOALA='\"../data/focus-3d.koa\"' show_koala.prg
```
(Yes, all the weird quotes are important)

Run with:
```sh
$ x64sc ./show_koala.prg
```




