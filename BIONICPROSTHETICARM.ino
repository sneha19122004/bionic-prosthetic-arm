#include <Wire.h>               
#include <Adafruit_GFX.h>       
#include <Adafruit_SSD1306.h>   
#include <max6675.h>            
#include <SPI.h>

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  

// OLED I2C Address (0x3C or 0x3D)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// MAX6675 Thermocouple Pins
#define SCK 18  
#define CS 5    
#define SD 19   

// Initialize MAX6675 module
MAX6675 thermocouple(SCK, CS, SD);

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
    Serial.println("OLED not found!");
    while (1); 
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("Initializing...");
  display.display();
  delay(2000);  

  Serial.println("MAX6675 Temperature Sensor Test");
}

void loop() {
  float totalTemp = 0;
  int readings = 3;

  // Collect 5 readings and calculate the average
  for (int i = 0; i < readings; i++) {
    float tempC = thermocouple.readCelsius();
    
    if (tempC < -1) {  // MAX6675 returns -1 if no thermocouple is connected
      Serial.println("Error: No Thermocouple Detected!");
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(10, 20);
      display.println("No Sensor!");
      display.display();
      delay(2000);
      return;
    }

    Serial.print("Reading ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(tempC);
    Serial.println(" °C");

    totalTemp += tempC;
    delay(1000);  // 1-second delay between readings
  }

  float avgTemp = totalTemp / readings; // Calculate average temperature
  Serial.print("Average Temperature: ");
  Serial.print(avgTemp);
  Serial.println(" °C");

  // Display on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("Avg Temp:");
  
  display.setCursor(30, 35);
  display.print(avgTemp, 1); // Display with 1 decimal place
  display.print(" C");

  // Alert Conditions
  if (avgTemp > 45) {
    display.setTextSize(1);
    display.setCursor(10, 55);
    display.print("🔥 Danger: Burn Risk!");
    Serial.println("🔥 Warning: High Temperature! Burn Risk!");
  } 
  else if (avgTemp < 10) {
    display.setTextSize(1);
    display.setCursor(10, 55);
    display.print("❄ Warning: Too Cold!");
    Serial.println("❄ Warning: Very Low Temperature!");
  }

  display.display();

  delay(5000);  // Wait 5 seconds before repeating
}
