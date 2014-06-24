clear all;

%% data and params initialization stage
F = fopen('../base.dat','rb');
tmp = fread(F, inf, 'float');
fclose(F);

vec_len = 13; % given by default
vec_num = length(tmp) / vec_len;
tmp1 = reshape(tmp, vec_num, vec_len);

% in this simple for visualization example we set vector length equal to 2
data = tmp1(:, 1:2);
vec_len = 2;
%data = [rand(vec_num, 1) rand(vec_num, 1)];

%figure;
%plot(data(:,1), data(:,2), '.');


eps = 0.1; % we here just presume some 'small' number, it's small size is defined by comparison with full range of input numbers
CV_NUM = 4; % we here take presume some number of resulting code vectors

%% algorithm initialization stage ...

C = mean(data);
D = calc_dist_measure(C, data);

% figure;
% hold on;
% plot(data(:,1), data(:,2), '.');
% plot(C(1), C(2), 'r*');

%% actual iterative algorithm
Q = cell(1, CV_NUM);
for code_num = 1 : CV_NUM
    N = 1;
    Ci = cell(1, N*2);

    for i = 1 : N
        Ci{i} = (1 + eps)*C
        Ci{N+i} = (1 - eps)*C
        i
        N+i
    end

%     figure;
%     hold on;
%     plot(data(:,1), data(:,2), '.');
%     plot(C(1), C(2), 'r*');
%     for i = 1:N
%         plot(Ci{i}(1), Ci{i}(2), '*m');
%         plot(Ci{N+i}(1), Ci{N+i}(2), '*m');
%     end

    N = N*2;
    
    iter_num = 0;
    Dmin_arr = zeros(1, N);
    for n = 1 : N
        Dmin_arr(n) = min(distance(Ci{n}, data));
    end
    
    [Dmin, n_min] = min(Dmin_arr);
    Q{code_num} = Ci{n_min};
    
%     figure;
%     hold on;
%     plot(data(:,1), data(:,2), '.');
%     plot(C(1), C(2), 'r*');
%     plot(Q{code_num}(1), Q{code_num}(2), '*g');
    
    return;
end % of 1 : CV_NUM
