// Online C compiler to run C program online
#include <stdio.h>
#include <string.h>

int main() {
  
    char str[100];
    scanf("%s" , str);
    
    int j=0;
    while(str[j] == 'a')
    {
        j++;
    }
    if(str[j] == 'b' && str[j+1] == 'b' && str[j+2] == '\0')
    {
        printf("Valid String!");
    }
    else{
        printf("Invalid String!");
    }
    
    return 0;
}