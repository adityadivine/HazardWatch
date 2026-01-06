#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <NewPing.h>

// Pin Definitions
#define DHTPIN D3          // DHT11 connected to pin D3
#define DHTTYPE DHT11      // DHT 11 sensor type
#define FLAME_PIN A0       // Flame sensor connected to analog pin A0
#define MQ2_PIN A0         // MQ2 sensor connected to analog pin A0 (same as flame sensor)
#define TRIGGER_PIN D5     // Ultrasonic sensor trigger pin connected to D5
#define ECHO_PIN D6        // Ultrasonic sensor echo pin connected to D6
#define BUZZER_PIN D7      // Buzzer connected to pin D7
#define SCREEN_ADDRESS 0x3C // OLED display I2C address

// OLED Display Parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT11 Sensor Initialization
DHT dht(DHTPIN, DHTTYPE);

// SR04 Ultrasonic Sensor Initialization
#define MAX_DISTANCE 200 // Maximum distance for the ultrasonic sensor (in cm)
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Flame Threshold (Adjust as necessary)
#define FLAME_THRESHOLD 280

void setup() {
  // Start Serial Communication
  Serial.begin(115200);
  
  // Initialize DHT Sensor
  dht.begin();

  // Initialize OLED display with error checking
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Loop forever if OLED initialization fails
  }

  // Clear display buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Set up Buzzer Pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Initially, buzzer is off

  Serial.println("Setup complete");
}

void loop() {
  // Print "WASTELESS WIZARDS" on top of the output
  Serial.println("WASTELESS WIZARDS");

  // Clear OLED display for new data
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WASTELESS WIZARDS");

  // DHT11 Sensor Data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();  // Default is Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    display.println("DHT11: Error");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    display.print("Hum: ");
    display.print(humidity);
    display.print(" % Temp: ");
    display.print(temperature);
    display.println(" C");
  }

  // Flame Sensor Data
  int flameValue = analogRead(FLAME_PIN);
  Serial.print("Flame Sensor Value: ");
  Serial.println(flameValue);
  
  if (flameValue < FLAME_THRESHOLD) {
    Serial.println("Flame Detected!");
    display.println("Flame: Detected");
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer on
  } else {
    Serial.println("No Flame Detected");
    display.println("Flame: None");
    digitalWrite(BUZZER_PIN, LOW); // Buzzer off
  }

  // Ultrasonic Sensor Data (SR04)
  unsigned int distance = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");

  // MQ2 Sensor Data (Read after flame sensor)
  int mq2Value = analogRead(MQ2_PIN);
  Serial.print("MQ2 Sensor Value: ");
  Serial.println(mq2Value);

  display.print("MQ2: ");
  display.println(mq2Value);
  if(mq2Value > 800){
    Serial.println("Unusual harmful gases detected!");
    display.println("Unusual harmful gases detected!");
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer on
  }


  // Display everything on the OLED screen
  display.display();

  // Adds spaces in the serial monitor output
  Serial.println();

  Serial.println("------------------------------------------------");

  Serial.println();

  // Delay for 2 seconds before the next reading
  delay(1000);
}