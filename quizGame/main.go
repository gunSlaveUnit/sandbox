package main

import (
	"flag"
	"os"
	"fmt"
	"encoding/csv"
)

func main() {
	csvFilename := flag.String("csv", "problems.csv", "a сsv file in the format of 'question, answer'")
	flag.Parse()
	file, err := os.Open(*csvFilename)
	if err != nil {
		exit(fmt.Sprintf("Failed to open %s", *csvFilename))
	}
	reader := csv.NewReader(file)
	lines, err := reader.ReadAll()
	if err != nil {
		exit("Failed to parse the provided CSV file")
	}
	fmt.Println(lines)
}

func exit(message string) {
	fmt.Println(message)
	os.Exit(1)
}
