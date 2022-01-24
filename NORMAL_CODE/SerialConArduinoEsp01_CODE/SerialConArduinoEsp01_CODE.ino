/**
 * Code for Esp 01
 * 
 * 
 **/

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }

  delay(1000);
  Serial.println("OK!, Im Ready!");
}

void loop() {
  
  while (Serial.available()){
    String received = Serial.readString();
    Serial.println("Received: " + received);
  }
  delay(100);
}
