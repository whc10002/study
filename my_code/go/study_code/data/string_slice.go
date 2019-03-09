package main

import "reflect"
import "fmt"
import "unsafe"

func main() {
	s := "abcdef"
	s1 := s[:3]
	s2 := s[1:4]
	s3 := s[2:]

	println(s1, s2, s3)

	fmt.Printf("%#V\n", (*reflect.StringHeader)(unsafe.Pointer(&s)))
	fmt.Printf("%#V\n", (*reflect.StringHeader)(unsafe.Pointer(&s1)))
}
