#include <Arduino.h>
#include "TimerCall.h"

TimerCall tc = TimerCall();

// same as basic_usage.cpp

void callbackA() {
  Serial.println("*****************************************************");
  Serial.println("callbackA");
  Serial.println("*****************************************************");
  delay(100);
}
void callbackB() {
  Serial.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  Serial.println("callbackB");
  Serial.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  delay(200);
}

void printStastics(std::vector<TimerCall::TimerCallTask> &tasks) {
    Serial.println("printStastics ------------------------");
    for (auto it = tasks.begin(), e = tasks.end(); it != e; ++it) {
        Serial.println(
           + "name=" + String(it->info.name)
           + " last=" + String(it->info.lastExecMills)
           + " last exec=" + String(it->info.lastElapsedMills)
           + " total=" + String(it->info.totalElapsedMills)
           + " count=" + String(it->info.callCount)
        );
    }
    Serial.println("--------------------------------------");
}

void setup() {
  Serial.begin(74880);

  tc.add(callbackA, "callbackA", 1000);
  tc.add(callbackB, "callbackB", 3500);
  tc.addStasticsFunction(printStastics, "stat1", 5000);
  Serial.println("setup end");
  tc.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  tc.loop();
  delay(100);
}
