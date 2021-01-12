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
        with open(os.path.dirname(os.path.realpath(__file__)) + "\\assettrackersettings.json") as f:
            data = json.load(f)

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
        data = self.getSettings()

        materials = []
        geometry  = []
        modifiers = []

        for classKey in data["BitmapClasses"]:
            paramName = data["BitmapClasses"][classKey]
            classValue = rt.execute(classKey)

            instances = []
            nodes = []
            if classValue != None:
                instances = rt.GetClassInstances(classValue)
                for inst in instances:
                    if (rt.getProperty(inst, paramName)) == filename:
                        nodes.append(inst)
            if len(nodes) > 0:
                for node in nodes:
                    refs = rt.refs.dependents(node)

                    for ref in refs:
                        if rt.superClassOf(ref) == self._materialClass:
                            materials.append(ref)
                        if rt.superClassOf(ref) == self._geometryClass:
                            geometry.append(ref)
                        if rt.superClassOf(ref) == self._modifierClass:
                            modifiers.append(ref)

                uniqueMaterials = list(set(materials))
                uniqueGeometry  = list(set(geometry))
                uniqueModifiers = list(set(modifiers))

                return  {
                            "materials" : uniqueMaterials,
                            "geometry" : uniqueGeometry,
                            "modifiers": uniqueModifiers
                        }

        return {}