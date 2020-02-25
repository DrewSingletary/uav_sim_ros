%% Compute gradients of dynamics
addpath('dynamics')
generateMatlabFunctions()
%% codegen
cfg = coder.config('lib');
cfg.GenCodeOnly = true;
cfg.TargetLang = 'C++';

codegen -config cfg UAVDynamicsWithGrad -args {zeros(17,1,'double'),zeros(4,1,'double')}
codegen -config cfg UAVDynamics -args {zeros(17,1,'double'),zeros(4,1,'double')}
%%
function generateMatlabFunctions()
clear all;
con = quad1_constants();

x = sym('x',[3,1],'real');
v = sym('v',[3,1],'real');
q = sym('q',[4,1],'real');
w = sym('w',[3,1],'real');
Omega = sym('Omega',[4,1],'real');
V=sym('V',[4,1],'real');
d = 0;
[dr, ddr, dxi_q, dw, dOmega] = full_eul(x,v,q,w,Omega,V*14.8,d,con.m,con.g,con.J_bod,con.D,con.J_rot, con.J_prop, con.K_v, con.R, con.k_t, con.k_f,true);
X = [x;q;v;w;Omega];
Xdot = [dr;dxi_q;ddr;dw;dOmega];
f = Xdot - jacobian(Xdot,V)*V;
g = jacobian(Xdot,V);
matlabFunction(f,'Vars',{X},'File','matf');
matlabFunction(g,'Vars',{X},'File','matg');
DF = jacobian(Xdot,X);
matlabFunction(DF,'Vars',{X,V},'File','matDF');
end
