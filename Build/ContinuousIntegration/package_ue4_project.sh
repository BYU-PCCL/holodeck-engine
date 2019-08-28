#!/bin/bash

# This script packages the entire project

ue4 setroot /home/ue4/UnrealEngine

# Use git to take a snapshot of the content directory

echo "👉 Initializing Git Repo..."
git init Content
cd Content
git add .
git config user.email "jenkins@holodeck.ml"
git config user.name "jenkins"
git commit -m "stock"
cd ..

# Package each
for packagepath in holodeck-worlds/*/; do
    packagename=$(basename $packagepath)

    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    echo "⚠ Packaging $packagename..."
    echo "⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠"
    

    # Copy everything in the worlds /Content directory into the UE4 projects
    echo "👉 Copying content folder from $packagepath..."
    mv -f holodeck-worlds/$packagename/Content/* Content/

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

    echo "👉 Moving $packagename.zip out of dist/ folder...."
    mv "$packagename.zip" ..

    echo "👉 Deleting config files for $packagename..."
    rm *.json

    cd ..

    # Get it back to stock for next build
    echo "👉 Cleaning up Content/ folder before next build..."
    cd Content
    git clean -f
    cd ..

    echo "👉 Done packaging package $packagename"
done

echo "👉 Sucessfully packaged all the packages 🎉🎉"