# VisOpt Slider

Qt-based implementation of VisOpt Slider widget [UIST 2014]

## Example

```cpp
#include <QApplication>
#include <visoptslider/visoptslider.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define a target function
    constexpr int target_dimension = 3;
    constexpr auto target_function = [](const Eigen::VectorXd& x)
    {
        return 1.0 - x.norm();
    };

    // Define a target bound
    const Eigen::Vector3d upper_bound(+ 1.0, + 1.0, + 1.0);
    const Eigen::Vector3d lower_bound(- 1.0, - 1.0, - 1.0);
    constexpr double maximum_value = 1.0;
    constexpr double minimum_value = 0.0;

    // Instantiate and initialize VisOpt Slider
    visopt::SlidersWidget sliders_widget;
    sliders_widget.initialize(target_dimension,
                              target_function,
                              upper_bound,
                              lower_bound,
                              maximum_value,
                              minimum_value);

    // Show VisOpt Slider
    sliders_widget.show();

    return app.exec();
}
```
