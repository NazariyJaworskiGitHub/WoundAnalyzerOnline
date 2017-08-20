#ifndef MATHUTILITIES
#define MATHUTILITIES

#include <cmath>
#include <opencv2/opencv.hpp>

namespace MathUtilities {

    double length(const cv::Point2d &a, const cv::Point2d &b)
    {
        return std::sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
    }

    bool isOnSegment(const cv::Point2d &a, const cv::Point2d &b, const cv::Point2d &c, double eps = 0.1)
    {

        double ab = length(a,b);
        double ac = length(a,c);
        double cb = length(c,b);
        return std::fabs(ac + cb - ab) < eps;
    }
}
#endif // MATHUTILITIES

