# HOLODECK

## About
Holodeck is an open source project built primarily for training reinforcement learning agents in the unreal engine. It is built for use with Unreal Engine 4.12, with bindings for python to communicate with the agents in the environments. For best results, use windows.

## Usage
This repository contains everything you need to build environments in the unreal engine. If you want to use worlds that have already built that you can communicate with in python, look at https://github.com/BYU-PCCL/HolodeckPythonBinding.

## Prerequisites
* git lfs
* Unreal Engine 4.17

## Installation
* Run the following:
`git clone https://github.com/BYU-PCCL/Holodeck`
* Navigate to the root folder and right click Holodeck.uproject -> Generate Visual Studio project files
* Right click Holodeck.uproject -> Switch Unreal Engine Version -> 4.17
* Run Holodeck.uproject (It will ask you to rebuild, click yes)

## Building For Linux
For a great tutorial, see the [wiki page](https://github.com/BYU-PCCL/Holodeck/wiki/Building-for-Linux)

## Best Practices
* To keep the repo clean, Holodeck should only contain ExampleLevel. It should also not contain any assets which are not salient to this project. For example, salient assets might be agents, tasks, and sensors.
* To build a new environment, first clone holodeck, then migrate any levels and assets into the project. This new holodeck environment should be kept separate from the main holodeck project.

## Troubleshooting
If you are having problems with holodeck, open the project in visual studio, then in the explorer right click on Holodeck, and click clean, then try to build again.

## Tips
* When running an agent in the world, the world is paused when waiting to receive an action. Unfortunately, this results in motion blur. To fix this, when building a world add a Post Process Volume with motion blur turned to zero, and make it unbounded.