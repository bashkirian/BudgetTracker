#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
using namespace std;

class Date
{
	int year, month, day;

	vector<int> monthDays
		= { 31, 28, 31, 30, 31, 30,
		   31, 31, 30, 31, 30, 31 };

	int countLeapYears() const
	{
		int years = this->year;

		if (this->month <= 2)
			years--;

		return years / 4
			- years / 100
			+ years / 400;
	}

	int countDays() const
	{
		int n = this->year * 365 + this->day;

		for (int i = 0; i < this->month - 1; i++)
			n += monthDays[i];

		n += this->countLeapYears();
		
		return n;
	}
public:
	explicit Date(int year_, int month_, int day_) :
		year(year_), month(month_), day(day_)
	{};

	void SetDate(int year_, int month_, int day_)
	{
		year = year_;
		month = month_;
		day = day_;
	}
	Date() = default;

	int getDifference(Date dt2) const
	{
		int n1 = this->countDays();
		int n2 = dt2.countDays();
		return (n2 - n1);
	}
};

istream& operator>> (istream& is, Date& date)
{
	int year, month, day;
	is >> year;
	is.ignore(1);
	is >> month;
	is.ignore(1);
	is >> day;
	date.SetDate(year, month, day);
	return is;
}

class Budget
{
	vector<double> dates_incomes = { 0.0 };
	vector<double> dates_spendings = { 0.0 };
	Date default_date = Date(2000, 1, 1);
public:
	double ComputeIncome(const Date date_from, const Date date_to) const
	{
		int days1 = default_date.getDifference(date_from);
		int days2 = default_date.getDifference(date_to);
	    int end_incomes = min(days2, (int)(dates_incomes.size() - 1));
		int end_spendings = min(days2, (int)(dates_spendings.size() - 1));
		double overall_spendings = 0.0;
		if (days1 <= (dates_spendings.size() - 1))
		{
			overall_spendings = accumulate(dates_spendings.begin() + days1, dates_spendings.begin() + end_spendings + 1, overall_spendings);
		}
		double overall_incomes = 0.0;
		if (days1 <= (dates_incomes.size() - 1))
		{
			overall_incomes = accumulate(dates_incomes.begin() + days1, dates_incomes.begin() + end_incomes + 1, overall_incomes);
		}
		return overall_incomes - overall_spendings;
	}

	void Earn(const Date date_from, const Date date_to, int income)
	{
		long int date1 = default_date.getDifference(date_from);
		long int date2 = default_date.getDifference(date_to);
		if (date2 > (dates_incomes.size() - 1)) dates_incomes.resize(date2 + 1);
		double income_by_day = income / ((double)(date2 - date1 + 1));
		for (int i = date1; i <= date2; ++i)
		{
			dates_incomes[i] += income_by_day;
		}
	}

	void PayTax(const Date date_from, const Date date_to, double percentage)
	{
		int date1 = default_date.getDifference(date_from);
		int date2 = default_date.getDifference(date_to);
		for (int i = date1; (i <= date2 && i < dates_incomes.size()); ++i)
		{
			dates_incomes[i] *= (1 - 0.01 * percentage);
		}
	}

	void Spend(const Date date_from, const Date date_to, double spendings)
	{
		long int date1 = default_date.getDifference(date_from);
		long int date2 = default_date.getDifference(date_to);
		if (date2 > (dates_spendings.size() - 1)) dates_spendings.resize(date2 + 1);
		double spendings_by_day = spendings / ((double)(date2 - date1 + 1));
		for (int i = date1; i <= date2; ++i)
		{
			dates_spendings[i] += spendings_by_day;
		}
	}
};
int main()
{
	int n;
	cin >> n;
	string quiery;
	Budget budget;
	Date date_from, date_to;
	for (int i = 0; i < n; ++i)
	{
		cin >> quiery;
		cin >> date_from;
		cin >> date_to;
		if (quiery == "Earn")
		{
			int income;
			cin >> income;
			budget.Earn(date_from, date_to, income);
		}
		if (quiery == "ComputeIncome")
		{
			cout << setprecision(25) << fixed << budget.ComputeIncome(date_from, date_to) << endl;
		}
		if (quiery == "PayTax")
		{
			double percentage;
			cin >> percentage;
			budget.PayTax(date_from, date_to, percentage);
		}
		if (quiery == "Spend")
		{
			double to_spend;
			cin >> to_spend;
			budget.Spend(date_from, date_to, to_spend);
		}
	}
	return 0;
}