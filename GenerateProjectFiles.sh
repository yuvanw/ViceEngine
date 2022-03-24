#!/bin/bash

BuildDirectory="Build"
ProjectName="NoviceEngine"
mkdir -p "${BuildDirectory}"
if [ -d "${ProjectName}.xcodeproj" ]; then
	echo "Xcode project already exists. Deleting xcode project"
	rm -r "${ProjectName}.xcodeproj"
fi

cd "${BuildDirectory}"
pwd

#run cmake to create the Xcode project
cmake -G Xcode ../Source
mv ${ProjectName}.xcodeproj ..//${ProjectName}.xcodeproj
