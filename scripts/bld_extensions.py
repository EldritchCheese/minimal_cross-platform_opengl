#!/usr/bin/env python

from SCons.Script import *

# Converting any ImageMagick-supported format to .dds
def texture_emitter(target, source, env):
    path, name = os.path.split(str(target[0]))
    basename, ext = os.path.splitext(name)
    ext = '.dds'
    path = path.replace('raw_textures','textures')
    target = os.path.join(path,name+ext)
    return target, source

def texture_generator(source,target,env,for_signature):
    source_ext = os.path.splitext(str(source[0]))[1]
    if source_ext.lower()=='.dds':
        return 'mkdir -p `dirname $TARGET` && cp $SOURCE $TARGET'
    else:
        return 'convert $SOURCE -define dds:compression=dxt1 -define dds:cluster-fit=true $TARGET'

texture_bld = Builder(generator = texture_generator,
                      emitter = texture_emitter)

def TOOL_TEXTURE(env):
    env.Append(BUILDERS={'Texture':texture_bld})



# Converting .blend objects to .obj
def model_emitter(target, source, env):
    #Correct target
    path, name = os.path.split(str(target[0]))
    basename, ext = os.path.splitext(name)
    ext = '.dat'
    path = path.replace('raw_models','models')
    target = os.path.join(path,name+ext)
    #export script as dependency
    source.append('#/scripts/export_model.py')
    return target, source

def model_generator(source,target,env,for_signature):
    source_ext = os.path.splitext(str(source[0]))[1]
    return 'blender -b $SOURCE --python scripts/export_model.py -- $TARGET'

model_bld = Builder(generator = model_generator,
                      emitter = model_emitter)

def TOOL_MODEL(env):
    env.Append(BUILDERS={'Model':model_bld})



# Recursive Install
def RecursiveInstall(env, target, src):
    root_items = []
    for src_list in src:
        root_items.extend(src_list)

    for root_item in root_items:
        unsearched = [root_item]
        base_dir = os.path.split(str(root_item))[0]
        #Recursive search
        while unsearched:
            current = unsearched.pop()
            if current.isdir():
                unsearched.extend(Glob(str(current)+'/*'))
            else:
                dir_initial, filename = os.path.split(str(current))
                dir_relative = dir_initial[len(str(base_dir))+1:]
                output_dir = (os.path.join(str(target),dir_relative)
                              if dir_relative else str(target))
                env.Install(output_dir,current)


def TOOL_RECURSIVE_INSTALL(env):
    env.AddMethod(RecursiveInstall)
