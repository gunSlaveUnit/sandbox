package main

import (
	"flag"
	"os"
	"fmt"
	"encoding/csv"
)

type problem struct {
	q string
	a string
}

func parseLines(lines [][]string) []problem {
	ret := make([]problem, len(lines))
	for i, line := range lines {
		ret[i] = problem {
			q: line[0], 
			a: line[1],
		}
	}
	return ret
}

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
	problems := parseLines(lines)
	for i := 0; i < len(problems); i++ {
		fmt.Println(problems[i])
	}
}

func exit(message string) {
	fmt.Println(message)
	os.Exit(1)
}
