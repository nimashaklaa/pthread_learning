#include <stdio.h>

double compute_pi_sequential(int n){
    double sum = 0.0;
    double factor =1.0;

    for(int i =0;i<n;i++,factor =-factor){
         double term =factor/(2*i+1);
         sum +=term;

         // Adding diagnostics to trace the values
        if (i < 10 || i >= n - 10) {  // Print the first and last 10 terms
            printf("Term %d: factor = %f, (2*i+1) = %d, term = %f, sum = %f\n", i, factor, 2*i+1, term, sum);
        }
    }
    printf("The sum: %f\n", sum);
    return 4.0*sum;
}

int main(){
    int n =1000000;
    double pi = compute_pi_sequential(n);
    printf("Approximated pi: %.12f\n",pi);
    return 0;
}