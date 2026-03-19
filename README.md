# RISCV-Project

> Bare-metal benchmarking of **NMSIS-DSP and NMSIS-NN kernels** on RISC-V Andes D25F —
> cycle-accurate measurement using **NDS_MCYCLE / NDS_MINSTRET CSRs** on real silicon.

Part of a Master's thesis at **TU Chemnitz** in collaboration with
**Infineon Technologies**, Dresden (2025).

Companion ARM repo: [ARM-Project](https://github.com/Karthik-Swaminathan98/ARM-Project)  
Full cross-architecture analysis: [arm-riscv-benchmark-results](https://github.com/Karthik-Swaminathan98/arm-riscv-benchmark-results)

![Language](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/platform-RISC--V%20Andes%20D25F-informational)
![Libraries](https://img.shields.io/badge/libraries-NMSIS--DSP%20%7C%20NMSIS--NN%20%7C%20Andes--DSP-orange)
![Board](https://img.shields.io/badge/board-Telink%20B91-lightgrey)

---

## Hardware platform

| Feature | Telink B91 |
|---|---|
| Core | Andes D25F (RV32IMACFDBP) |
| Pipeline | 5-stage in-order |
| Clock | 96 MHz max · **24 MHz used for benchmarking** |
| SRAM | 256 KB |
| Flash | 16 MB |
| Key extensions | FPU · DSP · P-extension (packed SIMD) |
| Toolchain | riscv-32-elf-gcc 7.4 (nds32le-elf-mculib-v5f) |
| IDE | AndeSight RDS |
| Debug | TLSR9 DEV KEY (JTAG) |

All benchmarks execute **entirely from RAM** — no flash wait-states.
Interrupts are disabled during measurement for cycle-accurate results.
Each kernel is built and flashed as a standalone binary.

---

## What is measured

Five metrics are captured per kernel:

| Metric | Method |
|---|---|
| Cycle count | NDS_MCYCLE CSR |
| Instruction count | NDS_MINSTRET CSR |
| Execution time | Derived from cycle count ÷ core frequency |
| Stack usage | Stack-paint technique (0xAAAAAAAA pattern) |
| Code size | .map file + objdump call-graph analysis |

---

## Measurement implementation

### Cycle count and instruction count — CSR counters

RISC-V provides direct hardware counters for both cycles and retired instructions
via the NDS_MCYCLE and NDS_MINSTRET Control and Status Registers:

```c
/* Reset both counters */
write_csr(NDS_MCYCLE,    0);
write_csr(NDS_MINSTRET,  0);

/* Capture start */
uint32_t start_cycles = read_csr(NDS_MCYCLE);
uint32_t start_instr  = read_csr(NDS_MINSTRET);

/* Run benchmark */
benchmark_function();

/* Capture end */
uint32_t end_cycles = read_csr(NDS_MCYCLE);
uint32_t end_instr  = read_csr(NDS_MINSTRET);

uint32_t cycle_count = end_cycles - start_cycles;
uint32_t instr_count = end_instr  - start_instr;
```

Unlike the ARM DWT estimation formula, RISC-V provides exact instruction counts
directly in hardware — no approximation needed.

### Execution time

```c
clkFastfreq = get_clk_fast_freq();   /* Telink SDK clock query */
float time_us = ((float)cycle_count / clkFastfreq) * 1e6f;
```

### Stack usage — stack-paint technique

The stack is filled with a known sentinel pattern before execution.
After the benchmark runs, the stack is scanned from the base to find
the deepest overwritten address:

```c
void fill_stack_pattern_to_sp(void) {
    register uint32_t *sp;
    __asm volatile ("mv %0, sp" : "=r" (sp));   /* RISC-V: mv not mov */
    uint32_t *p = (uint32_t*)&__StackLimit;
    while (p < sp) { *p++ = 0xAAAAAAAA; }
}

uint32_t measure_stack_usage(void) {
    register uint32_t *sp;
    __asm volatile ("mv %0, sp" : "=r" (sp));
    uint32_t *p = (uint32_t*)&__StackLimit;
    while (p < sp && *p == 0xAAAAAAAA) { p++; }
    return ((uint32_t)sp - (uint32_t)p);
}
```

This technique captures peak stack depth including all transitive function calls —
no RTOS or OS required.

### Complete benchmark loop

```c
/* 1. Paint stack */
fill_stack_pattern_to_sp();

/* 2. Reset and read counters */
uint32_t start_cycles, start_inst, end_cycles, end_inst;
write_csr(NDS_MCYCLE,   0);
write_csr(NDS_MINSTRET, 0);
read_perf_counters(&start_cycles, &start_inst);

/* 3. Run kernel */
riscv_convolve_wrapper_s8(&ctx, &conv_params, ...);

/* 4. Read counters */
read_perf_counters(&end_cycles, &end_inst);

/* 5. Measure stack */
uint32_t stack_used = measure_stack_usage();

/* 6. Calculate time */
float time_us = ((float)(end_cycles - start_cycles) / clkFastfreq) * 1e6f;
```

---

## Kernels benchmarked

### DSP — NMSIS-DSP and Andes-DSP

| Category | Functions | Data types |
|---|---|---|
| Transform | riscv_cfft_f32, riscv_cfft_q15 | F32, Q15 |
| Filtering | riscv_fir_f32, riscv_fir_q15 | F32, Q15 |
| Magnitude | riscv_cmplx_mag_f32, riscv_cmplx_mag_q15 | F32, Q15 |
| Fast Math | riscv_sqrt, riscv_atan2, riscv_sin, riscv_cos | F32, Q15 |

Both NMSIS and Andes libraries are benchmarked side-by-side using identical
input vectors, build flags, and memory layout.

### NN — NMSIS-NN

| Category | Functions | Data types |
|---|---|---|
| Activation | riscv_relu6_s8, riscv_nn_activation_s16 | S8, S16 |
| Convolution | riscv_convolve_wrapper_s8/s16, riscv_depthwise_conv_wrapper_s8/s16 | S8, S16 |
| Fully Connected | riscv_fully_connected_s8/s16 | S8, S16 |
| Pooling | riscv_avgpool_s8/s16 | S8, S16 |
| Softmax | riscv_softmax_s8/s16 | S8, S16 |

NN test vectors sourced from the official CMSIS-NN test suite to ensure
output correctness is verifiable against golden references.

---

## Build configuration

All kernels built in Release mode with these flags:

```
-O3
-ffunction-sections -fdata-sections
-flto
-Wl,-gc-sections
```

### Libraries

| Library | Source |
|---|---|
| NMSIS-DSP/NN | [github.com/Nuclei-Software/NMSIS](https://github.com/Nuclei-Software/NMSIS) |
| Andes-DSP (P-ext) | Precompiled via official Andes Technology repo |

> The open-source Andes DSP library lacks P-extension (packed SIMD) support.
> The precompiled version with full P-extension enabled was used for fair benchmarking,
> as confirmed with Andes Technology support.

---

## Key results (vs CMSIS baseline = 1.0)

Values > 1.0 = RISC-V outperforms ARM CMSIS.

| Function | Cycle speedup | Stack efficiency |
|---|---|---|
| FFT F32 (ANDES) | 1.47× | 9.96× less stack |
| FFT F32 (NMSIS) | 1.47× | 3.41× less stack |
| Magnitude Q15 (ANDES) | 3.32× | 14.8× less stack |
| FIR Q15 (NMSIS) | 2.14× | 2.64× less stack |
| Convolution S8 (NMSIS) | 1.31× | comparable |
| Convolution S16 (NMSIS) | 1.25× | comparable |
| Softmax S16 (NMSIS) | 1.15× | 1.79× less stack |

Full results with all metrics: [arm-riscv-benchmark-results](https://github.com/Karthik-Swaminathan98/arm-riscv-benchmark-results)

---

## How to build and run

**Requirements:**
- AndeSight RDS (IDE)
- riscv-32-elf-gcc 7.4 (nds32le-elf-mculib-v5f)
- Telink B91 evaluation board
- TLSR9 DEV KEY programmer (JTAG)

**Steps:**
1. Open AndeSight RDS → Import project from any benchmark folder (e.g. `RV_NN_Convolution_Benchmark`)
2. Select Release configuration
3. Build → Flash via TLSR9 DEV KEY USB
4. Open serial terminal at 115200 baud to read output

**Sample output:**
```
[BENCH] riscv_cfft_f32 N=1024
  Cycles      : 92504
  Instructions: 82833
  Stack used  : 276 bytes
  Exec time   : 964.6 us
```

---

## Related repos

| Repo | Description |
|---|---|
| [ARM-Project](https://github.com/Karthik-Swaminathan98/ARM-Project) | ARM Cortex-M4 counterpart — CMSIS-DSP/NN benchmarks on PSoC6 |
| [arm-riscv-benchmark-results](https://github.com/Karthik-Swaminathan98/arm-riscv-benchmark-results) | Full cross-architecture results — DSP, NN, and model inference |
| [mcu-function-size-analyser](https://github.com/Karthik-Swaminathan98/mcu-function-size-analyser) | Python tool — dependency-aware function code size analyser |

---

## Acknowledgements

Master's thesis at **Technische Universität Chemnitz**
(Chair of Computer Architectures and Systems)
in collaboration with **Infineon Technologies**, Dresden.

Supervised by:
- Prof. Dr. Alejandro Masrur (TU Chemnitz)
- Mr. Daniel Markert (Infineon Technologies)
- Dr. Elias Trommer (Infineon Technologies)
- Mr. Jerome Almon Swamidasan (Infineon Technologies)

---

## Author

**Karthik Swaminathan** — Embedded Firmware Engineer  
M.Sc. Embedded Systems · TU Chemnitz  
[LinkedIn](https://linkedin.com/in/karthik-swaminathan98) · karthik94870@gmail.com
