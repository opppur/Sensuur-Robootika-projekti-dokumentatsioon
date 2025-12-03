#include <OneWire.h>
#include <DHT11.h>
#include <DallasTemperature.h>
#include <MQ135.h>
#include <Adafruit_GFX.h>       
#include <Adafruit_ILI9341.h>

#include <iostream>
#include <string> 

#define TFT_CS    8
#define TFT_RST   9
#define TFT_DC    10

#define ONE_WIRE_BUS 4
#define sensor A0
#define PIN_MQ135 A1
#define wet 210
#define dry 510

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

MQ135 mq135_sensor(PIN_MQ135);
DHT11 dht11(2);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
    // Initialize serial communication to allow debugging and data readout.
    // Using a baud rate of 9600 bps.
    Serial.begin(9600);
    
    // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
    // dht11.setDelay(500); // Set this to the desired delay. Default is 500ms.

    // Start up the library
    sensors.begin();

    // Screen startup
    tft.begin();

    uint8_t rotation=1;
    tft.setRotation(rotation);

}

void loop() {
    int temperature = 0;
    int humidity = 0;
    int soilMoisture = analogRead(sensor);
    int moisture = map(soilMoisture, wet, dry, 100, 0);
    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int result = dht11.readTemperatureHumidity(temperature, humidity);

    //MQ135
    //correction based on DHT11 readings
    float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
    
    // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
    sensors.requestTemperatures(); 

    // Check the results of the readings.
    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.print(" %\tSoil Moisture: ");
        Serial.print(moisture);
        Serial.print(" %");
        Serial.print("  Soil (contact) temperature = ");
        Serial.print(sensors.getTempCByIndex(0));
        Serial.println(" °C");
        Serial.print("\t PPM: ");
        Serial.print(correctedPPM);
        Serial.println("ppm");

        delay(1000);
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    String d1 = String(temperature);
    String d2 = String(humidity);
    String d3 = String(moisture);
    int contactTemp = sensors.getTempCByIndex(0);
    String d4 = String(contactTemp);
    String d5 = String(correctedPPM);

    tft.setTextColor(ILI9341_WHITE);
    tft.println("Temperature: " + d1 + "C"); 	
    tft.println("Humidity: " + d2 + "%");
    tft.println();
    tft.println();
    tft.setTextColor(ILI9341_YELLOW);
    tft.println("Soil Moisture: " + d3 + "%");
    tft.println("Soil Temperature: " + d4 + "C");
    tft.println();
    tft.println();
    tft.setTextColor(ILI9341_GREEN);
    tft.println("Air quality: " + d5 + "ppm");;
    tft.println("");
    tft.setTextColor(ILI9341_PINK);
    //tft.println("         (^_^)  o");
    //tft.println("        /|   |\\/");
    //tft.println("       / |___|");
    //tft.println("      o  |   |");
    //tft.println("        _| | |_");
    tft.println("       _,-._ ");
    tft.println("      / \\_/ \\  ___");
    tft.println("      >-(_)-< / _/");
    tft.println("      \\_/ \\_//_/");
    tft.println("        `-' /");
}
