
# Setting up the Software

# Arduino Code
The Arduino sketch in this project uses [PlatformIO](http://platformio.org/), which provides automatic library fetching and cross-compiling for a bunch of different systems. If you're not satisfied by the stock Arduino IDE, I strongly suggest you give PlatformIO a try. The Atom integration is quite stable and easy to work with.

## Using PlatformIO to build the code

[![PlatformIO](http://cdn.platformio.org/images/platformio-logo.17fdc3bc.png)](http://platformio.org/)

Most of the hard work is done already.

1. Open your project folder in Atom or whatever IDE you choose to use with PlatformIO.

2. Initialise the project, which involves choosing your target platforms. Choose `Arduino Leonardo`. **This step may take a while if you have not initialised an Arduino project before, as PlatformIO will need to download the build tools for Arduino.**

3. Open the `platformio.ini` file and hit the `build` button to start compiling the code. PlatformIO will grab the necessary libraries automatically.

4. If the build fails from a 'File in use', 'File exists' or 'Access denied' error, try building again. These errors are usually transient and usually go away after another build attempt.

5. If the build passes, hit the `Upload` button to flash the code onto the Arduino.

## Using Arduino IDE to build the code

[![Arduino](https://www.arduino.cc/en/pub/skins/arduinoWide/img/ArduinoLogo_loop-01.svg)](https://www.arduino.cc/en/Main/Software)

The Arduino IDE is fine for building and uploading the code, but you'll need to grab the libraries yourself, which you can do using their inbuilt library manager.

1. Copy the libraries in the project's `lib` folder and paste them in your user library folder.

2.  In addition to those libraries, you'll need to find the following in the integrated library manager:
    - SimpleTimer
    - RTClib

3. Build and upload as per usual
