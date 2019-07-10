from PySide2.QtWidgets import QApplication, QWidget, QGroupBox

class SliderWidget(QGroupBox):
    def __init__(self, parent):
        QGroupBox.__init__(self, parent)

class VisualizationWidget(QWidget):
    def __init__(self, parent):
        QWidget.__init__(self, parent)

if __name__ == "__main__":
    import sys

    app = QApplication()
    widget = SliderWidget(None)
    widget.show()
    sys.exit(app.exec_())
