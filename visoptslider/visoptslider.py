from PySide2.QtWidgets import QApplication, QWidget

if __name__ == "__main__":
    import sys

    app = QApplication()
    widget = QWidget(None)
    widget.show()
    sys.exit(app.exec_())
