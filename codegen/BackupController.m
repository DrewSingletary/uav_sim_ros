function [u,du] = UAVDynamics(X,M)
    u = backupU(X,M);
    du = backupDU(X,M);
end