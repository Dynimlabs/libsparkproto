`libsparkproto` is a library which provides APIs for controlling and acquiring output data from Dynim's Spark cameras.

-----------------------------------------------------
**Main APIs:**

* `libspark::protocol::DeviceEnumeration` : discover and acquire a list of Spark cameras present in the network. Basic information of available Spark cameras will be stored in `libspark::protocol::DeviceInfo`.

* `libspark::protocol::DeviceInfo` : provides the access to basic camera information and specifications and the current status of the device.

* `libspark::protocol::DeviceParamConfigure` : communicates with Spark cameras to read/write device and sensor parameters (e.g. gain, exposure, calibration data).

* `libspark::protocol::ImageStreamProtocol` : allows users to set up a synchronous stream of images from Spark camera.

* `libspark::protocol::ImageStream` : allows users to set up a synchronous stream of images from Spark camera. It's an alias of `libspark::protocol::ImageStreamProtocol`.

* `libspark::protocol::AsyncImageStream` : a thread-safe API to communicate with Spark to acquire a stream of images in asynchronous mode. It’s also a wrapper of `libspark::protocol::ImageStreamProtocol`.


-----------------------------------------------------
**Examples:**

| Source file | Description |
| ----------- | ----------- |
| [devicediscovery_example.cc](devicediscovery__example_8cc_source.html) | Demonstrate how to acquire a list of available devices in the network with `libspark::protocol::DeviceEnumeration` |
| [asyncimagestream_example.cc](asyncimagestream__example_8cc_source.html) | Example of creating an asynchronous stream with `libspark::protocol::AsyncImageStream` |
| [imagestream_example.cc](imagestream__example_8cc_source.html) | Demonstration of synchronous stream with `libspark::protocol::ImageStream` |
| [imagestream_cv_example.cc](imagestream__cv__example_8cc_source.html) | Demonstration of synchronous stream with `libspark::protocol::ImageStream`. The video stream are displayed using OpenCV |
| [deviceparamconfigure_example.cc](deviceparamconfigure__example_8cc_source.html) | Demonstrate how to read/write parameters of Spark cameras with `libspark::protocol::DeviceParamConfigure` |
| [gainexposurecontrol_example.cc](gainexposurecontrol__example_8cc_source.html) | Example of manually setting gain and exposure while streaming with `libspark::protocol::DeviceEnumeration`|
| [sparkconfigure.cc](sparkconfigure_8cc_source.html) | A simple console tool for viewing and setting the device parameters|


-----------------------------------------------------
**How to compile:**

Require:
- Ubuntu 18 (tested)
- Depend packages: `sudo apt install libprotobuf-dev`

```
mkdir build
cd build
cmake -DENABLE_LOGGING=ON \
	    -DENABLE_EXAMPLES=ON \
		-DCMAKE_INSTALL_PREFIX=install ../

make -j$(($nproc)-1) && make install

```


-----------------------------------------------------
**How to use in your project**

- Include header `#include <libsparkproto/sparkproto.h>`
- Link library: `-lsparkproto -L/path/to/libsparkproto-installeddir/ -I/path/to/libsparkproto-installdir/`

