// Check if the ith bit is set in a number
// Set the ith bit of a number.

// Clear the ith bit of a number.

// Toggle the ith bit of a number.

#include <stdio.h>

int pos,num;

enum operation {
    OP_CHECK,
    OP_SET,
    OP_CLEAR,
    OP_TOGGLE
};

void checkithBit(){
    
    printf("Enter the Number:");
    scanf("%d",&num);

    printf("Enter you position to check:");
    scanf("%d",&pos);

    if (num & 1 << pos){
        printf("True");
    }else{
        printf("false");
    }
}

void setithBit()  {
     
    printf("Enter you position to set:");
    scanf("%d",&pos);

    num = num | (1 << pos);
    printf("After setting pos number is :%d",num);
}

 void clearithBit(){ 
    printf("Enter your position to clear");
    scanf("%d",&pos);

    num = num  & ~(num & 1 << pos);

    printf("After clearing the pos number is :%d\n",num);
    
}

void toggleithBit() { 
    printf("Enter bit to toggle");
    scanf("%d",&pos);

    num = num ^ (1 << pos );

    printf("After the toggle bit :%d ",num);

} 


int main(){

    printf("Enter the Number:");
      scanf("%d",&num);

     int opt;
    printf("Select your operation:\n");
    printf("0. Check ith Bit\n");
    printf("1. Set ith Bit\n");
    printf("2. Clear ith Bit\n");
    printf("3. Toggle ith Bit\n");
    printf("Enter your choice: ");

    scanf("%d",&opt);
      switch(opt){
        case OP_CHECK:
            checkithBit();
            break;
        
        case OP_SET:
            setithBit();
            break;
        
        case OP_CLEAR:
            clearithBit();
            break;
        
        case OP_TOGGLE:
            toggleithBit();
            break;
      }

      
  
    return 0;
}