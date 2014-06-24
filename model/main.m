clear all;

F = fopen('../base.dat','rb');
tmp = fread(F, inf, 'float');
fclose(F);

vec_len = 13; % given by default
vec_num = length(tmp) / vec_len;

data = reshape(tmp, vec_num, vec_len);


