#include <QApplication>
#include <visoptslider/visoptslider.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    visopt::SlidersWidget sliders_widget;
    sliders_widget.show();
    return app.exec();
}
