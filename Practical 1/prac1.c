#include<stdio.h>
#include<string.h>
int main(){

    char str[100];
    printf("Enter string: ");
    scanf("%s",str);
    int i=0;
    for(i=0;i<strlen(str)-2;i++){
        if(str[i]!='a'){
            printf("Invalid string\n");
            return 0;
        }
        
    }

    if(str[i]=='b' && str[i+1]=='b'){
        printf("Valid string\n");
    }
    else{
        printf("Invalid string\n");
    }
    return 0;  

}