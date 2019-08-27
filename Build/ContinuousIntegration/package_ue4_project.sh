#!/bin/bash

# This script packages the entire project

ue4 setroot /home/ue4/UnrealEngine

# Use git to take a snapshot of the content directory

git init Content
cd Content
git add .
git commit -m "stock"
cd ..

# Package each
for packagepath in holodeck-worlds/*/; do
    packagename=$(basename $packagepath)

    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    echo "⚠ Packaging $packagename..."
    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    

    # Copy everything in the worlds /Content directory into the UE4 projects

    mv -f "$packagename/DefaultWorlds/Content" Content/

    # Package it up
    ue4 package Development
    
    # Make sure it worked
    code=$?
    if [ code -ne 0 ]; then
        echo "Packaging failed with code $code!"
        exit $code
    fi

    # Open up the permissions in the output
    chmod 777 dist

    # Create the zip file
    cd dist

    # Copy configuration files into the output directory
    cp "../holodeck-configs/$packagename/*.json" .

    zip -r "$packagename.zip" *

    mv "$packagename.zip" ..

    rm *.json

    cd ..

    # Get it back to stock for next build
    cd Content
    git clean -f
    cd ..

done

