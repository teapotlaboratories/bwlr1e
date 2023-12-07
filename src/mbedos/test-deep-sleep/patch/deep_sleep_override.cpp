/**
 * Public domain code from https://github.com/ARMmbed/mbed-os/issues/15331
 * by  hallard (hallard.me)
 */

#include "hal/static_pinmap.h"
#include "platform/mbed_critical.h"

#define DEEPSLEEP_MODE_STOP1 1
#define DEEPSLEEP_MODE_STOP2 2
volatile uint8_t deep_sleep_mode = DEEPSLEEP_MODE_STOP2;

extern serial_t stdio_uart;

// May be check if console is enabled in mbed_conf ?
void restart_console()
{
    static const serial_pinmap_t console_pinmap = get_uart_pinmap(CONSOLE_TX, CONSOLE_RX);
    serial_init_direct(&stdio_uart, &console_pinmap);
#if MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
    int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE;
# else
    int baud = 9600;
#endif 
    serial_baud(&stdio_uart, baud);
}

extern "C"
{
    extern int mbed_sdk_inited;
    extern int serial_is_tx_ongoing(void);
    extern void save_timer_ctx(void);
    extern void restore_timer_ctx(void);
    extern void PWR_EnterStopMode(void);
    extern void PWR_ExitStopMode(void);
    extern void SetSysClock(void);
    extern void ForceOscOutofDeepSleep(void);
    extern void ForcePeriphOutofDeepSleep(void);
    extern void wait_loop(uint32_t timeout);
}

/*  Most of STM32 targets can have the same generic deep sleep
 *  function, but a few targets might need very specific sleep
 *  mode management, so this function is defined as WEAK.
 *  This one works on STM32WL only */
void hal_deepsleep(void)
{
    /*  WORKAROUND:
     *  MBED serial driver does not handle deepsleep lock
     *  to prevent entering deepsleep until HW serial FIFO is empty.
     *  This is tracked in mbed issue 4408 */
    if (serial_is_tx_ongoing())
    { /* FIXME: return or postpone deepsleep? */
        return;
    }

    // Disable IRQs
    core_util_critical_section_enter();

    save_timer_ctx();

    // Request to enter STOP mode with regulator in low power mode
    int pwrClockEnabled     = __HAL_RCC_PWR_IS_CLK_ENABLED();
    int lowPowerModeEnabled = PWR->CR1 & PWR_CR1_LPR;

    if (!pwrClockEnabled)
    {
        __HAL_RCC_PWR_CLK_ENABLE();
    }
    if (lowPowerModeEnabled)
    {
        HAL_PWREx_DisableLowPowerRunMode();
    }

    // If deep sleep stop 1 set it, else use default stop 2
    if (deep_sleep_mode == DEEPSLEEP_MODE_STOP1)
    {
        HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
    }
    else
    {
        HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
    }

    if (lowPowerModeEnabled)
    {
        HAL_PWREx_EnableLowPowerRunMode();
    }
    if (!pwrClockEnabled)
    {
        __HAL_RCC_PWR_CLK_DISABLE();
    }

    /* Prevent HAL_GetTick() from using ticker_read_us() to read the
     * us_ticker timestamp until the us_ticker context is restored. */
    mbed_sdk_inited = 0;

    /* We've seen unstable PLL CLK configuration when DEEP SLEEP exits just few
     * µs after being entered So we need to force clock init out of Deep Sleep.
     *  This init has been split into 2 separate functions so that the involved
     * structures are not allocated on the stack in parallel. This will reduce
     * the maximum stack usage in case on non-optimized / debug compilers
     * settings
     */
    ForceOscOutofDeepSleep();
    ForcePeriphOutofDeepSleep();
    SetSysClock();

    /*  Wait for clock to be stabilized.
     *  TO DO: a better way of doing this, would be to rely on
     *  HW Flag. At least this ensures proper operation out of
     *  deep sleep */
    /* Charles : Tried those 3 with no luck, so kept wait_loop(500)
       while (!(RCC->CR & RCC_CR_HSERDY));
       while (!(RCC->CR & RCC_CR_PLLRDY));
       while (!(RCC->CR & RCC_CR_HSIRDY));
   */
    wait_loop(500);

    restore_timer_ctx();

    /* us_ticker context restored, allow HAL_GetTick() to read the us_ticker
     * timestamp via ticker_read_us() again. */
    mbed_sdk_inited = 1;

    // Enable IRQs
    core_util_critical_section_exit();

    // Enable back serial console
    restart_console();

}