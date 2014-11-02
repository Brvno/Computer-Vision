#ifndef HAND_H
#define HAND_H

#include <opencv2/opencv.hpp>

class Hand
{
private:
    cv::Scalar HSVmin;
    cv::Scalar HSVmax;
public:
    Hand();
    Hand(const cv::Scalar HSVmin, const cv::Scalar HSVmax);

    cv::Scalar getHSVmax() const;
    cv::Scalar getHSVmin() const;

    void setHSVmax(const cv::Scalar value);
    void setHSVmin(const cv::Scalar value);
};

#endif // HAND_H
