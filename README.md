# BBB_TMP112_7SEGDISPLAY
TMP112 WITH SEVEN SEGMENT DISPLAY ALONG WITH CALIBRATION OFFSET COMMAND SUPPORT
1.  Read install file content and comment out if not required.
2.  Execute/run install file
3.  Create the service
4.  nano /lib/systemd/DISPLAY7_TEMP.service
Edit the above file as necessary to invoke the different functionalities like network. Enable these only if the code needs that particular service. Disable unwanted ones to decrease boot time.

[Unit]
After=rtcpindriver.service
Description=Display7Segment Temp
After=syslog.target network.target
[Service]
Type=simple
ExecStart=/usr/bin/DISPLAY7_TEMP
[Install]
WantedBy=multi-user.target



Create a symbolic link to let the device know the location of the service.

cd /etc/systemd/system/
ln /lib/systemd/DISPLAY7_TEM.service DISPLAY7_TEM.service
Make systemd reload the configuration file, start the service immediately (helps to see if the service is functioning properly) and enable the unit files specified in the command line.

systemctl daemon-reload
systemctl start DISPLAY7_TEMP.service
systemctl enable DISPLAY7_TEMP.service
Restart BBB immediately to see if it runs as intended.

reboot
