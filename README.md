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

## Best Practices
* To keep the repo clean, Holodeck should only contain ExampleLevel. It should also not contain any assets which are not salient to this project. For example, salient assets might be agents, tasks, and sensors.
* To build a new environment, first clone holodeck, then migrate any levels and assets into the project. This new holodeck environment should be kept separate from the main holodeck project.

## Troubleshooting
If you are having problems with holodeck, open the project in visual studio, then in the explorer right click on Holodeck, and click clean, then try to build again.

## Tips
* When running an agent in the world, the world is paused when waiting to receive an action. Unfortunately, this results in motion blur. To fix this, when building a world add a Post Process Volume with motion blur turned to zero, and make it unbounded.

## Branches
We publish source code for Holodeck in two main branches: 
* The [master branch](https://github.com/byu-pccl/holodeck-engine/tree/master) contains our production quality code. We try our best to keep it bug free, and only commit to this branch when we have tested the code and feel that it is ready.
* The [develop branch](https://github.com/byu-pccl/holodeck-engine/tree/develop) is where the action happens. When we make new features, we branch off from here. The feature branches are merged back onto here when they are ready. This keeps the master branch clean and stable while we get things together on this branch. 

Several other branches will come and go as needed, like hotfix or feature branches. Specifically, once we have a group of features working on the develop branch, we will debug and test them on a release branch. Once the release branch is stable, we will merge it back into both the devleop and master branch. A great guide for the workflow we emulate can be found [here](http://nvie.com/posts/a-successful-git-branching-model/).

