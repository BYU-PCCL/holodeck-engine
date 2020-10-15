# HOLODECK
![Build Status](https://jenkins.holodeck.ml/buildStatus/icon?job=holodeck-engine%2Fdevelop)

## Developer Docs
If you're here, you're probably interested in working on Holodeck instead of just using it. You might find
it helpful to consult the [Developer Wiki](https://github.com/BYU-PCCL/holodeck/wiki/Holodeck-Onboarding)

## About
Holodeck is an open source project for training reinforcement learning agents in Unreal Engine. It is built for use with Unreal Engine 4.22, with bindings for Python to communicate with agents in environments. For best results, use Windows.

## Usage
This repository contains everything you need to build environments in Unreal Engine for Holodeck. If you want to use pre-built worlds, look at the [Holodeck repo](https://github.com/byu-pccl/holodeck).

* A description of currently implemented sensors can be found [here.](Docs/sensors.md)
* Agent descriptions can be found [here.](Docs/agents.md)
* Holodeck Engine can be run either from the Unreal Engine editor or by selecting run from standalone game, but nothing will happen until the [Python client](https://github.com/byu-pccl/holodeck) is started.

## Requirements
* [Unreal Engine 4.22.](https://www.unrealengine.com/en-US/download)
    * After starting the Epic Games Launcher click on the Unreal Engine tab, then the Library option.
    * Click the "+" button to add a new version, select version 4.22, and install.
* [Git LFS.](https://git-lfs.github.com/)
* Visual Studio 2017 including "Game development with C++" and "Unreal Engine installer" modules as outlined [here.](https://docs.unrealengine.com/en-us/Programming/Development/VisualStudioSetup)

## Installation
* Clone the repository:
`git clone https://github.com/byu-pccl/holodeck-engine`
* Navigate to the root folder and right click Holodeck.uproject -> Generate Visual Studio project files.
* Right click Holodeck.uproject -> Switch Unreal Engine Version -> 4.22.
* Run Holodeck.uproject (it will ask you to rebuild, click yes).
  - If you get a "ExampleLevel.umap appears to be an asset file" error you have not configured git-lfs correctly.
    Try `git lfs pull`.
    
## Building For Linux
For a great tutorial, see the [wiki page.](https://github.com/byu-pccl/holodeck-engine/wiki/Cross-Compiling-for-Linux)

## Usage
BYU does not own the world assets used in the packaged binary version of Holodeck so we are not able to offer them in non binary format. However by migrating a level into Holodeck, any Unreal world can be built into a Holodeck environment. To do so follow [this guide.](https://github.com/BYU-PCCL/holodeck-engine/wiki/Packaging-and-Using-Custom-Worlds)

## Troubleshooting
If you are having problems with Holodeck, open the project in Visual Studio, right click on Holodeck in the explorer, click "clean," and try building again.
