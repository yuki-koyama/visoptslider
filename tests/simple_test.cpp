#include <QApplication>
#include <visoptslider/visoptslider.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    visopt::SlidersWidget sliders_widget;
    sliders_widget.setMinimumValue(0.0);
    sliders_widget.setMaximumValue(1.0);
    sliders_widget.setLowerBound(Eigen::Vector3d(- 1.0, - 1.0, - 1.0));
    sliders_widget.setUpperBound(Eigen::Vector3d(+ 1.0, + 1.0, + 1.0));
    sliders_widget.setArgument(Eigen::Vector3d(0.4, 0.2, 0.6));
    sliders_widget.setTargetFunction([](const Eigen::VectorXd& x)
                                     {
                                         return x.norm();
                                     });
    sliders_widget.show();
    return app.exec();
}
