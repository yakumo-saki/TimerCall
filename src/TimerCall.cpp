#include <Arduino.h>
#include "TimerCall.h"

void TimerCall::add(TimerCall::TimerCallFunction f, String name, unsigned long intervalMs) {

    TimerCallTask task;
    task.func = f;
    task.info.interval = intervalMs;
    task.info.name = name;
    task.info.callCount = 0;
    task.info.lastElapsedMills = 0;
    task.info.lastExecMills = 0;
    task.info.totalElapsedMills = 0;

    this->tasks.push_back(task);
};

void TimerCall::start() {

    if (this->started) {
        return; 
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

void TimerCall::stop() {

    if (!this->started) {
        return;
    }

    this->started = false;
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
        this->runTask(*it);
    }
};

void TimerCall::forceRunStasticsOnce() {
    for (auto it = this->statTasks.begin(), e = this->statTasks.end(); it != e; ++it) {
        this->runStatTask(*it);
    }
};

IRAM_ATTR void TimerCall::loop() {
    // startされていないなら何もしない
    if (!this->started) return;

    // millisのオーバーフローしても大丈夫なコードにした。
    // https://garretlab.web.fc2.com/arduino/lab/millis/
    for (auto it = this->tasks.begin(), e = this->tasks.end(); it != e; ++it) {
        unsigned long nowMillis = millis();
        unsigned long elapsedMillis = nowMillis - it->info.lastExecMills;
        // Serial.println("now=" + String(nowMillis) + " elapsed=" + String(elapsedMillis) + " lastExec = " + String(it->info.lastExecMills));
        if (it->info.interval < elapsedMillis) {
            this->runTask(*it);
        }
    }

    for (auto it = this->statTasks.begin(), e = this->statTasks.end(); it != e; ++it) {
        unsigned long nowMillis = millis();
        unsigned long elapsedMillis = nowMillis - it->info.lastExecMills;
        if (it->info.interval < elapsedMillis) {
            this->runStatTask(*it);
        }
    }
};

void TimerCall::initTaskInfo(TimerCall::TimerCallTaskInfo& info, unsigned long nowMillis) {
    info.lastExecMills = nowMillis;
    info.callCount = 0;
    info.lastElapsedMills = 0;
    info.totalElapsedMills = 0;
};

void TimerCall::updateInfo(TimerCall::TimerCallTaskInfo& info, unsigned long beforeExecMillis, unsigned long nowMillis) {
    info.lastExecMills = nowMillis;
    info.lastElapsedMills = info.lastExecMills - beforeExecMillis;

    info.totalElapsedMills = info.totalElapsedMills + info.lastElapsedMills;
    info.callCount++;
};

void TimerCall::runTask(TimerCall::TimerCallTask& task) {
    unsigned beforeExecMillis = millis();
    task.func();
    this->updateInfo(task.info, beforeExecMillis, millis());
}

void TimerCall::runStatTask(TimerCall::TimerCallStatTask& statTask) {
    unsigned beforeExecMillis = millis();
    statTask.statFunc(this->tasks);
    this->updateInfo(statTask.info, beforeExecMillis, millis());
}