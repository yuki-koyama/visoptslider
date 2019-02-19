#ifndef visoptslider_hpp
#define visoptslider_hpp

#include <QWidget>

namespace visopt
{
    class SlidersWidget : public QWidget
    {
    public:
        SlidersWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::Widget);

        void setTargetDimension(const int target_dimension)
        {
            target_dimension_ = target_dimension;
        }

        void setSliderValues(const Eigen::VectorXd& slider_values)
        {
            slider_values_ = slider_values;
        }

        void setTargetFunction(const std::function<double(const Eigen::VectorXd&)>& target_function)
        {
            target_function_ = target_function;
        }

        void setUpperBound(const Eigen::VectorXd& upper_bound)
        {
            upper_bound_ = upper_bound;
        }

        void setLowerBound(const Eigen::VectorXd& lower_bound)
        {
            lower_bound_ = lower_bound;
        }

        void setMaximumValue(const double maximum_value)
        {
            maximum_value_ = maximum_value;
        }

        void setMinimumValue(const double minimum_value)
        {
            minimum_value_ = minimum_value;
        }

    private:
        int target_dimension_;
        Eigen::VectorXd slider_values_;
        std::function<double(const Eigen::VectorXd&)> target_function_;

        Eigen::VectorXd upper_bound_;
        Eigen::VectorXd lower_bound_;
        double maximum_value_;
        double minimum_value_;
    };
}

#endif

