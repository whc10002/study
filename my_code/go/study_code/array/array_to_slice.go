package main

import (
	"fmt"
	"reflect"
	"unsafe"
	"testing"
)

func main() {
	// case 1
	x := [...]int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
	s := x[2:5]

	for i := 0; i < len(s); i++ {
		println(s[i])
	}

	// case 2
	s1 := make([]int, 3, 5)         // initialize:len=3 and cap=5
	s2 := make([]int, 3)		// initialize:len=cap=3
	s3 := []int{10, 20, 5:30}

	fmt.Println(s1, len(s1), cap(s1))
	fmt.Println(s2, len(s2), cap(s2))
	fmt.Println(s3, len(s3), cap(s3))

	// case 3
	var a []int
	b := []int{}

	fmt.Println("a:%#v\n", (*reflect.SliceHeader)(unsafe.Pointer(&a)))
	fmt.Println("b:%#v\n", (*reflect.SliceHeader)(unsafe.Pointer(&b)))
	fmt.Println("a size:%d\n", unsafe.Sizeof(a))

	// case 4
	y := [][]int{
		{1, 2},
		{10, 20, 30},
		{100},
	}
	fmt.Println(y[1])

	y[2] = append(y[2], 200, 300)
	fmt.Println(y[2])

	// case 5
}

