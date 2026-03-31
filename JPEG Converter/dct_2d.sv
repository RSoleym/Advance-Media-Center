module dct_2d(
	input logic clk,
	input logic rst_n,
	input logic signed [7:0] pixel_in,
	input logic valid_in,
	input logic pixel_done,
	output logic signed [33:0] pixel_out,
	output logic valid_out
	);

parameter signed [9:0] COS[8][8] = '{
    // k = 0 
    '{256, 256, 256, 256, 256, 256, 256, 256},
    // k = 1 
    '{251, 212, 142,  49,  -49, -142, -212, -251},
    // k = 2 
    '{236,  97,  -97, -236, -236,  -97,   97,  236},
    // k = 3
    '{212, -49, -251, -142,  142,  251,   49, -212},
    // k = 4
    '{181, -181, -181, 181, 181, -181, -181, 181},
    // k = 5
    '{142, -251,  49, 212, -212,  -49,  251, -142},
    // k = 6
    '{ 97, -236, 236, -97, -97, 236, -236,  97},
    // k = 7
    '{ 49, -142, 212, -251, 251, -212, 142, -49}
};

logic signed [28:0] buffer1 [8][8] = '{default = 0};
logic signed [28:0] buffer0 [8][8] = '{default = 0};

logic signed [28:0] dct_row = 0;
logic signed [28:0] column_data_in = 0;

logic [2:0] row1 = 0;
logic [2:0] col1 = 0;
logic [2:0] row2 = 0;
logic [2:0] col2 = 0;

logic col_block_done = 0;
logic stream_done = 0;

logic row_valid_out = 0;
logic col_valid_in = 0;

logic buffer_write = 0;
logic buffer_read;

logic [2:0] col_count_in = 0;
logic [2:0] col_count_out = 0;

logic col_busy;

assign buffer_read = !buffer_write;

dct_1d_row #(.COS(COS)) DCT_FIRST_TERM (
	.clk(clk),
	.rst_n(rst_n),
	.pixel_in(pixel_in),
	.valid_in(valid_in),
	.pixel_out(dct_row),
	.valid_out(row_valid_out)
	);
	
dct_1d_column #(.COS(COS)) DCT_SECOND_TERM (
	.clk(clk),
	.rst_n(rst_n),
	.pixel_in(column_data_in),
	.valid_in(col_valid_in),
	.pixel_out(pixel_out),
	.valid_out(valid_out)
	);

typedef enum logic [1:0] {
    IDLE,
    COL_START
} state_t;

state_t col_state;


always_ff @(posedge clk or negedge rst_n) begin
	if (!rst_n)
		stream_done <= 0;
	else if(pixel_done)
		stream_done <= 1;
end

always_ff @(posedge clk or negedge rst_n) begin
	
	if (!rst_n) begin 
		row1 <= 0;
		col1 <= 0;
		buffer_write <= 0;
		col_state <= IDLE;
		
		for (int i = 0; i < 8; i++) begin
			for (int j = 0; j < 8; j++) begin
				buffer1[i][j] <= 0;
				buffer0[i][j] <= 0;
			end
		end
	end else begin
		if (row_valid_out) begin
			if (buffer_write)
				buffer1 [row1][col1] <= dct_row;
			else 
				buffer0 [row1][col1] <= dct_row;
			
			col1 <= col1 + 1;
			if (col1 == 7) begin
				col1 <= 0;
				if (row1 < 7)
					row1 <= row1 + 1;
				else begin
					row1 <= 0;
					buffer_write <= !buffer_write;
					col_state <= COL_START;	
				end
			end
		end else if (stream_done && col_block_done) begin
			col_state <= IDLE;
			col_block_done <= 0;
		end
		// else: idle, hold state		
	end
end


always_ff @(posedge clk or negedge rst_n) begin

	if (!rst_n) begin 
		column_data_in <= 0;
		col_valid_in <= 0;
		
		row2 <= 0;
		col2 <= 0;
		
		col_block_done <= 0;
		col_count_in  <= 0;
		col_count_out <= 0;
		col_busy      <= 0;
		
	end else begin	
		col_valid_in <= 0;
		if(col_state != IDLE && !col_busy) begin

			column_data_in <= buffer_read 
									? buffer1 [row2][col2] 
									: buffer0 [row2][col2];
				
			col_valid_in <= 1;
			
			if (row2 < 7)
				row2 <= row2 + 1;
			else
				row2 <= 0;
			
			if (col_count_in < 7)
				col_count_in <= col_count_in + 1;
			else begin
				col_count_in <= 0;
				col_busy <= 1;
			end
		
		end else if (col_busy && valid_out) begin
	
			if (col_count_out < 7)
				col_count_out <= col_count_out + 1;
			else begin
				col_count_out <= 0;
				col_busy <= 0;
			
				if (col2 < 7)
					col2 <= col2 + 1;
				else begin
					col2 <= 0;
					if (stream_done)
						col_block_done <= 1;	
				end
			end
		end // else: idle, hold state
	end
end
endmodule 