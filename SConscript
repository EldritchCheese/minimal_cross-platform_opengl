# -*- python -*-

import os
from os.path import join as pjoin

Import('env')

env.Append(RPATH=[Literal('\\$$ORIGIN')])
env.Append(CPPPATH=['$BASEDIR/include'])

# GLEW
libGLEW = env.Command(
    'ext_libs/glew-1.10.0/lib/$GLEW',
    [pjoin('ext_libs','glew-1.10.0',line.strip())
     for line in open(pjoin(env['BASEDIR'],'ext_libs','glew-1.10.0','file_list.txt'))],
    'cd $DIR/ext_libs/glew-1.10.0 && make')
env.Append(CPPPATH=['$BASEDIR/ext_libs/glew-1.10.0/include'])

# GLFW
libGLFW = env.Command(
    'ext_libs/glfw-3.0.4/src/libglfw3.a',
    [pjoin('ext_libs','glfw-3.0.4',line.strip())
     for line in open(pjoin(env['BASEDIR'],'ext_libs','glfw-3.0.4','file_list.txt'))],
    'cd $DIR/ext_libs/glfw-3.0.4 && '
    'cmake $CMAKE_TOOLCHAIN $CMAKE_FLAGS . && '
    'make')
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
libASSIMP = env.Command(
    'ext_libs/assimp-3.0.1270/lib/libassimp.a',
    [pjoin('ext_libs','assimp-3.0.1270',line.strip())
     for line in open(pjoin(env['BASEDIR'],'ext_libs','assimp-3.0.1270','file_list.txt'))],
    'cd $DIR/ext_libs/assimp-3.0.1270 && '
    'cmake $CMAKE_TOOLCHAIN -D BUILD_STATIC_LIB=ON -D ENABLE_BOOST_WORKAROUND=ON '
                           '-D BUILD_ASSIMP_TOOLS=OFF $CMAKE_FLAGS . && '
    'make')

# custom engine
env.Append(CPPPATH=['$BASEDIR/engine/include'])
libNGine = env.SConscript('engine/SConscript',exports=['env'])

exe = env.Program(['Program.cc',Glob('src/*.cc'),libNGine,libASSIMP,libGLFW,libGLEW,libSOIL])
env.Install('dist',exe)
env.Install('dist/resources',Glob('resources/*'))
