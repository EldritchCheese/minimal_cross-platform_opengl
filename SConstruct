# -*- python -*-

import os

glob_env = Environment()
glob_env['BASEDIR'] = os.getcwd()
glob_env['CMAKE_FLAGS'] = '-D CMAKE_CXX_FLAGS=-O3'

win32 = glob_env.Clone()
win64 = glob_env.Clone()
native = glob_env.Clone()

#Define the working directory
win32['DIR'] = 'win32'
win64['DIR'] = 'win64'
native['DIR'] = 'native'

#Define the compilers, using C++11
win32.Replace(CC='i686-w64-mingw32-gcc')
win32.Replace(CXX='i686-w64-mingw32-g++')
win32.Append(CCFLAGS=['-std=c++0x'])
win64.Replace(CC='x86_64-w64-mingw32-gcc')
win64.Replace(CXX='x86_64-w64-mingw32-g++')
win64.Append(CCFLAGS=['-std=c++0x'])
native.Append(CCFLAGS=['-std=c++11'])

#Define the appropriate file formats
win32.Replace(SHLIBPREFIX='')
win32.Replace(SHLIBSUFFIX='.dll')
win32.Replace(PROGSUFFIX='.exe')
win32.Append(LINKFLAGS='-static')
win64.Replace(SHLIBPREFIX='')
win64.Replace(SHLIBSUFFIX='.dll')
win64.Replace(PROGSUFFIX='.exe')
win64.Append(LINKFLAGS='-static')

#Platform-specific cmake
win32['CMAKE_TOOLCHAIN'] = '-D CMAKE_TOOLCHAIN_FILE=$BASEDIR/ext_libs/win32.cmake'
win64['CMAKE_TOOLCHAIN'] = '-D CMAKE_TOOLCHAIN_FILE=$BASEDIR/ext_libs/win64.cmake'


#Platform-specific GLEW
win32['ENV']['SYSTEM'] = 'linux-mingw32'
win32['GLEW'] = 'libglew32.a'
win32.Append(CCFLAGS=['-DGLEW_STATIC'])
win32.Append(LINKFLAGS=['-DGLEW_STATIC'])
win64['ENV']['SYSTEM'] = 'linux-mingw64'
win64['GLEW'] = 'libglew32.a'
win64.Append(CCFLAGS=['-DGLEW_STATIC'])
win64.Append(LINKFLAGS=['-DGLEW_STATIC'])
native['GLEW'] = 'libGLEW.a'

#Platform-specific OpenGL
win32['OPENGL'] = 'opengl32'
win64['OPENGL'] = 'opengl32'
native['OPENGL'] = 'GL'
win32.Append(LIBS=['gdi32','comdlg32','user32']) #graphics libraries
win64.Append(LIBS=['gdi32','comdlg32','user32']) #graphics libraries
native.Append(LIBS=['X11','Xxf86vm','Xrandr','pthread','Xi']) #graphics libraries

for env in [win32,win64,native]:
    env['ENV']['CC'] = env['CC']
    env['ENV']['CXX'] = env['CXX']
    SConscript('SConscript',variant_dir=env['DIR'],src_dir='.',exports=['env'])
    Clean('.',env['DIR'])

Default(native['DIR'])

