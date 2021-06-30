#pragma once

#include "Arduino.h"

class TimerCall
{
    private:
        typedef void (*TimerCallFunction)();

    public:
        // Version of TimerCall. x.yz format.
        const double VERSION = 0.4;

        typedef struct {
            String name;
            unsigned long lastExecMills;
            unsigned long lastElapsedMills;   // 実行した時間（前回）
            unsigned long totalElapsedMills;  // 実行した時間（合計）
            unsigned long callCount;
            unsigned long interval;
        } TimerCallTaskInfo;

        typedef struct {
            TimerCall::TimerCallTaskInfo info;
            TimerCall::TimerCallFunction func;
        } TimerCallTask;

        typedef void (*TimerCallStatFunction)(std::vector<TimerCallTask> &tasks);

        typedef struct {
            TimerCall::TimerCallTaskInfo info;
            TimerCall::TimerCallStatFunction statFunc;
        } TimerCallStatTask;

        void add(TimerCall::TimerCallFunction f, String name, unsigned long interval);
        void addStasticsFunction(TimerCall::TimerCallStatFunction f, String name, unsigned long intervalinterval);
        
        // start timer (do nothing when timer is already started)
        void start();

        // restart timer (reset timer and start)
        void restart();

        // stop timer (no reset) 
        // Nothing happens when timer not started
        void stop();

        /** exec all tasks. (timer state not change) */
        void forceOnce();

        /** run AllStastics once */
        void forceRunStasticsOnce();

        /**
         * exec task when timer duration reached
         * call me at loop()
         */
        void loop();
        void dump();

    protected:
        unsigned long nowMills;
        std::vector<TimerCall::TimerCallTask> tasks;
        std::vector<TimerCall::TimerCallStatTask> statTasks;
        bool started = false;

        void initTaskInfo(TimerCall::TimerCallTaskInfo &info, unsigned long nowMillis);
        void updateInfo(TimerCall::TimerCallTaskInfo &info, unsigned long beforeExecMillis, unsigned long nowMillis);

    private:
        void runTask(TimerCall::TimerCallTask& task);
        void runStatTask(TimerCall::TimerCallStatTask& statTask);
};