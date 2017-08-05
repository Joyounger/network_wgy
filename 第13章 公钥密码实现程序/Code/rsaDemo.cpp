// rsaDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

typedef struct  RSA_PARAM_Tag
{
    unsigned __int64    p, q;   //������������������ܽ�������
    unsigned __int64    f;      //f=(p-1)*(q-1)����������ܽ�������
    unsigned __int64    n, e;   //���ף�n=p*q��gcd(e,f)=1
    unsigned __int64    d;      //˽�ף�e*d=1 (mod f)��gcd(n,d)=1
    unsigned __int64    s;      //�鳤������2^s<=n������s����log2(n)
} RSA_PARAM;
//С������
const static long       g_PrimeTable[]=
										{
											3,
											5,
											7,
											11,
											13,
											17,
											19,
											23,
											29,
											31,
											37,
											41,
											43,
											47,
											53,
											59,
											61,
											67,
											71,
											73,
											79,
											83,
											89,
											97
										};
const static long       g_PrimeCount=sizeof(g_PrimeTable) / sizeof(long);
const unsigned __int64  multiplier=12747293821;
const unsigned __int64  adder=1343545677842234541;

//�������
class RandNumber
{
private:
    unsigned __int64    randSeed;
public:
    RandNumber(unsigned __int64 s=0)
    {
		if(!s)
		{
			randSeed= (unsigned __int64)time(NULL);
		}
		else
		{
			randSeed=s;
		}
	}
	unsigned __int64    Random(unsigned __int64 n)
	{
		randSeed=multiplier * randSeed + adder;
		return randSeed % n;
	}
};


static RandNumber   cRadom;
/*
ģ�����㣬����ֵ x=a*b mod n
*/
inline unsigned __int64 MulMod(unsigned __int64 a, unsigned __int64 b, unsigned __int64 n)
{

    return (a%n) * (b%n) % n;
}
/*
ģ�����㣬����ֵ x=base^pow mod n
*/
unsigned __int64 PowMod(unsigned __int64 base, unsigned __int64 pow, unsigned __int64 n)
{
    unsigned __int64    a=base, b=pow, c=1;
    while(b)
    {
        while(!(b & 1))
        {
            b>>=1; 
			a=MulMod(a, a, n);
        }       
		b--;
        c=MulMod(a, c, n);
    }    
	return c;
}

long RabinMillerKnl(unsigned __int64 &n)
{
    unsigned __int64    a, q, k, v ;
    q=n - 1;
    k=0;  
    while(!(q & 1))
    {
        ++k;
        q>>=1;
    }    
    a=2 + cRadom.Random(n - 3);   
    v=PowMod(a, q, n);    
    if(v == 1)
    {
        return 1;
    }    
	for(int j=0;j<k;j++)
	{
		unsigned int z=1;
		for(int w=0;w<j;w++)
		{
			z*=2;
		}
		if(PowMod(a, z*q, n)==n-1)
			return 1;
	}
	return 0;
}

long RabinMiller(unsigned __int64 &n, long loop)
{
    //����С����ɸѡһ�Σ����Ч��
    for(long i=0; i < g_PrimeCount; i++)
    {
        if(n % g_PrimeTable[i] == 0)
        {
            return 0;
        }
    }    //ѭ������Rabin-Miller����loop�Σ�ʹ�÷�����ͨ�����Եĸ��ʽ�Ϊ(1/4)^loop
    for(long i=0; i < loop; i++)
    {
        if(!RabinMillerKnl(n))
        {
            return 0;
        }
    }   
	return 1;
}/*
�������һ��bitsλ(������λ)�����������32λ
*/
unsigned __int64 RandomPrime(char bits)
{
    unsigned __int64    base;
    do
    {
        base= (unsigned long)1 << (bits - 1);   //��֤���λ��1
        base+=cRadom.Random(base);               //�ټ���һ�������
        base|=1;    //��֤���λ��1������֤������
    } while(!RabinMiller(base, 30));    //�������������ղ���30��
    return base;    //ȫ��ͨ����Ϊ������
}

//�����Լ��
unsigned __int64 Gcd(unsigned __int64 &p, unsigned __int64 &q)
{
    unsigned __int64    a=p > q ? p : q;
    unsigned __int64    b=p < q ? p : q;
    unsigned __int64    t;
    if(p == q)
    {
        return p;   //������ȣ����Լ�����Ǳ���
    }
    else
    {
        while(b)    //շת�������gcd(a,b)=gcd(b,a-qb)
        {
            a=a % b;
            t=a;
            a=b;
            b=t;
        }       
		return a;
    }
}

unsigned __int64 Euclid(unsigned __int64 e, unsigned __int64 t_n)
{
	unsigned __int64 Max=0xffffffffffffffff-t_n;
	unsigned __int64 i=1;

	while(1)
	{
		if(((i*t_n)+1)%e==0)
		{
			return ((i*t_n)+1)/e;
		}
		i++;
		unsigned __int64 Tmp=(i+1)*t_n;
		if(Tmp>Max)
		{
			return 0;
		}
	}
	return 0; 
}

/*
�������һ��RSA���ܲ���
*/
RSA_PARAM RsaGetParam(void)
{
    RSA_PARAM           Rsa={ 0 };
    unsigned __int64    t;
    Rsa.p=RandomPrime(16);          //���������������
    Rsa.q=RandomPrime(16);
    Rsa.n=Rsa.p * Rsa.q;
    Rsa.f=(Rsa.p - 1) * (Rsa.q - 1);
    do
    {
        Rsa.e=cRadom.Random(65536);  //С��2^16��65536=2^16
        Rsa.e|=1;                   //��֤���λ��1������֤����������fһ����ż����Ҫ���أ�ֻ��������
    } while(Gcd(Rsa.e, Rsa.f) != 1);   
	Rsa.d=Euclid(Rsa.e, Rsa.f);
    Rsa.s=0;
    t=Rsa.n >> 1;
    while(t)
    {
        Rsa.s++;                   
        t>>=1;
    }    
	return Rsa;
}

int _tmain(int argc, _TCHAR* argv[])
{
    RSA_PARAM           r;
    char                pSrc[]="abcdefghijklmnopqrstuvwxyz";
    const unsigned long n=sizeof(pSrc);
    unsigned char       pDec[n];
    unsigned __int64    pEnc[n];
    r=RsaGetParam();
    cout << "p=" << r.p << endl;
    cout << "q=" << r.q << endl;
    cout << "f=(p-1)*(q-1)=" << r.f << endl;
    cout << "n=p*q=" << r.n << endl;
    cout << "e=" << r.e << endl;
    cout << "d=" << r.d << endl;
    cout << "s=" << r.s << endl;
    cout << "Source:\r\n" << pSrc << endl;
    cout << "Encode:\r\n";
    for(unsigned long i=0; i < n; i++)
    {
        pEnc[i]=PowMod(pSrc[i], r.e, r.n);
        cout << hex << pEnc[i] << " ";
    }   
	cout << endl;
    cout << "Decode:\r\n";
    for(unsigned long i=0; i < n; i++)
    {
        pDec[i]=PowMod(pEnc[i], r.d, r.n);
        cout << hex << (unsigned long)pDec[i] << " ";
    }
	cout << endl<<"Decryption:"<<endl;
    cout << (char *)pDec << endl;

	return 0;
}

