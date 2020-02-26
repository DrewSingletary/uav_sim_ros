function [f,g,DFcl] = UAVDynamicsWithGrad(X,V)
    f = matf(X);
    g = matg(X);
    DFcl = matDFcl(X,V);
end