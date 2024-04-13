/*
Largest Prime Factor
*/

package main

import "fmt"

func main() {
	n := 600851475143

	factor := 3
	last := factor
	for n > 1 {
		if n%factor == 0 {
			last = factor

			for n%factor == 0 {
				n /= factor
			}
		}

		factor += 2
	}

	fmt.Println(last)
}
