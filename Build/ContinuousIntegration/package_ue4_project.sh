#!/bin/bash

# This script packages the entire project

ue4 setroot /home/ue4/UnrealEngine

echo "👉 Backing up Content/ folder"
# Make a backup copy of the Content/ folder
cp -r Content/* Content-Backup

# Package each
for packagepath in holodeck-worlds/*/; do
    packagename=$(basename $packagepath)

    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    echo "⚠ Packaging $packagename..."
    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    

    # Copy everything in the worlds /Content directory into the UE4 projects
    echo "👉 Copying content folder from $packagepath..."
    cp -rl holodeck-worlds/$packagename/Content/* Content/

    # Package it up
    ue4 package Development
    
    # Make sure it worked
    code=$?
    if [ code -ne 0 ]; then
        >&2 echo "(╯°□°)╯︵ ┻━┻ Packaging $packagename failed with code $code!"
        exit $code
    fi

    # Open up the permissions in the output
    chmod 777 dist

    # Create the zip file
    cd dist

    # Copy configuration files into the output directory
    echo "👉 Copying config files into output directory..."
    cp ../holodeck-configs/$packagename/*.json .

    echo "👉 Compressing contents into $packagename.zip..."
    zip -r "$packagename.zip" *

    echo "👉 Moving $packagename.zip out of dist/ folder..."
    mv "$packagename.zip" ..

    echo "👉 Deleting config files for $packagename..."
    rm *.json

    cd ..

    # Get it back to stock for next build
    rm -r Content
    mkdir Content
    cp -r Content-Backup/* Content

    if [ $packagename -eq "Dexterity" ]; then
        
        sleep 900
    fi

    echo "👉 Done packaging package $packagename"
done

echo "👉 Sucessfully packaged all the packages 🎉🎉"