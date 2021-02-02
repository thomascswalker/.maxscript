from pymxs import runtime as rt
import sys, os

ROOT_DIR = os.path.dirname(os.path.realpath(__file__))

ITEM_NAME = 1
ITEM_EXT = 2
ITEM_PATH = 3
ITEM_TYPE = 4
ITEM_STATUS = 5
ITEM_SIZE = 6

MXS_MATERIAL_CLASS = rt.execute("material")
MXS_GEOMETRY_CLASS = rt.execute("geometryclass")
MXS_MODIFIER_CLASS = rt.execute("modifier")