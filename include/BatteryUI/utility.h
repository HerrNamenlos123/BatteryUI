#pragma once

#include <mutex>
#include <condition_variable>

namespace BatteryUI {

    class AsyncWait {
    public:
        AsyncWait() {

        }

        template<typename T, typename P>
        bool operator()(const std::chrono::duration<T, P>& time) {
            std::unique_lock<std::mutex> lock(m);
            return !cv.wait_for(lock, time, [&]{ return _terminate.load(); });
        }

        void terminate() {
            std::unique_lock<std::mutex> lock(m);
            _terminate = true;
            cv.notify_all();
        }

    private:
        std::condition_variable cv;
        std::mutex m;
        std::atomic<bool> _terminate = false;
    };

}
