#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node{
    char data;
    int freq;
    struct node* left;
    struct node* right;
    char code[100];//must be init
    int used;
}node;

node* huff[100]; //建立huffman tree node的陣列
int huff_count = 0;//計算huff[]目前總共有幾個node
int merge_count = 0;
void freq_sort(node *p[], int count){
	node *tmp;
	for(int i=0 ; i<count-1 ; i++){
		for(int j=0 ; j<count-1 ; j++){
			if(p[j]->freq > p[j+1]->freq){
				tmp = p[j];
				p[j] = p[j+1];
				p[j+1] = tmp;
			}
		}
	}
}
void sf_ascii_sort(node *p[], int count){
	node *tmp;
	for(int i=0 ; i<count-1 ; i++){
		for(int j=0 ; j<count-1 ; j++){
			if((p[j]->data-'0' > p[j+1]->data-'0')&&(p[j]->freq==p[j+1]->freq)){
				tmp = p[j];
				p[j] = p[j+1];
				p[j+1] = tmp;
			}
		}
	}
}
void pure_ascii_sort(node *p[],int count){
    node *tmp;
	for(int i=0 ; i<count-1 ; i++){
		for(int j=0 ; j<count-1 ; j++){
			if((p[j]->data-'0' > p[j+1]->data-'0')&&p[j]->code!=""){
				tmp = p[j];
				p[j] = p[j+1];
				p[j+1] = tmp;
			}
		}
	}
}
void encode(node *head,char ch[]){
    char z[] = "0";
    char o[] = "1";
    char tmp[100] = "";
    if(head->left==NULL&&head->right==NULL){
        strcat(head->code,ch);
    }else{
        strcpy(tmp,ch);
        encode(head->left,strcat(tmp,z));
        strcpy(tmp,ch);
        encode(head->right,strcat(tmp,o));
    }
}
int main()
{
   char arr[1000];//input string
   gets(arr);
   int len = strlen(arr);
   int real_len = 0;
   int repeat = 0;//input string 的字元曾經出現在huff[]中 ,repeat = 1
   for(int i= 0;i < len;i++){
        if(arr[i]==' '||(arr[i]>='a'&&arr[i]<='z')||(arr[i]>='A'&&arr[i]<='Z')){
            //A~Z,a~z,以及空白' '皆滿足條件
            real_len++;
            if(i==0){ //先建立第一個node好讓之後迴圈可以判斷
                //printf("arr[%d] = %c is new char!!\n",i,arr[i]);
                huff[huff_count] = (node*)malloc(sizeof(node));
                huff[huff_count]->data = arr[i];
                huff[huff_count]->freq = 1;
                huff[huff_count]->left = NULL;
                huff[huff_count]->right = NULL;
                huff[huff_count]->used = 0;
                strcpy(huff[huff_count]->code,"");
                //printf("huff[%d] = %c \n",huff_count,huff[huff_count]->data);
                huff_count++;
                continue;//now i start from 1
            }
            for(int j = 0;j<huff_count;j++){
                if(huff[j]->data==arr[i]){
                   // printf("arr[%d] = %c is repeat in huff[%d]\n",i,arr[i],j);
                    huff[j]->freq++;
                    repeat =1;
                    break;
                }
            }
            if(repeat==0){
               // printf("arr[%d] = %c is new char!!\n",i,arr[i]);
                huff[huff_count] = (node*)malloc(sizeof(node));
                huff[huff_count]->data = arr[i];
                huff[huff_count]->freq = 1;
                huff[huff_count]->left = NULL;
                huff[huff_count]->right = NULL;
                huff[huff_count]->used = 0;
                strcpy(huff[huff_count]->code,"");
                //printf("huff[%d] = %c \n",huff_count,huff[huff_count]->data);
                huff_count++;
            }
            repeat = 0;
        }
   }
    freq_sort(huff,huff_count);//huff node ascii sort
    sf_ascii_sort(huff,huff_count);//same freq's char ascii sort
   /*------------------------------------------------------------------Create tree*/
   int total_freq = 0;  //Calculate the sum of freq
   for(int i =0;i < huff_count;i++){
       total_freq+=huff[i]->freq;
   }
  // printf("total_freq is %d\n",total_freq);
    merge_count = huff_count;
  // printf("merge_count = %d\n",merge_count);
   for(int i = 0;i < merge_count;i+=2){//create tree , merge the nodes
        if(huff[merge_count-1]->freq==total_freq)break;//warning: huff[merge_count] now is nullptr
        huff[merge_count] = (node*)malloc(sizeof(node));
        huff[merge_count]->data = huff[i]->data;
        huff[merge_count]->freq = 0;
        int ff = huff[i]->freq+huff[i+1]->freq;
        huff[merge_count]->freq = ff;
        huff[i]->used = 1;
        huff[i+1]->used = 1;
        huff[merge_count]->left = huff[i];
        huff[merge_count]->right = huff[i+1];
        huff[merge_count]->used = 0;
        strcpy(huff[merge_count]->code,"");;
       // printf("new node huff[%d] = %c ,and freq is %d\n",merge_count,huff[merge_count]->data,huff[merge_count]->freq);
        if(huff[merge_count]->freq<=total_freq){ //Not root
           // printf("not root\n");
            merge_count++;
        }
       // printf("after merge_count++ , now merge_count is %d\n",merge_count);
        freq_sort(huff,merge_count);
        sf_ascii_sort(huff,merge_count);
        for(int j = 0;j < merge_count;j++){
           // printf("%c and my frequency is %d\n",huff[j]->data,huff[j]->freq);
        }
   }
   char n[] = "";
   //strcat(huff[0]->code,n);
   //printf("huff[0]->code = %s\n",huff[0]->code);
   encode(huff[merge_count-1],n);
   /*for(int i = 0;i < merge_count-1;i++){
        printf("huff[%d]->code = %s\n",i,huff[i]->code);
   }*/
   /*for(int i = 0;i < merge_count-1;i++){
        if(huff[i]->left==NULL&&huff[i]->right==NULL){
            printf("huff[%d] is %c and huff[%d]->right&&left = NULLptr\n",i,huff[i]->data,i);
        }
   }*/
   //space sort
   pure_ascii_sort(huff,merge_count);
   printf("Huffman code:\n");
   for(int i = 0;i < merge_count;i++){
    if((huff[i]->data==' ')&&strlen(huff[i]->code)>1){
        printf("Space : %s\n",huff[i]->code);
    }else if(strlen(huff[i]->code)>1&&huff[i]->data!=' '){
        printf("%c : %s\n",huff[i]->data,huff[i]->code);
    }
   }
   int freq_code = 0;
   for(int i = 0;i < merge_count;i++){
        if((huff[i]->left==NULL&&huff[i]->right==NULL)&&(strlen(huff[i]->code)>=1)){
            //printf("%c in\n",huff[i]->data);
            int t = strlen(huff[i]->code);
            int k = 0;
            k= t*huff[i]->freq;
            freq_code+=k;
        }
   }
   printf("Minimum weighted external path length: %d\n",freq_code);
   printf("Compression ratio: %d/%d\n",(real_len)*8-freq_code,real_len*8);
   /*------------------------------------decode---------------------------*/
   //010011100001001110000100101110
   printf("Please input the code:");
   char input[100]="";
  // gets(input);	// 讀入空行
   gets(input);	// 讀入 input code
   //printf("%s\n" , input);
   int lem = strlen(input);
   char cmp[10000];
   for(int i=0 ; i < lem ; i++){
       // 以下這段取代 strcat，input[i] 是 char 而不是 char*
       // strcat 是字串黏接，兩個傳入的參數都必須是字串
       int c_len = strlen(cmp);
       cmp[c_len] = input[i];
       //char temp[1] = input[i]
       cmp[c_len+1] = '\0';
       //printf("round %2d is %s\n",(i+1),cmp);
       // 以上取代  strcat(cmp,input[i]);

       for(int j=0 ; j < merge_count-1 ; j++){
           if((!strcmp(huff[j]->code,cmp))&&(huff[j]->right==NULL&&huff[j]->left==NULL)){
               printf("%c",huff[j]->data);
               memset(cmp,0,sizeof(cmp));
           }
       }
   }
    return 0;
}
//https://ideone.com/bUGu5u?fbclid=IwAR3JZh4Y68BkgP50kpkHk2RAWnbqMpVLCVm3WmY-Lsu8AOKg1ZKXR5rJfwc
