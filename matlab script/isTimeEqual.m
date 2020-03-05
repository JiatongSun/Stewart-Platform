function flag = isTimeEqual(varargin)
    extend_time = 0;
    retract_time = 0;
    is_extending = true;
    for i=1:nargin-1
        if is_extending
            extend_time = extend_time + varargin{i+1} - varargin{i};
        else
            retract_time = retract_time + varargin{i+1} - varargin{i};
        end
        is_extending = ~is_extending;
    end
    flag = (extend_time == retract_time);
end