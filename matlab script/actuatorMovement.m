function dist = actuatorMovement(varargin)
    dist = 0;
    direction = 1;
    for i = 1:nargin-1
        dist = dist + direction * (parabolicIntegral(varargin{i}, varargin{i+1}));
        direction = -direction;
    end
end