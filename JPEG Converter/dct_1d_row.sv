module dct_1d_row
	#(parameter signed [9:0] COS[8][8] = '{default: 0})
	(
	input logic clk,
	input logic rst_n,
	input logic signed [7:0] pixel_in,
	input logic valid_in,
	output logic signed [28:0] pixel_out,
	output logic valid_out
	);

logic send_out = 0;
logic [2:0] out_time = 0;
logic [2:0] pixel_count = 0;
logic signed [23:0] accum [8] = '{default: 0};


always_ff @(posedge clk or negedge rst_n) begin

	if (!rst_n) begin
		send_out 	<= 0;
		out_time    <= 0;
		pixel_count <= 0;
		valid_out   <= 0;
		for (int k = 0; k < 8; k++)
			accum[k] <= '0;
			
	end else begin
		valid_out <= 0; 
		unique if (valid_in && !send_out) begin 
			valid_out <= 0; 
			
			for (int k = 0; k < 8; k++)
				accum[k] <= pixel_in * COS[k][pixel_count] + accum[k];
			
			if (pixel_count < 7)
				pixel_count <= pixel_count + 1;
			else begin 
				send_out <= 1;
				pixel_count <= 0;
			end
			
		end else if (send_out) begin
			if (out_time < 7) begin
				pixel_out <= (out_time == 0) ? (accum[out_time] * 90) >>> 8 : (accum[out_time] * 128) >>> 8;
				valid_out <= 1;
				out_time <= out_time + 1;
			end else if (out_time == 7) begin
				pixel_out <= (accum[out_time] * 128) >>> 8;
				valid_out <= 1;
				send_out <= 0;
				out_time <= 0;
				for (int k = 0; k < 8; k++)
					accum[k] <= '0;
			end
		end
		// else: idle, hold state
	end
end

endmodule
