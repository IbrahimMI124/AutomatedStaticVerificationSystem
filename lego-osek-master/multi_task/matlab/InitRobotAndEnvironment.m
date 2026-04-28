%_________________________________________________________________________
% Init_Robot_and_Environment.m
%
% 1. ENVIRONNEMENT
% 2. ROBOT INITIAL POSE
% 3. ROBOT PARAMETERS
% 4. CONTROLER's PARAMETERS
%
%_________________________________________________________________________


%_________________________________________________________________________
%-------------------------------------------------------- 1. ENVIRONNEMENT
% 
%  width of the band from .05m to .1 m
%
% The origin (0,0) is located at the center of the image

%--- METER_BY_PIXEL --------
% pixel dimension in meter 
METER_BY_PIXEL = 0.002; %  [0.002] 

%--- CIRCUIT_NAME_FILE --------
CIRCUIT_NAME_FILE =  'circuit.png';
% 'circuit.png'; % METER_BY_PIXEL in [0.002, 0.005];
% 'circuit1.png'; % very narrow band  METER_BY_PIXEL in [0.005, 0.015],  
% 'circuit_2000x2000.dev1.png'; % METER_BY_PIXEL in [0.002, 0.003];
% 'circuit_2000x2000.dev2.png'; % METER_BY_PIXEL in [0.002, 0.003]; (0,0,Pi) (0,-0.8,0)
% 'circuit_2000x2000.png'; % METER_BY_PIXEL in [0.002, 0.005];

%--- ANIMATION_SAMPLING_TIME -------
% Animation frame rate
ANIMATION_SAMPLING_TIME = 0.01; % [0.1] 10 fps

%--- SENSOR_SAMPLING_TIME -------
% sampling time for the sensors [.001] -> 1KHz, [.02] -> 50Hz 
SENSOR_SAMPLING_TIME = .001; 



%_________________________________________________________________________
%--------------------------------------------------- 2. ROBOT INITIAL POSE
% Initial Condition defined in each integrator of the cinematic 
% model of the robot in Robot_and_Environment
% The origin (0,0) is located at the center of the image

%--- INITIAL_ROBOT_POS_X --------
INITIAL_ROBOT_POS_X = 0;

%--- INITIAL_ROBOT_POS_Y --------
INITIAL_ROBOT_POS_Y = 0;

%--- INITIAL_ROBOT_POS_THETA --------
INITIAL_ROBOT_POS_THETA = 1.5; % not exactly pi / 2



%_________________________________________________________________________
%----------------------------------------------------- 3. ROBOT PARAMETERS

%--- INTER_WHEEL_DISTANCE --------
% inter-wheel distance
INTER_WHEEL_DISTANCE = 0.12; % [0.12]

%--- MAX_SPEED -------
% wr = 2revolution per second
% with a wheel of diameter wd = 0.06, MaxSpeed = wr * PI * wd  ~= 0.5 m.s-1 
MAX_SPEED = .5; % [.5] 

%--- SENSOR_HEIGHT --------
% sensor radius
% (hyp. visual angle of 90�, then radius = height ( sin(45�) == cos(45�))
SENSOR_HEIGHT = 0.025; % [0.025]

%--- LEFT_LIGHT_SENSOR_POSITION_ X and Y --------
% Position (in robot referentiel) of the Luminosity Sensors (Left, Right)
LEFT_LIGHT_SENSOR_POSITION_X = INTER_WHEEL_DISTANCE/2;
LEFT_LIGHT_SENSOR_POSITION_Y = INTER_WHEEL_DISTANCE/2;

%--- RIGHT_LIGHT_SENSOR_POSITION_ X and Y --------
RIGHT_LIGHT_SENSOR_POSITION_X = INTER_WHEEL_DISTANCE/2;
RIGHT_LIGHT_SENSOR_POSITION_Y = -INTER_WHEEL_DISTANCE/2;



%_________________________________________________________________________
%----------------------------------------------- 4. CONTROLER's PARAMETERS

R = 1;  % gain for motors
l = .2;   % robot size (in meters)
Rc = .1; % Sensors radius
ts = .001; % sampling time for the sensors
ts_anim = .01; % sampling time to refresh animation;
v_max=5; % saturation speed for the motors
% Parameter here
alpha=1/l;
omega=1; 
ksi=1;
ki_teta=omega*omega/alpha;
kp_teta=2*ksi*omega/alpha;
black_threshold=10;
white_threshold=90;
obstacle_threshold=20;
rotate_speed=0.1;




























%_________________________________________________________________________
%------------------------------------------------ NE PAS EDITER EN DESSOUS
%--------------------------------------------- DO NOT EDIT BELOW THIS LINE
%

ts = SENSOR_SAMPLING_TIME;
ts_anim = ANIMATION_SAMPLING_TIME;
%
% circuit initialization
circuit = imread(CIRCUIT_NAME_FILE);
circuit = flipdim(circuit,1);
max_light = max(max(max(circuit)));
%
%
n = size(circuit);  % Array dimensions, returns the sizes of each dimension 
%
% xmin et xmax de l'envir
XMAX = n(1) * METER_BY_PIXEL / 2; 
XMIN = - XMAX;
YMAX = n(2) * METER_BY_PIXEL / 2; 
YMIN = - YMAX; 
%
%
Env(1) = METER_BY_PIXEL;
Env(2) = XMIN;
Env(3) = XMAX;
Env(4) = YMIN;
Env(5) = YMAX;
%
Robot(1) = INTER_WHEEL_DISTANCE;
Robot(2) = MAX_SPEED;
Robot(3) = SENSOR_HEIGHT;
Robot(4) = LEFT_LIGHT_SENSOR_POSITION_X;
Robot(5) = LEFT_LIGHT_SENSOR_POSITION_Y; 
Robot(6) = RIGHT_LIGHT_SENSOR_POSITION_X;
Robot(7) = RIGHT_LIGHT_SENSOR_POSITION_Y;
%
RobotInit(1) = INITIAL_ROBOT_POS_X;
RobotInit(2) = INITIAL_ROBOT_POS_Y;
RobotInit(3) = INITIAL_ROBOT_POS_THETA;
%
%___ eof Init_Robot_and_Environment.m ____________________________________
%_________________________________________________________________________

