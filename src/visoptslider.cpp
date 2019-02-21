#include <visoptslider/visoptslider.hpp>
#include <QSlider>
#include <QBoxLayout>

namespace visopt
{
    SlidersWidget::SlidersWidget(QWidget* parent) :
    QGroupBox(parent)
    {
        QBoxLayout *sliders_layout = new QBoxLayout(QBoxLayout::TopToBottom);

        sliders_layout->addWidget(new QSlider(Qt::Horizontal));
        sliders_layout->addWidget(new internal::VisualizationWidget(0, this));
        sliders_layout->addWidget(new QSlider(Qt::Horizontal));
        sliders_layout->addWidget(new internal::VisualizationWidget(1, this));
        sliders_layout->addWidget(new QSlider(Qt::Horizontal));
        sliders_layout->addWidget(new internal::VisualizationWidget(2, this));

        setLayout(sliders_layout);
    }

    void SlidersWidget::initialize(const int target_dimension,
                                   const std::function<double(const Eigen::VectorXd&)>& target_function,
                                   const Eigen::VectorXd& upper_bound,
                                   const Eigen::VectorXd& lower_bound,
                                   const double maximum_value,
                                   const double minimum_value)
    {
        assert(target_dimension == upper_bound.rows());
        assert(target_dimension == lower_bound.rows());
        assert(maximum_value > minimum_value);

        setTargetDimension(target_dimension);
        setTargetFunction(target_function);
        setUpperBound(upper_bound);
        setLowerBound(lower_bound);
        setMaximumValue(maximum_value);
        setMinimumValue(minimum_value);

        setArgument(0.5 * (upper_bound + lower_bound));
    }

    namespace internal
    {
        VisualizationWidget::VisualizationWidget(const int target_dimension, SlidersWidget* parent) :
        QWidget(parent),
        target_dimension_(target_dimension),
        parent_widget_(parent)
        {
            setMinimumHeight(32);
        }

        void VisualizationWidget::paintEvent(QPaintEvent* event)
        {

        }
    }
}
