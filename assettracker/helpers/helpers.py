"""!@package Helper Functions
Helper functions related to actions.
"""

from pymxs import runtime as rt
import json, os, subprocess
from collections import OrderedDict

from PySide2.QtWidgets import QMenu, QAction

import constants
reload(constants)

class Actions(object):
    """!
    The primary class used to store actions related to context menus
    as well as the top menubar of the main window.
    """
    def RevealInExplorer(self, items):
        """!
        @param items The asset items which are selected.
        """
        allPaths = []
        for item in items:
            data = item.itemData
            filepath = data[constants.ITEM_EXT]
            allPaths.append(filepath)

        allPaths = set(allPaths)
        for path in allPaths:
            os.startfile(path)

    def SetFilepath(self, items):
        print("Set filepath")
        print (items)

    def RenameFile(self, items):
        print("Rename file")
        print (items)

def getSettings():
    """!
    Gets the settings from the settings .json file.
    @return {dictionary}
    """
    # Open the .json file and load the data into a dictionary
    with open(constants.ROOT_DIR + "\\settings\\default.json") as f:
        data = json.load(f, object_pairs_hook=OrderedDict)

    # Return the dictionary of data
    return data

def getMenu(treeView):
    """!
    Gets the context menu for the assets in the tree view which
    were right-clicked, therefore requesting the context menu.
    @param {QTreeView} treeView: The tree view to extract the selected
                                 indexes from.
    @return {QMenu}
    """
    model = treeView.model()
    actions = Actions()

    # This is getting the selected indexes of the proxy model attached
    # to the tree view. This is not to be confused with the source
    # model, which is the actual AssetListModel.
    indexes = treeView.selectedIndexes()
    context = None

    if len(indexes) == 0:
        return

    # Get the actual item(s) selected
    items = []
    columns = model.columnCount()

    # Loop through the selected items, but only get one per group of items (rows).
    # The indexes selected includes each cell, but we only care for the row
    # that's selected.
    for proxyIndex in indexes[::columns]:
        proxyModel = model
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
    data = getSettings()

    # Depending on the depth of the selection, add different actions
    if depth == 0:
        for menuItem in data["ContextMenus"]["File"]:
            if (menuItem == "-"):
                menu.addSeparator()
            else:
                func = data["ContextMenus"]["File"][menuItem]
                method = getattr(actions, func)
                menu.addAction(menuItem, lambda method=method:method(items))
                

    if depth == 1:
        menu.addAction("Select all children")

    if depth == 2:
        if item.context() == "Materials":
            menu.addAction("Open in SME")

        if item.context() == "Geometry":
            menu.addAction("Select object(s)")

        if item.context() == "Modifiers":
            menu.addAction("Select parent object(s)")

    return menu

def getAssetRefs(filename):
    """
    Gets all references in the scene, based on the BitmapClasses list
    from the settings .json file. This returns a dictionary with the
    materials, geometry, and modifiers associated with the filename.
    @param {str} filename: The full filename to get all references from
    @return {dictionary}
    """
    # Get the .json settings first. This'll list all of the classes to
    # iterate through, and what their respective 'filename' parameter
    # is called.
    data = getSettings()

    materials = []
    geometry  = []
    modifiers = []

    # Iterates through the different MAXScript classes to check through
    for classKey in data["BitmapClasses"]:

        # Get the respective class' denoted 'filename' parameter
        # These vary a bit per class
        paramNames = data["BitmapClasses"][classKey]

        # Get the MAXScript Value of the class
        classValue = rt.execute(classKey)

        # Create an empty list of the nodes we'll look through
        nodes = []

        # If it DOES exist, meaning the standard or 3rd party plugins
        # are found (Max, Arnold, V-Ray, etc.)
        if classValue != None:

            # Get all the instances of that node
            instances = rt.GetClassInstances(classValue)

            for inst in instances:
                # If that node matches the same filename we're currently
                # checking against, then it's a match!
                try:
                    for param in paramNames:
                        if (rt.getProperty(inst, param)) == filename:
                            nodes.append(inst)
                except:
                    pass

        # If we've found 1 or more matching nodes, we'll iterate through them
        if len(nodes) > 0:
            for node in nodes:

                # Get the node's dependents. This'll be a giant array of every
                # dependent found in the scene, which'll include a bunch of
                # nonsense nodes.
                deps = rt.refs.dependents(node)

                # Filter through each dependent and add to each respective list
                # whether it's a subclass of materials, geometry, or modifiers.
                for dep in deps:
                    if rt.superClassOf(dep) == constants.MXS_MATERIAL_CLASS:
                        materials.append(dep)
                    if rt.superClassOf(dep) == constants.MXS_GEOMETRY_CLASS:
                        geometry.append(dep)
                    if rt.superClassOf(dep) == constants.MXS_MODIFIER_CLASS:
                        modifiers.append(dep)

            # Return the mapped dictionary of nodes we found
            return  {
                        "Materials" : materials,
                        "Geometry" : geometry,
                        "Modifiers": modifiers
                    }

    # If we didn't find anything, return an empty dictionary
    return {}

def getFileSize(size):
    """
    Returns the human-readable size of a file from its
    number of bytes.
    @param {int} size: The file size in bytes.
    @return {str}
    """
    if size < 1024.0: # B
        return (str(size) + "B")
    elif size < 1048576: # KB
        size = size / 1024.0
        return (str(round(size, 2)) + "KB")
    elif size < 1073741824: # MB
        size = size / (1024.0 * 1024.0)
        return (str(round(size, 2)) + "MB")
    else: # GB
        size = size / (1024.0 * 1024.0 * 1024.0)
        return (str(round(size, 2)) + "GB")