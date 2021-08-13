package main

import (
	"flag"
	"os"
	"fmt"
	"encoding/csv"
	"strings"
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
			a: strings.TrimSpace(line[1]),
		}
	}
	return ret
}

func main() {
	csvFilename := flag.String("csv", "problems.csv", "a сsv file in the format of 'question, answer'")
	timeLimit := flag.Int("limit", 30, "the time limit for the quiz in seconds")
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

	amountCorrectAnswers := 0
	problems := parseLines(lines)
	for i, p := range problems {
		fmt.Printf("Problem %d: %s = \n", i + 1, p.q)
		var answer string
		fmt.Scanf("%s", &answer)
		if answer == p.a {
			amountCorrectAnswers++
		}
	}
	fmt.Printf("You scored %d of %d\n", amountCorrectAnswers, len(problems))
}

func exit(message string) {
	fmt.Println(message)
	os.Exit(1)
}
