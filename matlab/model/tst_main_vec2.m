clear all;

addpath('../common');

%% data and params initialization stage
F = fopen('../../base.dat','rb');
tmp = fread(F, inf, 'float');
fclose(F);

% THESE ARE THE MAIN INPUT PARAMETERS OF THE ALGORITHM
eps = 0.01; % we here just presume some 'small' number, it's small size is defined by comparison with full range of input numbers
CV_NUM = 512; % we here take presume some number of resulting code vectors as a power of 2
vec_len = 13; % given by default

vec_num = length(tmp) / vec_len;
tmp1 = reshape(tmp, vec_len, vec_num);
tmp1 = tmp1';

% in this simple for visualization example we set vector length equal to 2
vec_len = 2;
vec_num = 5000;

% cutting some data off to speed up calculations in testing example 
data = tmp1(:, 1:vec_len);
data = data(1:vec_num, :);

%figure;
%plot(data(:,1), data(:,2), '.');


%% algorithm initialization stage ...

C_init = mean(data);
D_init = calc_dist_measure(C_init, data, vec_num);

D_final_init = sqrt(D_init * vec_len);

% figure;
% hold on;
% plot(data(:,1), data(:,2), '.');
% plot(C(1), C(2), 'r*');

%% actual iterative algorithm
NUM_STEPS = ceil(log2(CV_NUM));
Q = cell(1, CV_NUM);
N = 1;
C_cur = cell(1, N);
C_cur{1} = C_init; % this one will later become a cell array whos' length grows by factor of 2 on each iteration...
D_cur = D_init;
for code_num = 1 : NUM_STEPS
    Ci = cell(1, N*2);

    for i = 1 : N
        Ci{i} = (1 + eps)*C_cur{i};
        Ci{N+i} = (1 - eps)*C_cur{i};
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
    DO = 1;
    D_tmp = D_cur;
    D_final(code_num) = D_cur;
    D_final1(code_num) = D_final_init * (1-eps)^(log2(N));
    while (DO)
        % DBG
        fprintf('Working on code %d ... iter_num = %d ... D_cur = %f ...\n', code_num, iter_num, D_tmp);
              
        Ci1 = cell(1, N);
        tmp_sum = cell(1, N);
        tmp_num = zeros(1,N);
        for n = 1:N
            tmp_sum{n} = zeros(1, vec_len);
        end
        for m = 1: vec_num
            [~, code_idx] = select_code_for_input(Ci, data(m,:));
            tmp_sum{code_idx} = tmp_sum{code_idx} + data(m,:);
            tmp_num(code_idx) = tmp_num(code_idx) + 1;
        end
        for n = 1:N
                    Ci1{n} = tmp_sum{n} ./ tmp_num(n);
        end

        %Ci
        %Ci1
        
        iter_num = iter_num + 1;
        D_tmp = calc_dist_measure_by_codes(Ci1, data);
    
        %D_cur
        %Davg(iter_num)
        
        DD = D_cur - D_tmp;
        
        %DBG
        if ( DD < 0)
            error('Oooops ... STRANGE! DD is supposed to be positive ... ');
        end
        
        if ((DD / D_cur) <= eps)
            DO = 0; %end of current iteration ...
        end
        
        D_cur = D_tmp;
        Ci = Ci1;
        
        %DBG
        fprintf('Results of iteration: DD/D_tmp = %f ...\n', DD/D_tmp);
    end % of while(DO)
    
    % DBG
    if (code_num >= 200)
        figure;
        hold on;
        plot(data(:,1), data(:,2), '.');
        for i = 1:N/2
            plot(C_cur{i}(1), C_cur{i}(2), 'r*');
        end
        for i = 1:N
            plot(Ci{i}(1), Ci{i}(2), '*m');
            %plot(Ci{N+i}(1), Ci{N+i}(2), '*m');
        end
        return;
    end
    
    D_cur = D_tmp;
    C_cur = Ci;
end % of 1 : CV_NUM

% plotting final algorithm result
figure;
hold on;
plot(data(:,1), data(:,2), '.c');
for i = 1:CV_NUM
    plot(C_cur{i}(1), C_cur{i}(2), 'r*');
end

figure;
hold on;
plot(sqrt(D_final*13));
plot(D_final1,'r');
str = sprintf('EPSILON = %1.2f', eps);
title(str);

rmpath('../common');

return;
