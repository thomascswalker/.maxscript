import os, sys, imp
from PySide2 import QtGui, QtCore
from PySide2.QtCore import Qt
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import assetlistitem
from assetlistitem import AssetListItem

# Import 3ds Max modules
try:
    # If Max 2020
    import MaxPlus
    from pymxs import runtime as mxs
except ImportError:
    # If Max 2021+
    from pymxs import runtime as mxs

class AssetListModel(QtCore.QAbstractItemModel):
    def __init__(self, parent=None):
        super(AssetListModel, self).__init__(parent)

        headers = ("Name", "Path", "Type")
        rootData = [header for header in headers]
        self.rootItem = AssetListItem(rootData)

        assets = MaxPlus.AssetManager.GetAssets()
        self.setupModelData(assets, self.rootItem)

    def columnCount(self, parent=QtCore.QModelIndex()):
        return self.rootItem.columnCount()

    def data(self, index, role):
        if not index.isValid():
            return None

        if role != QtCore.Qt.DisplayRole and role != QtCore.Qt.EditRole:
            return None

        item = self.getItem(index)
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

        return self.rootItem

    def headerData(self, section, orientation, role):
        if role == Qt.DisplayRole and orientation == Qt.Horizontal:
            return self.rootItem.data(section)

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
        success = self.rootItem.insertColumns(position, columns)
        self.endInsertColumns()

        return success

    def insertRows(self, position, rows, parent=QtCore.QModelIndex()):
        parentItem = self.getItem(parent)
        self.beginInsertRows(parent, position, position + rows - 1)
        success = parentItem.insertChildren(position, rows,
                self.rootItem.columnCount())
        self.endInsertRows()

        return success

    def parent(self, index):
        if not index.isValid():
            return QtCore.QModelIndex()

        childItem = self.getItem(index)
        parentItem = childItem.parent()

        if parentItem == self.rootItem:
            return QtCore.QModelIndex()

        return self.createIndex(parentItem.childNumber(), 0, parentItem)

    def removeColumns(self, position, columns, parent=QtCore.QModelIndex()):
        self.beginRemoveColumns(parent, position, position + columns - 1)
        success = self.rootItem.removeColumns(position, columns)
        self.endRemoveColumns()

        if self.rootItem.columnCount() == 0:
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

    def setupModelData(self, assets, parent):
        parents = [parent]

        for i in range(0, assets.GetCount()):
            asset = assets.At(i)

            assetFilename = asset.GetResolvedFileName()
            assetBasename = os.path.basename(assetFilename)

            assetName     = os.path.splitext(assetBasename)[0]
            assetPath     = os.path.dirname(assetFilename)
            assetExt      = os.path.splitext(assetBasename)[1]

            # Read the column data from the rest of the line.
            columnData = [assetName, assetPath, assetExt]

            # Append a new item to the current parent's list of children.
            parent = parents[-1]
            parent.insertChildren(parent.childCount(), 1,self.rootItem.columnCount())
            for column in range(len(columnData)):
                parent.child(parent.childCount() -1).setData(column, columnData[column])
