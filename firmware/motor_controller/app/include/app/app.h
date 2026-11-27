/**
 * @file app.h
 * @brief Application entry points. main.c calls only these.
 */
#ifndef APP_APP_H
#define APP_APP_H

/** Called once after all MX_*_Init() peripheral setup. */
void app_init(void);

/** Called repeatedly from the main superloop. Must not block. */
void app_loop(void);

#endif /* APP_APP_H */
