from pymxs import runtime as rt
import json, os

class AssetListFunctions(object):
    _materialClass = rt.execute("material")
    _geometryClass = rt.execute("geometryclass")
    _modifierClass = rt.execute("modifier")

    """
    @name: getSettings
    @desc: Gets the settings from the settings .json file.
    @returns {dictionary}
    """
    def getSettings(self):
        # Open the .json file and load the data into a dictionary
        with open(os.path.dirname(os.path.realpath(__file__)) + "\\assettrackersettings.json") as f:
            data = json.load(f)

        # Return the dictionary of data
        return data

    """
    @name: getAssetRefs
    @desc: Gets all references in the scene, based on the BitmapClasses list
        from the settings .json file. This returns a dictionary with the
        materials, geometry, and modifiers associated with the filename.
    @param {string} filename: The full filename to get all references from
    @returns {dictionary}
    """
    def getAssetRefs(self, filename):
        # Get the .json settings first. This'll list all of the classes to
        # iterate through, and what their respective 'filename' parameter
        # is called.
        data = self.getSettings()

        materials = []
        geometry  = []
        modifiers = []

        # Iterates through the different MAXScript classes to check through
        for classKey in data["BitmapClasses"]:

            # Get the respective class' denoted 'filename' parameter
            # These vary a bit per class
            paramName = data["BitmapClasses"][classKey]

            # Get the MAXScript Value of the class
            classValue = rt.execute(classKey)

            # If it DOES exist, meaning the standard or 3rd party plugins
            # are found (Max, Arnold, V-Ray, etc.)
            if classValue != None:

                # Get all the instances of that node
                instances = rt.GetClassInstances(classValue)

                for inst in instances:
                    # If that node matches the same filename we're currently
                    # checking against, then it's a match!
                    if (rt.getProperty(inst, paramName)) == filename:
                        nodes.append(inst)

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
                        if rt.superClassOf(dep) == self._materialClass:
                            materials.append(dep)
                        if rt.superClassOf(dep) == self._geometryClass:
                            geometry.append(dep)
                        if rt.superClassOf(dep) == self._modifierClass:
                            modifiers.append(dep)

                # Return the mapped dictionary of nodes we found
                return  {
                            "materials" : materials,
                            "geometry" : geometry,
                            "modifiers": modifiers
                        }

        # If we didn't find anything, return an empty dictionary
        return {}