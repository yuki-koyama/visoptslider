from matplotlib import cm
from PySide2.QtCore import QRectF, Qt
from PySide2.QtGui import QColor, QFont, QPainter, QPen
from PySide2.QtWidgets import QApplication, QGridLayout, QGroupBox, QLabel, QLineEdit, QSlider, QWidget
import math
import numpy as np


class SlidersWidget(QGroupBox):

    __sliders = []
    __visualization_widgets = []
    __value_labels = []

    def __init__(self, parent=None):
        QGroupBox.__init__(self, parent)

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

        # Instantiate widgets
        for dimension in range(num_dimensions):
            # Instantiate a slider and set a callback
            slider = QSlider(Qt.Horizontal, self)
            slider.valueChanged.connect(self.__sliders_manipulated_via_gui)
            self.__sliders.append(slider)
            grid_layout.addWidget(slider, dimension * 2, 1)

            # Instantiate a visualization widget
            visualization_widget = _VisualizationWidget(dimension, self)
            self.__visualization_widgets.append(visualization_widget)
            grid_layout.addWidget(visualization_widget, dimension * 2 + 1, 1)

            # Instantiate a parameter label widget (if requested)
            if has_labels:
                grid_layout.addWidget(QLabel(labels[dimension]), dimension * 2, 0)

            # Instantiate a value label widget (if requested)
            if show_values:
                font = QFont()
                font.setStyleHint(QFont.Monospace)
                line_edit = QLineEdit()
                line_edit.setReadOnly(True)
                line_edit.setFixedWidth(46)
                line_edit.setMaxLength(5)
                line_edit.setFont(font)
                self.__value_labels.append(line_edit)
                grid_layout.addWidget(line_edit, dimension * 2, 2)

            # Change the slider's resolution (this needs to be done after adding all the widgets)
            slider.setMaximum(slider.width())

        self.num_dimensions = num_dimensions
        self.target_function = target_function
        self.upper_bound = upper_bound
        self.lower_bound = lower_bound
        self.maximum_value = maximum_value
        self.minimum_value = minimum_value
        self.resolution = 200

        self.set_argument_and_update_sliders(0.5 * (upper_bound + lower_bound))

    def calculate_value(self, argument):
        return self.target_function(argument)

    def num_dimensions():
        doc = "The num_dimensions property."

        def fget(self):
            return self.__num_dimensions

        def fset(self, value):
            self.__num_dimensions = value

        def fdel(self):
            del self.__num_dimensions

        return locals()

    num_dimensions = property(**num_dimensions())

    def target_function():
        doc = "The target_function property."

        def fget(self):
            return self.__target_function

        def fset(self, value):
            self.__target_function = value

        def fdel(self):
            del self.__target_function

        return locals()

    target_function = property(**target_function())

    def upper_bound():
        doc = "The upper_bound property."

        def fget(self):
            return self.__upper_bound

        def fset(self, value):
            self.__upper_bound = value

        def fdel(self):
            del self.__upper_bound

        return locals()

    upper_bound = property(**upper_bound())

    def lower_bound():
        doc = "The lower_bound property."

        def fget(self):
            return self.__lower_bound

        def fset(self, value):
            self.__lower_bound = value

        def fdel(self):
            del self.__lower_bound

        return locals()

    lower_bound = property(**lower_bound())

    def maximum_value():
        doc = "The maximum_value property."

        def fget(self):
            return self.__maximum_value

        def fset(self, value):
            self.__maximum_value = value

        def fdel(self):
            del self.__maximum_value

        return locals()

    maximum_value = property(**maximum_value())

    def minimum_value():
        doc = "The minimum_value property."

        def fget(self):
            return self.__minimum_value

        def fset(self, value):
            self.__minimum_value = value

        def fdel(self):
            del self.__minimum_value

        return locals()

    minimum_value = property(**minimum_value())

    def argument():
        doc = "The argument property."

        def fget(self):
            return self.__argument

        def fdel(self):
            del self.__argument

        return locals()

    argument = property(**argument())

    def resolution():
        doc = "The resolution property."

        def fget(self):
            return self.__resolution

        def fset(self, value):
            self.__resolution = value

        def fdel(self):
            del self.__resolution

        return locals()

    resolution = property(**resolution())

    def set_argument_and_update_sliders(self, argument):
        self.__argument = argument
        self.__set_labels_using_current_argument()
        self.__set_slider_values_using_current_argument()

    def __sliders_manipulated_via_gui(self):
        self.__argument = self.__calculate_argument_from_current_sliders()
        self.__set_labels_using_current_argument()
        self.update()

    def __calculate_argument_from_current_sliders(self):
        normalized_argument = np.ndarray(self.num_dimensions)
        for dimension in range(self.num_dimensions):
            slider = self.__sliders[dimension]
            v = slider.value()
            v_min = slider.minimum()
            v_max = slider.maximum()

            normalized_argument[dimension] = float(v - v_min) / float(v_max - v_min)

        return np.multiply(normalized_argument, self.upper_bound - self.lower_bound) + self.lower_bound

    def __set_slider_values_using_current_argument(self):
        normalized_argument = np.divide(self.argument - self.lower_bound, self.upper_bound - self.lower_bound)

        for dimension in range(self.num_dimensions):
            slider = self.__sliders[dimension]
            v_min = slider.minimum()
            v_max = slider.maximum()
            v = int(normalized_argument[dimension] * float(v_max - v_min)) + v_min

            slider.blockSignals(True)
            slider.setValue(v)
            slider.blockSignals(False)

    def __set_labels_using_current_argument(self):
        if len(self.__value_labels) == 0:
            return

        for dimension in range(self.num_dimensions):
            label = ("+" if self.argument[dimension] >= 0.0 else "") + str(self.argument[dimension])
            self.__value_labels[dimension].setText(label)


class _VisualizationWidget(QWidget):

    __target_dimension = 0
    __parent_widget = None

    def __init__(self, dimension, parent):
        QWidget.__init__(self, parent)
        self.__target_dimension = dimension
        self.__parent_widget = parent

        MINIMUM_HEIGHT = 32
        MINIMUM_WIDTH = 200

        self.setMinimumHeight(MINIMUM_HEIGHT)
        self.setMinimumWidth(MINIMUM_WIDTH)

    def paintEvent(self, event):
        painter = QPainter(self)

        indicator_pen = QPen(QColor(0x20, 0x20, 0x20))
        indicator_pen.setWidth(2)

        boundary_pen = QPen(QColor(0x20, 0x20, 0x20))
        boundary_pen.setWidth(4)

        INDICATOR_WIDTH = 10.0

        resolution = self.__parent_widget.resolution
        x = self.__parent_widget.argument
        upper = self.__parent_widget.upper_bound
        lower = self.__parent_widget.lower_bound
        maximum_value = self.__parent_widget.maximum_value
        minimum_value = self.__parent_widget.minimum_value

        original_to_scaled_converter = lambda x: np.divide(x - lower, upper - lower)
        scaled_to_original_converter = lambda x_scaled: np.multiply(x_scaled, upper - lower) + lower

        x_scaled = original_to_scaled_converter(x)

        painter.setRenderHint(QPainter.Antialiasing)
        painter.setRenderHint(QPainter.HighQualityAntialiasing)
        painter.setRenderHint(QPainter.TextAntialiasing)

        w = event.rect().width()
        h = event.rect().height()

        # Draw gradation
        gradation_width = int(w / resolution)
        for i in range(int(gradation_width / 2), w, gradation_width):
            x_scaled_temp = np.copy(x_scaled)
            x_scaled_temp[self.__target_dimension] = float(i) / float(w - 1)
            value = self.__parent_widget.calculate_value(scaled_to_original_converter(x_scaled_temp))

            # Normalize the value into [0, 1]
            value = (value - minimum_value) / (maximum_value - minimum_value)
            if hasattr(math, "isfinite"): # Python 3.2 or later
                value = value if math.isfinite(value) else 0.5
            else:
                value = value if not (math.isnan(value) or math.isinf(value)) else 0.5

            # Get mapped color
            cmap = cm.get_cmap(name="viridis")
            rgba = cmap(value)
            color = QColor(int(rgba[0] * 255), int(rgba[1] * 255), int(rgba[2] * 255))

            # Draw
            painter.fillRect(i - gradation_width / 2, 0, gradation_width * 2, h, color)

        # Draw current position
        indicator_position = x_scaled[self.__target_dimension] * float(w)
        indicator_rect = QRectF(indicator_position - INDICATOR_WIDTH / 2.0, 0.0, INDICATOR_WIDTH, float(h))
        painter.setPen(indicator_pen)
        painter.drawRect(indicator_rect)

        # Draw boundary
        painter.setPen(boundary_pen)
        painter.drawRect(event.rect())
