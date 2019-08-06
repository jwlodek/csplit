### Installation

Because csplit is a single-header library, all you need to do to use it for your projects is to copy the `csplit.h` file into your project location, and include it at the top of your source file in which you would like to use it. Clone this repository with:
```
git clone https://github.com/jwlodek/csplit
```
Then copy `csplit.h` into your repository, add the
```C
#include "csplit.h"
```
line at the top of your code, and, if necessary, update your project's include path. Once you have compiled your project the `csplit.h` file is no longer needed as it has been compiled into your binary.

### Running Unit Tests

Unit testing for `csplit` is done with the help of the [Criterion](https://github.com/Snaipe/Criterion) library. To simplify setup, scripts have been added to the `tests/` directory that setup this libarary, and run the tests. Simply run:
```
cd tests
./initUnitTests.sh
```
to initialize the [Criterion](https://github.com/Snaipe/Criterion) Library, and then
```
./runUnitTests.sh
```
to run the tests. Note that this only works on linux systems, and on windows you must set up the unit testing environment manually. To do this, you will have to download a Criterion binary release, then add it to your library/include path for Visual Studio/MinGW as appropriate. Then you will need to build the unit tests either with gcc + make via the included `Makefile`, or with Visual Studio. Once the test file has been built, it can be executed by running:
```
./csplit_core_tests
```

### Building Examples

In order to build the examples included with csplit, you will need gcc and make installed on your system. Run the following commands:
```
cd examples
make clean all
```
if you wish to build the examples in release mode. Alternatively,
```
cd examples
make clean debug
```
will compile the examples with gdb debugging enabled.