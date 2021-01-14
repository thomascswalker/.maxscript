# Standard imports
import os, sys, imp
from PySide2 import QtGui, QtCore, QtWidgets
from PySide2.QtCore import Qt, QSortFilterProxyModel
from PySide2.QtWidgets import QFileIconProvider
from pymxs import runtime as rt

class AssetListProxyModel(QSortFilterProxyModel):
    """Sorting proxy model that always places folders on top."""
    def __init__(self, model):
        super().__init__()
        self.setSourceModel(model)

    def lessThan(self, left, right):
        """Perform sorting comparison.

        Since we know the sort order, we can ensure that folders always come first.
        """
        left_is_folder = left.data(Qt.UserRole)
        left_data = left.data(Qt.DisplayRole)
        right_is_folder = right.data(Qt.UserRole)
        right_data = right.data(Qt.DisplayRole)
        sort_order = self.sortOrder()

        if left_is_folder and not right_is_folder:
            result = sort_order == Qt.AscendingOrder
        elif not left_is_folder and right_is_folder:
            result = sort_order != Qt.AscendingOrder
        else:
            result = left_data < right_data
        return result