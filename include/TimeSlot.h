#ifndef TIMESLOT_H
#define TIMESLOT_H
#include <string>
#include <compare>
class TimeSlot {
private:
    int start_hour_;
    int end_hour_;
public:
    TimeSlot(int start_hour, int end_hour);
    int getStartHour() const;
    int getEndHour() const;
    int getDuration() const;
    bool overlaps(const TimeSlot& other) const;
    auto operator<=>(const TimeSlot&) const = default;
};
#endif // TIMESLOT_H
