%% retrieve stuff
clear all
addpath("/Volumes/Shared/ROS/bags")

bag = rosbag('new_contracts_2.bag');
bsel = select(bag,'Topic','/uav1/uav_state');
msgStructs = readMessages(bsel,'DataFormat','struct');
for i=1:length(msgStructs)
    x(i)=msgStructs{i}.X;
    y(i)=msgStructs{i}.Y;
    z(i)=msgStructs{i}.Z;
    q(1,i)=msgStructs{i}.Qw;
    q(2,i)=msgStructs{i}.Qx;
    q(3,i)=msgStructs{i}.Qy;
    q(4,i)=msgStructs{i}.Qz;
    v(1,i)=msgStructs{i}.Vx;
    v(2,i)=msgStructs{i}.Vy;
    v(3,i)=msgStructs{i}.Vz;
    w(1,i)=msgStructs{i}.Omegax;
    w(2,i)=msgStructs{i}.Omegay;
    w(3,i)=msgStructs{i}.Omegaz;
    vb(1,i)=msgStructs{i}.Vbx;
    vb(2,i)=msgStructs{i}.Vby;
    vb(3,i)=msgStructs{i}.Vbz;
    Omega(1,i)=msgStructs{i}.Omega1;
    Omega(2,i)=msgStructs{i}.Omega2;
    Omega(3,i)=msgStructs{i}.Omega3;
    Omega(4,i)=msgStructs{i}.Omega4;
    t(i) = msgStructs{i}.Header.Stamp.Sec;
    tn(i) = msgStructs{i}.Header.Stamp.Nsec;
end
Tx = double(t)+double(tn)/1e9;
X = [x',y',z',q',v',w',Omega',vb'];
fprintf("state rate: %f\n",1/((Tx(end)-Tx(1))/length(t)));
%%
clear t tn
bsel = select(bag,'Topic','/uav1/uav_input_des');
msgStructs = readMessages(bsel,'DataFormat','struct');
for i=1:length(msgStructs)
    U(i,:) = msgStructs{i}.Input;
    t(i) = msgStructs{i}.Header.Stamp.Sec;
    tn(i) = msgStructs{i}.Header.Stamp.Nsec;
end
Tu = double(t)+double(tn)/1e9;
fprintf("controller rate: %f\n",1/((Tu(end)-Tu(1))/length(t)));
%%
clear t tn
bsel = select(bag,'Topic','/uav1/uav_cmd');
msgStructs = readMessages(bsel,'DataFormat','struct');
for i=1:length(msgStructs)
    cmd(i,:) = msgStructs{i}.VDes;
    t(i) = msgStructs{i}.Header.Stamp.Sec;
    tn(i) = msgStructs{i}.Header.Stamp.Nsec;
end
Tc = double(t)+double(tn)/1e9;
fprintf("vel command rate: %f\n",1/((Tc(end)-Tc(1))/length(t)));
%%
tStart = max([Tu(1),Tx(1), Tc(1)]);
tEnd = min([Tu(end),Tx(end),Tc(end)]);
 
tIdx = Tx>=tStart & Tx<=tEnd; Tx = Tx(tIdx); X=X(tIdx,:); q = q(:,tIdx); v = v(:,tIdx); vb = vb(:,tIdx); 
 
tIdx = Tu>=tStart & Tu<=tEnd; Tu = Tu(tIdx); U=U(tIdx,:); 

tIdx = Tc>=tStart & Tc<=tEnd; Tc = Tc(tIdx); cmd=cmd(tIdx,:); 
 
Tx = Tx-tStart;
Tu = Tu-tStart;
Tc = Tc-tStart;
%% rotation stuff
for i = 1:length(q)
   R = quat2rotm(q(:,i)'); 
   [yaw(i),pitch(i),roll(i)] = quat2angle(q(:,i)');
   vw(:,i) = R*vb(:,i);
   vb2(:,i) = R^-1*v(:,i);
end
% plots for checking rotation matrices
figure(1)
ax1 = subplot(3,1,1); plot(Tx,v(1,:),Tx,vw(1,:)), title('world vx')
ax2 = subplot(3,1,2); plot(Tx,v(2,:),Tx,vw(2,:)), title('world vy')
ax3 = subplot(3,1,3); plot(Tx,v(3,:),Tx,vw(3,:)), title('world vz')
linkaxes([ax1,ax2,ax3],'x')
figure(2)
ax1 = subplot(3,1,1); plot(Tx,vb(1,:),Tx,vb2(1,:)), title('body vx')
ax2 = subplot(3,1,2); plot(Tx,vb(2,:),Tx,vb2(2,:)), title('body vy')
ax3 = subplot(3,1,3); plot(Tx,vb(3,:),Tx,vb2(3,:)), title('body vz')
linkaxes([ax1,ax2,ax3],'x')
%% plot stuff
figure(1)
x2 = cumtrapz(Tx,X(:,8));
y2 = cumtrapz(Tx,X(:,9));
z2 = cumtrapz(Tx,X(:,10));
sgtitle('World Frame Positions and Velocities')
ax1 = subplot(4,3,1); plot(Tx,X(:,1),Tx,x2), title('x'),legend('real','integral of vx')
ax2 = subplot(4,3,2); plot(Tx,X(:,2),Tx,y2), title('y'),legend('real','integral of vy')
ax3 = subplot(4,3,3); plot(Tx,X(:,3),Tx,z2), title('z'),legend('real','integral of vz')
ax4 = subplot(4,3,4); plot(Tx,X(:,8)), title('vx')
ax5 = subplot(4,3,5); plot(Tx,X(:,9)), title('vy')
ax6 = subplot(4,3,6); plot(Tx,X(:,10)), title('vz')
ax7 = subplot(4,3,7); plot(Tc,cmd(:,1)), title('cmdvx')
ax8 = subplot(4,3,8); plot(Tc,cmd(:,2)), title('cmdvy')
ax9 = subplot(4,3,9); plot(Tc,cmd(:,3)), title('cmdvz')
ax10 = subplot(4,3,10:12); plot(Tu,U), title('U')
linkaxes([ax1,ax2,ax3,ax4,ax5,ax6,ax7,ax8,ax9,ax10],'x')

figure(2)
sgtitle('Rotations and Angular Rates')
roll2 = cumtrapz(Tx,X(:,11));
pitch2 = cumtrapz(Tx,X(:,12));
yaw2 = cumtrapz(Tx,X(:,13));
ax1 = subplot(2,3,1); plot(Tx,roll,Tx,roll2), title('roll'),legend('real','integral of wx')
ax2 = subplot(2,3,2); plot(Tx,pitch,Tx,pitch2), title('pitch'),legend('real','integral of wy')
ax3 = subplot(2,3,3); plot(Tx,yaw,Tx,yaw2), title('yaw'),legend('real','integral of wz')
ax4 = subplot(2,3,4); plot(Tx,X(:,11)), title('wx')
ax5 = subplot(2,3,5); plot(Tx,X(:,12)), title('wy')
ax6 = subplot(2,3,6); plot(Tx,X(:,13)), title('wz')
linkaxes([ax1,ax2,ax3,ax4,ax5,ax6],'x')