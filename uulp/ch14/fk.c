struct w{
    int (*with)(void);
};

int main(void)
{
    struct w hhhh;
    hhhh.with = main;
}
