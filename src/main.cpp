#include <SPI.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

  #include "Wire.h" // Library for I2C communication
  #include "LiquidCrystal_I2C.h" // Library for LCD

const char* ssid = "Your Wifi";
const char* password = "Your Wifi Password";


Adafruit_AHTX0 aht;
AsyncWebServer server(80);

  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  //while (!Serial);

  delay(200);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Version 0.9");
    delay(250);


  if (aht.begin()) {
    Serial.println("Found AHT20 Sensor");
  } else {
    Serial.println("Didn't find AHT20");
  }


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
    lcd.print("Wifi Connecting");  
  }
  
  Serial.println("Connected to WiFi");
  lcd.setCursor(0, 1);
  lcd.print("Wifi Connected!");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());
  delay(3000); // Display IP address for 3 seconds
  lcd.clear();

  // Print the ESP32's IP address
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    sensors_event_t humidity, temp;
  
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    
    String temperature = String(temp.temperature * 1.8 + 32);
    String humidityReading = String(humidity.relative_humidity);

    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /");        
    request->send(200, "text/html", "<style>body { font-size: 24px; }</style>"
        "<html><body><h2 style=\"font-size: 24px;\">Temperature: " + temperature + " degrees F</h2>"
        "<p style=\"font-size: 24px;\">Humidity: " + humidityReading + "% RH</p></body></html>");
  });

  // Start the server
  server.begin();
 
}

void loop() {
  sensors_event_t humidity, temp;
  
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  
        lcd.setCursor(2, 0); // Set the cursor on the first column and first row.
        lcd.print("Temp: ");
        lcd.print(String(temp.temperature * 1.8 + 32));
        lcd.print("F");
        
        
        lcd.setCursor(3, 1); //Set the cursor on the first column and the second row (counting starts at 0!).
        lcd.print("Hum: ");
        lcd.print(String(humidity.relative_humidity));
        lcd.print("%");

        yield();

  delay(1000);
}