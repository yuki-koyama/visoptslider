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
