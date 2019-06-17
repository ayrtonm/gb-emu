package main

import (
  "fmt"
  "os"
  "io"
  "strings"
  "bufio"
  "strconv"
  "encoding/binary"
)

type section int

const (
  title section = iota
  start
  address
  label
  comment
  srcCode
  blank
)

const commentPrefix string = "//"
const labelSuffix string = ":"
const hexPrefix string = "0x"
const regPrefix string = "$"
const ptrPrefix string = "["
const ptrSuffix string = "]"
const numTitleChars int = 16
const titleAddress uint16 = 0x134
const startAddress uint16 = 0x0100

var labels map[string]uint16 = make(map[string]uint16, 0)
var pc uint16 = startAddress

func lowByte(x uint16) uint8 {
  return uint8(x & 0x00ff)
}

func hiByte(x uint16) uint8 {
  return uint8(x >> 8)
}

func check(e error) {
  if e != nil {
    panic(e)
  }
}
func updatePc(newPc uint16, file *os.File) {
  file.Seek(int64(newPc), 0)
  pc = newPc
}

func isPtr(line string) bool {
  return strings.HasPrefix(line, ptrPrefix) && strings.HasSuffix(line, ptrSuffix)
}

func isReg(line string) bool {
  return strings.HasPrefix(line, regPrefix)
}

func isHex(line string) bool {
  return strings.HasPrefix(line, hexPrefix)
}

func isComment(line string) bool {
  return strings.HasPrefix(line, commentPrefix)
}

func isLabel(line string) bool {
  return strings.HasSuffix(line, labelSuffix)
}

func isAddress(line string) bool {
  return isHex(line) && isLabel(line)
}

func getSectionType(line string) section {
  switch line {
    case "":
      return blank
    case "title"+labelSuffix:
      return title
    case "start"+labelSuffix:
      return start
    default:
      if isComment(line) {
        return comment
      } else if isAddress(line) {
        return address
      } else if isLabel(line) {
        return label
      } else {
        return srcCode
      }
  }
}

func parseHex(input string, numChars int) []byte {
  output := make([]byte, numChars)
  const charsPerByte = 2
  input = input[2:]
  for i := 0; i < numChars; i++ {
    charByte,_ := strconv.ParseInt(input[charsPerByte*i:(charsPerByte*i)+charsPerByte],16,64)
    output[i] = byte(charByte)
  }
  return output
}

func parseWord(input string) uint16 {
  return binary.BigEndian.Uint16(parseHex(input, 2))
}

func parseByte(input string) uint8 {
  return parseHex(input, 1)[0]
}

func readTitle(line string) []byte {
  var endIndex int = numTitleChars
  if len(line) < numTitleChars {
    endIndex = len(line)
  }
  titleBytes := make([]byte, endIndex)
  if isHex(line) {
    titleBytes = parseHex(line, endIndex)
  } else {
    for i := 0; i < endIndex; i++ {
      titleBytes[i] = line[i]
    }
  }
  return titleBytes
}

func getNextline(rd *bufio.Reader) (line string, more bool) {
  line, err := rd.ReadString('\n')
  if len(line) == 0 && err != nil {
    if err == io.EOF {
      return "", false 
    }
  } else if getSectionType(line) == comment {
    return getNextline(rd)
  }
  return strings.TrimSuffix(line, "\n"), true
}

func readCode(line string) (byteCode []byte, codeLength uint16) {
  output := make([]byte,0)
  cmd := strings.Fields(line)
  instruction := cmd[0]
  //handle instructions with no arguments here
  switch instruction {
    case "nop":
      output = append(output, 0x00)
    case "stop":
      output = append(output, 0x10)
    case "halt":
      output = append(output, 0x76)
    case "ei":
      output = append(output, 0xfb)
    case "di":
      output = append(output, 0xf3)
    case "ret":
      output = append(output, 0xc9)
    case "retz":
      output = append(output, 0xc8)
    //if instruction not found, read one argument then switch case with one-argument instructions
    default:
      data := cmd[1]
      switch instruction {
        case "jp":
          newAddress := parseWord(data)
          output = append(output, 0xc3)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "jpz":
          newAddress := parseWord(data)
          output = append(output, 0xca)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "jpnz":
          newAddress := parseWord(data)
          output = append(output, 0xc2)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "inc":
        case "dec":
        //instruction not found, read second argument the switch case with two-argument instructions
        default:
          switch instruction {
            case "ld":
            default:
              output = append(output , 0xff, 0xfe)
          }
      }
  }
  return output, uint16(len(output))
}

func writeCode(file *os.File, data []byte) {
  _, err := file.Write(data)
  check(err)
  //fmt.Println(bytesWritten)
}

func readSection(outfile *os.File, rd *bufio.Reader) bool {
  for line, more := getNextline(rd); getSectionType(line) == srcCode; line, more = getNextline(rd) {
    byteCode, codeLength := readCode(line)
    writeCode(outfile, byteCode)
    updatePc(pc + codeLength, outfile)
    if !more {
      return false
    }
  }
  return true
}

func main() {
  if (len(os.Args)) < 2 {
    fmt.Println("not enough arguments")
    os.Exit(1)
  }

  infile, err := os.Open(os.Args[1])
  check(err)
  outfile, err := os.Create(os.Args[2])
  check(err)
  defer infile.Close()
  defer outfile.Close()

  rd := bufio.NewReader(infile)
  for {
    line, more := getNextline(rd)
    switch getSectionType(line) {
      case title:
        line, more = getNextline(rd)
        outfile.Seek(int64(titleAddress), 0)
        writeCode(outfile, readTitle(line))
      case start:
        updatePc(startAddress, outfile)
        readSection(outfile, rd)
      case address:
        updatePc(parseWord(line), outfile)
        readSection(outfile, rd)
      case label:
        labels[strings.TrimSuffix(line, labelSuffix)] = pc
        readSection(outfile, rd)
    }
    if !more {
      break
    }
  }
  outfile.Sync()
}
