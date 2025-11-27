
int findMissingNum(int a[], int n) {
    int total = n * (n + 1) / 2; 
    int sum = 0;

   
    for (int i = 0; i < n - 1; i++) {
        sum += a[i];
    }

    return total - sum;  
}
