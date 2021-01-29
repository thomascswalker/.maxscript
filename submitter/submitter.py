# Standard imports
import sys, os, imp, PySide2

from PySide2.QtWidgets import QWidget, QDialog, QMainWindow, QVBoxLayout, QMenu
from PySide2.QtCore import Qt, QFile, QSortFilterProxyModel, QSettings
from PySide2.QtUiTools import QUiLoader
from pymxs import runtime as rt

class SubmitterDialog(QMainWindow):
    def __init__(self, parent=QWidget.find(rt.windows.getMAXHWND())):
        QMainWindow.__init__(self, parent)
        
        # Load UI from .ui file
        loader = QUiLoader()
        ui_file_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'submitter.ui')
        ui_file = QFile(ui_file_path)
        ui_file.open(QFile.ReadOnly)
        self.ui = loader.load(ui_file, self)
        ui_file.close()
        self.setCentralWidget(self.ui)

def main():
    # Try to close any existing dialogs so there aren't
    # duplicates open.
    global pySubmitterDialog
    try:
        pySubmitterDialog.ui.close()
    except:
        pass

    # Instantiate the main dialog
    pySubmitterDialog = SubmitterDialog()
    ui = pySubmitterDialog.ui

    # Show the UI
    ui.show()
    ui.setWindowTitle("Submitter")

if __name__ == '__main__':
    main()
