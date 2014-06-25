#!/usr/bin/env python

import sys
import bpy
import tempfile
import ctypes

if len(sys.argv) < 7:
    print("Usage: export.py")
    exit(1)

# Usage:
# blender -b INFILE.blend --python "export.py" -- OUTFILE

outfile = sys.argv[6]

bpy.context.scene.objects.active = bpy.data.objects[0]

# Write the object in .obj format to temporary file
temp = tempfile.NamedTemporaryFile()
bpy.ops.export_scene.obj(
    filepath = temp.name,
    use_selection = True,
    use_edges = False,
    use_normals = True,
    use_uvs = True,
    use_materials = False,
    use_triangles = True)

# Parse the .obj format
vertices = []
uvs = []
normals = []
indices = []

for line in temp.file:
    line = line.decode('utf-8').split()
    if line[0]=='v':
        x,y,z = map(float,line[1:])
        vertices.append((x,y,z))
    elif line[0]=='vt':
        u,v = map(float,line[1:])
        uvs.append((u,v))
    elif line[0]=='vn':
        x,y,z = map(float,line[1:])
        normals.append((x,y,z))
    elif line[0]=='f':
        for ind_list in line[1:]:
            v_ind,uv_ind,n_ind = [int(s)-1 for s in ind_list.split('/')]
            indices.append((v_ind,uv_ind,n_ind))

full_indices = []
full_vertices = []
index_to_fullIndex = {}
for v_ind,uv_ind,n_ind in indices:
    try:
        full_indices.append(index_to_fullIndex[(v_ind,uv_ind,n_ind)])
    except KeyError:
        index = len(index_to_fullIndex)
        index_to_fullIndex[(v_ind,uv_ind,n_ind)] = len(index_to_fullIndex)
        full_vertices.append( (vertices[v_ind], uvs[uv_ind], normals[n_ind]) )
        full_indices.append(index)

# Write to a binary file
f = open(outfile,'wb')

f.write(ctypes.c_uint16(len(full_vertices)))
for vertex,_,_ in full_vertices:
    f.write(ctypes.c_float(vertex[0]))
    f.write(ctypes.c_float(vertex[1]))
    f.write(ctypes.c_float(vertex[2]))
for _,uv,_ in full_vertices:
    f.write(ctypes.c_float(uv[0]))
    f.write(ctypes.c_float(uv[1]))
for _,_,normal in full_vertices:
    f.write(ctypes.c_float(normal[0]))
    f.write(ctypes.c_float(normal[1]))
    f.write(ctypes.c_float(normal[2]))

f.write(ctypes.c_uint16(len(full_indices)))
for index in full_indices:
    f.write(ctypes.c_uint16(index))

f.close()
