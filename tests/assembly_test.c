int x, y;
long k, m, n;

int main(void)
{
    x + y;                /* one addl */
    x + y + 1;            /* two addls in a row */
    (x + 1) + (y + 1);    /* three addls but not in a row */
    m + n;                /* one addq */
    m + n + k;            /* two addqs in a row */
    (m + k) + (n + k);    /* three addqs but not in a row */
}