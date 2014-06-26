function [C, cidx] = select_code_for_input(codes, x)
    % we here asume, that 'codes' is a cell array of 1xN vectors, 'x' - 1xN vector 

    D = zeros(1, length(codes));
    for n = 1 : length(codes)
        D(n) = distance(codes{n}, x, 1);
    end

    [~, cidx] = min(D);
    C = codes{cidx};
end % of function