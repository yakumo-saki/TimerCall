#include <Arduino.h>
#include "TimerCall.hpp"

TimerCall tc = TimerCall();

void callbackA() {
  Serial.println("callbackA");
  delay(100);
}
void callbackB() {
  Serial.println("callbackB");
  delay(200);
}
void printStastics(std::vector<TimerCall::TimerCallTask_t> &tasks) {
    for (auto it = tasks.begin(), e = tasks.end(); it != e; ++it) {
        Serial.println(
          "last=" + String(it->lastExecMills)
           + " next=" + String(it->nextExecMills)
           + " last exec=" + String(it->lastElapsedMills)
           + " total=" + String(it->totalElapsedMills)
        );
    }
}

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  tc.add(callbackA, "callbackA", 1000);
  tc.add(callbackB, "callbackB", 3500);
  tc.setStasticsFunction(printStastics, 5000);
  Serial.println("setup end");
  tc.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  tc.loop();
}