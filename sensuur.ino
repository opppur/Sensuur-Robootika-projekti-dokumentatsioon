#include <OneWire.h>
#include <DHT11.h>
#include <DallasTemperature.h>
#include <MQ135.h>


#define ONE_WIRE_BUS 4
#define sensor A0
#define PIN_MQ135 A1
#define wet 210
#define dry 510

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
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
}
