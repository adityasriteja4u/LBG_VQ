clear all;

addpath('../common');



%% data and params initialization stage

VEC_LEN = 13;

orig_data = get_data_vector_from_file('../../test.dat', VEC_LEN);

VEC_NUM = length(orig_data(:,1));

cb_lens = [4 8 16 32 64 128 256 512 1024];
epsilon = 0.01;

%% actual calculations

dists = zeros(1, VEC_NUM);
mean_dists = zeros(1, length(cb_lens));
max_dists = zeros(1, length(cb_lens));
for k = 1: length(cb_lens)
    cur_cb_len = cb_lens(k);
    decoded = get_decoded_data_vector_from_file('../../lbg_vq_cpp/Release', 'decoded', 'dec', cur_cb_len, epsilon, VEC_LEN);
    
    for m = 1 : VEC_NUM
        dists(m) = sqrt(sum((orig_data(m,:) - decoded(m,:)).^2));
    end
    
    mean_dists(k) = mean(dists);
    max_dists(k) = max(dists);
end

%% visualization

figure;
stem( cb_lens, mean_dists, 'r*');
legend('Mean norms');
xlabel('code book length');
ylabel('mean norm');

figure;
stem( cb_lens, max_dists, 'r*');
legend('Max norms');
xlabel('code book length');
ylabel('max norm');

%% cleanup ...
rmpath('../common');