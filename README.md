# HOLODECK

## About
Holodeck is an open source project built primarily for training reinforcement learning agents in the unreal engine. It is built for use with Unreal Engine 4.17, with bindings for python to communicate with the agents in the environments. For best results, use windows.

## Usage
This repository contains everything you need to build environments in the Unreal Engine for Holodeck. If you want to use worlds that have already been built that you can communicate with in python, look at the [holodeck repo](https://github.com/byu-pccl/holodeck).

* A description of currently implemented sensors can be found [here](Docs/sensors.md).
* Agent descriptions can be found [here](Docs/agents.md).
* Holodeck can be run either from editor or by selecting run from standalone game, however nothing will happen until the python client begins.

## Installation
* If not installed, [Install Unreal Engine 4.17](https://www.unrealengine.com/en-US/download)
    * After starting the Epic Games Launcher click on the Unreal Engine tab, then the Library option
    * Select add a new engine version and then version 4.17 and install
* Clone the repository:
`git clone https://github.com/byu-pccl/holodeck-engine`
* Navigate to the root folder and right click Holodeck.uproject -> Generate Visual Studio project files
* Right click Holodeck.uproject -> Switch Unreal Engine Version -> 4.17
* Run Holodeck.uproject (It will ask you to rebuild, click yes)

## Building For Linux
For a great tutorial, see the [wiki page](https://github.com/byu-pccl/holodeck-engine/wiki/Cross-Compiling-for-Linux)

## Usage
BYU does not own the world assets used in the packaged binary version of holodeck so we are not able to offer them in non binary format. However by migrating a level into holodeck, any unreal world can be built into a holodeck environment. To do so follow [this guide.](https://github.com/BYU-PCCL/holodeck-engine/wiki/Packaging-and-Using-Custom-Worlds)

## Troubleshooting
If you are having problems with holodeck, open the project in visual studio, then in the explorer right click on Holodeck, and click clean, then try to build again.
