#include <vector>
#include <string>
#include <map>
#include <iostream>

int g_old_reduce = 3500;
typedef struct Tax_Rate
{
	int begin;
	int end;
	float ratio;
	int quick_reduce;
}Tax_Rate;

Tax_Rate g_old_tax_rate[] = 
{
	{0, 1500, 0.03 , 0},
	{1500, 4500, 0.10, 105},
	{4500, 9000, 0.20, 555},
	{9000, 35000, 0.25, 1005},
	{35000, 55000, 0.30, 2755},
	{55000, 80000, 0.35, 5505},
	{80000, INT_MAX, 0.45, 13505}	
};

int g_new_reduce = 5000;
Tax_Rate g_new_tax_rate[] = 
{
	{0, 3000, 0.03, 0},
	{3000, 12000, 0.10, 210},
	{12000, 25000, 0.20, 1410},
	{25000, 35000, 0.25, 2660},
	{35000, 55000, 0.30, 4410},
	{55000, 80000, 0.35, 7160},
	{80000, INT_MAX, 0.45, 15160}	
};

using namespace std;

int calculate_tax_step(int income, const Tax_Rate* tax_rate, int count, int reduce)
{
	int tax = 0;
	if (income <= reduce)
		return tax;
	int current = income - reduce;
	for (int i = 0; i < count; i++)
	{
		if (current <= tax_rate[i].end)
		{
			tax += current * tax_rate[i].ratio;
			break;
		}
		else
		{
			tax += tax_rate[i].ratio * tax_rate[i].end;
			current -= tax_rate[i].end;
		}

	}
	return tax;
}

int calculate_tax_quick(int income, const Tax_Rate* tax_rate, int count, int reduce)
{
	int tax = 0;
	if (income <= reduce)
		return tax;
	int current = income - reduce;
	for (int i = 0; i < count; i++)
	{
		if (current <= tax_rate[i].end)
		{
			tax = current * tax_rate[i].ratio - tax_rate[i].quick_reduce;
			break;
		}

	}
	return tax;
}

int usage(int argc, char**argv)
{
	if (argc < 3)
	{
		std::cout << "tax_cap {before_tax_income} {cal_method} {tax ratio}(1:ratio step; 2:quick calculation)" << std::endl;
		std::cout << "cal_method: 1:ratio step; 2:quick calculation" << std::endl;
		std::cout << "tax ratio: 1: old tax ratio; 2:new tax ratio" << std::endl;
		return -1;
	}
	else
		return 0;
}

int main(int argc, char**argv)
{
	int tax = 0;
	if (usage(argc, argv) < 0)
		return -1;
	else
	{
		int input = std::atoi(argv[1]);
		int method = std::atoi(argv[2]);
		int ratio = std::atoi(argv[3]);
		switch(method)
		{
			case 1:
				switch(ratio)
				{
					case 1:
						tax = calculate_tax_step(input, g_old_tax_rate, sizeof(g_old_tax_rate)/sizeof(Tax_Rate), g_old_reduce);
						break;
					case 2:
						tax = calculate_tax_step(input, g_new_tax_rate, sizeof(g_new_tax_rate)/sizeof(Tax_Rate), g_new_reduce);
						break;
					default:
						std::cout << "ratio should be in (1, 2)" << std::endl;
				}
				break;
			case 2:
				switch(ratio)
				{
					case 1:
						tax = calculate_tax_quick(input, g_old_tax_rate, sizeof(g_old_tax_rate)/sizeof(Tax_Rate), g_old_reduce);
						break;
					case 2:
						tax = calculate_tax_quick(input, g_new_tax_rate, sizeof(g_new_tax_rate)/sizeof(Tax_Rate), g_new_reduce);
						break;
					default:
						std::cout << "ratio should be in (1, 2)" << std::endl;
				}
				break;
			default:
				std::cout << "method should be in (1, 2)" << std::endl;
		}
		std::cout << "Your taxable income:" << input << std::endl;
		std::cout << "Your tax shoule be:" << tax << std::endl;
		std::cout << "Your Post tax revenue" << input - tax << std::endl;
	}
	return 0;
}
