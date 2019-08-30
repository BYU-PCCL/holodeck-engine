#!/bin/bash

# This script packages the entire project

ue4 setroot /home/ue4/UnrealEngine

echo "👉 Backing up Content/ folder"
# Make a backup copy of the Content/ folder
mkdir Content-Backup
cp -r Content/* Content-Backup
ls Content-Backup

# Package each
for packagepath in holodeck-worlds/*/; do
    packagename=$(basename $packagepath)

    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    echo "⚠ Packaging $packagename..."
    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    

    # Copy everything in the worlds /Content directory into the UE4 projects
    # Delete the previous Content folder. This is so that we can mv the worlds quickly, and
    # then paste the Holodeck changes on top of it.
    echo "👉 Deleting Content/ folder..."
    rm -r Content

    echo "👉 Making empty Content/ folder..."
    mkdir Content

    echo "👉 Copying Holodeck content folder from $packagepath..."
    mv holodeck-worlds/$packagename/Content/* Content/

    echo "👉 Applying Holodeck changes..."
    cp -r Content-Backup/* Content

    # Package it up
    echo "👉 Starting Packaging Process..."
    ue4 package Development
    
    # Make sure it worked
    code=$?
    if [ code -ne 0 ]; then
        >&2 echo "(╯°□°)╯︵ ┻━┻ Packaging $packagename failed with code $code!"
        exit $code
    fi

    mkdir dist

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

    echo "👉 Done packaging package $packagename"
done

echo "👉 Sucessfully packaged all the packages 🎉🎉"