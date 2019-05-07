#!/bin/bash

# This script creates the installable .zip files that are posted in the S3 bucket
# TODO: Update this script to handle multiple worlds

# Copy configuration files into the output directory
cp holodeck-configs/DefaultWorlds/*.json dist/*

# Create the zip file
cd dist

zip -r DefaultWorlds.zip *

mv DefaultWorlds.zip ..

# Done!
