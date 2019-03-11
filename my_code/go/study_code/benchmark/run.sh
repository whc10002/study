#!/bin/bash
go test -test.bench="."
go test -test.bench=".*"
