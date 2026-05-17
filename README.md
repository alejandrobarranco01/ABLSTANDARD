# ABLStandard

![Build Status](https://github.com/alebar/ABLStandard/actions/workflows/build-success.yml/badge.svg)

General purpose STM32 Nucleo starter project I wrote to test random stuff. Gives me a GUI and buttons so I can prototype without rebuilding the same foundation every time.

## Note to self

Display uses I2C with DMA. Make sure to configure Cube MX as so

- Normal byte data width
- Memory increment enabled
- Address checked
