# Sensors

## Types
Currently in Holodeck we have the following sensors:
* [UHolodeckSensor](#uholodecksensor) - abstract base class for sensors in holodeck
* [UHolodeckViewportClientPublisher](#uholodeckviewportclientpublisher)- publishes data from the main camera
* [UCameraSensorArray2D](#ucamerasensorarray2d) - small camera sensors that can be attached to agents
* [UIMUSensor](#uimusensor) - an 'inertial measurement unit' which measures forces and angular rates
* [UJointRotationSensor](#ujointrotationsensor) - a sensor for the Android agent which returns the rotations of all joints
* [UOrientationSensor](#uorientationsensor) - a sensor which gives forward, right, and up vectors for a static mesh
* [UPressureSensor](#upressuresensor) - a sensor which returns the location that an agent is touching something
* [URelativeSkeletalPositionSensor](#urelativeskeletalpositionsensor) - 

## UHolodeckSensor
UHolodeckSensor is the abstract base class for all other sensors.
It handles the publishing of sensor data.

Member Variables:
* Controller - the AHolodeckPawnController for this agent. It is looked for at begin play.
* ResultData - FHolodeckSensorData, the data which will be published

To extend this base class:
* Override SetDataType - e.g. `ResultData.Type = "IMUSensor";`
* Override TickSensorComponent - This is primary tick function for sensors, instead of TickComponent, which cannot be overridden.
* Ensure that in BeginPlay you call `Super::BeginPlay();`

## UHolodeckViewportClientPublisher
To get in python, add "PrimaryPlayerCamera" to state_sensors.
Attach UHolodeckViewportClientPublisher to an agent, and set the viewport for your project to be HolodeckViewport.
The returned image is whatever resolution you load your project in.

## UCameraSensorArray2D
This is a sensor that can be added to an agent to capture multiple points of views. Just attach it to your agent, then add as many SceneCaptureComponents as you want.

Each SceneCaptureComponent should be constrained to capture 256x256 RGB images only, until further updates can be made.

To access this sensor from python, add "CameraSensorArray2D" to the state_sensors. This will return a list containing 256x256x3 numpy arrays for each SceneCaptureComponent.

## UIMUSensor
An intertial measurement unit.
Returns a 1D numpy array of:
`[acceleration_x, acceleration_y, acceleration_z, velocity_roll, velocity_pitch, velocity_yaw]`
To access this sensor from python, add "IMUSensor" to the state_sensors.

## UJointRotationSensor
Only supported for the android agent.
Gets the rotation of each join in the android in the following order:
```
[head_swing1, head_twist, head_swing2,
neck_01_swing1,
spine_02_swing1, spine_02_twist,
spine_01_swing1, spine_01_twist, spine_01_swing2,
upperarm_l_swing1, upperarm_l_twist, upperarm_l_swing2,
lowerarm_l_swing1,
hand_l_swing1, hand_l_twist, hand_l_swing2,
thumb_01_l_swing1, thumb_01_l_swing2,
thumb02_l_swing1,
thumb_03_l_swing1,
index_01_l_swing1, index_01_l_swing2,
index_02_l_swing1,
index_03_l_swing1,
middle_01_l_swing1, middle_01_l_swing2,
middle_02_l_swing1,
middle_03_l_swing1,
ring_01_l_swing1, ring_01_l_swing2,
ring_02_l_swing1,
ring_03_l_swing1,
pinky_01_l_swing1, pinky_01_l_swing2,
pinky_02_l_swing1,
pinky_03_l_swing1, 
upperarm_r_swing1, upperarm_r_twist, upperarm_r_swing2,
lowerarm_r_swing1,
hand_r_swing1, hand_r_twist, hand_r_swing2,
thumb_01_r_swing1, thumb_01_r_swing2,
thumb02_r_swing1,
thumb_03_r_swing1,
index_01_r_swing1, index_01_r_swing2,
index_02_r_swing1,
index_03_r_swing1,
middle_01_r_swing1, middle_01_r_swing2,
middle_02_r_swing1,
middle_03_r_swing1,
ring_01_r_swing1, ring_01_r_swing2,
ring_02_r_swing1,
ring_03_r_swing1,
pinky_01_r_swing1, pinky_01_r_swing2,
pinky_02_r_swing1,
pinky_03_r_swing1,
thigh_l_swing1, thigh_l_twist, thigh_l_swing2,
calf_l_swing1,
foot_l_swing1, foot_l_swing2,
ball_l_swing1, ball_l_twist,
thigh_r_swing1, thigh_r_twist, thigh_r_swing2,
calf_r_swing1,
foot_r_swing1, foot_r_swing2,
ball_r_swing1, ball_r_twist]
shape=[79,]
```
To access this sensor from python, add "JointRotationSensor" to the state_sensors.

## UOrientationSensor
UOrientationSensor gets the forward, right, and up vector for whatever element it is attached to.
MUST be attached to a static mesh.
Returns a 2D numpy array of:
```
[[forward_x, forward_y, forward_z],
[right_x, right_y, right_z],
[up_x, up_y, up_z]]
Shape = [3, 3]
```
To access this sensor from python, add "OrientationSensor" to the state_sensors.

## UPressureSensor
Currently only supported for Android.
Returns a JSON string which contains hit components
To access this sensor from python, add "PressureSensor" to the state_sensors.

## URelativeSkeletalPositionSensor
Gets the position of each bone as a quaternion. Can be attached to any skeletal mesh.
The returned shape is [67, 4] for the android.
To use in python, add "RelativeSkeletalPositionSensor" to the state_sensors