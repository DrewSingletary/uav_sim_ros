function [XDot] = UAVDynamics(X,V)
    f = matf(X);
    g = matg(X);
    XDot = f+g*V;
end