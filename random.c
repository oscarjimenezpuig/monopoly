/* MONOPOLY 26/4/26 */

int rnd(int a,int b) {
    static u1 def=0;
    if(!def) {
        srand(time(NULL));
        def=1;
    }
    int min=(a<b)?a:b;
    int max=(a>b)?a:b;
    int dif=max-min+1;
    return min+rand()%dif;
}

u2 dado(u1 n) {
    u2 total=0;
    for(u1 k=0;k<n;k++) total+=rnd(1,6);
    return total;
}
    
