/*
32-bit non-negative integer.
Find the nearest smaller non-negative integer with the same number of bits
*/

package main

import "fmt"

func getTrailingBitsCount(number int) (int, int) {
	if number == 0 || number == -1 {
		return -1, -1
	}

	trailingZeroesCount := 0
	trailingOnesCount := 0

	for number & 1 == 1 {
		trailingOnesCount++
		number = number >> 1
	}

	if number == 0 {
		return 32 - trailingOnesCount, trailingOnesCount
	}

	for number & 1 == 0 {
		trailingZeroesCount++
		number = number >> 1
	}

	if trailingZeroesCount + trailingOnesCount == 31 {
		return -1, -1
	}

	return trailingZeroesCount, trailingOnesCount
}

func clearRightBits(number, amountClearRightBits int) int {
	return (-1 << amountClearRightBits) & number
}

func setBits(number, start, amountSetBits int) int {
	return (((1 << amountSetBits) - 1) << start) | number
}

func getClosestLowerNumber(number int) int {
	amountZeroes, amountOnes := getTrailingBitsCount(number)
	if amountZeroes == -1 && amountOnes == -1 {
		return -1
	}

	bitsToClear := amountZeroes + amountOnes + 1

	if amountZeroes + amountOnes == 33 {
		return number << amountZeroes
	}

	number = clearRightBits(number, bitsToClear)
	number = setBits(number, amountZeroes - 1, amountOnes + 1)

	return number
}

func main() {
	fmt.Print("Please, enter a non-negative number: ")
	var number int
	_, err := fmt.Scanf("%d", &number)
	if err != nil {
		fmt.Println(err)
	}

	fmt.Println(getClosestLowerNumber(number))
}