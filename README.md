# person_tracker
A program in c++ that uses yolov3 or yolov4 to detect persons from video file or web camera and then tracks the selected person that is clicked on, or tracks the object that is marked by click and drag.
Includes some external libs
- Boost (for nice file handling stuff in c++)
- fmt (for modern string handling)
- opencv (for graphics)

### Requirements
The following packages needs to be installed
- libfmt-dev (for modern string handling with fmt)
- libboost-all-dev (for Boost)
- OpenCV (see below)

```sudo apt install -y build-essential libboost-all-dev cmake libfmt-dev```

### How to install OpenCV for Ubuntu 19.10/18.04
``` bash
$ cd deb_packages
# Open CV 4.5.0
$ sudo apt install ./opencv_1.0.1_amd64.deb
```

If you want to build OpenCV the old way [see](/docs/opencv.md)

### Yolo
For yolo detection to work you need some yolo files.
``` bash
$ cd <your project folder>
$ mkdir -p resources/yolo
$ cd resources/yolo
$ touch coco.names
```
Copy the text found in the following url into your coco.names file: ```https://github.com/pjreddie/darknet/blob/master/data/coco.names```

For YoloV4:
``` bash
$ wget https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v3_optimal/yolov4.weights
$ wget https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4.cfg
```
For YoloV3:
``` bash
$ wget https://pjreddie.com/media/files/yolov3.weights
```
The yolov3.cfg has to be opened by ```https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg```. Then mark every line and copy them. Then paste them into an new file that you call yolov3.cfg.

## Compiling and running
### Create build directory:
``` bash
$ mkdir build
```

### Include paths for OpenCV
You need to run the following in your terminal before building. Alternatively you can enter
the lines at the end of your .bashrc file (that will be executed every time you open your terminal)  
``` bash
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/lib/
```

### Compile this with:
``` bash
$ cd build
$ cmake .. -DCMAKE_PREFIX_PATH=/opt/lib/cmake/opencv4/
$ cd ..
$ make person_tracker -C build/
```
Note: this can no longer be built from terminal by g++ in an easy way. And tasks.json is not updated so it cannot be built from VsCode Run -> 'Run without debugging' or Terminal -> 'Run Build Task'. You need to use cmake and make as described above.

### Execute this with:
./build/person_tracker

