// AUTHOR:              A. R. Ansari
// E-mail:               ansarirahim1@gmail.com
// Contact:              +919024304883/+919462225303
// VERSION HISTORY: VER 1.0.0 26th OCT 2017
// VER 1.0.1 Dated 19th April 2022
// Calibration offset command support is provided
// Command to set calibration : SET_TEMP_CALIB_DATA<SPACE><OFFSET VALUE>
// Command to read calibration: SET_TEMP_CALIB_DATA
// edit: nano -c tmp112_display.c
// build:  gcc tmp112_display.c -o tmp112_display
// run: ./tmp112_display 1000 (1000 is referesh delay)
// run with bash: GET_TEMP
// rtcpindriver==>source /etc/environment
// GET_VER Command is implented to get version and date of revision
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "tmp112.h"
void main()
{

    const char *cmd = "GET_TEMP_CALIB_DATA"; // This the shell command
    char buf[MYBUFSIZE];                     // Output of the command
    FILE *ptr;
    int c;

    if ((ptr = popen(cmd, "r")) != NULL)
        while (fread(buf, sizeof(buf), 1, ptr))
            while ((c = getchar()) != EOF)
                //   printf("output = %s", buf);

                (void)pclose(ptr);
    float calibdata = atof(buf); //
    // Create I2C bus
    int file;
    char *bus = "/dev/i2c-1";

    if ((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the i2c bus. \r\n");
        exit(1);
    }
    // Get I2C device, TMP112 I2C address is 0x48(72)
    ioctl(file, I2C_SLAVE, 0x48);

    // Select configuration register(0x01)
    // Continous Conversion mode, 12-Bit Resolution, Fault Queue is 1(0x60)
    // Polarity low, Thermostat in Comparator mode, Disables Shutdown mode(0xA0)
    char config[3] = {0};
    config[0] = 0x01;
    config[1] = 0x60;
    config[2] = 0xA0;
    write(file, config, 3);
    /// sleep(0.2);

    // Read 2 bytes of data from register(0x00)
    // temp msb, temp lsb
    char reg[1] = {0x00};
    write(file, reg, 1);
    char data[2] = {0};
    if (read(file, data, 2) != 2)
    {
        printf("ERROR: CONNECTION ERROR\r\n");
        exit(1);
    }
    else
    {
        // Convert the data to 12-bits
        int temp = (data[0] * 256 + data[1]) / 16;
        if (temp > 2047)
        {
            temp -= 4096;
        }
        float cTemp = temp * 0.0625;
        float fTemp = (cTemp * 1.8) + 32;

        // Output data to screen
        printf("TEMP=%.2f\r\n", (cTemp + calibdata));
        // printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
    }
}

