function [f,g,DFcl] = UAVDynamicsWithGrad(X,V)
    f = matf(X);
    g = matg(X);
    DFcl = matDF(X,V);
end