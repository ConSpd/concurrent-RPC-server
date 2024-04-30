struct data{
    double a;
    int y<>;
};

struct d_ret{
    int a[2];
};

struct var_array{
    double j<>;
};

program Service{
    version Srv_1{
        double average(data) = 1;
        d_ret min_max(data) = 2;
        var_array prod(data) = 3; 
    } = 1;
} = 0x24291294;