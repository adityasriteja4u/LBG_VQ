function D = distance(center, data)
% we here assume, that center is 1xN vector, data is MxN matrix

M = length(data(:,1));
D = zeros(1, M);

for i = 1 : M
    D(i) = sqrt( sum( (data(i,:) - center).^2 ) );
end % of function