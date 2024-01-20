# IoT Security -- Lab03

This excercise was changed due to problems with Tizen IDE.

The goal of the excercise was to securely transmit data using lightweight cryptography. A small device like a smartwatch, single-board-computer or microcontroller needs to collect data from a sensor, encrypt this data using a lightweight encryption algorithm from NIST finalists list and send it to a server. The server then decrypts this data.

## Implementation

Due to previously stated problems, this project uses a RaspberryPi along with a DHT11 sensor for data gathering and encryption. The code can be found under `RaspberryPi`. 

Data gathering was not tested on a real device, as such was lacking.

Data encryption, sending and decryption was tested on a Linux machine. Because of that it should work on most high-end RaspberryPIs.

Secret NONCE values and Keys need to be exchanged beforehand.

## RaspberryPi

The code reads data from the sensor using a working tutorial. This data is encrypted with **Romulus-M**. More information on this algorithm can be found on [their GitHub page](https://github.com/romulusae/romulus).

The PI connects to our server via standard TCP connection. Since the data is encrypted, it should be secure without the use of HTTPS.

## Decryptor

The decryptor waits for a TCP connection from the PI. It then decrypts the data as soon as it arrives and prints the result to the screen.

## Testing the solution

If you lack a Raspberry PI to test the application on, you can compile `main_test`.

1. Enter the *Decryptor* folder and run `make main`.
2. Enter the *RaspberryPi* folder run `make test` (`make main` if tou have the PI).
3. Run the server named `main` from *Decryptor*.
4. Run the client named `main_test` providing the ip address of the server: ex. `main_test 127.0.0.1`.

To stop the application simply send a SIGTERM signal to the application `CTRL + C` usually.

## Original data

> Submit your work using this GitHub repository. Tizen project place in Tizen_project directory, and the decryptor that can be used on the server side for file decryption in Decryptor directory.