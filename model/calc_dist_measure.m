function D = calc_dist_measure(center, data)
% we here assume, that center is 1xN vector, data is MxN matrix

N = length(center); 
M = length(data(:,1));

% Dsum = 0;
% for i = 1 : M
%     Dc = distance(center, data(M,:));
%     Dsum = Dsum + Dc;
% end
Dc = distance(center, data);
Dsum = sum(Dc);

D = Dsum / (N*M);

end % of function