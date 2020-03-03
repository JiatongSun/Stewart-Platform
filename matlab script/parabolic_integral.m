function dist = parabolic_integral(t_start, t_end)
    syms t
    h = (t_start + t_end) / 2;
    t_tot = t_end - t_start;
    pwm = 245 - 230 / (t_tot / 2) ^ 2 * (t - h) ^ 2;
    dist = int(pwm, t_start, t_end);
end