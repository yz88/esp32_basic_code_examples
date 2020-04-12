# ESP32 configuring and handling GPIO interrupts

Web-Links

- [c't Make: ESP32 SPECIAL 2019 page 6](https://shop.heise.de/katalog/make-esp32-special)
- [ESP32-WROOM-32U datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)
- [Espressif Programming Guide](<https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/intr_alloc.html>)
- [Example: Configuring & Handling ESP32 GPIO Interrupts In Arduino IDE](<https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/>)

The ESP32 chip features 40 physical GPIO pads. Some GPIO pads cannot be used or do not have the corresponding pin on the chip package(refer to technical reference manual). Each pad can be used as a general purpose I/O or can be connected to an internal peripheral signal. The ESP32-WROOM-32D and ESP32-WROOM-32U have 38 GPIO pins.

- Note that GPIO6-11 are usually used for SPI flash.
- GPIO34-39 can only be set as input mode and do not have software pullup or pulldown functions.

There is also separate “RTC GPIO” support, which functions when GPIOs are routed to the “RTC” low-power and analog subsystem. These pin functions can be used when in deep sleep, when the Ultra Low Power co-processor is running, or when analog functions such as ADC/DAC/etc are in use.

![ESP-WROOM-32U pinout](https://github.com/yz88/esp32_basic_code_examples/blob/master/esp32_interrupts/fritzing/esp32-DevKitC-pinout.png)

The ESP32 offers up to 32 interrupt slots for each of it's two cores. Each interrupt has a certain priority level and can be categorized into two types.

- **Hardware Interrupts** – These occur in response to an external event. For example, GPIO Interrupt(when a key is pressed down) or a Touch Interrupt(when touch is detected)
- **Software Interrupts** – These occur in response to a software instruction. For example, a Simple Timer Interrupt or Watchdog Timer Interrupt(when timer times out)

In ESP32, we can define an interrupt service routine function that will be called when a GPIO pin changes its signal value.

## Attaching Interrupt to a GPIO Pin

In Arduino IDE, we use a function called `attachInterrupt()` to set an interrupt on a pin by pin basis. The recommended syntax looks like below.

```c
attachInterrupt(GPIOPin, ISR, Mode);
```

This function takes three parameters:

- **GPIOPin** – Sets the GPIO pin as an interrupt pin, which tells the ESP32 which pin to monitor.
- **ISR** – Is the name of the function that will be called every time the interrupt is triggered.
- **Mode** – Defines when the interrupt should be triggered. Five constants are predefined as valid values:

| mode    | description                                                                        |
| ------- | ---------------------------------------------------------------------------------- |
| LOW     | Triggers interrupt whenever the pin is LOW                                         |
| HIGH    | Triggers interrupt whenever the pin is HIGH                                        |
| CHANGE  | Triggers interrupt whenever the pin changes value, from HIGH to LOW or LOW to HIGH |
| FALLING | Triggers interrupt when the pin goes from HIGH to LOW                              |
| RISING  | Triggers interrupt when the pin goes from LOW to HIGH                              |

## Detaching Interrupt from a GPIO Pin

You can optionally call `detachInterrupt()` function when you no longer want ESP32 to monitor a pin. The recommended syntax looks like below.

```c
detachInterrupt(GPIOPin);
```

## Interrupt Service Routine

Interrupt Service Routine is invoked when an interrupt occurs on any GPIO pin. Its syntax looks like below.

```c
void IRAM_ATTR ISR() {
    Statements;
}
```

ISRs in ESP32 are special kinds of functions that have some unique rules most other functions do not have.

- The interrupt service routine must have an execution time as short as possible, because it blocks the normal program execution.
- Interrupt service routines should have the IRAM_ATTR attribute, according to the [ESP32 documentation(<https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/general-notes.html#iram-instruction-ram>)]

---

**Note**

**What is IRAM_ATTR?**

By flagging a piece of code with the `IRAM_ATTR` attribute we are declaring that the compiled code will be placed in the Internal RAM (IRAM) of the ESP32.

Otherwise the code is placed in the Flash. And flash on the ESP32 is much slower than internal RAM.

If the code we want to run is an interrupt service routine (ISR), we generally want to execute it as quickly as possible. If we had to ‘wait’ for an ISR to load from flash, things would go horribly wrong.

---
