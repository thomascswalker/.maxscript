# Standard imports
import os, sys, imp
from PySide2 import QtGui, QtCore, QtWidgets
from PySide2.QtCore import Qt, QSortFilterProxyModel
from PySide2.QtWidgets import QFileIconProvider
from pymxs import runtime as rt

# Local imports
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import assetlistitem
import assetfunctions
try:
    imp.reload(assetlistitem)
    imp.reload(assetfunctions)
except:
    pass
from assetlistitem import AssetListItem
from assetfunctions import AssetListFunctions

class AssetListModel(QtCore.QAbstractItemModel):
    def __init__(self, parent=None):
        super(AssetListModel, self).__init__(parent)

        # Define the visible headers in the main tree view
        headers = ("Name", "Ext", "Path", "Type", "Status")
        self._rootHeaders = [header for header in headers]

        # Build the root (invisible) item in the tree view.
        # This will allow the headers to be set and visible.
        self._rootItem = AssetListItem(self._rootHeaders)

        # Setup the rest of the model data
        self.setupModelData(self._rootItem)

    def columnCount(self, parent=QtCore.QModelIndex()):
        return self._rootItem.columnCount()

    def data(self, index, role):
        item = self.getItem(index)

        if not index.isValid():
            return None

        if role == QtCore.Qt.DecorationRole and index.column() == 0:
            return item.icon()

        if role != QtCore.Qt.DisplayRole and role != QtCore.Qt.EditRole:
            return None

        return item.data(index.column())

    def flags(self, index):
        if not index.isValid():
            return 0

        return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable

    def getItem(self, index):
        if index.isValid():
            item = index.internalPointer()
            if item:
                return item

        return self._rootItem

    def headerData(self, section, orientation, role):
        if role == Qt.DisplayRole and orientation == Qt.Horizontal:
            return self._rootItem.data(section)

        return None

    def index(self, row, column, parent=QtCore.QModelIndex()):
        if parent.isValid() and parent.column() != 0:
            return QtCore.QModelIndex()

        parentItem = self.getItem(parent)
        childItem = parentItem.child(row)
        if childItem:
            return self.createIndex(row, column, childItem)
        else:
            return QtCore.QModelIndex()

    def insertColumns(self, position, columns, parent=QtCore.QModelIndex()):
        self.beginInsertColumns(parent, position, position + columns - 1)
        success = self._rootItem.insertColumns(position, columns)
        self.endInsertColumns()

        return success

    def insertRows(self, position, rows, parent=QtCore.QModelIndex()):
        parentItem = self.getItem(parent)
        self.beginInsertRows(parent, position, position + rows - 1)
        success = parentItem.insertChildren(position, rows,
                self._rootItem.columnCount())
        self.endInsertRows()

        return success

    def parent(self, index):
        if not index.isValid():
            return QtCore.QModelIndex()

        childItem = self.getItem(index)
        parentItem = childItem.parent()

        if parentItem == self._rootItem:
            return QtCore.QModelIndex()

        return self.createIndex(parentItem.childNumber(), 0, parentItem)

    def removeColumns(self, position, columns, parent=QtCore.QModelIndex()):
        self.beginRemoveColumns(parent, position, position + columns - 1)
        success = self._rootItem.removeColumns(position, columns)
        self.endRemoveColumns()

        if self._rootItem.columnCount() == 0:
            self.removeRows(0, rowCount())

        return success

    def removeRows(self, position, rows, parent=QtCore.QModelIndex()):
        parentItem = self.getItem(parent)

        self.beginRemoveRows(parent, position, position + rows - 1)
        success = parentItem.removeChildren(position, rows)
        self.endRemoveRows()

        return success

    def rowCount(self, parent=QtCore.QModelIndex()):
        parentItem = self.getItem(parent)

        return parentItem.childCount()

    def setData(self, index, value, role=QtCore.Qt.EditRole):
        if role != QtCore.Qt.EditRole:
            return False

        item = self.getItem(index)
        result = item.setData(index.column(), value)

        if result:
            self.dataChanged.emit(index, index)

        return result

    def setupModelData(self, parent):
        functions = AssetListFunctions()
        parents = [parent]
        numAssets = rt.AssetManager.getNumAssets()

        # For each asset (row)
        for i in range(1, numAssets + 1):

            # Get the asset object
            asset         = rt.AssetManager.getAssetByIndex(i)

            # Get the asset properties
            assetFilename = asset.GetFilename()
            assetBasename = os.path.basename(assetFilename)
            assetName     = os.path.splitext(assetBasename)[0]
            assetPath     = os.path.dirname(assetFilename)
            assetExt      = os.path.splitext(assetBasename)[1]
            assetType     = str(asset.GetType())

            # We'll use QFileIconProvider to grab the icon that the OS
            # is currently using to grab the icon to display in the view
            fileInfo = QtCore.QFileInfo(assetFilename)
            iconProvider = QFileIconProvider()
            assetIcon = iconProvider.icon(fileInfo)

            # Get the references associated with the asset filename
            assetRefs = functions.getAssetRefs(assetFilename)

            # Read the column data from the rest of the line.
            columnData = [assetName, assetExt, assetPath, assetType]

            # Append a new item to the current parent's list of children.
            parent = parents[-1]
            parent.insertChildren(parent.childCount(), 1, self._rootItem.columnCount())
            node = parent.child(parent.childCount() - 1)

            # Fill each column in the current row
            for column in range(len(columnData)):
                node.setData(column, columnData[column])
                node.setIcon(assetIcon)

            # Insert three children to the new node for the referenced
            # materials, geometry, and modifiers
            if len(assetRefs) > 0:
                if len(assetRefs["materials"]) > 0:
                    node.insertChildren(0, 1, self._rootItem.columnCount())
                    materialNode = node.child(0)
                    if (materialNode != None):
                        materialNode.setData(0, "Materials")
                        for i in range(len(assetRefs["materials"])):
                            assetType = assetRefs["materials"][i]
                            materialNode.insertChildren(i, 1, self._rootItem.columnCount())
                            refNode = materialNode.child(i)
                            refNode.setData(0, str(assetType))

                if len(assetRefs["geometry"]) > 0:
                    node.insertChildren(1, 1, self._rootItem.columnCount())
                    geometryNode = node.child(1)
                    if (geometryNode != None):
                        geometryNode.setData(0, "Geometry")
                        for i in range(len(assetRefs["geometry"])):
                            assetType = assetRefs["geometry"][i]
                            geometryNode.insertChildren(i, 1, self._rootItem.columnCount())
                            refNode = geometryNode.child(i)
                            refNode.setData(0, str(assetType))

                if len(assetRefs["modifiers"]) > 0:
                    node.insertChildren(2, 1, self._rootItem.columnCount())
                    modifierNode = node.child(2)
                    if modifierNode != None:
                        modifierNode.setData(0, "Modifiers")
                        for i in range(len(assetRefs["modifiers"])):
                            assetType = assetRefs["modifiers"][i]
                            modifierNode.insertChildren(i, 1, self._rootItem.columnCount())
                            refNode = modifierNode.child(i)
                            refNode.setData(0, str(assetType))
