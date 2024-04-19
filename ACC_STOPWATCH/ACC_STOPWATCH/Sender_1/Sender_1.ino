int receiver = D0;
int Lpbd1, Cpbd1;
int counter = 0;
long currentTime = 0;
long startTime = 0;
long lap1Time, lap2Time, lap3Time = 0;
long elapsedTime = 0;
int sw = 0;

//to avoid error
int error = 1000;


void setup() 
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(receiver, INPUT);

}

void loop() 
{
  // put your main code here, to run repeatedly:
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

  int minutes = elapsedTime / (60 * 1000); // Convert milliseconds to minutes
  int seconds = (elapsedTime % (60 * 1000)) / 1000; // Get remaining seconds after removing minutes
  int milliseconds = elapsedTime % 1000; // Get remaining milliseconds

  Serial.print("Elapsed Time: ");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds, DEC);
  Serial.print(":");
  Serial.println(milliseconds, DEC);
  delay(10); // Small delay to avoid ove

}
