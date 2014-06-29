function data = get_decoded_data_vector_from_file( dir, prefix, extension, cb_size, epsilon, vec_len)

    file_name = sprintf('%s/%s_codebook_sz_%d_eps_%1.2f.%s', dir, prefix, cb_size, epsilon, extension);

    data = get_data_vector_from_file(file_name, vec_len);

end % of function