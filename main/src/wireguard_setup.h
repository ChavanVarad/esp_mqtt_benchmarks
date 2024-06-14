#ifndef WIREGUARD_SETUP_H
#define WIREGUARD_SETUP_H

#include <esp_err.h>
#include <esp_wireguard.h>

esp_err_t wireguard_setup(wireguard_ctx_t* ctx);

#endif // WIREGUARD_SETUP_H
