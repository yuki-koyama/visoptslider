# VisOpt Slider

Qt-based implementation of VisOpt Slider widget [UIST 2014]

![Interactive exploration of a 3-dimensional Rosenbrock function](./docs/3d_rosenbrock.gif "Interactive exploration of a 3-dimensional Rosenbrock function.")

## Features

VisOpt Slider is a GUI widget consisting of multiple sliders. It is specifically designed for interactive exploration of a high-dimensional scalar-valued function. It has the following special features.

### Visualization

VisOpt Slider visualizes the values of the target function along with the sliders in the interface using a colormap.

### Optimization

_Not available yet_. Please refer to the original paper [(Koyama et al. 2014)](https://koyama.xyz/project/CrowdPoweredAnalysis/) and its extended version [(Koyama et al. 2016)](https://koyama.xyz/project/SelPh/).

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

## Dependencies

### Prerequisites

- Eigen (e.g., `brew install eigen`)
- Qt5 (e.g., `brew install qt`)

### Included as Submodules

- tinycolormap <https://github.com/yuki-koyama/tinycolormap>
- optimization-test-functions <https://github.com/yuki-koyama/optimization-test-functions>

## References

- Yuki Koyama, Daisuke Sakamoto, and Takeo Igarashi. 2014. __Crowd-Powered Parameter Analysis for Visual Design Exploration__. In _Proceedings of the 27th annual ACM symposium on User interface software and technology_ (UIST '14), pp.65-74. DOI: <https://doi.org/10.1145/2642918.2647386>
  - Project page: <https://koyama.xyz/project/CrowdPoweredAnalysis/>
- Yuki Koyama, Daisuke Sakamoto, and Takeo Igarashi. 2016. __SelPh: Progressive Learning and Support of Manual Photo Color Enhancement__. In _Proceedings of the 2016 CHI Conference on Human Factors in Computing Systems_ (CHI '16), pp.2520--2532. DOI: <https://doi.org/10.1145/2858036.2858111>
  - Project page: <https://koyama.xyz/project/SelPh/>

## License

MIT License.
