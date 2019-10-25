#### 1. Create CLion project files
- Open Holodeck in Unreal Engine
- Goto File -> Generate CLion Project (this will be "Refresh CLion Project"  if the project was already generated)

#### 2. Open the project in CLion
Open the project from the project root in CLion or in Unreal Engine Editor goto File -> Open CLion

#### 3. Edit the configurations
- In CLion goto Run -> Edit Configurations (also available in the run section of the toolbar)
- Select Holodeck on the left (doesn't really matter, you can choose any configuration and edit the name to what you want)
- Set the value in `Executable` to be the path to the Unreal Engine Launcher (wherever you installed Unreal Engine). For me it's `~/repos/UnrealEngine/Engine/Binaries/Linux/UE4Editor`
- For `Program arguments` enter `"$PROJECT_DIR$/Holodeck.uproject" -UNATTENDED`
> Note: Include the quotes between $ and .uproject
- Click OK

#### 4. Run Holodeck in debug mode
After the setup, you should be able to goto Run -> Debug <configuration_name>, click on the debug icon on the top right corner of the toolbar or simply use Shift + F9 to debug Holodeck. 

> Note: This will build the project and open Unreal Engine in debug mode. Running Holodeck in Unreal Engine should hit any reachable breakpoints you set.
