void setup(void);
void transaction(void);
void finish();
int main(void)
{
    setup();

    while (1) {
        transaction();
    }

    finish();
    return 0;
}
