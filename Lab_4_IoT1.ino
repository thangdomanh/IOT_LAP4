#include <AdafruitIO.h> // tiến hành thêm thư viện AdafruitIO.h vào
#include <AdafruitIO_WiFi.h>  // Khai báo thư viện AdafruitIO_WiFi.h để kết nối đến server.

#define IO_USERNAME  "giangcutebhc2"
#define IO_KEY       "aio_kZkU83AHREenNPBP8HObmVnSnYa7"

#define WIFI_SSID "Camelot" // Tên wifi để ESP 32 kết nối vào và truy cập đến server.
#define WIFI_PASS "21012002"  // Pass wifi

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);  // Gọi hàm kết nối đến server.
#define LED_PIN 2 // LED on Board là GPIO 2.

// set up the feed 
AdafruitIO_Feed *digital = io.feed("button"); // khai báo con trỏ digital để chứ dữ liệu lấy từ feed của server.
AdafruitIO_Feed *temp = io.feed("temp");
AdafruitIO_Feed *humi = io.feed("humi");

// thu vien dht11
#include <DHT.h>

#define DHTPIN 27 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

//delay
static uint32_t oldtime  = millis();

// doc du lieu dht 11
void sendSensor()
{
  float h = dht.readHumidity();  //đọc độ ẩm
  float t = dht.readTemperature();  // đọc nhiệt độ

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);


    temp->save(t);
    humi->save(h);
}

void setup() {
  // set chân số 2, chân led là output
  pinMode(LED_PIN, OUTPUT); // Khai báo output.
 
  // start the serial connection
  Serial.begin(115200); 
 
  // wait for serial monitor to open
  while(! Serial);
 
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO"); // tiến hành kết nối đến server.
  io.connect(); // Gọi hàm kết nối
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print("."); // Nếu chưa kết nối được đến server sẽ tiến hành xuất ra màn hình đấu "."
    delay(500);
  }
 
  // we are connected
  Serial.println();
  Serial.println(io.statusText()); // Nếu đã kết nối thành công tiến hành xuất ra màn hình trạng thái.
  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below) will be called whenever a message is received from adafruit io.
  digital->get(); // lấy dữ liệu từ feed 'digital' của server.
  digital->onMessage(handleMessage); // Gọi hàm đọc dữ liệu và tiến hành điều khiển led và xuất ra trạng thái trên màn hình.
  dht.begin();
}

void loop() {
  io.run(); // gọi hàm Run.
  if  (millis()-oldtime >  20000)
    {
      sendSensor();
      oldtime = millis();
    }
}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) { // hàm handleMessage để đọc dữ liệu.

 // xuất ra màn hình trạng thái của nút nhấn trên feed vừa đọc được.
  Serial.print("received <- ");
 
  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

 // cài đặt trạng thái bật tắt led on board tương ứng với nút nhấn.
  // write the current state to the led
  digitalWrite(LED_PIN, data->toPinLevel());
 
}

