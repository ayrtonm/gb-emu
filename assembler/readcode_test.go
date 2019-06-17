package main

import (
  "testing"
  "fmt"
)

func TestReadCode(t *testing.T) {
  var testPush = []string {"push $bc", "push $de", "push $hl", "push $af"}
  var testPop = []string {"pop $bc", "pop $de", "pop $hl", "pop $af"}
  var testInc = []string {"inc $bc", "inc $de", "inc $hl", "inc $af"}
  var testDec = []string {"dec $bc", "dec $de", "dec $hl", "dec $af"}
  var testMap = map[string][]string{"push":testPush, "pop":testPop, "inc":testInc, "dec":testDec}
  var allTests = []string{"push", "pop", "inc", "dec"}

  for _,testName := range allTests {
    for _,i := range testMap[testName] {
      fmt.Println("testing", i)
      bytes := readCode(i)
      if testName == "push" {
        if bytes[0] & 0x0F != 0x05 {
          t.Errorf(i+" wrote %x want x5",bytes)
        }
    } else if testName == "pop" {
        if bytes[0] & 0x0F != 0x01 {
          t.Errorf(i+" wrote %x want x5",bytes)
        }
    } else if testName == "inc" {
        if bytes[0] & 0x0F != 0x03 {
          t.Errorf(i+" wrote %x want x5",bytes)
        }
    } else if testName == "dec" {
        if bytes[0] & 0x0F != 0x0b {
          t.Errorf(i+" wrote %x want x5",bytes)
        }
    }
    }
  }
}
