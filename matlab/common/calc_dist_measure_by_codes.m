function D = calc_dist_meadure_by_codes(centers, data)
% we here assume, that:
% 1) 'centers' is 1xK cell array, each cell is 1xN vector
% 2) 'data' is MxN matrix

N = length(centers{1}); 
M = length(data(:,1));

D = 0;
for k = 1 : M
    Xk = data(k,:);
    Ck = select_code_for_input(centers, Xk);
    Dk = distance(Ck, Xk, 1);
    D = D + Dk;
end

D = D / (N*M);

end % of function
