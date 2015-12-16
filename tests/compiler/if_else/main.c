int some_func()
{
	return 1;
}

int main()
{
	// int cnd = 1;
	// int counter = 0;
	// int test_number = 3;
	// int seq_prime = 5;
	// while ((seq_prime - counter))
	// {
	// 	// test if prime

	// 	int prime = 1;
	// 	int prob_prime = 1;
	// 	int test_divisor = 2;
	// 	while(prob_prime)
	// 	{
	// 		int division = test_number / test_divisor;
	// 		int remainder = test_number - division * test_divisor;
	// 		if (!remainder) // Divides
	// 		{
	// 			prime = 0;
	// 			prob_prime = 0;
	// 		}
	// 	}

	// 	if (prime)
	// 	{
	// 		counter = counter + 1;
	// 	}
		
	// 	test_number = test_number + 2;		
	// }

	// int k = 1;
	// if (k) { k = 8; }
	// int two = 10;
	// int div = 5 / 3;
	// int s = 0;
	// s = some_func();


	// int test_cond1 = 12;
	// int test_cond2 = 12;
	// if (test_cond2 - test_cond1)
	// {
	// 	counter = 23;
	// }

	//return s + k + 3 * 4 * 9 + (1+1) * 3;

	int test_number = 11;
	int divisor = 2;
	int prime = 1;
	while(test_number - divisor - 1)
	{
		int division = test_number / divisor;
		int remainder = test_number - division * divisor;

		if (!remainder)
		{
			prime = 0;
		}

		divisor = divisor + 1;
	}

	

	return 6000;
}