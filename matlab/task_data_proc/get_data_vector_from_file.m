function data = get_data_vector_from_file( file_name, vec_len)

    F = fopen(file_name);
    tmp = fread(F, inf, 'float');
    fclose(F);

    vec_num = length(tmp) / vec_len;
    tmp1 = reshape(tmp, vec_len, vec_num);
    tmp1 = tmp1';
    data = tmp1(:, 1:vec_len);

end % of function