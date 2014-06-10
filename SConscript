# -*- python -*-

import os
from os.path import join as pjoin

Import('env')

env.Append(RPATH=[Literal('\\$$ORIGIN')])
env.Append(CPPPATH=['include'])
env.Append(LIBPATH=['bin'])

exe_reqs = []


# GLEW
glew = env.Command(
    'ext_libs/glew-1.10.0/lib/$GLEW',
    [pjoin('ext_libs','glew-1.10.0',line.strip())
     for line in open(pjoin(env['BASEDIR'],'ext_libs','glew-1.10.0','file_list.txt'))],
    'cd $DIR/ext_libs/glew-1.10.0 && make')
env.Append(CPPPATH=['ext_libs/glew-1.10.0/include'])

# GLFW
glfw = env.Command(
    'ext_libs/glfw-3.0.4/src/$GLFW',
    [pjoin('ext_libs','glfw-3.0.4',line.strip())
     for line in open(pjoin(env['BASEDIR'],'ext_libs','glfw-3.0.4','file_list.txt'))],
    'cd $DIR/ext_libs/glfw-3.0.4 && '
    'cmake $GLFW_CMAKE_OPTS . && '
    'make')
env.Append(CPPPATH=['ext_libs/glfw-3.0.4/include'])

# GL
env.Append(LIBS=['$OPENGL'])

# GLM
# env.Append(LIBS=['glm'])

exe = env.Program(['Program.cc',Glob('src/*.cc'),glfw,glew])
env.Requires(exe,exe_reqs)
env.Install('bin',exe)

