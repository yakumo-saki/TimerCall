#include <Arduino.h>
#include "TimerCall.h"

void TimerCall::add(TimerCall::TimerCallFunction f, String name, unsigned long intervalMs) {

    TimerCallTask task;
    task.func = f;
    task.info.interval = intervalMs;
    task.info.name = name;

    this->tasks.push_back(task);
};

void TimerCall::start() {

    if (this->started) {
        return;  // 
    }

    this->restart();
    this->started = true;
};

void TimerCall::restart() {

    unsigned long now = millis();

    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        initTaskInfo(it->info, now);
    }

    for (auto it = this->statTasks.begin(), e = this->statTasks.end(); it != e; ++it) {
        initTaskInfo(it->info, now);
    }

}

void TimerCall::addStasticsFunction(TimerCall::TimerCallStatFunction f,  String name, unsigned long intervalMs = 5000) {
    TimerCallStatTask task;
    task.statFunc = f;
    task.info.name = name;
    task.info.interval = intervalMs;

    this->statTasks.push_back(task);
}

void TimerCall::forceOnce() {
    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        it->func();
    }
};

void TimerCall::loop() {
    // millisのオーバーフローしても大丈夫なコードにした。
    // https://garretlab.web.fc2.com/arduino/lab/millis/
    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        unsigned long nowMillis = millis();
        unsigned long elapsedMillis = nowMillis - it->info.lastExecMills;
        if (it->info.interval < elapsedMillis) {
            unsigned beforeExecMillis = millis();
            it->func();
            this->updateInfo(it->info, beforeExecMillis, millis());
        }
    }

    for (auto it = this->statTasks.begin(), e = this->statTasks.end(); it != e; ++it) {
        unsigned long nowMillis = millis();
        unsigned long elapsedMillis = nowMillis - it->info.lastExecMills;
        if (it->info.interval < elapsedMillis) {
            unsigned long beforeExecMillis = millis();
            it->statFunc(this->tasks);
            this->updateInfo(it->info, beforeExecMillis, millis());
        }
    }
};

void TimerCall::initTaskInfo(TimerCall::TimerCallTaskInfo &info, unsigned long nowMillis) {
    info.lastExecMills = nowMillis;
    info.callCount = 0;
    info.lastElapsedMills = 0;
    info.totalElapsedMills = 0;
};

void TimerCall::updateInfo(TimerCall::TimerCallTaskInfo &info, unsigned long beforeExecMillis, unsigned long nowMillis) {
    info.lastExecMills = nowMillis;
    info.lastElapsedMills = info.lastExecMills - beforeExecMillis;

    info.totalElapsedMills = info.totalElapsedMills + info.lastElapsedMills;
    info.callCount++;
};
