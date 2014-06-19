#!/usr/bin/env python

import sys
import bpy

if len(sys.argv) < 7:
    print("Usage: export.py")
    exit(1)

# Usage:
# blender -b INFILE.blend --python "export.py" -- OUTFILE

outfile = sys.argv[6]

bpy.context.scene.objects.active = bpy.data.objects[0]

for i,ob in enumerate(bpy.data.objects):
    ob.select = (ob==0)

bpy.ops.export_scene.obj(
    filepath = outfile,
    use_selection = True,
    use_edges = False,
    use_normals = True,
    use_uvs = True,
    use_materials = False,
    use_triangles = True)
