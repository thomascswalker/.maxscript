from pymxs import runtime as rt
import json

"""
@name: getSettings
@desc: Gets the settings from the settings .json file.
@returns {dictionary}
"""
def getSettings():
    with open(os.path.dirname(os.path.realpath(__file__)) + "\\assettrackersettings.json") as f:
        data = json.load(f)

    return data

"""
@name: getAssetContext
@desc: Gets all references in the scene, based on the BitmapClasses list
       from the settings .json file. This returns a dictionary with the
       materials, geometry, and modifiers associated with the filename.
@param {string} filename: The full filename to get all references from
@returns {dictionary}
"""
def getAssetContext(filename):
    data = self.getSettings()

    materials = []
    geometry = []
    modifiers = []

    for classType in data["BitmapClasses"]:
        theClass = rt.execute(classType)
        if theClass != None:
            nodes = rt.GetClassInstances(theClass)
        
        for node in nodes:
            refs = rt.refs.dependents(node)
            for ref in refs:
                if rt.superClassOf(ref) == rt.execute("material"):
                    materials.append(ref)
                if rt.superClassOf(ref) == rt.execute("geometryclass"):
                    geometry.append(ref)
                if rt.superClassOf(ref) == rt.execute("modifier"):
                    modifiers.append(ref)

    uniqueMaterials = list(set(materials))
    uniqueGeometry = list(set(geometry))
    uniqueModifiers = list(set(modifiers))

    return {"materials" : uniqueMaterials, "geometry" : uniqueGeometry, "modifiers": uniqueModifiers}
