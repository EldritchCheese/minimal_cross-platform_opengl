#!/usr/bin/env python3

import sys
import bpy
import tempfile
import ctypes
import shutil

if len(sys.argv) < 7:
    print("Usage: export.py")
    exit(1)

# Usage:
# blender -b INFILE.blend --python "export.py" -- OUTFILE
outfile = sys.argv[6]

class Mesh:
    def __init__(self, obj):
        bpy.context.scene.objects.active = obj

        self.LoadMesh()
        self.MakeFullVertices()

    def LoadMesh(self):
        self.vertices = []
        self.uvs = []
        self.normals = []
        self.indices = []

        temp = tempfile.NamedTemporaryFile()
        bpy.ops.export_scene.obj(
            filepath = temp.name,
            use_selection = True,
            use_edges = False,
            use_normals = True,
            use_uvs = True,
            use_materials = False,
            #use_triangles = True,
            keep_vertex_order = True)

        for line in temp.file:
            line = line.decode('utf-8').split()
            if line[0]=='v':
                x,y,z = map(float,line[1:])
                self.vertices.append((x,y,z))
            elif line[0]=='vt':
                u,v = map(float,line[1:])
                self.uvs.append((u,v))
            elif line[0]=='vn':
                x,y,z = map(float,line[1:])
                self.normals.append((x,y,z))
            elif line[0]=='f':
                ind1 = tuple(int(s)-1 for s in line[1].split('/'))
                for ind_list2, ind_list3 in zip(line[2:],line[3:]):
                    ind2 = tuple(int(s)-1 for s in ind_list2.split('/'))
                    ind3 = tuple(int(s)-1 for s in ind_list3.split('/'))
                    self.indices.append(ind1)
                    self.indices.append(ind2)
                    self.indices.append(ind3)

    def MakeFullVertices(self):
        index_to_fullIndex = {}
        self.full_vertices = []
        self.full_indices = []
        for key in self.indices:
            try:
                self.full_indices.append(index_to_fullIndex[key])
            except KeyError:
                v_ind,uv_ind,n_ind = key
                index = len(index_to_fullIndex)
                index_to_fullIndex[key] = index
                self.full_indices.append(index)
                self.full_vertices.append( (self.vertices[v_ind],
                                            self.uvs[uv_ind],
                                            self.normals[n_ind]) )

    def Write(self,f):
        f.write(ctypes.c_uint16(len(self.full_vertices)))
        for vertex,_,_ in self.full_vertices:
            f.write(ctypes.c_float(vertex[0]))
            f.write(ctypes.c_float(vertex[1]))
            f.write(ctypes.c_float(vertex[2]))
        for _,uv,_ in self.full_vertices:
            f.write(ctypes.c_float(uv[0]))
            f.write(ctypes.c_float(uv[1]))
        for _,_,normal in self.full_vertices:
            f.write(ctypes.c_float(normal[0]))
            f.write(ctypes.c_float(normal[1]))
            f.write(ctypes.c_float(normal[2]))

        f.write(ctypes.c_uint16(len(self.full_indices)))
        for index in self.full_indices:
            f.write(ctypes.c_uint16(index))

class Animation:
    def __init__(self,obj):
        self.keyframes = []
        for active_keyframe,_ in enumerate(obj.data.shape_keys.key_blocks):
            for keyframe_num,keyframe in enumerate(obj.data.shape_keys.key_blocks):
                keyframe.value = 1.0 if (active_keyframe==keyframe_num) else 0.0
            self.AddCurrentScene(obj)
        self.MakeFullVertices()

    def AddCurrentScene(self,obj):
        self.keyframes.append(Mesh(obj))

    def MakeFullVertices(self):
        #Empty out whatever is already there.
        for keyframe in self.keyframes:
            keyframe.full_indices = []
            keyframe.full_vertices = []
        index_to_fullIndex = {}
        for key in zip(*[keyframe.indices for keyframe in self.keyframes]):
            #Find the appropriate index
            try:
                index = index_to_fullIndex[key]
            except KeyError:
                index = len(index_to_fullIndex)
                index_to_fullIndex[key] = index
                #Construct/load the full_vertex
                for keyframe,(v_ind,uv_ind,n_ind) in zip(self.keyframes,key):
                    keyframe.full_vertices.append( (keyframe.vertices[v_ind],
                                                    keyframe.uvs[uv_ind],
                                                    keyframe.normals[n_ind]) )
            #Add the index to each keyframe
            for keyframe in self.keyframes:
                keyframe.full_indices.append(index)

    def Write(self,f):
        f.write(ctypes.c_uint16(len(self.keyframes)))
        for keyframe in self.keyframes:
            keyframe.Write(f)



obj = bpy.data.objects[0]

# Write to a binary file
with open(outfile,'wb') as f:
    if obj.data.shape_keys:
        Animation(obj).Write(f)
    else:
        Mesh(obj).Write(f)
