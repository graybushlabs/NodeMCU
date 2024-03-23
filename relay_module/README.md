# Relay Module
This sketch sets up a node server with a basic health check endpoint, an API endpoint, and a web page to toggle the relay.

This uses a [Relay module](https://www.amazon.com/gp/product/B07TKJ6XSM/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&th=1) that is not just a relay on a board. There are LED's to indicate power and state, and a transistor to let you use 3.3v logic, and a diode for protection against the feedback voltage. These are great little boards and can be found fairly cheap and in quantities from 1 to 12 or more. 
I accidentally hooked it up wrong the first time and would have fried my MCU if it werent for the protections.
