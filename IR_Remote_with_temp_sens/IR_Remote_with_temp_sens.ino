#define BLYNK_TEMPLATE_ID "TMPL3KjnEeY21"
#define BLYNK_TEMPLATE_NAME "IR Receiver"
#define BLYNK_AUTH_TOKEN "IXJjunHYgdty9x2Wfmstpu-dPhmhlGTN"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <IRremoteESP8266.h>  // use IRremoteESP32 library for ESP32
#include <IRrecv.h>
#include <IRsend.h>
#include <DHT.h>

const int LED_PIN = 2; // onboard LED pin
const uint32_t kRecvPin = 33;   // Pin 14 for IR receiver
const uint32_t kIrLed = 14;      // Pin 5 for IR LED
#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
IRrecv irrecv(kRecvPin);        // Set up IRrecv object for specified pin
IRsend irsend(kIrLed);          // Set up IRsend object for specified pin

decode_results results;         // Object to store received IR data

uint32_t hexCodes[] = {0xF7C03F, 0xF740BF, 0xF7807F, 0xF700FF};
int numCodes = sizeof(hexCodes) / sizeof(hexCodes[0]); // Calculate number of codes in array

const char* ssid = "Galaxy"; // replace with your WiFi network name
const char* password = "tufj7077"; // replace with your WiFi network password

void setup() {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  irrecv.enableIRIn();          // Initialize IRrecv object to receive IR signals
  irsend.begin();               // Initialize IRsend object to send IR signals
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); // print the received value in hexadecimal format
    irrecv.resume();

    float temperature = dht.readTemperature();    // Read temperature value from DHT sensor
    float humidity = dht.readHumidity();          // Read humidity value from DHT sensor

    if (isnan(temperature) || isnan(humidity)) {  // Check if any reading is NaN
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Send temperature and humidity values to Blynk
    Blynk.virtualWrite(V5, temperature);
    Blynk.virtualWrite(V6, humidity);

    delay(1000);

  }
}

BLYNK_WRITE(V1) {
  if (param.asInt() == 1) {
    irsend.sendNEC(hexCodes[0], 32);    // Send the first hex code using NEC protocol
  }
}

BLYNK_WRITE(V2) {
  if (param.asInt() == 1) {
    irsend.sendNEC(hexCodes[1], 32);    // Send the second hex code using NEC protocol
  }
}

BLYNK_WRITE(V3) {
  if (param.asInt() == 1) {
    irsend.sendNEC(hexCodes[2], 32);    // Send the third hex code using NEC protocol
  }
}

BLYNK_WRITE(V4) {
  if (param.asInt() == 1) {
    irsend.sendNEC(hexCodes[3], 32);    // Send the fourth hex code using NEC protocol
  }
}
BLYNK_WRITE(V7) {
    irsend.sendNEC(hexCodes[4], 32);    // Send the fourth hex code using NEC protocol
  }
}
BLYNK_WRITE(V8) {
  if (param.asInt() == 1) {
    irsend.sendNEC(hexCodes[5], 32);    // Send the fourth hex code using NEC protocol
  }
}
