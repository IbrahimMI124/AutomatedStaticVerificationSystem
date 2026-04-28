close all
clear


%parametre du robot 
l=0.2;  %axe inter-roue



%Configuration du controleur PI de l'angle 
alpha=1/l;
omega=1;
ksi=1;
ki_teta=omega*omega/alpha;
kp_teta=2*ksi*omega/alpha;

%exemple
%sys = tf([1 2],[1 0 10])
%specifies the transfer function (s+2)/(s^2+10) while 


%la fonction de transfert C(s) = ki_teta/s + kp_teta
%la fonction de transfert H_teta(s) = 1/(ls)
%la fonction de transfert en boucle ouvert 
%H_BO(s) = (kp_teta*s + ki_teta)/(ls^2)

sysbo = tf([0 kp_teta ki_teta], [ l 0 0])

[g p f wc] = margin(sysbo);

wc

T=0.05/wc;

%frequence de Nyquist
ws=2*pi/T;
wn= 2*ws;
gn= 0.1;
xi_f = 0.707;
wf = wn*sqrt(gn);

%filtre
filtre = tf([ wf^2 ], [ 1  2*xi_f*wf wf^2])


sysbo1 = sysbo*filtre;
[g p f wc1] = margin(sysbo1);

wc1

T1=0.05/wc1


%periode d'echantillonnage
T=0.05/wc
