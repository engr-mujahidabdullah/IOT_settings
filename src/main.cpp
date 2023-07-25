#include <Arduino.h>

// put function declarations here:
int cout;
int count(int delay_);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  Serial.print(count(1000));
  }

// put function definitions here:
int count(int delay_){

 cout ++;
 vTaskDelay(delay_);
 return cout;
}