from PySide2.QtWidgets import QApplication, QGridLayout, QGroupBox, QLabel, QSlider, QWidget
from PySide2.QtCore import Qt
import numpy as np


class SliderWidget(QGroupBox):
    def __init__(self, parent):
        QGroupBox.__init__(self, parent)

    def slidersManipulatedViaGui(self):
        print("slidersManipulatedViaGui")

    def initialize(self,
                   num_dimensions,
                   target_function,
                   upper_bound,
                   lower_bound,
                   maximum_value,
                   minimum_value,
                   labels=[],
                   show_values=False):
        assert num_dimensions == upper_bound.shape[0]
        assert num_dimensions == lower_bound.shape[0]
        assert maximum_value > minimum_value
        assert len(labels) == 0 or len(labels) == num_dimensions

        has_labels = len(labels) != 0

        # Initialize the widget layout
        grid_layout = QGridLayout()
        self.setLayout(grid_layout)

        sliders = []
        visualization_widgets = []

        # Instantiate widgets
        for dimension in range(num_dimensions):
            # Instantiate a slider and set a callback
            slider = QSlider(Qt.Horizontal, self)
            slider.valueChanged.connect(self.slidersManipulatedViaGui)
            sliders.append(slider)
            grid_layout.addWidget(slider, dimension * 2, 1)

            # Instantiate a visualization widget
            visualization_widget = VisualizationWidget(dimension, self)
            visualization_widgets.append(visualization_widget)
            grid_layout.addWidget(visualization_widget, dimension * 2 + 1, 1)

            # Instantiate a parameter label widget (if requested)
            if has_labels:
                grid_layout.addWidget(QLabel(labels[dimension]), dimension * 2, 0)

            # Instantiate a value label widget (if requested)
            if show_values:
                # TODO
                pass

            # Change the slider's resolution (this needs to be done after adding all the widgets)
            slider.setMaximum(slider.width())

        # TODO


class VisualizationWidget(QWidget):
    def __init__(self, dimension, parent):
        QWidget.__init__(self, parent)


if __name__ == "__main__":
    import sys

    app = QApplication()

    num_dimensions = 3

    def target_function(x):
        value = 0.0
        for i in range(x.shape[0] - 1):
            value += 100.0 * (x[i + 1] - x[i] * x[i]) * (
                x[i + 1] - x[i] * x[i]) + (1.0 - x[i]) * (1.0 - x[i])
        return value

    upper_bound = np.array([+2.0, +2.0, +2.0])
    lower_bound = np.array([-2.0, -2.0, -2.0])
    maximum_value = 200.0
    minimum_value = 0.0

    labels = ["x1", "x2", "x3"]
    show_values = True

    sliders_widget = SliderWidget(None)
    sliders_widget.initialize(num_dimensions=num_dimensions,
                              target_function=target_function,
                              upper_bound=upper_bound,
                              lower_bound=lower_bound,
                              maximum_value=maximum_value,
                              minimum_value=minimum_value,
                              labels=labels,
                              show_values=show_values)

    sliders_widget.show()
    sys.exit(app.exec_())
