# JPEG Converter

FPGA-side JPEG / DCT exploration for a Cyclone IV target.

## Included modules
- `dct_1d_row.sv`
- `dct_1d_column.sv`
- `dct_2d.sv`
- `quantizer.sv`
- `zigzag.sv`
- `rle_encoder.sv`
- `huffman_encoder.sv`

## Project files
- `JPEG_Converter.qpf`
- `JPEG_Converter.qsf`
- `jpeg_fpga_init.v` — current Quartus top-level entity
- `jpeg_tb.sv` — testbench snapshot

## Helper scripts
- `scripts/generate_dct_cosines.py` — prints Q8.8 cosine terms used for DCT coefficient work
