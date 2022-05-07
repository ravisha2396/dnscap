# ETHERNET OVER USB IN BUILDROOT

***Version 1.0.0*** 

  The steps below show how Ethernet over USB can be configured in buildroot. Some of these steps might not be directly related to Ethernet over USB itself, but are helpful in configuring the buildroot kernel itself.

1. You might want to update the software packages on your host computer by doing:

``` console

$ sudo apt update
$ sudo apt dist-upgrade

```


2. Start by installing the latest buildroot from github source itself.

``` console

$ git clone https://git.buildroot.net/buildroot
$ chown -R myuser.myuser buildroot
$ cd buildroot

```

3. We want to use the default configuration of buildroot for the beaglebone black to start with and make modifications to it as we see necessary.

``` console

$ make menuconfig

```
4. For the most part, the default configuration provided by buildroot for beaglebone black should be enough, however, we might want to choose a few additional packages for the custom kernel we want to generate. 

```
Target packages  --->  
    Networking Applications --->
		[*] dropbear
```
	
Exit menuconfig and save the changes as you exit.

5. Next we configure our Linux Kernel, to set up the power usb as a gadget, and invoke various gadget functions:

``` console

$ make linux-menuconfig  

```

Having enetered the linux configuration menu, navigate to:

     Device Drivers  --->
	[*] USB support  ---> 
		<*>   Support for Host-side USB
		[*]   OTG support
		<*>   EHCI HCD (USB 2.0) support
		<*>   USB Gadget Support  ---> 
			<*>   USB Gadget functions configurable through configfs
			[*]     Ethernet Control Model (CDC ECM)
			[*]     Ethernet Control Model (CDC ECM) subset
			[*]     RNDIS
			[*]     Ethernet Emulation Model (EEM)
			[*]     Mass storage
     Character devices  --->
		<*> Hardware Random Number Generator Core support  ---> 
			<*>   OMAP Random Number Generator support
			<*>   OMAP3 ROM Random Number Generator support 
  		
6. Configfs is a linux driver for managing usb gadget functions from user space. Since it operates in user space, we write a user space script to invoke these usb gadget function drivers when the beaglebone device boots up. Since we cannot ssh to the device at this point we can use the rootfs overlay provided by buildroot to overlay files that we want into the beaglebone black rootfs filesystem when it is booted with our custom kernel.

    The BeagleBone Black Wireless does not have any Ethernet interface, so we will use Ethernet over USB to provide network connectivity between our embedded system and the development
    PC. To achieve this we will need to:
	      a) Add an init script to setup network over USB
	      b) Add a configuration file that configures the network interface with the appropriate IP address

    The init script basically invokes the usb gadget functions required for ethernet over usb and can be and is placed in /etc/init.d/ with a name SXX, so it is invoked when the system is booted with the kernel.

    This script is uploaded to the google drive and can be found at: 
	  https://drive.google.com/file/d/1zG8ITWELM03tmUFZOoWFZ_jSQT9at-NX/view?usp=sharing

``` console
$ mkdir board/bootlin/beaglebone/rootfs
$ cd buildroot/board/bootlin/beaglebone/rootfs
$ mkdir etc
$ cd etc/
$ mkdir init.d
$ mkdir network

```

   Place the file at buildroot/board/bootlin/beaglebone/rootfs/etc/init.d/ . 

   Since we create a new network interface for ethernet of usb, we statically define a ip address for it on the beaglebone black. We modify the inbuilt interfaces file in /etc/network by overwriting it with a interfaces file of our own. The file can be found at:
	 https://drive.google.com/file/d/1XMMk36pWeJ3tykf6vWLSkg2h-rburzmT/view?usp=sharing

   Place the file at buildroot/board/bootlin/beaglebone/rootfs/etc/network/

7. To bring our rootfs overlay modifications to effect, enable the rootfs overlay in buildroot configurations by doing:

``` console

make menuconfig

```

	System configuration  --->  
		() Root filesystem overlay directories

  When you press enter on this option it will prompt you to enter the path of the overlay. Provide the path board/bootlin/beaglebone/rootfs/ as the rootfs overlay path.

8. By now we have completed majority of buildroot configuration and we are ready for our first build.

``` console

$ make

```

   You can check in output/target/etc/init.d/ and output/target/etc/network/ if both the init script and network configuration files were properly copied.

   Reflash the root filesystem on the SD card, and boot your BeagleBone Black. It should now have an IP address configured for usb0 by default.

9. Next we configure the host with a static ip to communicate the beaglebone black device over power usb. On Ubuntu, the network interfaces corresponding to Ethernet-over-USB connections are named
    enx<macaddr>. The host MAC address is hardcoded in the S30usbgadget script to f8:dc:7a:00:00:01, so the interface will be named enxf8dc7a000001.

   To configure an IP address for this interface on your host machine, we’ll use NetworkManager and its command line interface:      

``` console

$ nmcli con add type ethernet ifname enxf8dc7a000001 ip4 192.168.0.1/24

```

   Once this is done, make sure you can communicate with your target using ping.

10. Once ping is successful, we try to ssh:

``` console

$ ssh root@192.168.0.2

```

11. If all goes smooth, you should be able to communicate with the beaglebone black over usb and ssh into it! Good luck!
