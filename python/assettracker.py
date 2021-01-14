# Standard imports
import sys, os, imp, PySide2

from PySide2.QtWidgets import QWidget, QDialog, QMainWindow, QVBoxLayout
from PySide2.QtCore import QFile, QSortFilterProxyModel
from PySide2.QtUiTools import QUiLoader
from pymxs import runtime as rt

# Local imports
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import assetlistmodel
try:
    # Try to reload all modules, if this doesn't work
    # it'll crash
    imp.reload(assetlistmodel)
except:
    pass
from assetlistmodel import AssetListModel

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
    # Try to close any existing dialogs so there aren't
    # duplicates open.
    global pyAssetTrackerDialog
    try:
        pyAssetTrackerDialog.ui.close()
    except:
        pass

    # Instantiate the main dialog
    pyAssetTrackerDialog = AssetTrackerDialog()
    ui = pyAssetTrackerDialog.ui

    # Create the source model, but map it to a proxy model to enable
    # sorting, filtering, etc.
    sourceModel = AssetListModel()
    proxyModel = QSortFilterProxyModel()
    proxyModel.setSourceModel(sourceModel)

    # Assign the proxy model to the tree view
    ui.treeView.setModel(proxyModel)

    # Show the UI
    ui.show()
    ui.setWindowTitle("Better Asset Tracker")

if __name__ == '__main__':
    main()
