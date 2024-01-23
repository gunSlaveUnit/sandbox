package main

import "fmt"

func factorialRecursive(number uint64) uint64 {
	if number == 0 {
		return 1
	}
	return number * factorialRecursive(number-1)
}

func factorialIteration(number uint64) uint64 {
	var result uint64 = 1
	for number > 1 {
		result *= number
		number--
	}
	return result
}

func main() {
	var numberToCalculateFactorial uint64
	fmt.Print("Please, enter a number to calculate factorial: ")
	_, _ = fmt.Scanf("%d", &numberToCalculateFactorial)

	fmt.Printf("Recursive Factorial of %d is %d\n", numberToCalculateFactorial,
		factorialRecursive(numberToCalculateFactorial))

	fmt.Printf("Iteration Factorial of %d is %d\n", numberToCalculateFactorial,
		factorialIteration(numberToCalculateFactorial))
}
