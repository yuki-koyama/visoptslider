#include <string>
#include <vector>
#include <Eigen/Core>
#include <QApplication>
#include <visoptslider/visoptslider.hpp>
#include <optimization-test-functions.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define the target function and bound
    constexpr int target_dimension = 3;
    constexpr auto target_function = [](const Eigen::VectorXd& x)
    {
        return otf::GetValue(x, otf::FunctionType::Rosenbrock);
    };
    const Eigen::Vector3d upper_bound(+ 2.0, + 2.0, + 2.0);
    const Eigen::Vector3d lower_bound(- 2.0, - 2.0, - 2.0);
    constexpr double maximum_value = 200.0;
    constexpr double minimum_value = 0.0;

    // Optional settings
    const std::vector<std::string> labels = { "x1", "x2", "x3" };
    constexpr bool show_values = true;

    // Instantiate and initialize the widget
    visopt::SlidersWidget sliders_widget;
    sliders_widget.initialize(target_dimension,
                              target_function,
                              upper_bound,
                              lower_bound,
                              maximum_value,
                              minimum_value,
                              labels,
                              show_values);

    // Show the widget
    sliders_widget.show();
    return app.exec();
}
