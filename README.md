# Holodeck
![Build
Status](https://jenkins.holodeck.ml/buildStatus/icon?job=holodeck-engine%2Fdevelop)

Holodeck is a simulator for training reinforcement learning agents in Unreal Engine.
We currently support Unreal Engine 4.22.

This repository contains everything you need to _build environments_ for
Holodeck in Unreal Engine. If you want to use pre-built worlds, use the [Python
client](https://github.com/byu-pccl/holodeck).

## Contributing

If you're still here, you're probably interested in contributing to Holodeck.
Get started by looking at the [developer
wiki](https://github.com/BYU-PCCL/holodeck/wiki/Holodeck-Onboarding).

## Documentation

See https://holodeck.readthedocs.io.

## Requirements

Setup and development is generally much smoother on Windows.

* [Unreal Engine 4.22](https://www.unrealengine.com/en-US/download)
    * After starting the Epic Games Launcher, click "Unreal Engine", then click
      "Library"
    * Click the "+" button to add a new version, select version 4.22, and install
* [Git LFS](https://git-lfs.github.com/)
* Visual Studio configured for Unreal Engine development
  ([tutorial](https://docs.unrealengine.com/en-us/Programming/Development/VisualStudioSetup))

## Installation

* Clone this repository:
`git clone https://github.com/byu-pccl/holodeck-engine`
* Navigate to the root folder and right click Holodeck.uproject -> Generate
  Visual Studio project files
* Right click Holodeck.uproject -> Switch Unreal Engine Version -> 4.22
* Run Holodeck.uproject (it will ask you to rebuild, click yes)
  - If you get a "ExampleLevel.umap appears to be an asset file" error, you have
    not configured git-lfs correctly. Try `git lfs pull`.

## Usage

See [How to Debug](https://github.com/BYU-PCCL/holodeck/wiki/How-To-Debug).

## Useful wiki guides

- [Building custom
  worlds](https://github.com/BYU-PCCL/holodeck-engine/wiki/Packaging-and-Using-Custom-Worlds)
- [Building for
  Linux](https://github.com/byu-pccl/holodeck-engine/wiki/Cross-Compiling-for-Linux)
- [Debugging on
  Linux](https://github.com/BYU-PCCL/holodeck/wiki/How-to-Configure-Debugger-on-Linux)

## Disclaimer

BYU doesn't have a license to distribute assets for Holodeck's default worlds,
so we can only provide packaged binaries.

## Troubleshooting

Try cleaning your [Visual
Studio](https://docs.microsoft.com/en-us/visualstudio/ide/building-and-cleaning-projects-and-solutions-in-visual-studio?view=vs-2019)
or [Clion](https://www.jetbrains.com/help/clion/build-actions.html#43c4f)
project.
