# JPEG / FPGA extension notes

## Purpose

The `JPEG Converter/` directory contains the FPGA-side exploration tied to the media-center project. It is organized as a modular SystemVerilog snapshot centered on DCT and later JPEG-style pipeline stages.

## Module overview

- `dct_1d_row.sv` — row-wise 1-D DCT stage
- `dct_1d_column.sv` — column-wise 1-D DCT stage
- `dct_2d.sv` — buffering and control around the row/column DCT flow
- `quantizer.sv` — quantization stage
- `zigzag.sv` — zigzag reorder stage
- `rle_encoder.sv` — run-length encoding stage
- `huffman_encoder.sv` — Huffman stage scaffold
- `jpeg_fpga_init.v` — current board-level top entity used by the Quartus project
- `jpeg_tb.sv` — testbench snapshot
- `scripts/generate_dct_cosines.py` — helper for generating Q8.8 cosine coefficients

## Current repo status

This folder should be read as an **extension / development branch of the overall project**, not as a polished standalone JPEG product. The modular files are useful for understanding the intended architecture and the DCT/JPEG direction of the project, but the board-level integration is still separate from the embedded media-center application.

## Quartus notes

The Quartus project file targets:
- **Family:** Cyclone IV E
- **Device:** `EP4CE115F29C7`

Two placeholder files (`jpeg_main.sv` and `jpeg_pkg.sv`) are kept as valid SystemVerilog stubs in this snapshot so the source tree is complete and cleaner to browse on GitHub.
