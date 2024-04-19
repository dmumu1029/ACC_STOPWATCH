#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message 
{
  // char a[32];
  // int b;
  // float c;
  // bool d;
  long elapsed;
} struct_message;

// Create a struct_message called myData
struct_message myData;

LiquidCrystal_I2C lcd(0x3F,16,2); 

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  // Serial.print("Char: ");
  // Serial.println(myData.a);
  // Serial.print("Int: ");
  // Serial.println(myData.b);
  // Serial.print("Float: ");
  // Serial.println(myData.c);
  // Serial.print("Bool: ");
  // Serial.println(myData.d);
  // Serial.println();
  // Serial.println(myData.elapsed);
  // Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  lcd.init();                     
  lcd.backlight();
  lcd.clear();

  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

  long Timer = myData.elapsed;

  int minutes = Timer / (60 * 1000); // Convert milliseconds to minutes
  int seconds = (Timer % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int milliseconds = Timer % 1000; // Get remaining milliseconds

  lcd.setCursor(3, 1);
  lcd.print((minutes < 10) ? "0" : "");  // Print leading zero if minutes less than 10
  lcd.print(minutes);

  lcd.setCursor(5, 1);
  lcd.print(":");

  lcd.setCursor(6, 1);
  lcd.print((seconds < 10) ? "0" : "");  // Print leading zero if seconds less than 10
  lcd.print(seconds);

  lcd.setCursor(8, 1);
  lcd.print(":");

  lcd.setCursor(9, 1);
  lcd.print(milliseconds);

  Serial.print("Elapsed Time: ");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds, DEC);
  Serial.print(":");
  Serial.println(milliseconds, DEC);
  delay(10); 

}