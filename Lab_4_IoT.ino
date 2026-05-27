#include <AdafruitIO.h>       // Include the Adafruit IO library.
#include <AdafruitIO_WiFi.h>  // Include the Adafruit IO Wi-Fi connection library.

#define IO_USERNAME  "*********" // Adafruit IO username placeholder. Replace with your actual username before uploading.
#define IO_KEY       "*********" // Adafruit IO key placeholder. Replace with your actual key before uploading.

#define WIFI_SSID "*****" // Wi-Fi network name used by the ESP32 to access the server.
#define WIFI_PASS "****"    // Wi-Fi password placeholder. Replace with the real password before uploading.

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);  // Create the Adafruit IO Wi-Fi client.
#define LED_PIN 2 // The on-board LED is connected to GPIO 2.

// Set up the Adafruit IO feeds.
AdafruitIO_Feed *digital = io.feed("button"); // Feed used to receive button data from Adafruit IO.
AdafruitIO_Feed *temp = io.feed("temp");
AdafruitIO_Feed *humi = io.feed("humi");

// DHT11 sensor library.
#include <DHT.h>

#define DHTPIN 27 // Connect the DHT11 output pin to GPIO 27 on the ESP32.
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Timer used to control the sensor upload interval.
static uint32_t oldtime = millis();

// Read DHT11 data and send it to Adafruit IO.
void sendSensor()
{
  float h = dht.readHumidity();     // Read humidity.
  float t = dht.readTemperature();  // Read temperature.

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // You can send any value at any time.
  // Please do not send more than 10 values per second.
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);

  temp->save(t);
  humi->save(h);
}

void setup() {
  // Set the on-board LED pin as an output.
  pinMode(LED_PIN, OUTPUT);

  // Start the serial connection.
  Serial.begin(115200);

  // Wait for the serial monitor to open.
  while(! Serial);

  // Connect to io.adafruit.com.
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Wait until the connection is established.
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Print the connection status after a successful connection.
  Serial.println();
  Serial.println(io.statusText());

  // Set up a message handler for the "button" feed.
  // The handleMessage function is called whenever a message is received from Adafruit IO.
  digital->get();
  digital->onMessage(handleMessage);
  dht.begin();
}

void loop() {
  io.run();

  if (millis() - oldtime > 20000)
  {
    sendSensor();
    oldtime = millis();
  }
}

// This function is called whenever a "button" feed message is received from Adafruit IO.
void handleMessage(AdafruitIO_Data *data) {
  // Print the received button state to the serial monitor.
  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

  // Update the on-board LED according to the button state.
  digitalWrite(LED_PIN, data->toPinLevel());
}
