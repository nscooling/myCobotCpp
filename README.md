# myCobotCpp


## Dependences

* Boost `<boost/asio.hpp>`
* GoogleTest

## Build

```
cmake -S . -B build
cmake --build build
```


## Build for test

```
$ cmake -S . -B build -DBUILD_TESTING=ON
$ cmake --build build -- test
```

or

```
$ ctest --test-dir build --output-on-failure
```