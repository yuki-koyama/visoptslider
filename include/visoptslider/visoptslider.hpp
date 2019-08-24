#ifndef visoptslider_hpp
#define visoptslider_hpp

#include <vector>
#include <Eigen/Core>
#include <QGroupBox>

class QLineEdit;
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
                        const double minimum_value,
                        const std::vector<std::string>& labels = {},
                        const bool show_values = false,
                        const int resolution = 200,
                        const int visualization_minimum_width = 200,
                        const int visualization_minimum_height = 32);

        int getNumDimensions() const { return num_dimensions_; }
        void setNumDimensions(const int num_dimensions)
        {
            num_dimensions_ = num_dimensions;
        }

        const Eigen::VectorXd& getArgument() const { return argument_; }

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

        int getResolution() const { return resolution_; }
        void setResolution(const int resolution)
        {
            resolution_ = resolution;
        }

        double calculateValue(const Eigen::VectorXd& argument) const
        {
            return target_function_(argument);
        }

        void setArgumentAndUpdateSliders(const Eigen::VectorXd& argument);

        void setCallback(const std::function<void(void)>& callback)
        {
            callback_ = callback;
        }

        void setVisualizationMinimumSize(const int minimum_width, const int minimum_height);

    private:
        void slidersManipulatedViaGui();
        Eigen::VectorXd calculateArgumentFromCurrentSliders() const;
        void setSliderValuesUsingCurrentArgument();
        void setLabelsUsingCurrentArgument();

        int resolution_;

        int num_dimensions_;
        Eigen::VectorXd argument_;
        std::function<double(const Eigen::VectorXd&)> target_function_;

        Eigen::VectorXd upper_bound_;
        Eigen::VectorXd lower_bound_;
        double maximum_value_;
        double minimum_value_;

        std::vector<QSlider*> sliders_;
        std::vector<internal::VisualizationWidget*> visualizations_widgets_;
        std::vector<QLineEdit*> value_labels_;

        std::function<void(void)> callback_;
    };

    namespace internal
    {
        class VisualizationWidget : public QWidget
        {
        public:
            VisualizationWidget(const int target_dimension,
                                SlidersWidget* parent,
                                const int minimum_width,
                                const int minimum_height);

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            const int target_dimension_;
            const SlidersWidget* parent_widget_ = nullptr;
        };
    }
}

#endif
