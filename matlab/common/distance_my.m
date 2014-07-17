function D = distance_my(center, data, data_len)
% we here assume, that center is 1xN vector, data is MxN matrix

    M = data_len;
    D = zeros(1, M);
    tmp = zeros(1, length(center));
    for i = 1 : M
%           D(i) = (norm(data(i,:) - center))^2;
          tmp = data(i,:) - center;
          D(i) = tmp * tmp';
    end
end % of function