package main

import (
	"fmt"
)

type data struct {
	*int
	string
}

func main() {
	x := 100
	d := data {
		int : &x,
		string: "abc",
	}
	// %v: {0xc000084010 abc}
	// %#v: main.data{int:(*int)(0xc000084010), string:"abc"} 
	fmt.Printf("%#v\n", d)
}
