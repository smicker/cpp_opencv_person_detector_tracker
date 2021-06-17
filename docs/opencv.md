# HowTo build OpenCV and libmosquitto and package it as deb packages

## Build OpenCV with QT for KDE/Lxde and other
``` bash
$ sudo apt install -y qtbase5-dev
$ git clone https://github.com/opencv/opencv.git
$ git clone https://github.com/opencv/opencv_contrib.git
$ cd ~/opencv
$ git checkout tags/4.5.0 -b temp_branch
$ cd ~/opencv_contrib
$ git checkout tags/4.5.0 -b temp_branch
$ cd ~/opencv
$ mkdir build
$ cd build
```

In build run:

``` bash
$ cmake -D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=$HOME/myopencv/opt .. \
-DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules/ ../ \
-DWITH_OPENGL=ON \
-DWITH_QT=ON
```

``` bash
$ cd ..
$ make -C build
$ make install -C build
```

## How to make deb package
``` bash
$ mkdir -p $HOME/myopencv/DEBIAN
```
Go to $HOME/myopencv and create a file
``` bash
echo -e "Package: opencv-tracking
Version: 1.0.0
Maintainer: Smicker <smicker@gmail,com>
Architecture: all
Depends: qt5-default, qtbase5-dev
Description: Opencv with some contrib libs built with qt
" >> ./DEBIAN/control
```

Your file structure should look something like this.
``` bash
./myopencv/
├── DEBIAN
│   └── control
└── opt
    └── opencv
        ├── bin
        ├── include
        ├── lib
        └── share
```

### build
from $HOME
``` bash
$ dpkg -b ./myopencv ./opencv_1.0.0-0_amd64.deb
```

### install
from $HOME
``` bash
$ sudo apt install ./opencv_1.0.0-0_amd64.deb
```


## Build mqtt
``` bash
$ git clone https://github.com/ActionSportFilming/mosquitto.git
$ cd ~/mosquitto
$ mkdir build
$ cd build
```

In build run:

``` bash
$ cmake -D CMAKE_BUILD_TYPE=Release WITH_BUNDLED_DEPS=no \
-D CMAKE_INSTALL_PREFIX=$HOME/mymqtt/opt ..

```

``` bash
$ cd ..
$ make -C build
$ make install -C build
```
## How to make deb package
``` bash
$ mkdir -p $HOME/mymqtt/DEBIAN
```

Go to $HOME/mymqtt and create a file
``` bash
echo -e "Package: libmqtt
Version: 1.0.0
Maintainer: Smicker <smicker@gmail,com>
Architecture: all
Description: Latest libmosquitto
" >> ./DEBIAN/control
```

### build
from $HOME
``` bash
$ dpkg -b ./mymqtt ./libmqtt_1.0.0-0_amd64.deb
```

### install
from $HOME
``` bash
$ sudo apt install ./libmqtt_1.0.0-0_amd64.deb
```
