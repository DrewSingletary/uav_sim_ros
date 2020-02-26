function [DF,DG] = UAVDynamicsGradient(X)
    DF = matDF(X);
    DG = matDG(X);
end