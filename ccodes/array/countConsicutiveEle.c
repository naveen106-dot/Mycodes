#include <stdio.h>
#include <stdlib.h>

int checkValue(int value ,int *arrcopy,int size){

    int i = 0,j = size;
    int count = 1;
    int left = value-1,right = value+1;
    printf("Size:%d\n",size);     // size of the array

    // mark the starting value as visited
    for (int k = 0; k < size; k++) {
        if (arrcopy[k] == value) {
            arrcopy[k] = 0;
            break;
        }
    }
    while(i < j){
        if (arrcopy[i] == 0) {   // already visited
            i++;
            continue;
        }
        if( left ==  arrcopy[i]){
            count++;
            left--;
            // arrcopy[i] = 0;
            i=0;
        }else if(right  == arrcopy[i] ){
            count++;
            right++;
            // arrcopy[i] = 0;
            i=0;
        }
        i++;
    }
    return count;
}

int countConsicitive(int arr[],int size){
    int count =1,maxCount = 1;
    int *arrcopy = malloc(size * sizeof(int));
    for(int j=0;j<size;j++){
        arrcopy[j] = arr[j];
        //printf("arrcopy[%d] : %d\n",j,arrcopy[j]);
    }
    for(int i=0;i<size;i++){

        if (arrcopy[i] == 0)   // already processed
            continue;
        
        count = checkValue(arrcopy[i],arrcopy,size);
        //arrcopy[i] = 0;
        printf("count :%d\n",count);
        if (maxCount < count){
            maxCount = count;
        }
    }
     free(arrcopy);
    printf("maxCount :%d\n",maxCount);
}



int main(){
    int arr[] = {100,1,2,3,4,16,4,2,1,3,200,7,9,3,5,6,101};

    int size = sizeof(arr)/sizeof(arr[0]);
    printf("Size:%d\n",size);             // size of the array
    countConsicitive(arr,size);          

}