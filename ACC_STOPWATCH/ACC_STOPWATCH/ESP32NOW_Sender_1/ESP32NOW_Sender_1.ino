#include <esp_now.h>
#include <WiFi.h>

int receiver = D0;
int Lpbd1, Cpbd1;
int counter = 0;
long currentTime = 0;
long startTime = 0;
long lap1Time, lap2Time, lap3Time = 0;
long elapsedTime = 0;
int sw = 0;
//to avoid error
int error = 2500;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x64, 0xE8, 0x33, 0x83, 0xDD, 0x98};

// Structure example to send data
// Must match the receiver structure
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

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() 
{
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(receiver, INPUT);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  currentTime = millis();

  Lpbd1 = Cpbd1;              //SAVE
  Cpbd1 = digitalRead(receiver);  //READ

  if (Lpbd1 == 1 && Cpbd1 == 0)
  {
    Serial.println("Triggered");
    counter = counter + 1;
    Serial.println(counter);
    // long startTime = currentTime;

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
    
    // //temp
    // delay(5000);
    // counter = 0;
    // elapsedTime = 0;
    // sw = 0;
  }

  // int minutes = elapsedTime / (60 * 1000); // Convert milliseconds to minutes
  // int seconds = (elapsedTime % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  // int milliseconds = elapsedTime % 1000; // Get remaining milliseconds

  // Serial.print("Elapsed Time: ");
  // Serial.print(minutes);
  // Serial.print(":");
  // Serial.print(seconds, DEC);
  // Serial.print(":");
  // Serial.println(milliseconds, DEC);
  // delay(10); // Small delay to avoid ove

////////////////////////////////////////////////////////////////////


  // Set values to send
  // strcpy(myData.a, "THIS IS A CHAR");
  // myData.b = random(1,20);
  // myData.c = 1.2;
  // myData.d = false;
  myData.elapsed = elapsedTime;
  myData.lapcount = counter;
  myData.lap1T = lap1Time;
  myData.lap2T = lap2Time;
  myData.lap3T = lap3Time;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10);
}