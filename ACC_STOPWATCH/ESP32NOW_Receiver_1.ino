#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message 
{
  long elapsed;
  
  int lapcount;
  long lap1T;
  long lap2T;
  long lap3T;

} struct_message;

// Create a struct_message called myData
struct_message myData;

LiquidCrystal_I2C lcd(0x3F,16,2); 

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
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

  lcd.setCursor(2, 0);
  lcd.print("Stopwatch:")

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
  lcd.print((milliseconds < 100) ? "00" : (milliseconds < 1000) ? "0" : "");  // Adjust for 3 digits
  lcd.print(milliseconds);

  int Counter = myData.lapcount;
  long lap1 = myData.lap1T;
  long lap2 = myData.lap2T;
  long lap3 = myData.lap3T;

  int minuteslap1 = lap1 / (60 * 1000); // Convert milliseconds to minutes
  int secondslap1 = (lap1 % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int millisecondslap1 = lap1 % 1000; // Get remaining milliseconds

  int minuteslap2 = lap2 / (60 * 1000); // Convert milliseconds to minutes
  int secondslap2 = (lap2 % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int millisecondslap2 = lap2 % 1000; // Get remaining milliseconds

  int minuteslap3 = lap3 / (60 * 1000); // Convert milliseconds to minutes
  int secondslap3 = (lap3 % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int millisecondslap3 = lap3 % 1000; // Get remaining milliseconds

  Serial.print("  Lap Count: ");
  Serial.print(Counter);

  Serial.print("  Lap 1 Time: ");
  Serial.print(minuteslap1);
  Serial.print(":");
  Serial.print(secondslap1, DEC);
  Serial.print(":");
  Serial.print(millisecondslap1, DEC);

  Serial.print("  Lap 2 Time: ");
  Serial.print(minuteslap2);
  Serial.print(":");
  Serial.print(secondslap2, DEC);
  Serial.print(":");
  Serial.print(millisecondslap2, DEC);

  Serial.print("  Lap 3 Time: ");
  Serial.print(minuteslap3);
  Serial.print(":");
  Serial.print(secondslap3, DEC);
  Serial.print(":");
  Serial.print(millisecondslap3, DEC);

  Serial.print("  Elapsed Time: ");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds, DEC);
  Serial.print(":");
  Serial.println(milliseconds, DEC);
  delay(10); 

}
