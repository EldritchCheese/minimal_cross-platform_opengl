# -*- python -*-

import os
from os.path import join as pjoin

Import('env')

# GLEW
env.Append(CPPDEFINES=['GLEW_STATIC'])
libGLEW = env.SConscript('ext_libs/glew-1.10.0/SConscript',exports=['env'])
env.Append(CPPPATH=['$BASEDIR/ext_libs/glew-1.10.0/include'])

# GLFW
libGLFW = env.SConscript('ext_libs/glfw-3.0.4/SConscript',exports=['env'])
env.Append(CPPPATH=['$BASEDIR/ext_libs/glfw-3.0.4/include'])

# GL
env.Append(LIBS=['$OPENGL'])

# GLM
env.Append(CPPPATH=['$BASEDIR/ext_libs/glm-0.9.5.3'])
env.Append(CCFLAGS=['-DGLM_FORCE_RADIANS'])

# SOIL
env.Append(CPPPATH=['$BASEDIR/ext_libs/soil'])
libSOIL = env.SConscript('ext_libs/soil/SConscript',exports=['env'])

# assimp
env.Append(CPPPATH=['$BASEDIR/ext_libs/assimp-3.0.1270/include'])
env.Append(LIBS=['z'])
libASSIMP = env.SConscript('ext_libs/assimp-3.0.1270/SConscript',exports=['env'])

# custom engine
env.Append(CPPPATH=['$BASEDIR/engine/include'])
libNGine = env.SConscript('engine/SConscript',exports=['env'])

env.Append(RPATH=[Literal('\\$$ORIGIN')])
env.Append(CPPPATH=['$BASEDIR/include'])

exe = env.Program(['Program.cc',Glob('src/*.cc'),libNGine,libASSIMP,libGLFW,libGLEW,libSOIL])
env.Install('dist',exe)
env.Install('dist/resources',Glob('resources/*'))
