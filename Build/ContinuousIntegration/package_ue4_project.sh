#!/bin/bash

# This script packages the entire project

ue4 setroot /home/ue4/UnrealEngine

echo "👉 Backing up Content/ folder"
# Make a backup copy of the Content/ folder
mkdir Content-Backup
cp -r Content/* Content-Backup

# Package each
for packagepath in holodeck-worlds/*/; do
    packagename=$(basename $packagepath)

    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    echo "⚠ Packaging $packagename..."
    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    

    # Copy everything in the worlds /Content directory into the UE4 projects
    # Delete the previous Content folder. This is so that we can mv the worlds quickly, and
    # then paste the Holodeck changes on top of it.
    rm -r Content
    mkdir Content
    mv holodeck-worlds/$packagename/Content/* Content/

    echo "👉 Copying content folder from $packagepath..."
    cp -r Content-Backup/* Content

    # Package it up
    # ue4 package Development
    
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

    # Get it back to stock for next build
    rm -r Content
    mkdir Content
    cp -r Content-Backup/* Content

    echo "Peeky Time"
    sleep 120

    echo "👉 Done packaging package $packagename"
done

echo "👉 Sucessfully packaged all the packages 🎉🎉"