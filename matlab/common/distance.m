function D = distance(center, data, data_len)
% we here assume, that center is 1xN vector, data is MxN matrix

    M = data_len;
    D = zeros(1, M);

    for i = 1 : M
%         D(i) = sqrt( sum( (data(i,:) - center).^2 ) );
          D(i) = norm(data(i,:) - center);
    end
end % of function