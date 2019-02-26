#include <visoptslider/visoptslider.hpp>
#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
#include <QBoxLayout>
#define TINYCOLORMAP_WITH_QT5
#include <tinycolormap.hpp>

namespace visopt
{
    SlidersWidget::SlidersWidget(QWidget* parent) :
    QGroupBox(parent)
    {
        setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    }

    void SlidersWidget::initialize(const int num_dimensions,
                                   const std::function<double(const Eigen::VectorXd&)>& target_function,
                                   const Eigen::VectorXd& upper_bound,
                                   const Eigen::VectorXd& lower_bound,
                                   const double maximum_value,
                                   const double minimum_value)
    {
        assert(num_dimensions == upper_bound.rows());
        assert(num_dimensions == lower_bound.rows());
        assert(maximum_value > minimum_value);

        for (int dimension = 0; dimension < num_dimensions; ++ dimension)
        {
            sliders_.push_back(new QSlider(Qt::Horizontal));
            visualizations_widgets_.push_back(new internal::VisualizationWidget(dimension, this));

            connect(sliders_[dimension], &QSlider::valueChanged, this, [&](){ this->slidersManipulatedViaGui(); });

            this->layout()->addWidget(sliders_[dimension]);
            this->layout()->addWidget(visualizations_widgets_[dimension]);
        }

        setNumDimensions(num_dimensions);
        setTargetFunction(target_function);
        setUpperBound(upper_bound);
        setLowerBound(lower_bound);
        setMaximumValue(maximum_value);
        setMinimumValue(minimum_value);

        setArgument(0.5 * (upper_bound + lower_bound));
    }

    void SlidersWidget::slidersManipulatedViaGui()
    {
        const Eigen::VectorXd x = calculateArgumentFromCurrentSliders();
    }

    Eigen::VectorXd SlidersWidget::calculateArgumentFromCurrentSliders() const
    {
        Eigen::VectorXd normalized_argument(num_dimensions_);
        for (int dimension = 0; dimension < num_dimensions_; ++ dimension)
        {
            const auto slider = sliders_[dimension];
            const int  v      = slider->value();
            const int  v_min  = slider->minimum();
            const int  v_max  = slider->maximum();

            normalized_argument(dimension) = static_cast<double>(v - v_min) / static_cast<double>(v_max - v_min);
        }

        return normalized_argument.cwiseProduct(upper_bound_ - lower_bound_) + lower_bound_;
    }

    namespace internal
    {
        VisualizationWidget::VisualizationWidget(const int target_dimension, SlidersWidget* parent) :
        QWidget(parent),
        target_dimension_(target_dimension),
        parent_widget_(parent)
        {
            constexpr int minimum_height = 32;
            constexpr int minimum_width = 200;

            setMinimumHeight(minimum_height);
            setMinimumWidth(minimum_width);
        }

        void VisualizationWidget::paintEvent(QPaintEvent* event)
        {
            QPainter painter;

            // Set design parameters
            QPen indicator_pen(QColor(0x20, 0x20, 0x20));
            indicator_pen.setWidth(2);

            QPen boundary_pen(QColor(0x20, 0x20, 0x20));
            boundary_pen.setWidth(4);

            constexpr double indicator_width = 10.0;

            const int gradient_resolution = parent_widget_->getGradientResolution();
            const Eigen::VectorXd& x = parent_widget_->getArgument();
            const Eigen::VectorXd& upper = parent_widget_->getUpperBound();
            const Eigen::VectorXd& lower = parent_widget_->getLowerBound();
            const double maximum_value = parent_widget_->getMaximumValue();
            const double minimum_value = parent_widget_->getMinimumValue();

            auto original_to_scaled_converter = [&](const Eigen::VectorXd& x)
            {
                return (x - lower).cwiseProduct((upper - lower).cwiseInverse());
            };

            auto scaled_to_original_converter = [&](const Eigen::VectorXd& x_scaled)
            {
                return x_scaled.cwiseProduct(upper - lower) + lower;
            };

            const Eigen::VectorXd& x_scaled = original_to_scaled_converter(x);

            // Begin
            painter.begin(this);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::HighQualityAntialiasing);
            painter.setRenderHint(QPainter::TextAntialiasing);

            const int w = event->rect().width();
            const int h = event->rect().height();

            // Draw gradation
            const int gradient_width = w / gradient_resolution;
            for (int i = gradient_width / 2; i < w; i += gradient_width)
            {
                Eigen::VectorXd x_scaled_temp = x_scaled;
                x_scaled_temp(target_dimension_) = static_cast<double>(i) / static_cast<double>(w - 1);
                double value = parent_widget_->calculateValue(scaled_to_original_converter(x_scaled_temp));

                // Normalize the value into [0, 1]
                value = (value - minimum_value) / (maximum_value - minimum_value);
                value = std::isnan(value) ? 0.5 : value;

                // Get mapped color
                const QColor color = tinycolormap::GetColor(value).ConvertToQColor();

                // Draw
                painter.fillRect(i - gradient_width / 2, 0, gradient_width * 2, h, color);
            }

            // Draw current position
            const double indicator_position = x_scaled[target_dimension_] * static_cast<double>(w);
            const QRectF indicator_rect(indicator_position - indicator_width / 2.0, 0.0, indicator_width, static_cast<double>(h));
            painter.setPen(indicator_pen);
            painter.drawRect(indicator_rect);

            // Draw boundary
            painter.setPen(boundary_pen);
            painter.drawRect(event->rect());

            // End
            painter.end();
        }
    }
}
