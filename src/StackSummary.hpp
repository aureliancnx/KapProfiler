#ifndef RTYPE_STACKSUMMARY_HPP
#define RTYPE_STACKSUMMARY_HPP

#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace KapEngine {

    namespace Profiler {

        class StackSummary {
          private:
            std::string _raw;
            long long _callCount;
            long long _totalTime;
            float _callPerSecond;
            float _meanCallPerSecond;
            long long _start;
            long long _time;
            long long _totTimeAllCalls;
            double _percentageTime;

            void compute() {
                long long onel = 1;
                _callPerSecond = ((float)_callCount / ((float)std::max((_time - _start) / 1000000000, onel)));
                _meanCallPerSecond = (long)(_totalTime / std::max(_callCount, onel));
                _percentageTime = ((double)_totalTime / (double)std::max(_totTimeAllCalls, onel)) * 100.0;
            }

          public:
            StackSummary(std::string& raw, long long callCount, long long totalTime, long long start, long long time,
                         long long totTimeAllCalls) {
                _raw = raw;
                _callCount = callCount;
                _totalTime = totalTime;
                _start = start;
                _time = time;
                _totTimeAllCalls = totTimeAllCalls;
                compute();
            }

            std::string& getRaw() { return _raw; }

            long long getCallCount() const { return _callCount; }

            long long getTotalTime() const { return _totalTime; }

            long long getStartTime() const { return _start; }

            long long getTime() const { return _time; }

            long long getTotTimeAllCalls() const { return _totTimeAllCalls; }

            double getCallsPerSecond() const { return _callPerSecond; }

            double getMeanCallPerSecond() const { return _meanCallPerSecond; }

            double getPercentageTime() const { return _percentageTime; }

            void serialize(std::ostream& stream) { stream << _raw << _callCount << _totalTime << _start << _time << _totTimeAllCalls; }

            bool operator>(const StackSummary& summary) const { return (_totalTime > summary._totalTime); }
        };

    }; // namespace Profiler

} // namespace KapEngine

#endif // RTYPE_STACKSUMMARY_HPP
