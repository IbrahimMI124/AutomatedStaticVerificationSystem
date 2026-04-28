function [sys,x0]=robot_anim(t,x,u,flag,ts,env,robot,robotinit)
%ROBOT_ANIM S-function Level-1 for animating the motion of a doube-wheeled robot.

%   Alexandre Donze, 02-01-2011
%   O Lebeltel 03-2012


  % ----------------------------------  flag == 2
if flag==2, % MdlUpdate


    
  anim_fig = find_anim_fig();
  circuit = evalin('base', 'circuit');
  
  set(0,'currentfigure', anim_fig);
  hndlList=get(gca,'UserData');
  

  X = u(1);
  Y = u(2);
  alpha = u(3);
  theta =alpha-pi/2;
  
  captG = u(4);
  captD = u(5);
  
  % inter wheel distance
  dW = robot(1);

  % sensor radius
  rad_c = robot(3);

  rw = dW / 2;
  
  x1 = X - rw * cos(theta);      
  x2 = X + rw * cos(theta);
  y1 = Y - rw * sin(theta);
  y2 = Y + rw * sin(theta);
             

   
  set(hndlList(1),'XData',[x1 x2],'YData',[y1 y2]);
  set(hndlList(2),'XData',[X X + rw * cos(alpha)],'YData',[Y Y + rw * sin(alpha)]); 
  
  set(hndlList(4),'String',['Capteur D: ' num2str(captD)]);
  set(hndlList(5),'String',['Capteur G: ' num2str(captG)]);


  
  % ------------------------------ draw Left sensor 
  % Left sensor position
  srx = robot(4);
  sry = robot(5);
  
  scx= X + srx * cos(alpha) - sry * sin(alpha);      
  scy= Y + srx * sin(alpha) + sry * cos(alpha);

  th = 0:pi/50:2*pi;
  xunit = scx + (rad_c * cos(th));
  yunit = scy + (rad_c * sin(th));
  
  set(hndlList(6),'XData',xunit,'YData',yunit);
 
 % set(hndlList(8),'String',['Left(' num2str(scx ) ', ' num2str(scy) '), ('  num2str(srx) ', ' num2str(sry) ')' ] );
  
 % sx= stopsim;
  % ------------------------------ draw Right sensor 
  % Left sensor position
  srx = robot(6);
  sry = robot(7);
  
  scx= X + srx * cos(alpha) - sry * sin(alpha);      
  scy= Y + srx * sin(alpha) + sry * cos(alpha);
  
  th = 0:pi/50:2*pi;
  xunit = scx + (rad_c * cos(th));
  yunit = scy + (rad_c * sin(th));
  
  set(hndlList(7),'XData',xunit,'YData',yunit);
  
 
  if (X<-3 && Y<-3) 
    set(hndlList(3),'Color',[1 0 0]);
  else
    set(hndlList(3),'String',['Time: ' num2str(t)]);
  end
  
  drawnow;
    
  sys=[];

  % ---------------------------------- flag == 3 
elseif flag == 3        
    
  sys = [];
  
  
  % ---------------------------------- flag == 4 
elseif flag == 4 % Return next sample hit
  
  % ns stores the number of samples
  ns = t/ts;

  % This is the time of the next sample hit.
  sys = (1 + floor(ns + 1e-13*(1+ns)))*ts;

  
  % ---------------------------------- flag == 0
elseif flag==0,
   
  % _________________________  TODO !!!!!!!  
  % ax = [-1 1 -1 1];
  
  ax = [env(2) env(3) env(4) env(5)];   
   
 
  h = find_anim_fig();
  
  if isempty(h)
    h = figure('Name', 'Robot Animation');
    axis(ax);
    hold on;     
  end
  
  figure(h); 
  cla; 
  grid on;     
  
  % affiche le circuit
  circuit = evalin('base', 'circuit');  
  image(ax(1:2), ax(3:4), circuit);
  
  
  % INITIAL POSITION ---------- 
  X = robotinit(1);
  Y = robotinit(2);
  alpha = robotinit(3);
  theta = alpha - pi/2;
    
  % captG = 1;
  % captD = 1;

  % inter wheel distance
  dW = robot(1);
  rw = dW / 2;
  
  x1 = X - rw * cos(theta);      
  x2 = X + rw * cos(theta);
  y1 = Y - rw * sin(theta);
  y2 = Y + rw * sin(theta);
  
  
  hndlList(1) = plot([x1 x2], [y1 y2],'LineWidth',2,'EraseMode','normal');
  hndlList(2) = plot([X X + rw * cos(alpha)], [Y Y + rw * sin(alpha)],'LineWidth',1,'EraseMode','normal');
  
  hndlList(3) = text(ax(2)*1.1, ax(4)*1.1, num2str(t));
  hndlList(4) = text(ax(2)*1.1, ax(4)*1.1 - 0.3, num2str(t));
  hndlList(5) = text(ax(2)*1.1, ax(4)*1.1 - 0.6, num2str(t));
    
 % hndlList(8) = text(ax(1)*1.1, ax(4)*1.1, num2str(t));
  
  
  % ajout capteur
  
  % http://www.mathworks.fr/support/solutions/en/data/1-15I2I/index.html?product=ML&solution=1-15I2I
  % hold on
  % sensor radius
  rad_c = robot(3);
  
  % sensor position in robot ref
  lslx = robot(4);
  lsly = robot(5);
  
  th = 0:pi/50:2*pi;
  sx = X + (lslx * cos(alpha));      
  sy = Y + (lsly * sin(alpha));
  xunit = rad_c * cos(th) + sx;
  yunit = rad_c * sin(th) + sy;  
  h = plot(xunit, yunit,'LineWidth',1,'EraseMode','normal');
  hndlList(6) = h;
  
  
  % sensor position in robot ref
  lsrx = robot(6);
  lsry = robot(7);
  
  th = 0:pi/50:2*pi;
  sx = X + (lsrx * cos(alpha));      
  sy = Y + (lsry * sin(alpha));
  xunit = sx + (rad_c * cos(th));
  yunit = sy + (rad_c * sin(th));  
  h = plot(xunit, yunit,'LineWidth',1,'EraseMode','normal');
  hndlList(7) = h;
  
  
  
  set(gca,'DataAspectRatio',[1 1 1]);
  set(gca,'UserData',hndlList);
  
  sizes = simsizes;  
  sizes.NumContStates = 0;   %Number of continuous states
  sizes.NumDiscStates = 2;   %Number of discrete states
  sizes.NumOutputs    = 0;   %Number of outputs
  sizes.NumInputs     = 5;   %Number of inputs
  sizes.DirFeedthrough =0;   %Flag for direct feedthrough
  sizes.NumSampleTimes = 0;  %Number of sample times

  %  After initializing the structure above to fit the
  %  specifications of the S-function, SIMSIZES should be called
  %  again to convert the structure into a vector that can be 
  %  processed by Simulink. For example:
    
  sys = [ 0 2 0 5 0 0 ];

  x0=[1 1];

end

function h = circle(x,y,r)

hold on
th = 0:pi/50:2*pi;
xunit = r * cos(th) + x;
yunit = r * sin(th) + y;
h = plot(xunit, yunit);
hold off


function h = circle2(x,y,r)
d = r*2;
px = x-r;
py = y-r;
h = rectangle('Position',[px py d d],'Curvature',[1,1]);
daspect([1,1,1])



function h= find_anim_fig()
  
  shh = get(0,'ShowHiddenHandles');
  set(0,'ShowHiddenHandles','on');
  h = findobj('Type','figure','Name','Robot Animation')';  
  set(0,'ShowHiddenHandles',shh);
