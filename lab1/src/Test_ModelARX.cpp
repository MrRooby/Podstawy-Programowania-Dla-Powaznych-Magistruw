#include "Test_ModelARX.h"


void Test_ModelARX::performTests()
{
	test_ModelArx_noExcitation();
	test_ModelArx_unitStep_v1();
	test_ModelArx_unitStep_v2();
	test_ModelArx_unitStep_v3();
}

void Test_ModelARX::reportErrorInSequence(std::vector<double>& expected, std::vector<double>& actual)
{
	constexpr size_t PREC = 3;
	std::cerr << std::fixed << std::setprecision(PREC);
	std::cerr << "  Expected:\t";
	for (auto& el : expected)
		std::cerr << el << ", ";
	std::cerr << "\n  Actual:\t";
	for (auto& el : actual)
		std::cerr << el << ", ";
	std::cerr << std::endl << std::endl;
}

bool Test_ModelARX::compareSequences(std::vector<double>& expected, std::vector<double>& actual)
{
	constexpr double TOL = 1e-3;	// tolerance for comparison of floating point values
	bool result = actual.size() == expected.size();
	for (int i = 0; result && i < actual.size(); i++)
		result = std::fabs(actual[i] - expected[i]) < TOL;
	return result;
}

void Test_ModelARX::test_ModelArx_noExcitation()
{
	// Signature of the test:
	std::cerr << "ModelArx (-0.4 | 0.6 | 1 | 0 ) -> zero excitation test: ";
	try
	{
		// Data preparation:
		ModelArx testInstance({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t ITER_COUNT = 30;
		std::vector<double> signalIn(ITER_COUNT);          // model excitation (here: all zeroes)
		std::vector<double> expectedSignalOut(ITER_COUNT); // expected output sequence (here: all zeroes)
		std::vector<double> actualSignalOut(ITER_COUNT);   // actual output sequence

		// Model simulation:
		for (int i = 0; i < ITER_COUNT; i++)
			actualSignalOut[i] = testInstance.simulate(signalIn[i]);

		// Validation of correctness, reporting of test result:
		if (compareSequences(expectedSignalOut, actualSignalOut))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			reportErrorInSequence(expectedSignalOut, actualSignalOut);
		}

    testInstance.serialize("ModelArx_test1_output.json");
	}
	catch (...)
	{
		std::cerr << "INTERRUPTED! (unexpected exception)\n";
	}
}

void Test_ModelARX::test_ModelArx_unitStep_v1()
{
	// Signature of the test:
	std::cerr << "ModelArx (-0.4 | 0.6 | 1 | 0 ) -> step response test, version 1: ";

	try
	{
		// Data preparation:
		ModelArx testInstance({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t ITER_COUNT = 30;
		std::vector<double> signalIn(ITER_COUNT);          // model excitation
		std::vector<double> expectedSignalOut(ITER_COUNT); // expected output sequence
		std::vector<double> actualSignalOut(ITER_COUNT);   // actual output sequence

		// Simulation of a unit step signal at time 1 (!!i is a double negation of i and it gives 1 for i != 0):
		for (int i = 0; i < ITER_COUNT; i++)
			signalIn[i] = !!i;

		expectedSignalOut = { 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Model simulation:
		for (int i = 0; i < ITER_COUNT; i++)
			actualSignalOut[i] = testInstance.simulate(signalIn[i]);

		// Validation of correctness, reporting of test result:
		if (compareSequences(expectedSignalOut, actualSignalOut))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			reportErrorInSequence(expectedSignalOut, actualSignalOut);
		}
	}
	catch (...)
	{
		std::cerr << "INTERRUPTED! (unexpected exception)\n";
	}
}

void Test_ModelARX::test_ModelArx_unitStep_v2()
{
	// Signature of the test:
	std::cerr << "ModelArx (-0.4 | 0.6 | 2 | 0 ) -> step response test, version 2: ";

	try
	{
		// Data preparation:
		ModelArx testInstance({ -0.4 }, { 0.6 }, 2, 0);
		constexpr size_t ITER_COUNT = 30;
		std::vector<double> signalIn(ITER_COUNT);          // model excitation
		std::vector<double> expectedSignalOut(ITER_COUNT); // expected output sequence
		std::vector<double> actualSignalOut(ITER_COUNT);   // actual output sequence

		// Simulation of a unit step signal at time 1 (!!i is a double negation of i and it gives 1 for i != 0):
		for (int i = 0; i < ITER_COUNT; i++)
			signalIn[i] = !!i;

		expectedSignalOut = { 0, 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Model simulation:
		for (int i = 0; i < ITER_COUNT; i++)
			actualSignalOut[i] = testInstance.simulate(signalIn[i]);

		// Validation of correctness, reporting of test result:
		if (compareSequences(expectedSignalOut, actualSignalOut))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			reportErrorInSequence(expectedSignalOut, actualSignalOut);
		}
	}
	catch (...)
	{
		std::cerr << "INTERRUPTED! (unexpected exception)\n";
	}
}

void Test_ModelARX::test_ModelArx_unitStep_v3()
{
	// Signature of the test:
	std::cerr << "ModelArx (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> step response test, version 3: ";
	try
	{
		// Data preparation:
		ModelArx testInstance({ -0.4, 0.2 }, { 0.6, 0.3 }, 2, 0);
		constexpr size_t ITER_COUNT = 30;
		std::vector<double> signalIn(ITER_COUNT);          // model excitation
		std::vector<double> expectedSignalOut(ITER_COUNT); // expected output sequence
		std::vector<double> actualSignalOut(ITER_COUNT);   // actual output sequence

		// Simulation of a unit step signal at time 1 (!!i is a double negation of i and it gives 1 for i != 0):
		for (int i = 0; i < ITER_COUNT; i++)
			signalIn[i] = !!i;

		expectedSignalOut = { 0, 0, 0, 0.6, 1.14, 1.236, 1.1664, 1.11936, 1.11446, 1.12191, 1.12587, 1.12597, 1.12521, 1.12489, 1.12491, 1.12499, 1.12501, 1.12501, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125 };

		// Model simulation:
		for (int i = 0; i < ITER_COUNT; i++)
			actualSignalOut[i] = testInstance.simulate(signalIn[i]);

		// Validation of correctness, reporting of test result:
		if (compareSequences(expectedSignalOut, actualSignalOut))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			reportErrorInSequence(expectedSignalOut, actualSignalOut);
		}
	}
	catch (...)
	{
		std::cerr << "INTERRUPTED! (unexpected exception)\n";
	}
}
