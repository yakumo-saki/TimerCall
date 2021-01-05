#include <Arduino.h>
#include "TimerCall.hpp"

void TimerCall::add(TimerCallFunction f, String name, unsigned long intervalMs) {

    TimerCallTask_t task;
    task.func = f;
    task.interval = intervalMs;
    task.name = name;

    this->tasks.push_back(task);
};

void TimerCall::start() {
    Serial.println("start");

    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        Serial.println(it->interval);
        it->lastExecMills=millis();
        it->nextExecMills=it->lastExecMills + it->interval;
    }
};

void TimerCall::setStasticsFunction(TimerCallStatFunction f, unsigned long intervalMs = 5000) {
    TimerCallTask_t task;
    task.func = f;
    task.interval = intervalMs;

    this->statTask = f;

}

void TimerCall::forceOnce() {
    Serial.println("forceOnce");
    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        it->func();
    }
};

void TimerCall::loop() {
    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        if (it->nextExecMills < millis()) {
            unsigned beforeExecMillis = millis();
            it->func();
            it->lastExecMills=millis();
            it->lastElapsedMills = it->lastExecMills - beforeExecMillis;

            // 統計
            it->totalElapsedMills = it->totalElapsedMills + it->lastElapsedMills;
            it->nextExecMills=it->lastExecMills + it->interval;
            it->callCount++;
        }
    }
};

void TimerCall::dump() {
    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        Serial.println("last=" + String(it->lastExecMills) + " next=" + String(it->nextExecMills) + " now=" + String(millis()));
    }
};