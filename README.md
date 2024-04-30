# CSC 428 Blinds Project
The goal of this project was to create a device that was capable of controlling the pull cord blinds in my apartment's bedroom remotely without removing the current functionality.

## Final Product
![](https://github.com/ChristopherBrown200/CSC-428-Blinds/blob/main/Pictures/Fianl%20Product%20Casing.jpg)
![](https://github.com/ChristopherBrown200/CSC-428-Blinds/blob/main/Pictures/Final%20Product%20No%20Casing.jpg)

## Parts Used
Part | |Price
| --- | ---: | ---:|
[ESP 32](https://www.amazon.com/dp/B0B764963C?psc=1&ref=ppx_yo2ov_dt_b_product_details) | |$15.88
[Rotary Encoder](https://www.amazon.com/dp/B0CF57FT4J?psc=1&ref=ppx_yo2ov_dt_b_product_details) | | $7.99
[Continuous Servo Motor](https://www.amazon.com/dp/B08JCVLSCK?psc=1&ref=ppx_yo2ov_dt_b_product_details) | | $29.99
[1.54" eInk Display](https://www.adafruit.com/product/4196) | | $24.99
[M2 Screws](https://www.amazon.com/dp/B07W5J19Y5?psc=1&ref=ppx_yo2ov_dt_b_product_details) | | $12.99
[Solderable Breadboard](https://www.digikey.com/en/products/detail/digikey/DKS-SOLDERBREAD-02/15970925) | | $1.10
Black 3D Printing Filament (Fab Lab) | | -
3M Screws (Fab Lab) | | -
Wire (Fab Lab) | | -
| |Total: |$92.94

## Arduino Libraries
### [Adafruit ThinkInk](https://github.com/adafruit/Adafruit_EPD/tree/master)
This library was used to control the eInk display via SPI (Serial Peripheral Interface).
### Standard Library
The standard library was used for its inclusion of strings.
### [ESP32 Servo](https://www.arduino.cc/reference/en/libraries/esp32servo/)
This library was used to control the servo motor via PWM.
### [WiFi](https://www.arduino.cc/reference/en/libraries/wifi/)
This library was used to connect to a LAN and set the ESP up as a web server

## [Code](https://github.com/ChristopherBrown200/CSC-428-Blinds)
The code for this project can be found in this GitHub repository.


## Wiring Diagram
![](https://github.com/ChristopherBrown200/CSC-428-Blinds/blob/main/Pictures/Wiring.jpg)

## 3D Prints 
The `.stl` files used for this project can be found [here](https://github.com/ChristopherBrown200/CSC-428-Blinds/tree/main/3D%20Models), with thanks to [Tenthrow](https://www.printables.com/@Tenthrow_13787) on Printables for his knob design.
