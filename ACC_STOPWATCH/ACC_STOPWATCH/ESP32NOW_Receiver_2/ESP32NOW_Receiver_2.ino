#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

int Lpbd1, Cpbd1;
long elapsedTime, currentTime, startTime, lap1Time, lap2Time, lap3Time = 0;

int sw = 0;
int counter = 0;

int error = 2500;

int resetbutton = D10;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message 
{

  int Rdata;

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

  pinMode(resetbutton, INPUT_PULLUP);

  
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

  currentTime = millis();
  int ResetData = digitalRead(resetbutton);

  if (ResetData == 0)
  {
    counter = 0; 
    elapsedTime = 0;
    currentTime = 0;
    startTime = 0;
    lap1Time = 0;
    lap2Time = 0;
    lap3Time = 0;
    sw = 0;
  }

  int ReceiverData = myData.Rdata;

  Lpbd1 = Cpbd1;              //SAVE
  Cpbd1 = ReceiverData;       //READ

  if (Lpbd1 == 1 && Cpbd1 == 0)
  {
    Serial.println("Triggered");
    counter = counter + 1;
    Serial.println(counter);

    switch (counter)
    {
      case 1:
      Serial.print("  Timer started");
      startTime = currentTime;
      sw = 1;
      delay(error);
      break;

      case 2:
      Serial.print("  Lap 1 ended.");
      lap1Time = currentTime - startTime;
      delay(error);
      break;

      case 3:
      Serial.print("  Lap 2 ended.");
      lap2Time = currentTime - startTime;
      delay(error);
      break;

      case 4:
      Serial.print("  Lap 3 ended, Timer stopped");
      lap3Time = currentTime - startTime;
      sw = 2;
      delay(error);
      break;
    }
  }

  if (sw == 1)
  {
    elapsedTime = currentTime - startTime;
  }
  else if (sw == 2)
  {
    elapsedTime = lap3Time;
  }

/////////////////////////////////////////////////////   LCD   ///////////////////////////////////////////

  int minutes = elapsedTime / (60 * 1000); // Convert milliseconds to minutes
  int seconds = (elapsedTime % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int milliseconds = elapsedTime % 1000; // Get remaining milliseconds
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Stopwatch:");

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
  lcd.print((milliseconds < 100) ? "0" : (milliseconds < 1000) ? "" : "");  // Adjust for 3 digits
  lcd.print(milliseconds);

/////////////////////////////////////////////////   TERA TERM   ///////////////////////////////////////////////

  int minuteslap1 = lap1Time / (60 * 1000); // Convert milliseconds to minutes
  int secondslap1 = (lap1Time % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int millisecondslap1 = lap1Time % 1000; // Get remaining milliseconds

  int minuteslap2 = lap2Time / (60 * 1000); // Convert milliseconds to minutes
  int secondslap2 = (lap2Time % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int millisecondslap2 = lap2Time % 1000; // Get remaining milliseconds

  int minuteslap3 = lap3Time / (60 * 1000); // Convert milliseconds to minutes
  int secondslap3 = (lap3Time % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int millisecondslap3 = lap3Time % 1000; // Get remaining milliseconds

  Serial.print("  Lap Count: ");
  Serial.print(counter);

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
  delay(50); 

}