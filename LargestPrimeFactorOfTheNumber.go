package main

import (
	"fmt"
	"math"
)

func isPrime(n int64) bool {
	sqrtN := int64(math.Sqrt(float64(n)))
	prime := true
	var j int64
	for j = 2; j <= sqrtN; j++ {
		if n % j == 0 {
			prime = false
		}
	}
	return prime
}

func main() {
	var number int64 = 600851475143
	var i int64
	for i = 2; i <= number / 2; i++ {
		if number % i == 0{
			n := number / i
			if isPrime(n) {
				fmt.Println("Largest prime factor is", n)
				break
			}
		}
	}
}