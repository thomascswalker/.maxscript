import sys, os, imp, PySide2

from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QWidget
from PySide2.QtWidgets import QDialog
from PySide2.QtWidgets import QMainWindow
from PySide2.QtWidgets import QPushButton
from PySide2.QtCore import QFile
from PySide2.QtUiTools import QUiLoader
from pymxs import runtime as rt

sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import assetlistmodel
from assetlistmodel import AssetListModel
imp.reload(assetlistmodel)

class AssetTrackerDialog(QMainWindow):
    def __init__(self, parent=QWidget.find(rt.windows.getMAXHWND())):
        QMainWindow.__init__(self, parent)
        loader = QUiLoader()
        ui_file_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'assettracker.ui')
        ui_file = QFile(ui_file_path)
        ui_file.open(QFile.ReadOnly)
        self.ui = loader.load(ui_file, self)
        ui_file.close()
        self.setCentralWidget(self.ui)

def main():
    dialog = AssetTrackerDialog()
    ui = dialog.ui

    treeModel = AssetListModel()
    ui.treeView.setModel(treeModel)

    ui.show()
    ui.setWindowTitle("Asset Tracker")

if __name__ == '__main__':
    main()
