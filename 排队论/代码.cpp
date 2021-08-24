#include<bits/stdc++.h> 
using namespace std;
const int N =10;

double factorial(int n)//阶乘
{
	int i;
	int s=1;
	for(i=1;i<=n;i++)
		s*=i;
	return s*1.0;
}
double sum(double a[]){
	int n=sizeof(a) / sizeof(a[0]);
	double ans=0;
	for(int i=0;i<n;i++){
		ans+=a[i];
	}
	return ans;
}
double ffunction(double lambda,double mu)
{
	double rho = lambda / mu;
	double rho_slash = lambda / (N * mu); 
	double factor_mat[100]={0};
	double P[100]={0};//每个状态的概率 
	double length=0;
	int i;
	if (rho_slash>=1)
	{
		printf("The result is infinite.\n");
		return -1;
	}
	for( i = 1 ;i<=(N+1);i++)//p[i]的系数
		{
		factor_mat[i] = 1 / factorial(i-1) * (pow(rho,i-1));//p[i] 表示实际的p(i-1) 
		cout<<factor_mat[i]<<endl;
	}

	P[1] = 1 / (sum(factor_mat) + rho_slash / (1-rho_slash) *pow(rho , N)/ factorial(N));//P[0]
	printf("P[0] = %.5f\n", P[1]);

	for( i = 2 ;i<= (N+1) ;i+=1)//p[i]
		P[i] = factor_mat[i] * P[1];

	printf("P[N]= %.5f\n", P[N+1]);//输出p[N]

	for( i = 1 ;i<=(N+1);i++)//0——N 队长
		length+=(i-1)* P[i];

	i = 1;
	while (1)
	{
		double delta;
		double precision;//精度
		delta = (N + i) * pow(rho_slash, i) * P[N + 1];//P[N+1]即P[N]
		precision = P[N+1] * pow(rho_slash, i) * (N / (1-rho_slash) + (i * (1-rho_slash) + rho_slash) / pow((1-rho_slash) , 2));
		if (precision < 1e-6)//达到精度
		{
			printf("已达到精度：%.8f\n", precision);
			printf("此时队长：");
			break;
		}
		length = length + delta;
		printf("num %d: length = %.8f\n", i, length);//输出每一次循环判断精度的结果
		i=i+1;
	}
	return length;
}
int main(){
	cout<<ffunction(8,6);
return 0;
}

