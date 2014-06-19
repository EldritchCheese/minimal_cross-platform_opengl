#!/usr/bin/env python

import sys
import bpy

if len(sys.argv) < 7:
    print("Usage: export.py")
    exit(1)

# Usage:
# blender -b INFILE.blend --python "export.py" -- ModelName OUTFILE

blenderObjectName = sys.argv[6]
outfile = sys.argv[7]

bpy.context.scene.objects.active = bpy.context.scene.objects[blenderObjectName]

for ob in bpy.data.objects:
    ob.select = (ob.name == blenderObjectName)

bpy.ops.export_scene.obj(
    filepath = outfile,
    use_selection = True,
    use_edges = False,
    use_normals = True,
    use_uvs = True,
    use_materials = False,
    use_triangles = True)
