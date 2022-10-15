#include <iostream>
#include <sstream>
#include <vector>

struct Params
{
	int m;
	int a;
	int c;
};

int convert_to_int(char argv[])
{
	std::istringstream string_stream(argv);
	int number;
	if (!(string_stream >> number))
	{
		std::cerr << "Invalid number: " << argv << std::endl;
		throw std::exception();
	}
	return number;
}

int modular_inverse(int a, int m)
{
	int t, nt, r, nr, q, tmp;
	t = 0;
	nt = 1;
	r = m;
	nr = a % m;
	while (nr != 0)
	{
		q = r / nr;
		tmp = nt;
		nt = t - q * nt;
		t = tmp;
		tmp = nr;
		nr = r - q * nr;
		r = tmp;
	}
	if (r > 1) return -1;
	if (t < 0) t += m;
	return t;
}

std::vector <Params>
calculate_params(const int number_1, const int number_2, const int number_3, const int number_4)
{
	std::vector <Params> params;
	// Считаем стартовое значение m
	const int min_m = std::max(std::max(number_1, number_2), std::max(number_3, number_4)) + 1;
	const int max_m = 65535;
	for (int m = min_m; m < max_m; ++m)
	{
		// Считаем разницы между числами
		int diff_1 = (number_2 - number_1);
		if (diff_1 < 0)
		{
			diff_1 += m;
		}
		int diff_2 = (number_3 - number_2);
		if (diff_2 < 0)
		{
			diff_2 += m;
		}
		int diff_3 = (number_4 - number_3);
		if (diff_3 < 0)
		{
			diff_3 += m;
		}

		// Находим мультипликативное обратное значение
		// diff_2 = a * diff_1  ->  a = 1/diff_1 * diff_2
		int inv_diff_1 = modular_inverse(diff_1, m);
		if (inv_diff_1 < 0) continue;
		int a = (diff_2 * inv_diff_1) % m;

		int possible_diff_2 = (a * diff_1) % m;
		int possible_diff_3 = (a * diff_2) % m;

		if (possible_diff_2 == diff_2 && possible_diff_3 == diff_3)
		{
			Params param = Params();
			param.m = m;
			param.a = a;
			params.emplace_back(param);
		}
	}

	for (auto& param: params)
	{
		for (int c = 0; c < param.m; c++)
		{
			int possible_number_2 = (param.a * number_1 + c) % param.m;
			int possible_number_3 = (param.a * number_2 + c) % param.m;
			int possible_number_4 = (param.a * number_3 + c) % param.m;
			if (number_2 == possible_number_2 && number_3 == possible_number_3 &&
				number_4 == possible_number_4)
			{
				param.c = c;
			}
		}
	}

	return params;
}

int main(int argc, char* argv[])
{
	// 4 числа принимаются в качестве аргументов командной строки
	if (argc != 5)
	{
		std::cerr << "Incorrect arguments.\n"
					 "Correct usage:\n"
					 "<program_executable> NUMBER1 NUMBER2 NUMBER3 NUMBER4\n"
					 "\n" << std::endl;
		return 1;
	}
	// Конвертация чисел в int
	int number_1;
	int number_2;
	int number_3;
	int number_4;
	try
	{
		number_1 = convert_to_int(argv[1]);
		number_2 = convert_to_int(argv[2]);
		number_3 = convert_to_int(argv[3]);
		number_4 = convert_to_int(argv[4]);
	}
	catch(...) {
		return 1;
	}

//	for (int m = min_m; m <= max_m; m++)
//	{
//		for (int a = 0; a < m; ++a)
//		{
//			int c = number_2 - ((number_1 * a) % m);
//			if (c < 0)
//			{
//				c += m;
//			}
//			int my_x2 = (number_1 * a + c) % m;
//			int my_x3 = (number_2 * a + c) % m;
//			int my_x4 = (number_3 * a + c) % m;
//			int my_x5 = (number_4 * a + c) % m;
//			if (my_x2 == number_2 && my_x3 == number_3 && my_x4 == number_4)
//			{
//				std::cout << "m=" << m << "\ta=" << a << "\tc=" << c << "\tnext number: " << my_x5 << std::endl;
//			}
//		}
//	}

	auto params = calculate_params(number_1, number_2, number_3, number_4);

	std::cout << "All possible m, a, c combinations and projected next number:" << std::endl;
	for (auto& param: params)
	{
		std::cout << "m=" << param.m << "\ta=" << param.a << "\tc=" << param.c << "\t\tnext number: "
				  << (param.a * number_4 + param.c) % param.m << std::endl;
	}

	return 0;
}