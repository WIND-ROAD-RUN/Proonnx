#ifndef ROLLINGAVERAGE_H_
#define ROLLINGAVERAGE_H_

#include <deque>
#include <numeric>
#include <type_traits>

template <typename T>
class RollingAverage {
public:
    
    explicit RollingAverage(int capacity) : capacity_(capacity) {}

    
    double addData(T data);

private:
    int capacity_;
    double sum_ = 0;
    std::deque<T> data_points_;
};

template<typename T>
double RollingAverage<T>::addData(T data)
{
    static_assert(std::is_arithmetic<T>::value, "Type must be numeric");

    if (data_points_.size() == capacity_) {
        sum_ -= data_points_.front();
        data_points_.pop_front();
    }
    data_points_.push_back(data);
    sum_ += data;
    return sum_ / data_points_.size();
}

#endif // !ROLLINGAVERAGE_H_


