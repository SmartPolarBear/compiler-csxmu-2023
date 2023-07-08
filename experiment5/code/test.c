{
    int i;
    float sum;
    bool flag;
    i = 1;
    sum = 0;
    flag = false;
    while (i <= 100 || 3000 < sum && sum < 5000 || !flag)
    {
        sum = sum + i;
        i = i + 2;
        if (sum < 3000 || sum > 5000)
        {
            char c;
            c = 'y';
            flag = true;
            i = -1 + !(c == 'y');
            break;
        }
    }
}
