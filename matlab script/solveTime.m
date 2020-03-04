function t_span = solveTime(varargin)
    syms t t0
    pwm = 245 - 230 * (t-t0/2)^2 / (t0/2)^2;
    distance = int(pwm,t,0,t0);
    t_span = zeros(1,nargin);
    for i = 1:nargin-1
        ratio_1 = varargin{i};
        ratio_2 = varargin{i+1};
        interval = abs(solve(distance==255*30*(ratio_2-ratio_1)));
        t_span(i+1) = t_span(i)+interval;
    end
end