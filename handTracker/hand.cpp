#include "hand.h"


cv::Scalar Hand::getHSVmin() const {
    return HSVmin;
}

void Hand::setHSVmin(const cv::Scalar value){
    HSVmin = value;
}

cv::Scalar Hand::getHSVmax() const{
    return HSVmax;
}

void Hand::setHSVmax(const cv::Scalar value){
    HSVmax = value;
}

Hand::Hand()
{}

Hand::Hand(const cv::Scalar HSVmin, const cv::Scalar HSVmax)
{
    this->HSVmin = HSVmin;
    this->HSVmax = HSVmax;
}
