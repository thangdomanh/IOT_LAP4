# IoT Lab 4 - ESP32, DHT11, and Adafruit IO

## Assignment Overview

This project is an IoT course assignment for Embedded Systems and IoT students. The lab demonstrates how an ESP32 can connect to Wi-Fi, communicate with the Adafruit IO cloud platform, publish environmental sensor data, and receive a remote control command for an on-board LED.

Students use this assignment to practice:

- Configuring an ESP32 Wi-Fi connection.
- Connecting an embedded device to Adafruit IO.
- Reading temperature and humidity from a DHT11 sensor.
- Sending sensor values to cloud feeds.
- Receiving cloud feed data and controlling GPIO output.
- Monitoring device behavior through the serial monitor.

## Hardware Requirements

- ESP32 development board
- DHT11 temperature and humidity sensor
- Jumper wires
- Breadboard
- USB cable for programming and serial monitoring

## Software Requirements

- Arduino IDE
- ESP32 board support package
- Adafruit IO Arduino library
- Adafruit IO WiFi library
- DHT sensor library

## Code Content

The main sketch is `Lab_4_IoT.ino`.

### Adafruit IO and Wi-Fi Configuration

The code defines the Adafruit IO account information and Wi-Fi credentials:

```cpp
#define IO_USERNAME  "********"
#define IO_KEY       "************"

#define WIFI_SSID "************"
#define WIFI_PASS "****"
```

The Wi-Fi password/IO_USERNAME/IO_KEY/WIFI_PASS is intentionally written as `****` in this repository. Students must replace it with the correct local Wi-Fi password before uploading the sketch to the ESP32.

### Cloud Feeds

The project uses three Adafruit IO feeds:

- `button`: receives remote button data from Adafruit IO.
- `temp`: stores temperature readings from the DHT11 sensor.
- `humi`: stores humidity readings from the DHT11 sensor.

### Sensor Reading

The DHT11 sensor is connected to GPIO 27. The `sendSensor()` function reads humidity and temperature values, prints them to the serial monitor, and uploads them to the `temp` and `humi` feeds.

Sensor data is sent every 20 seconds using a `millis()` timer.

### LED Control

The on-board LED is connected to GPIO 2. The `handleMessage()` function receives data from the `button` feed and updates the LED state:

- `HIGH`: turn the LED on.
- `LOW`: turn the LED off.

## How the Program Works

1. The ESP32 starts the serial monitor at 115200 baud.
2. The ESP32 connects to Wi-Fi and Adafruit IO.
3. The program subscribes to the `button` feed.
4. The DHT11 sensor begins reading temperature and humidity.
5. Every 20 seconds, the ESP32 sends sensor values to Adafruit IO.
6. When the `button` feed changes, the ESP32 updates the on-board LED.

## Student Tasks

- Update the Wi-Fi password before uploading the sketch.
- Verify the Adafruit IO username and key.
- Create the `button`, `temp`, and `humi` feeds in Adafruit IO.
- Connect the DHT11 sensor output pin to GPIO 27.
- Upload the sketch to the ESP32.
- Open the serial monitor and confirm successful cloud connection.
- Use the Adafruit IO dashboard to observe sensor values and control the LED.

## Expected Result

After completing the assignment, the ESP32 should publish DHT11 temperature and humidity data to Adafruit IO and respond to remote button commands by turning the on-board LED on or off.
