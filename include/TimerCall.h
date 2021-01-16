#pragma once

#include "Arduino.h"

class TimerCall
{
    private:
        typedef void (*TimerCallFunction)();

    public:
        typedef struct {
            String name;
            unsigned long lastExecMills;
            unsigned long lastElapsedMills;   // 実行した時間（前回）
            unsigned long totalElapsedMills;  // 実行した時間（合計）
            unsigned long callCount;
            unsigned long interval;
        } TimerCallTaskInfo;

        typedef struct {
            TimerCallTaskInfo info;
            TimerCallFunction func;
        } TimerCallTask;

        typedef void (*TimerCallStatFunction)(std::vector<TimerCallTask> &tasks);

        typedef struct {
            TimerCallTaskInfo info;
            TimerCallStatFunction statFunc;
        } TimerCallStatTask;

        void add(TimerCallFunction f, String name, unsigned long interval);
        void addStasticsFunction(TimerCallStatFunction f, String name, unsigned long intervalinterval);
        
        // start timer (do nothing when timer is already started)
        void start();

        // start timer (reset timer and start)
        void restart();

        // exec all tasks. (timer state not change)
        void forceOnce();

        // exec task when timer duration reached
        // call me at 
        void loop();
        void dump();

    protected:
        unsigned long nowMills;
        std::vector<TimerCallTask> tasks;
        std::vector<TimerCallStatTask> statTasks;
        bool started = false;

        void initTaskInfo(TimerCallTaskInfo &info, unsigned long nowMillis);
        void updateInfo(TimerCallTaskInfo &info, unsigned long beforeExecMillis, unsigned long nowMillis);
};