#pragma once

#include "Arduino.h"

class TimerCall
{
    private:
        typedef void (*TimerCallFunction)();

    public:

        typedef struct TimerCallTask_s {
            String name;
            TimerCallFunction func;
            unsigned long lastExecMills;
            unsigned long nextExecMills;
            unsigned long lastElapsedMills;   // 実行した時間（前回）
            unsigned long totalElapsedMills;  // 実行した時間（合計）
            unsigned long callCount;
            unsigned long interval;
        } TimerCallTask;

        void add(TimerCallFunction f, String name, unsigned long interval);
        // void setStasticsFunction(TimerCallStatFunction f, unsigned long interval = 5000);
        void start();
        void forceOnce();
        void loop();
        void dump();

    protected:
        unsigned long nowMills;
        //   typedef void (*CallbackFunction)(Button2 &);
        std::vector<TimerCallTask> tasks;
        // TimerCallStatFunction statTask;
};