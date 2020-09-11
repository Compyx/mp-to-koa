# mp-to-koa

## Convert Mutipaint to Koala

This is a simple tool to convert Multipaint .bin files to a native-C64 Koala format. The idea is when some creates a new file in Multipaint and saves it as .bin, we can just drop the file into a project dir and run `make` without having to run Multipaint to export to Advanced Art Studio.

### Prerequisites

* A C99-compliant compiler (Don't use MSVC, use msys2 for Windows)
* A fairly recent [64tass](https://sourceforge.net/projects/tass64/) if you want to run the ./run_test.sh script
* VICE if you want to run the ./run_test.sh script

### Building
`$ make`



