/*
An integer is given as separate decimal digits.
Find the smallest nearest number.
*/

package main

import "fmt"

func swap(array []int, i1, i2 int) {
	array[i1] = array[i1]^array[i2]
	array[i2] = array[i1]^array[i2]
	array[i1] = array[i1]^array[i2]
}

func reverse(array []int, start, end int) {
	swapsAmount := (end-start + 1) / 2
	for i := 0; i < swapsAmount; i++ {
		swap(array, start + i, end - i)
	}
}

func getSwapIndexInNum(array []int) int{
	for i := len(array) - 1; i >= 1; i-- {
		if array[i-1] > array[i] {
			return i - 1
		}
	}
	return -1
}

func getSwapIndexInSubNum(array []int, swapValue int) int {
	for i := len(array) - 1; i >= 0; i-- {
		if array[i] < swapValue {
			return i
		}
	}
	return -1
}

func getClosestLowerNumber(array []int) []int {
	if array == nil {
		return nil
	}
	swapIndexInNum := getSwapIndexInNum(array)
	if swapIndexInNum == -1 {
		return nil
	}
	swapIndexInSubNum := getSwapIndexInSubNum(array, array[swapIndexInNum])
	swap(array, swapIndexInNum, swapIndexInSubNum)
	reverse(array, swapIndexInNum + 1, len(array) - 1)
	return array
}

func main() {
	number := []int{3, 1, 2, 0, 8, 4, 1, 2, 5, 5}
	fmt.Println(getClosestLowerNumber(number))
}
