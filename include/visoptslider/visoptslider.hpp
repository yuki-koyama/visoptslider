#ifndef visoptslider_hpp
#define visoptslider_hpp

#include <vector>
#include <Eigen/Core>
#include <QGroupBox>

class QSlider;
namespace visopt { namespace internal { class VisualizationWidget; } }

namespace visopt
{
    class SlidersWidget : public QGroupBox
    {
    public:
        SlidersWidget(QWidget* parent = nullptr);

        void initialize(const int num_dimensions,
                        const std::function<double(const Eigen::VectorXd&)>& target_function,
                        const Eigen::VectorXd& upper_bound,
                        const Eigen::VectorXd& lower_bound,
                        const double maximum_value,
                        const double minimum_value);

        void setNumDimensions(const int num_dimensions)
        {
            num_dimensions_ = num_dimensions;
        }

        const Eigen::VectorXd& getArgument() const { return argument_; }
        void setArgument(const Eigen::VectorXd& argument)
        {
            argument_ = argument;
        }

        void setTargetFunction(const std::function<double(const Eigen::VectorXd&)>& target_function)
        {
            target_function_ = target_function;
        }

        const Eigen::VectorXd& getUpperBound() const { return upper_bound_; }
        void setUpperBound(const Eigen::VectorXd& upper_bound)
        {
            upper_bound_ = upper_bound;
        }

        const Eigen::VectorXd& getLowerBound() const { return lower_bound_; }
        void setLowerBound(const Eigen::VectorXd& lower_bound)
        {
            lower_bound_ = lower_bound;
        }

        double getMaximumValue() const { return maximum_value_; }
        void setMaximumValue(const double maximum_value)
        {
            maximum_value_ = maximum_value;
        }

        double getMinimumValue() const { return minimum_value_; }
        void setMinimumValue(const double minimum_value)
        {
            minimum_value_ = minimum_value;
        }

        int getGradientResolution() const { return gradient_resolution_; }

        double calculateValue(const Eigen::VectorXd& argument) const
        {
            return target_function_(argument);
        }

    private:
        void slidersManipulatedViaGui();

        const int gradient_resolution_ = 50;

        int num_dimensions_;
        Eigen::VectorXd argument_;
        std::function<double(const Eigen::VectorXd&)> target_function_;

        Eigen::VectorXd upper_bound_;
        Eigen::VectorXd lower_bound_;
        double maximum_value_;
        double minimum_value_;

        std::vector<QSlider*> sliders_;
        std::vector<internal::VisualizationWidget*> visualizations_widgets_;
    };

    namespace internal
    {
        class VisualizationWidget : public QWidget
        {
        public:
            VisualizationWidget(const int target_dimension, SlidersWidget* parent);

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            const int target_dimension_;
            const SlidersWidget* parent_widget_ = nullptr;
        };
    }
}

#endif
