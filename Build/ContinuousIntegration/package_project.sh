#!/bin/bash

# This script packages the entire project

# Note - this is creating the combined binary by copying over the Worlds folder from
# holodeck-worlds into the build directory of holodeck-engine. This will produce one
# big .pak file. 
#
# In the future, when we want to have each world in a different pak file,
# we will have to modify holodeck-worlds to generate .pak files, archive those .pak files as
# part of the build, and then just copy those .pak files in to the build output of 
# holodeck-engine

# Move the worlds from holodeck-worlds to this project
mv holodeck-worlds/DefaultWorlds/Content/Worlds Content/

ue4 setroot /home/ue4/UnrealEngine 

ue4 clean

# Package it up
ue4 package Development
