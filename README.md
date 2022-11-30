cpp_makefile_helloworld
=======================

C++ hello world with Makefile

### List all native libraries
```
pkg-config --list-all
```


### generate include statement
```
pkg-config --libs fuse xmu
```
```
-L/usr/local/homebrew/Cellar/libxmu/1.1.3/lib -L/usr/local/homebrew/Cellar/libxt/1.2.1/lib -L/Volumes/numerous/usr/local/homebrew/Cellar/libx11/1.8.1/lib -L/usr/local/lib -lfuse -pthread -lXmu -lXt -lX11
````
