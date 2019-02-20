#include <visoptslider/visoptslider.hpp>
#include <QSlider>
#include <QBoxLayout>

namespace visopt
{
    SlidersWidget::SlidersWidget(QWidget* parent)
    : QGroupBox(parent)
    {
        QBoxLayout *sliders_layout = new QBoxLayout(QBoxLayout::TopToBottom);

        sliders_layout->addWidget(new QSlider(Qt::Horizontal));
        sliders_layout->addWidget(new QSlider(Qt::Horizontal));
        sliders_layout->addWidget(new QSlider(Qt::Horizontal));

        setLayout(sliders_layout);
    }

    namespace internal
    {
        void VisualizationWidget::paintEvent(QPaintEvent* event)
        {

        }
    }
}
