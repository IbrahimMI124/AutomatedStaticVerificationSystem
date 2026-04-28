function [sys,x0]=sensors(t,x,u,flag,ts,env,robot)
%ROBOT_ANIM S-function Level-1 for animating the motion of a doube-wheeled robot.

%   Alexandre Donze, 02-01-2011
%   Olivier Lebeltel, 03-2012

 % ----------------------------------  flag == 2
 if flag==2, % MdlUpdate


   circuit = evalin('base', 'circuit');  
   max_light = evalin('base','max_light');

   
  % current robot pos
  X = u(1);
  Y = u(2);
  alpha = u(3);
  
  
%  dist = robot(1);
  rad_c = robot(3);

  pixsize = env(1);
  
  % x domain : env(2) to env(3)
  % y domain : env(4) to env(5)  'lo Mum =)  
  ax = [env(2) env(3) env(4) env(5)];   
  
  % Left sensor position
  srx = robot(4);
  sry = robot(5);
  
  scx = X + srx * cos(alpha) - sry * sin(alpha);      
  scy = Y + srx * sin(alpha) + sry * cos(alpha);
  
%  sensL =  lebol_sensor(circuit, max_light, ax, scx, scy, rad_c, pixsize);
  sensL =  adonze_sensor(circuit, max_light, ax, scx, scy, rad_c, pixsize);
  
  % Right sensor position
  srx = robot(6);
  sry = robot(7);
  
  scx= X + srx * cos(alpha) - sry * sin(alpha);      
  scy= Y + srx * sin(alpha) + sry * cos(alpha);
  
  
%  sensR =  lebol_sensor(circuit, max_light, ax, scx, scy, rad_c, pixsize);
  sensR =  adonze_sensor(circuit, max_light, ax, scx, scy, rad_c, pixsize);
  
  % return values
  sys = [sensL sensR];
  
 % ----------------------------------  flag == 3 
elseif flag == 3        
    
  sys = x;
  
 % ----------------------------------  flag == 4
 elseif flag == 4 % Return next sample hit
  
  % ns stores the number of samples
  ns = t/ts;

  % This is the time of the next sample hit.
  sys = (1 + floor(ns + 1e-13*(1+ns)))*ts;

  % ----------------------------------  flag == 2
elseif flag==0,
   
 % x = robotinit(1);
 % y = robotinit(2);
 % theta = robotinit(3);
    
 % captG = 1;
 % captD = 1;

  
  sizes = simsizes;  
  sizes.NumContStates = 0;   %Number of continuous states
  sizes.NumDiscStates = 2;   %Number of discrete states
  sizes.NumOutputs    = 2;   %Number of outputs
  sizes.NumInputs     = 3;   %Number of inputs
  sizes.DirFeedthrough =0;   %Flag for direct feedthrough
  sizes.NumSampleTimes = 0;  %Number of sample times

  %  After initializing the structure above to fit the
  %  specifications   n = size(circuit);  % Array dimensions, returns the sizes of each dimension of array X in a vector d with of the S-function, SIMSIZES should be called
  %  again to convert the structure into a vector that can be 
  %  processed by Simulink. For example:
    
  sys = [ 0 2 2 3 0 0 ];

  x0=[1 1];

end

    
function value = adonze_sensor(circuit, max_light, ax, x, y, radius, pixsize)
% MY_SENSOR computes the output of one sensor given a circuit and a
% position
  
% Alexandre Donze, 02-11-2011
     
   n = size(circuit);  % Array dimensions, returns the sizes of each dimension of array X in a vector d with ndims(X) element
   
   % coordonates in pixel of the center of the sensor
   nx = x2pix(x,ax,n);
   ny = y2pix(y,ax,n);

   % radius in pixel
   rnx = floor(radius* (n(2)-1)/(ax(2)-ax(1)));
   rny = floor(radius* (n(1)-1)/(ax(4)-ax(3)));
    
   tot =0;
   value =0;

   

   for j = nx-rnx:nx+rnx 
     ry = floor( sqrt( rny^2 - ((j-nx)*rny/rnx)^2 ));
     i = ny-ry;
     for i = ny-ry:ny+ry
       tot = tot+1;
       if ((i >= 0) && ( j>=  0) && (i < n(1)) && (j <  n(2)))
       if (circuit(i,j,1) == max_light)
         value = value+1;
       end 
       else
           value = value+1; 
       end
       %       value = value + (circuit(i,j,1)+circuit(i,j,2)+circuit(i,j,3))/(3*max_light);    
     end
   end
  
   value = 100*double(value/tot);
       
function value = lebol_sensor(circuit, max_ts,l,Rclight, ax, x, y, radius, pixsize)
% MY_SENSOR computes the output of one sensor given a circuit and a
% position
     
   n = size(circuit);  % Array dimensions, returns the sizes of each 
   % dimension of array X in a vector d with ndims(X) element
   
    % radius in pixel
   nr = floor((radius / pixsize) - .5) + 1;
     
   % coordonates in pixel of Left top corner of the sensor
   sx = x2pix(x-radius,ax,n);
   sy = y2pix(y-radius,ax,n);

    
   tot =0;
   value =0;

   % square zone  
   for j = sx:sx+2*nr
       for i = sy:sy+2*nr
       tot = tot+1;
       if ((i >= 0) && ( j>=  0) && (i < n(1)) && (j <  n(2)))
           if (circuit(i,j,1) >= max_light)
               value = value+1;
           end
       else
           value = value+1; 
       end
       %       value = value + (circuit(i,j,1)+circuit(i,j,2)+circuit(i,j,3))/(3*max_light);    
     end
   end
  
   value = 100*double(value/tot);

function nx =  x2pix(x, ax, n)
% x position in pixel  
  nx = floor( (x-ax(1)) * (n(2)-1)/(ax(2)-ax(1))) +1;
  
function ny =  y2pix(y, ax, n)
  % y positoin in pixel
  ny = floor( (y-ax(3))* (n(1)-1)/(ax(4)-ax(3))) +1;
 