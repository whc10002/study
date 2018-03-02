#include <string>
#include <iostream>

using namespace std;

class base
{
	public:
		base(int bv):bval(bv){};
		virtual void base_f(){cout<<"base::f()" << endl;}
		virtual void base_g(){cout<<"base::g()" << endl;}
	private:
		int bval;
};

class base_1
{
	public:
		base_1(int bv_1):bval_1(bv_1){};
		virtual void base_1_f(){cout<<"base_1::f()" << endl;}
		virtual void base_1_g(){cout<<"base_1::g()" << endl;}
	private:
		int bval_1;
};

class derived: public base
{
	public:
		derived(int bv1, int dv):base(bv1), dval(dv){};
		virtual void derived_h(){cout<<"derived::h()"<<endl;}
	private:
		int dval;
};

class derived_multi: public base, public base_1
{
	public:
		derived_multi(int bv1, int bv2):base(bv1), base_1(bv2){};
		virtual void derived_multi_h(){cout << "derived_multi::h()" << endl;}
};

typedef void (*FUN)();

int main(int argc,char** argv)
{
	FUN fun;

	//base:vptr+bval
	//	vptr->[0]->base::base_f()
	//	vptr->[1]->base::base_g()
	base b(100);
	FUN **pvtab = (FUN**)&b;
	cout<<"[0]:base->vptr:" << pvtab <<endl;

	for(int i=0; i<2; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}

	int * pval = (int*)(pvtab + 1);
	cout<<"[1]:bval "<<pval[0]<<endl;
	
	//derived:vptr+bval+dval
	//	vptr->[0]->base::base_f()
	//	vptr->[1]->base::base_g()
	//	vptr->[2]->derived::derived_h()
	derived d(10, 100);
	pvtab = (FUN**)&d;
	cout<<"[0]:derived->vptr "<<endl;
	for(int i=0; i<3; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}

	cout<<"[1]:bval "<<pval[0]<<endl;
	cout<<"[2]:dval "<<pval[1]<<endl;

	//derived_multi:vptr1+bval+vptr2+bval_1
	//	base:vptr1+bval
	//		vptr1->[0]->base::base_f()
	//		vptr1->[1]->base::base_g()
	//		vptr1->[2]->derived_multi::derived_multi_h()
	//		bval
	//	base_1:vptr2+bval_1
	//		vptr2->[0]->base_1::base_1_f()
	//		vptr2->[1]->base_1::base_1_g()
	//		bval_1
	derived_multi d_m(234,456);
	pvtab = (FUN**)&d_m;
	cout<<"[0]:derived_multi->vptr1(base->vptr) " <<endl;
	for (int i=0; i<3; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}
	pval = (int*)(pvtab+1);
	cout<<"[1]:base::bval "<<pval[0]<<endl;

	pvtab = pvtab + 2;
	cout<<"[2]:derived_multi->vptr2(base_1->vptr) " <<endl;
	for (int i=0; i<2; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}
	pval = (int*)(pvtab+1);
	cout<<"[3]:base_1::bval_1 "<<pval[0]<<endl;

	return 0;
}
