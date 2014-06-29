function D = calc_dist_measure(center, data, data_len)
% we here assume, that center is 1xN vector, data is MxN matrix

N = length(center); 
M = data_len;

% Dsum = 0;
% for i = 1 : M
%     Dc = distance(center, data(M,:));
%     Dsum = Dsum + Dc;
% end
Dc = distance(center, data, data_len);
Dsum = sum(Dc);

D = Dsum / (N*M);

end % of function