package main

import (
	"flag"
	"os"
	"fmt"
)

func main() {
	csvFilename := flag.String("csv", "problems.csv", "a сsv file in the format of 'question, answer'")
	flag.Parse()
	file, err := os.Open(*csvFilename)
	if err != nil {
		exit(fmt.Sprintf("Failed to open %s", *csvFilename))
	}
	_ = file
}

func exit(message string) {
	fmt.Println(message)
	os.Exit(1)
}
