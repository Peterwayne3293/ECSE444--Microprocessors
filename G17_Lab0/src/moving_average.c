#include <stdio.h>

/*
int main()
{
    int a[6] = {0,1,2,3,4,5,6};
    int b[6] = {0};
    printf("%d\n",movingAverage(&a,&b,6,5));
}

int movingAverage(int *ptrSample, int *ptrFiltered, int N, int D){
  int result;
  int n = 3; // calculate filtered signal at index 3
  int sum = 0;// numerator sum variable
  int temp; // temp index to increment;
  int *tmp; // temporary pointer to move.
  int i = 0;//second temp counter
  int sum_upper;
  int sum_lower;
  if(D%2!=0){  // odd depth, calculate upper and lower bounds as shown.
     int upper_lower_bound = D/2; // automatically rounds down
      int index_to_start_at = n-upper_lower_bound; //index to start summing at
      int index_to_end_at = n+upper_lower_bound; //index to stop summing at
      temp = n;
      while(temp<=(index_to_end_at)){ //sum up to upper bound.
        if(temp>N){
          break;    //prevent index out of bounds.
        }
        sum_upper += *(ptrSample+i);
        i++;
        temp++;
      }
      i = 1;
      temp = n;
      while(temp>=(index_to_start_at)){ //sum up to upper bound.(accessing -1)
        if(temp<0){
          break;  //prevent index out of bounds exception.
        }
        sum_lower += *(ptrSample-i);
        i++;
        temp--;
      }

      sum = sum_upper + sum_lower;
      *(ptrFiltered) = sum/D;
      result = sum/D;
  }
 return result;
}
*/
#include <stdio.h>

int main()
{
    int movingAverage;
    int a[6] = {10,11,22,31,48,52};        //x[n-D/2]...+x[n-1]+x[n]
    int b[6] = {0};
    printf("%d\n",sumUpto(&a,&b,5,2));
    movingAverage = sumUpto(&a,&b,5,2)+sumDownto(&a,&b,5,2);
    printf("%d\n",movingAverage/5);
}

int sumDownto(int *sample, int *result,int D, int n){
    int sum;
    int temp = 0;
    int lower_bound = D/2;
    for(int i=n;i>=(n-D/2);i--){
        sum += *(sample+n-temp);
        temp++;
    }
  return sum;
}
int sumUpto(int *sample, int *result,int D, int n){
    int sum;
    int amount_to_be_removed = *(sample+n);
    int temp = 0;
    int lower_bound = D/2;
    for(int i=n;i<=(n+D/2);i++){
        sum += *(sample+n+temp);
        temp++;
    }
    sum -= amount_to_be_removed;
    return sum;
}
