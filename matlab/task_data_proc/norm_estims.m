clear all;

addpath('../common');



%% data and params initialization stage

VEC_LEN = 13;

orig_data = get_data_vector_from_file('../../test.dat', VEC_LEN);
learn_data = get_data_vector_from_file('../../base.dat', VEC_LEN);

%DBG!!!!
% orig_data = [orig_data; orig_data; orig_data; orig_data];

VEC_NUM = length(orig_data(:,1));

cb_lens = [4 8 16 32 64 128 256 512 1024];
epsilon = 0.05;
i_mean = 1.1;

%% actual calculations

dists = zeros(1, VEC_NUM);
mean_dists = zeros(1, length(cb_lens));
max_dists = zeros(1, length(cb_lens));

ref_mean = zeros(1, length(cb_lens));
ref_max = zeros(1, length(cb_lens));

% VEC_LEN-normalized sum of std-deviations of learning data
% V = sum(sqrt(diag(cov(learn_data))))/VEC_LEN; 

% VEC-LEN-normalized sqrt of sum of variances of learning data
V = sqrt(mat_trait(cov(learn_data)));
% V = sqrt(mat_trait(cov(learn_data)));

V1 = V*3; % 3-sigmas-rule
M = (1 - epsilon);
% M = (1 - 0.05); % somehow it works for all tested values of epsilon (0.01, 0.05, 0.1) ...

%dbg
% orig_data = learn_data;

for k = 1: length(cb_lens)
    cur_cb_len = cb_lens(k);
    decoded = get_decoded_data_vector_from_file('../../lbg_vq_cpp/Release', 'decoded', 'dec', cur_cb_len, epsilon, VEC_LEN);
    
    %DBG!!!!
%     decoded = [decoded; decoded; decoded; decoded];
    
    for m = 1 : VEC_NUM
        %dists(m) = sqrt(sum((orig_data(m,:) - decoded(m,:)).^2));
        dists(m) = norm(orig_data(m,:) - decoded(m,:));
    end
    
    mean_dists(k) = mean(dists);
    max_dists(k) = max(dists);
    
    K = M^(log2(cb_lens(k)) * i_mean);
    ref_mean(k) = V * K;
    ref_max(k) = 3 * ref_mean(k);
end

%% visualization

figure;
hold on;
stem( log2(cb_lens), mean_dists, 'g*');
plot( log2(cb_lens), ref_mean, '--g');
legend('Error mean norm', 'D(size)');
xlabel('log2(size)');
ylabel('estimated values');
str = sprintf('Epsilon = %1.2f, i_mean = %1.2f', epsilon, i_mean);
title(str);

figure;
hold on;
stem( log2(cb_lens), max_dists, 'r*');
plot( log2(cb_lens), mean_dists, 'g');
legend();
xlabel('log2(size)');
ylabel('estimated values');
legend('Error max norm', 'Error mean norm');
str = sprintf('Epsilon = %1.2f, i_mean = %1.2f', epsilon, i_mean);
title(str);

%% cleanup ...
rmpath('../common');