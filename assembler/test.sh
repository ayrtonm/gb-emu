go test -run ''
go run assembler.go test.asm output.gb
hexdump -C output.gb
