#include <visoptslider/visoptslider.hpp>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
#include <tinycolormap.hpp>

namespace visopt
{
    SlidersWidget::SlidersWidget(QWidget* parent) :
    QGroupBox(parent)
    {
    }

    void SlidersWidget::initialize(const int num_dimensions,
                                   const std::function<double(const Eigen::VectorXd&)>& target_function,
                                   const Eigen::VectorXd& upper_bound,
                                   const Eigen::VectorXd& lower_bound,
                                   const double maximum_value,
                                   const double minimum_value,
                                   const std::vector<std::string>& labels,
                                   const bool show_values)

    {
        assert(num_dimensions == upper_bound.rows());
        assert(num_dimensions == lower_bound.rows());
        assert(maximum_value > minimum_value);
        assert(labels.empty() || labels.size() == num_dimensions);

        const bool has_labels = !labels.empty();

        // Initialize the widget layout
        QGridLayout* grid_layout = new QGridLayout();
        setLayout(grid_layout);

        // Instantiate widgets
        for (int dimension = 0; dimension < num_dimensions; ++ dimension)
        {
            // Instantiate a slider and set a callback
            QSlider* slider = new QSlider(Qt::Horizontal, this);
            connect(slider, &QSlider::valueChanged, this, [&](){ this->slidersManipulatedViaGui(); });
            sliders_.push_back(slider);
            grid_layout->addWidget(sliders_[dimension], dimension * 2, 1);

            // Instantiate a visualization widget
            visualizations_widgets_.push_back(new internal::VisualizationWidget(dimension, this));
            grid_layout->addWidget(visualizations_widgets_[dimension], dimension * 2 + 1, 1);

            // Instantiate a parameter label widget (if requested)
            if (has_labels)
            {
                grid_layout->addWidget(new QLabel(QString::fromStdString(labels[dimension])), dimension * 2, 0);
            }

            // Instantiate a value label widget (if requested)
            if (show_values)
            {
                QFont font("");
                font.setStyleHint(QFont::Monospace);

                QLineEdit* line_edit = new QLineEdit();
                line_edit->setReadOnly(true);
                line_edit->setFixedWidth(46);
                line_edit->setMaxLength(5);
                line_edit->setFont(font);
                value_labels_.push_back(line_edit);
                grid_layout->addWidget(line_edit, dimension * 2, 2);
            }

            // Change the slider's resolution (this needs to be done after adding all the widgets)
            slider->setMaximum(slider->width());
        }

        setNumDimensions(num_dimensions);
        setTargetFunction(target_function);
        setUpperBound(upper_bound);
        setLowerBound(lower_bound);
        setMaximumValue(maximum_value);
        setMinimumValue(minimum_value);

        setArgumentAndUpdateSliders(0.5 * (upper_bound + lower_bound));
    }

    void SlidersWidget::setArgumentAndUpdateSliders(const Eigen::VectorXd& argument)
    {
        argument_ = argument;
        setLabelsUsingCurrentArgument();
        setSliderValuesUsingCurrentArgument();
    }

    void SlidersWidget::slidersManipulatedViaGui()
    {
        argument_ = calculateArgumentFromCurrentSliders();
        setLabelsUsingCurrentArgument();
        update();
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

    void SlidersWidget::setSliderValuesUsingCurrentArgument()
    {
        const Eigen::VectorXd normalized_argument = (argument_ - lower_bound_).cwiseProduct((upper_bound_ - lower_bound_).cwiseInverse());

        for (int dimension = 0; dimension < num_dimensions_; ++ dimension)
        {
            const auto slider = sliders_[dimension];
            const int  v_min  = slider->minimum();
            const int  v_max  = slider->maximum();
            const int  v      = static_cast<int>(normalized_argument(dimension) * static_cast<double>(v_max - v_min)) + v_min;

            slider->blockSignals(true);
            slider->setValue(v);
            slider->blockSignals(false);
        }
    }

    void SlidersWidget::setLabelsUsingCurrentArgument()
    {
        if (value_labels_.empty()) { return; }

        for (int dimension = 0; dimension < num_dimensions_; ++ dimension)
        {
            const QString label = QString((argument_[dimension] >= 0.0) ? "+" : "") + QString::number(argument_[dimension]);
            value_labels_[dimension]->setText(label);
        }
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

            const int resolution = parent_widget_->getResolution();
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
            const int gradation_width = w / resolution;
            for (int i = gradation_width / 2; i < w; i += gradation_width)
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
                painter.fillRect(i - gradation_width / 2, 0, gradation_width * 2, h, color);
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
