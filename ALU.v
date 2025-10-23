module ALU (
    input  [7:0] A,
    input  [7:0] B,
    input  [2:0] opcode,
    output reg [7:0] Result,
    output reg Carry,
    output reg Zero
);

always @(*) begin
    Carry = 0;
    case (opcode)
        3'b000: {Carry, Result} = A + B;      // ADD
        3'b001: {Carry, Result} = A - B;      // SUB
        3'b010: Result = A & B;               // AND
        3'b011: Result = A | B;               // OR
        3'b100: Result = A ^ B;               // XOR
        3'b101: Result = ~(A | B);            // NOR
        3'b110: Result = A << 1;              // SHIFT LEFT
        3'b111: Result = A >> 1;              // SHIFT RIGHT
        default: Result = 8'b00000000;
    endcase

    Zero = (Result == 0);
end

endmodule
