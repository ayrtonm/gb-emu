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

//offset pattern used in inc/dec for 16 bit registers
var regOffsets1 = map[string]byte{"bc":0, "de":1, "hl":2, "sp":3}
//offset pattern used in inc/dec for 8 bit registers
//also used for ld between two 8 bit registers
var regOffsets2 = map[string]byte{"b":0, "c":1, "d":2, "e":3, "h":4, "l":5, "a":7}
//offset pattern used in push/pop
var regOffsets3 = map[string]byte{"bc":0, "de":1, "hl":2, "af":3}

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

func isValidRst(rstVector uint8) bool {
  return (rstVector & 0xc7) == 0
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
  if !isHex(input) {
    os.Exit(2)
  }
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

func readCode(line string) (byteCode []byte) {
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
      if len(cmd) < 1 {
        //instructions missing arguments
        os.Exit(5)
      }
      dest := cmd[1]
      switch instruction {
        case "jp":
          newAddress := parseWord(dest)
          output = append(output, 0xc3)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "jpz":
          newAddress := parseWord(dest)
          output = append(output, 0xca)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "jpnz":
          newAddress := parseWord(dest)
          output = append(output, 0xc2)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "jpc":
          newAddress := parseWord(dest)
          output = append(output, 0xda)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "jpnc":
          newAddress := parseWord(dest)
          output = append(output, 0xd2)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "call":
          newAddress := parseWord(dest)
          output = append(output, 0xcd)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "callz":
          newAddress := parseWord(dest)
          output = append(output, 0xcc)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "callnz":
          newAddress := parseWord(dest)
          output = append(output, 0xc4)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "callc":
          newAddress := parseWord(dest)
          output = append(output, 0xdc)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "callnc":
          newAddress := parseWord(dest)
          output = append(output, 0xd4)
          output = append(output, lowByte(newAddress))
          output = append(output, hiByte(newAddress))
        case "rst":
          newAddress := parseByte(dest)
          if !isValidRst(newAddress) {
            //reset vector is not valid
            os.Exit(6)
          }
          output = append(output, 0xc7 + newAddress)
        case "push":
          if isReg(dest) {
            reg := dest[len(regPrefix):]
            output = append(output, 0xc5 + (regOffsets3[reg] * 0x10))
          } else {
            //argument to increment is not a register or pointer
            os.Exit(3)
          }
        case "pop":
          if isReg(dest) {
            reg := dest[len(regPrefix):]
            output = append(output, 0xc1 + (regOffsets3[reg] * 0x10))
          } else {
            //argument to increment is not a register or pointer
            os.Exit(3)
          }
        case "inc":
          if isReg(dest) {
            reg := dest[len(regPrefix):]
            if len(dest)-len(regPrefix) == 2 {
              output = append(output, 0x03 + (regOffsets1[reg] * 0x10))
            } else if len(dest)-len(regPrefix) == 1 {
              output = append(output, 0x04 + (regOffsets2[reg] * 0x08))
            } else {
              //reg is not a valid register
              os.Exit(4)
            }
          } else if isPtr(dest) {
            reg := dest[len(ptrPrefix):len(dest)-len(ptrSuffix)]
            if reg != "hl" {
              //reg is not a valid register
              os.Exit(4)
            }
            output = append(output, 0x34)
          } else {
            //argument to increment is not a register or pointer
            os.Exit(3)
          }
        case "dec":
          if isReg(dest) {
            reg := dest[len(regPrefix):]
            if len(dest)-len(regPrefix) == 2 {
              output = append(output, 0x0b + (regOffsets1[reg] * 0x10))
            } else if len(dest)-len(regPrefix) == 1 {
              output = append(output, 0x05 + (regOffsets2[reg] * 0x08))
            } else {
              //reg is not a valid register
              os.Exit(4)
            }
            output = append(output, 0x35)
          } else if isPtr(dest) {
            reg := dest[len(ptrPrefix):len(dest)-len(ptrSuffix)]
            if reg != "hl" {
              //reg is not a valid register
              os.Exit(4)
            }
          } else {
            //argument to increment is not a register or pointer
            os.Exit(3)
          }
        //instruction not found, read second argument the switch case with two-argument instructions
        default:
          if len(cmd) < 2 {
            os.Exit(5)
          }
          data := cmd[2]
          switch instruction {
            case "ld":
              if isReg(dest) && isReg(data) {
                destReg := dest[len(regPrefix):]
                dataReg := data[len(regPrefix):]
                output = append(output, 0x40 + (regOffsets2[destReg] * 0x08) + regOffsets2[dataReg])
              } else {
                output = append(output , 0xff, 0xfe)
              }
            default:
              output = append(output , 0xff, 0xfe)
          }
      }
  }
  return output
}

func writeCode(file *os.File, data []byte) {
  _, err := file.Write(data)
  check(err)
  //fmt.Println(bytesWritten)
}

func readSection(outfile *os.File, rd *bufio.Reader) bool {
  for line, more := getNextline(rd); getSectionType(line) == srcCode; line, more = getNextline(rd) {
    byteCode := readCode(line)
    writeCode(outfile, byteCode)
    updatePc(pc + uint16(len(byteCode)), outfile)
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
