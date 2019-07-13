# VisOpt Slider

![GitHub](https://img.shields.io/github/license/yuki-koyama/visoptslider.svg)
![PyPI](https://img.shields.io/pypi/v/visoptslider.svg)

Qt-based implementation of VisOpt Slider widget [UIST 2014]

If your applications are based on Qt (PySide2), it is quite easy to integrate a VisOpt Slider widget into your applications.

## Features

VisOpt Slider is a GUI widget consisting of multiple sliders. It is specifically designed for interactive exploration of a high-dimensional scalar-valued function. It has the following special features.

- __Visualization__: VisOpt Slider visualizes the values of the target function along with the sliders in the interface using a colormap.
- __Optimization__: _Not available yet_. Please refer to the original paper [(Koyama et al. 2014)](https://koyama.xyz/project/CrowdPoweredAnalysis/) and its extended version [(Koyama et al. 2016)](https://koyama.xyz/project/SelPh/).

## Install

This package can be install via `pip`:
```
pip install visoptslider
```
By this, the dependencies (`matplotlib`, `numpy`, `PySide2`, and their dependencies) will be automatically installed together.

## Example

```python
from PySide2.QtWidgets import QApplication
import numpy as np
import visoptslider

if __name__ == "__main__":
    app = QApplication()

    # Define a target function
    num_dimensions = 3
    def target_function(x):
        return 1.0 - np.linalg.norm(x)

    # Define a target bound
    upper_bound = np.array([+1.0, +1.0, +1.0])
    lower_bound = np.array([-1.0, -1.0, -1.0])
    maximum_value = 1.0
    minimum_value = 0.0

    # Instantiate and initialize VisOpt Slider
    sliders_widget = visoptslider.SlidersWidget()
    sliders_widget.initialize(num_dimensions=num_dimensions,
                              target_function=target_function,
                              upper_bound=upper_bound,
                              lower_bound=lower_bound,
                              maximum_value=maximum_value,
                              minimum_value=minimum_value)

    # Show VisOpt Sliders
    sliders_widget.show()

    app.exec_()
```

## References

- Yuki Koyama, Daisuke Sakamoto, and Takeo Igarashi. 2014. __Crowd-Powered Parameter Analysis for Visual Design Exploration__. In _Proceedings of the 27th annual ACM symposium on User interface software and technology_ (UIST '14), pp.65-74. DOI: <https://doi.org/10.1145/2642918.2647386>
  - Project page: <https://koyama.xyz/project/CrowdPoweredAnalysis/>
- Yuki Koyama, Daisuke Sakamoto, and Takeo Igarashi. 2016. __SelPh: Progressive Learning and Support of Manual Photo Color Enhancement__. In _Proceedings of the 2016 CHI Conference on Human Factors in Computing Systems_ (CHI '16), pp.2520--2532. DOI: <https://doi.org/10.1145/2858036.2858111>
  - Project page: <https://koyama.xyz/project/SelPh/>
