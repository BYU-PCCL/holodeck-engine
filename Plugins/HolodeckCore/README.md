Installation
------------
	Create an Unreal Project (ex. Simulator)
	Create a folder named "Plugins" in /Simulator
	cd /Simulator/Plugins
	git clone http://pccmdb.cs.byu.edu/holodeck/HolodeckMasterProject.git

	In your Unreal Project settings:
	Set your World Settings class to "HolodeckWorldSettings"
		Edit > Project Settings > Engine > General Settings > Default Classes > World Settings Class
	Set your Game Instance class to "HolodeckGameInstance"
		Edit > Project Settings > Project > Maps & Modes > Game Instance > Game Instance Class
	
	In your Unreal Level:
	Add any pawn of your choice
	In the Details Panel of the pawn:
	Set the ID of the pawn to your desired name, this is the name used to control the pawn
	Set the AI Controller Class to "Holodeck[Type]Controller" or "Holodeck[Type]Controller"
	Set the Auto Possess Player to "Disabled"
	Set the Auto Possess AI to Placed in World

Camera Sensor Installation
--------------------------
	In your pawn, attach a "Camera Sensor Array 2D" Scene Component
	Nest any number of "Scene Capture Component 2D" components under the Camera Sensor Array 2D
	Set up the Scene Capture Component 2D components in the normal manner
		See here for more info: https://docs.unrealengine.com/latest/INT/Resources/ContentExamples/Reflections/1_7/index.html
	The camera messages will be published automatically every tick.