# Standard imports
import sys, os, imp, PySide2

from PySide2.QtWidgets import QWidget, QDialog, QMainWindow, QVBoxLayout, QMenu
from PySide2.QtCore import Qt, QFile, QSortFilterProxyModel, QSettings
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
        
        # Load UI from .ui file
        loader = QUiLoader()
        ui_file_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'assettracker.ui')
        ui_file = QFile(ui_file_path)
        ui_file.open(QFile.ReadOnly)
        self.ui = loader.load(ui_file, self)
        ui_file.close()
        self.setCentralWidget(self.ui)
        
        # Set events
        self.ui.treeView.setContextMenuPolicy(Qt.CustomContextMenu)
        self.ui.treeView.customContextMenuRequested.connect(self.openMenu)

        # Load settings
        self.settings = QSettings("MaxExtended", "BetterAssetTracker")
        self.readSettings()

    def writeSettings(self):
        self.settings.beginGroup("mainWindow")
        self.settings.setValue("pos", self.MainWindow.pos())
        self.settings.setValue("maximized", self.MainWindow.isMaximized())
        if not self.MainWindow.isMaximized():
            self.settings.setValue("size", self.MainWindow.size())

        self.settings.endGroup()

    def readSettings(self):
        self.settings.beginGroup("mainWindow")
        # No need for toPoint, etc. : PySide converts types
        try:
            self.MainWindow.move(self.settings.value("pos"))
            if self.settings.value("maximized") in 'true':
                self.MainWindow.showMaximized()
            else:
                self.MainWindow.resize(self.settings.value("size"))
        except:
            pass
        self.settings.endGroup()
        
    # https://wiki.python.org/moin/PyQt/Creating%20a%20context%20menu%20for%20a%20tree%20view
    def openMenu(self, position):
        # This is getting the selected indexes of the proxy model attached
        # to the tree view. This is not to be confused with the source
        # model, which is the actual AssetListModel.
        indexes = self.ui.treeView.selectedIndexes()
        context = None

        if len(indexes) == 0:
            return

        # Get the actual item(s) selected
        items = []
        for proxyIndex in indexes:
            proxyModel = self.ui.treeView.model()
            sourceIndex = proxyModel.mapToSource(proxyIndex)
            sourceModel = proxyModel.sourceModel()
            item = sourceModel.getItem(sourceIndex)

            items.append(item)

        # Get the depth of the item(s) selected
        if len(indexes) > 0:
            depth = 0
            index = indexes[0]
            while index.parent().isValid():
                index = index.parent()
                depth += 1

        # Create a new menu
        menu = QMenu()

        # Depending on the depth of the selection, add different actions
        if depth == 0:
            print(items)
            menu.addAction(self.tr("Reveal in explorer..."))
            menu.addAction(self.tr("Set path..."))

        if depth == 1:
            menu.addAction(self.tr("Select all children"))

        if depth == 2:
            if item.context() == "Materials":
                menu.addAction(self.tr("Open in SME"))

            if item.context() == "Geometry":
                menu.addAction(self.tr("Select object(s)"))

            if item.context() == "Modifiers":
                menu.addAction(self.tr("Select parent object(s)"))

        menu.exec_(self.ui.treeView.viewport().mapToGlobal(position))

    def closeEvent(self, args):
        print(args)
        self.writeSettings()

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
