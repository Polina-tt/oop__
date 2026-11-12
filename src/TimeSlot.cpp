#include "TimeSlot.h"
#include <stdexcept>
TimeSlot::TimeSlot(int start_hour, int end_hour) : start_hour_(start_hour), end_hour_(end_hour) {
    if (start_hour_ < 0 || start_hour_ > 23 || end_hour_ < 0 || end_hour_ > 23) throw std::invalid_argument("Invalid hours");
    if (start_hour_ >= end_hour_) throw std::invalid_argument("Start >= end");
}
int TimeSlot::getStartHour() const { return start_hour_; }
int TimeSlot::getEndHour() const { return end_hour_; }
int TimeSlot::getDuration() const { return end_hour_ - start_hour_; }
bool TimeSlot::overlaps(const TimeSlot& other) const { return (start_hour_ < other.end_hour_) && (other.start_hour_ < end_hour_); }
