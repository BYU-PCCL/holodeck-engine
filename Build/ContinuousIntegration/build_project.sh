#!/bin/bash

# This script validates that the project will build before attempting
# to package it


ue4 setroot /home/ue4/UnrealEngine 
ue4 clean

# Do the actual build
ue4 build
