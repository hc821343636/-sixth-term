#include<bits/stdc++.h> 
using namespace std;
#define N 200

//整数乘幂运算函数
int Pow(int a, int b)
{
    int i = 0, result = 1;
    for(i = 0; i < b; ++i)
    {
        result *= a;
    }
    return result;
}

int n;
void add(){
	char stra[N], strb[N];      //字符串数组，以字符形式储存两个大数；
    int i = 0, step = 4, carry = 0; //step表示块长，carry为进位位；
    int lengtha, lengthb, maxlength, resultsize;    //maxlength表示stra和strb二者长度较大的那个；
    int numa[N], numb[N],numc[N];   //依次储存被加数，加数，和；
    memset(numa, 0, sizeof(numa));
    memset(numb, 0, sizeof(numb));
    memset(numc, 0, sizeof(numc));         //初始化为零；
    scanf("%s%s", stra, strb);
    lengtha = strlen(stra);
    lengthb = strlen(strb);     //计算两个大数的长度
    //字符数字转为四位一块的整数数字
    for(i = lengtha-1; i >= 0; --i)
    {
        numa[(lengtha-1-i)/step] += (stra[i]-'0')*Pow(10,(lengtha-1-i)%step);
    }
    for(i = lengthb-1; i >= 0; --i)
    {
        numb[(lengthb-1-i)/step] += (strb[i]-'0')*Pow(10,(lengthb-1-i)%step);
    }
    maxlength = lengtha > lengthb ? lengtha : lengthb;

    //逐块相加，并进位
    for(i = 0; i <= maxlength/step; ++i)
    {
        numc[i] = (numa[i] + numb[i])%Pow(10, step) + carry;    //计算和
        carry = (numa[i] + numb[i])/Pow(10, step);  //计算进位
    }

    //计算最后和的块的总数
    resultsize = numc[maxlength/step] > 0 ? maxlength/step : maxlength/step - 1;
    printf("%d", numc[resultsize]);
    for(i = resultsize-1; i >= 0; --i)
    {
        printf("%04d", numc[i]);    //右对齐，补零输出；
    }
    printf("\n");
}
void subtract(){
	char stra[N], strb[N];     //字符串数组，以字符形式储存两个大数；
    int i = 0, step = 4, borrow = 0, mark = 0; //step表示块长，borrow为借位位, mark为结果符号位；
    int lengtha, lengthb, maxlength, resultsize;    //maxlength表示stra和strb二者长度较大的那个；
    int numa[N], numb[N],numc[N],  *maxnum, *minnum;   //依次储存被减数，减数，和；
    memset(stra, 0, sizeof(stra));
    memset(strb, 0, sizeof(strb));
    memset(numa, 0, sizeof(numa));
    memset(numb, 0, sizeof(numb));
    memset(numc, 0, sizeof(numc));         //初始化为零；
    scanf("%s%s", stra, strb);
    lengtha = strlen(stra);
    lengthb = strlen(strb);     //计算两个大数的长度
    maxlength = lengtha >= lengthb ? lengtha : lengthb;

    //字符数字转为四位一块的整数数字
    for(i = lengtha-1; i >= 0; --i)
    {
        numa[(lengtha-1-i)/step] += (stra[i]-'0')*Pow(10,(lengtha-1-i)%step);
    }
    for(i = lengthb-1; i >= 0; --i)
    {
        numb[(lengthb-1-i)/step] += (strb[i]-'0')*Pow(10,(lengthb-1-i)%step);
    }

    //找出较大的数
    maxnum = numa;
    minnum = numb;
    mark = 1;
    for(i = (maxlength-1)/step; i >= 0; --i)
    {
        if(numa[i] > numb[i])
        {
            maxnum = numa;
            minnum = numb;
            mark = 1;
            break;
        }
        else if(numa[i] < numb[i])
        {
            maxnum = numb;
            minnum = numa;
            mark = -1;
            break;
        }
    }

    //逐块相减，并借位
    for(i = 0; i <= maxlength/step; ++i)
    {
        numc[i] = (maxnum[i] - minnum[i] + Pow(10, step) + borrow)%Pow(10,step);    //计算差
        borrow = (maxnum[i] - minnum[i] + Pow(10, step) + borrow)/Pow(10, step) - 1;  //计算借位
    }

    //计算最后和的块的总数
    resultsize = maxlength/step;
    while(!numc[resultsize])    --resultsize;
    printf("%d", mark*numc[resultsize]);
    for(i = resultsize-1; i >= 0; --i)
    {
        printf("%04d", numc[i]);    //右对齐，补零输出；
    }
    printf("\n");
	
}
int main(){
	printf("加法：1，减法：2\n");
	cin>>n;
	if(n==1) add();
	if(n==2) subtract();
return 0;
}

