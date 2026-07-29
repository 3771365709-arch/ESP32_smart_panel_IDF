#include "relay.h"

static bool current_state = false;

void relay_init(void) {
    gpio_reset_pin(RELAY_PIN);
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_PIN, 0);
    current_state = false;
}

void relay_set(bool on) {
    gpio_set_level(RELAY_PIN, on ? 1 : 0);
    current_state = on;
}

bool relay_get_state(void) {
    return current_state;
}
