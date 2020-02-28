#!/usr/bin/env python

#
# setup scons build
#

# https://github.com/SCons/scons/blob/master/src/engine/SCons/Environment.py

import os
import subprocess

import SCons

from SCons.Script import Import
from SCons.Script import AlwaysBuild

Import("env")


def report_args(*args, **kwargs):
    for entry in args:
        print(f"{entry}")
    for key, value in sorted(kwargs.items()):
        print(f"{key}={value}")


def report_env_var():
    for key, value in sorted(os.environ.items()):
        print(f"{key}={value}")


def report_dir_list(root):
        for path in sorted(os.listdir(root)):
            print(path)

# def setup_invoke(
#         env,
#         source:SCons.Node.FS.File,
#         target:SCons.Node.Alias.Alias):
#     firmware = source[0].abspath
#     print(f"### firmware={firmware}")
#     disk_name = disk_find()
#     print(f"### disk_name={disk_name}")
#     disk_setup(disk_name, firmware)


print('====================================')
print('extra_script')
print('------------------------------------')

# print(env.Dump())

root_dir = env['PROJECT_DIR']
this_dir = os.path.join(root_dir, "buildroot/user")
setup_disk = os.path.join(this_dir, "setup_disk.py")

#     f"{setup_disk} --firmware_path $SOURCE.abspath"
#     "$BUILD_DIR/$PROGNAME",

setup_target = env.Alias("setup",
    "$BUILD_DIR",
    "ls -las"
    )

env.AlwaysBuild(setup_target)

print('====================================')
